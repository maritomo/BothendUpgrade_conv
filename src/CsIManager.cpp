//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include "CsIManager.h"

CsIManager::CsIManager() {
    m_cosmi = CosmicRay::GetInstance();
    Branch();

    if(!Init()) {
        std::cout << "CsIManager initialization failed\n";
        m_isInit = 0;
    } else {
        std::cout << "CsIManager initialized\n";
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
    m_tout->Branch("csi.isUsed", m_BRout.isUsed, "csi.isUsed[2716][2]/O");

    m_tout->Branch("csi.data", m_BRout.data, "csi.data[2716][2][64]/S");
    m_tout->Branch("csi.ped", m_BRout.ped, "csi.ped[2716][2]/F");
    m_tout->Branch("csi.peak", m_BRout.peak, "csi.peak[2716][2]/F");
    m_tout->Branch("csi.integ", m_BRout.integ, "csi.integ[2716][2]/F");
    m_tout->Branch("csi.pt", m_BRout.pt, "csi.pt[2716][2]/F");
    m_tout->Branch("csi.cft", m_BRout.cft, "csi.cft[2716][2]/F");

    m_tout->Branch("csi.TD", m_BRout.TD, "csi.TD[2716]/F");
    m_tout->Branch("csi.MT", m_BRout.MT, "csi.MT[2716]/F");

    m_tout->Branch("csi.isHit", m_BRout.isHit, "csi.isHit[2716]/O");
    m_tout->Branch("csi.hitpos", m_BRout.hitpos, "csi.hitpos[2716][3]/F");
}

void CsIManager::Fill(){
    for(int id=0; id<nCSI; ++id) {
        for(int side=0; side<2; ++side) {
            m_BRout.isUsed[id][side] = m_csi[id]->IsUsed(side);
            if(!m_csi[id]->IsUsed(side)) {
                continue;
            }

            for(int smpl=0; smpl<64; ++smpl) {
                m_BRout.data[id][side][smpl] = (Short_t) m_csi[id]->GetData(side)[smpl];
            }
            m_BRout.ped[id][side] = (Float_t) m_csi[id]->GetPedestal(side);
            m_BRout.peak[id][side] = (Float_t) m_csi[id]->GetPeak(side);
            m_BRout.integ[id][side] = (Float_t) m_csi[id]->GetIntegration(side);
            m_BRout.pt[id][side] = (Float_t) m_csi[id]->GetPeakTime(side);
            m_BRout.cft[id][side] = (Float_t) m_csi[id]->GetCFTime(side);
        }
        if(m_csi[id]->IsUsed(0) && m_csi[id]->IsUsed(1)) {
            m_BRout.TD[id] = m_csi[id]->GetTimeDiff();
            m_BRout.MT[id] = m_csi[id]->GetMeanTime();
            m_BRout.isHit[id] = m_csi[id]->IsHit();
        }
    }
}

// Initialization
bool CsIManager::Init() {
    m_isInit = 0;
    std::cout << "Initializing CsIManager -----------------------------------------\n";

    if(!Init_map()) return false;
    if(!Init_DAQconfig()) return false;

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

    std::cout << " map                         [OK]\n";
    return true;
}

bool CsIManager::Init_DAQconfig() {
    // ADC channels
    // find configuration files
    int fname_runID;
    for(int runID = m_runID; runID>-1; --runID) {
        std::stringstream ss;
        ss << "./data/ADCchMap/run" << runID << "_pmt.txt";
        std::string filename = ss.str();
        std::ifstream ifs(filename.c_str());
        if(ifs) {
            fname_runID = runID;
            break;
        }
        if(!runID) {
            std::cout << "Error: ADC channel map not found\n";
            return false;
        }
    }

    for(int side = 0; side < 2; side++) {
        std::stringstream ss;
        if(side == 0){
            ss << "./data/ADCchMap/run" << fname_runID << "_mppc.txt";
        }
        if(side == 1){
            ss << "./data/ADCchMap/run" << fname_runID << "_pmt.txt";
        }

        std::string filename = ss.str();
        std::ifstream ifs(filename.c_str());
        if (!ifs) {
            std::cout << filename << " not found\n";
            return false;
        } else {
            std::cout << filename << "\n";
        }

        int id, crate, mod, ch;
        while (ifs >> id >> crate >> mod >> ch) {
            m_csi[id]->SetADCconfig(side, crate, mod, ch);
        }
    }

    // Delays
    for(int id=0; id < nCSI; ++id) {
        for(int side=0; side < 2; ++side) {
            int delay = 15;
            if(m_csi[id]->IsUsed(side))
                m_csi[id]->SetDelay(side, delay);
        }
    }

    return true;
}

//    bool Init_calibConst();
//    bool Init_hitCondition();

void CsIManager::Process() {
    for(int id = 0; id < nCSI; ++id){
        m_csi[id]->Process();
    }
}

void CsIManager::RecHitPosition() {
    for(int id=0; id<nCSI; ++id) {
        double hitpos[3];
        hitpos[0] = m_csi[id]->GetPosition()[0];
        hitpos[1] = m_csi[id]->GetPosition()[1];
        hitpos[2] = (m_csi[id]->GetPosition()[1]-m_cosmi->GetTrack(1)[0])/m_cosmi->GetTrack(1)[1];
        m_csi[id]->SetHitpos(hitpos);
    }
}


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
        if(m_csi[id]->IsUsed(0)||m_csi[id]->IsUsed(1)) {
            usedCSI.push_back(m_csi[id]);
            continue;
        }
        m_csi[id]->Display(plane);
    }

    for(auto itr=usedCSI.begin(); itr!=usedCSI.end(); ++itr) {
        (*itr)->Display(plane);
    }

    m_canv->Update();
    m_canv->Modified();
}
