//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include <sstream>

#include "TGraph.h"
#include "TF1.h"

#include "CsIManager.h"

CsIManager::CsIManager() {
    m_cosmi = CosmicRay::GetInstance();
    Branch();

    if(!Init()) {
        std::cout << "[Error] CsIManager initialization failed\n";
        m_isInit = 0;
    } else {
        m_isInit = 1;
    }
    m_isVis = 0;
}

CsIManager::~CsIManager() {
    for(int id=0; id < nCSI; ++id) {
        delete m_csi[id];
    }
}

void CsIManager::Branch() {
    m_eventTree->Branch("csi.isUsed", m_BRout.isUsed, "csi.isUsed[2716][2]/O");

    m_eventTree->Branch("csi.data", m_BRout.data, "csi.data[2716][2][64]/S");
    m_eventTree->Branch("csi.ped", m_BRout.ped, "csi.ped[2716][2]/F");
    m_eventTree->Branch("csi.peak", m_BRout.peak, "csi.peak[2716][2]/F");
    m_eventTree->Branch("csi.sumADC", m_BRout.sumADC, "csi.sumADC[2716][2]/F");
    m_eventTree->Branch("csi.pt", m_BRout.pt, "csi.pt[2716][2]/F");
    m_eventTree->Branch("csi.cft", m_BRout.cft, "csi.cft[2716][2]/F");
    m_eventTree->Branch("csi.isDataComplemented", m_BRout.isDataComplemented, "csi.isDataComplemented[2716][2]/O");
    m_eventTree->Branch("csi.TD", m_BRout.TD, "csi.TD[2716]/F");
    m_eventTree->Branch("csi.MT", m_BRout.MT, "csi.MT[2716]/F");

    m_eventTree->Branch("csi.isHit", m_BRout.isHit, "csi.isHit[2716]/O");
    m_eventTree->Branch("csi.nHit", &m_BRout.nHit, "csi.nHit/S");
    m_eventTree->Branch("csi.hitpos", m_BRout.hitpos, "csi.hitpos[2716][3]/F");

    m_eventTree->Branch("csi.Edep", m_BRout.Edep, "csi.Edep[2716]/F");
    m_eventTree->Branch("csi.range", m_BRout.range, "csi.range[2716]/F");
}

void CsIManager::Fill(){
    for(int id=0; id<nCSI; ++id) {
        for(int side=0; side<2; ++side) {
            m_BRout.isUsed[id][side] = m_csi[id]->IsUsed(side);
            for(int smpl=0; smpl<64; ++smpl) {
                m_BRout.data[id][side][smpl] = m_csi[id]->GetData(side)[smpl];
            }
            m_BRout.ped[id][side] = (Float_t) m_csi[id]->GetPedestal(side);
            m_BRout.peak[id][side] = (Float_t) m_csi[id]->GetPeak(side);
            m_BRout.sumADC[id][side] = (Float_t) m_csi[id]->GetIntegratedADC(side);
            m_BRout.pt[id][side] = (Float_t) m_csi[id]->GetPeakTime(side);
            m_BRout.cft[id][side] = (Float_t) m_csi[id]->GetCFTime(side);
            m_BRout.isDataComplemented[id][side] = m_csi[id]->IsDataComplemented(side);
        }

        if(m_csi[id]->IsUsed(0) && m_csi[id]->IsUsed(1)) {
            m_BRout.TD[id] = (Float_t) m_csi[id]->GetTimeDiff();
            m_BRout.MT[id] = (Float_t) m_csi[id]->GetMeanTime();
        }

        m_BRout.isHit[id] = m_csi[id]->IsHit();
        m_BRout.nHit = m_nHit;
        for(int axis = 0; axis<3; ++axis) {
            m_BRout.hitpos[id][axis] = (Float_t) m_csi[id]->GetHitPos()[axis];
        }

        m_BRout.Edep[id] = (Float_t) m_csi[id]->GetEnergyDeposit();
        m_BRout.range[id] = (Float_t) m_csi[id]->GetRange();
    }
}

// Initialization
bool CsIManager::Init() {
    m_isInit = 0;
    std::cout << "Initializing CsIManager -----------------------------------------\n";

    if(!Init_map()) return false;
    if(!Init_DAQconfig()) return false;
    if(!Init_calibration()) return false;

    std::cout << "-----------------------------------------------------------------\n";
    m_isInit = 1;
    return true;
}

bool CsIManager::Init_map() {
    std::string filename = "./data/map_csi.txt";
    std::ifstream ifs(filename.c_str());
    if (!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    int locID, lineID, crysID, size;
    double posx, posy;
    while(ifs >> lineID >> locID >> crysID >> posx >> posy >> size) {
        m_csi[locID] = new CsI(locID, crysID, lineID, posx, posy, size);
    }
    ifs.close();

    std::cout << "* Location map                  [OK]\n";
    return true;
}

bool CsIManager::Init_DAQconfig() {
    // ADC channels
    int fname_runID = GetFirstRunID(m_runID);
    if(!fname_runID) {
        std::cout << "[Error] run" << m_runID << " is not registered with data/runset.txt\n";
        return false;
    }

    for(int side = 0; side < 2; side++) {
        std::stringstream ss;
        if(side == 0){
            ss << "data/ADCchMap/run" << fname_runID << "_mppc.txt";
        }
        if(side == 1){
            ss << "data/ADCchMap/run" << fname_runID << "_pmt.txt";
        }

        std::string filename = ss.str();
        std::ifstream ifs(filename.c_str());
        if (!ifs) {
            std::cout << "\t[Error] " << filename << " not found\n";
            return false;
        } else {
            std::cout << "\t"  << filename << "\n";
        }

        int id, crate, mod, ch;
        while (ifs >> id >> crate >> mod >> ch) {
            m_csi[id]->SetData(side, crate, mod, ch);
        }

        for(int id=0; id<nCSI; ++id) {
            if(m_csi[id]->IsUsed(side)) continue;
            m_csi[id]->SetData(side, -1, -1, -1); // All elements in data arrays are set to 0
        }
    }

    // Digital delays
    for(int id=0; id < nCSI; ++id) {
        for(int side=0; side < 2; ++side) {
            if(m_csi[id]->IsUsed(side)) m_csi[id]->SetDelay(side, 0);
        }
    }

    // Dead channels
    for(int side = 0; side < 2; side++) {
        std::stringstream ss;
        if(side==0) {
            ss << "data/deadCSI/run" << fname_runID << "_mppc.txt";
        }
        if(side==1) {
            ss << "data/deadCSI/run" << fname_runID << "_pmt.txt";
        }

        std::string filename = ss.str();
        std::ifstream ifs(filename.c_str());
        if(!ifs) {
            if(side==0) std::cout << "\tNo dead channels in MPPC side\n";
            else std::cout << "\tNo dead channels in PMT side\n";
            continue;
        } else {
            std::cout << "\t" << filename << "\n";
        }

        int csiID;
        while(ifs >> csiID) {
            m_csi[csiID]->SetDeadChannel(side);
        }
    }

    std::cout << "* ADC channel map               [OK]\n";
    return true;
}

bool CsIManager::Init_calibration() {
    // Deposit energy
    int fname_runID = GetFirstRunID(m_runID);
    if(!fname_runID) {
        std::cout << "[Error] run" << m_runID << " does not exist in data/runset.txt\n";
        return false;
    }
    TString filename = Form("data/CsIEdep/cali_csiEdep_run%d.txt", fname_runID);
    std::ifstream ifs(filename.Data());
    if(!ifs) {
        std::cout << "[Warning] " << filename << " not found\n";
        for(int id = 0; id<nCSI; ++id) {
            m_csi[id]->SetEdepCalibConst(0);
        }
    } else {
        std::cout << "\t" << filename << "\n";
        int id;
        double cc;
        while(ifs >> id >> cc) {
            m_csi[id]->SetEdepCalibConst(cc);
        }
    }
    ifs.close();
    ifs.clear();

    // Hit z position
    filename = Form("data/csi_zhit.txt");
    ifs.open(filename.Data());
    if(!ifs) {
        std::cout << "[Error] " << filename << " not found\n";
        return false;
    }

    int id;
    while(ifs >> id) {
        double z[36];
        for(int i = 0; i<36; ++i) {
            ifs >> z[i];
        }
        m_csi[id]->SetZhitTable(z);
    }

    std::cout << "* Calibration of CsI            [OK]\n";
    return true;
}

// Processes
void CsIManager::Process() {
    HitDecision();
    RecZhit();
}

void CsIManager::HitDecision() {
    m_nHit = 0;
    for(int id = 0; id < nCSI; ++id) {
        m_csi[id]->Process();
        if(m_csi[id]->IsHit()) {
            ++m_nHit;
            m_cosmi->AddHitPoint(m_csi[id]->GetHitPos(), m_csi[id]->GetPosres());
        }
    }
}

void CsIManager::RecZhit() {
    for(int id = 0; id < nCSI; ++id) {
        if(m_csi[id]->IsHit()) {
            // m_csi[id]->SetHitpos(2, (m_csi[id]->GetPos()[1]-m_cosmi->GetTrack(1)[0])/m_cosmi->GetTrack(1)[1]);
            int i_track = m_cosmi->GetTrackID() - 1;
            m_csi[id]->SetHitpos(2, m_csi[id]->GetHitzTable()[i_track]);
        }
    }
}

void CsIManager::RecRange() {
    for(int id = 0; id < nCSI; ++id) {
        if(m_csi[id]->IsUsed(1)) {
            m_csi[id]->RecRange();
        }
    }
}

// Visualization
void CsIManager::Visualize() {
    for(int i=0; i<nCSI; ++i) {
        m_csi[i]->Visualize();
    }
    m_isVis = 1;
}

void CsIManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }

    std::vector<CsI*> usedCSI;
    for(int id=0; id<nCSI; ++id) {
        if(m_csi[id]->IsUsed(0) || m_csi[id]->IsUsed(1)) {
            usedCSI.push_back(m_csi[id]);
            continue;
        }
        m_csi[id]->Display(plane);
    }

    for(std::vector<CsI*>::iterator itr=usedCSI.begin(); itr!=usedCSI.end(); ++itr) {
        (*itr)->Display(plane);
    }

    m_canv->Update();
    m_canv->Modified();
}
