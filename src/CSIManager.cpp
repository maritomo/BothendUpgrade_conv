//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <TGraphErrors.h>

#include "TGraph.h"
#include "TF1.h"
#include "TMath.h"

#include "CSIManager.h"

CSIManager::CSIManager() {
    m_cosmic_ray = CosmicRay::GetInstance();

    if(!Init()) {
        std::cout << "Error : CSIManager initialization failed\n";
        m_isInit = false;
    } else {
        std::cout << "CSIManager initialized.\n";
        m_isInit = true;
    }
    m_isVis = 0;
}

CSIManager::~CSIManager() {
    for(int id=0; id < nCSI; ++id) {
        delete m_CSI[id];
    }
}


void CSIManager::SetBranchAddress(TChain* tin) {
    tin->SetBranchAddress("CSI.timeStamp", m_inBR.timeStamp);
    tin->SetBranchAddress("CSI.data", m_inBR.data);
}


void CSIManager::Branch(TTree* tout) {
    tout->Branch("CSI.timeStamp", m_outBR.timeStamp, "CSI.timeStamp[2716]/S");

    tout->Branch("CSI.data", m_outBR.data, "CSI.data[2716][2][64]/S");
    tout->Branch("CSI.ped", m_outBR.ped, "CSI.ped[2716][2]/F");
    tout->Branch("CSI.peak", m_outBR.peak, "CSI.peak[2716][2]/F");
    tout->Branch("CSI.sumADC", m_outBR.sumADC, "CSI.sumADC[2716][2]/F");
    tout->Branch("CSI.pt", m_outBR.pt, "CSI.pt[2716][2]/F");
    tout->Branch("CSI.cft", m_outBR.cft, "CSI.cft[2716][2]/F");
    tout->Branch("CSI.deltaT", m_outBR.deltaT, "CSI.deltaT[2716]/F");
    tout->Branch("CSI.meanT", m_outBR.meanT, "CSI.meanT[2716]/F");

    tout->Branch("CSI.isHit", m_outBR.isHit, "CSI.isHit[2716]/O");
    tout->Branch("CSI.nHit", &m_outBR.nHit, "CSI.nHit/S");
    tout->Branch("CSI.hitpos", m_outBR.hitpos, "CSI.hitpos[2716][3]/F");
    tout->Branch("CSI.localZ", m_outBR.localZ, "CSI.localZ[2716]/F");

    tout->Branch("CSI.Edep", m_outBR.Edep, "CSI.Edep[2716]/F");

    tout->Branch("CSI.track", m_outBR.track, "CSI.track[2]/F");
    tout->Branch("CSI.chi2", &m_outBR.chi2, "CSI.chi2/F");
    tout->Branch("CSI.ndf", &m_outBR.ndf, "CSI.ndf/I");
}

void CSIManager::Fill(){
    for(int id=0; id<nCSI; ++id) {
        for(int side=0; side<2; ++side) {
            for(int smpl=0; smpl<64; ++smpl) {
                m_outBR.data[id][side][smpl] = m_CSI[id]->GetData(side)[smpl];
            }
            m_outBR.ped[id][side] = m_CSI[id]->GetPedestal(side);
            m_outBR.peak[id][side] = m_CSI[id]->GetPeak(side);
            m_outBR.sumADC[id][side] = m_CSI[id]->GetIntegratedADC(side);
            m_outBR.pt[id][side] = m_CSI[id]->GetPeakTime(side);
            m_outBR.cft[id][side] = m_CSI[id]->GetCFTime(side);
        }

        if(m_CSI[id]->GetHybridID() != -1) {
            m_outBR.deltaT[id] = m_CSI[id]->GetDeltaT();
            m_outBR.meanT[id] = m_CSI[id]->GetMeanTime();
        }

        m_outBR.isHit[id] = m_CSI[id]->IsHit();
        m_outBR.nHit = m_nHit;
        for(int axis = 0; axis<3; ++axis) {
            m_outBR.hitpos[id][axis] = m_CSI[id]->GetHitPos()[axis];
        }
        m_outBR.localZ[id] = m_CSI[id]->GetHitPos()[2] - m_CSI[id]->GetZsurface();

        m_outBR.Edep[id] = m_CSI[id]->GetEnergyDeposit();
    }

    m_outBR.track[0] = m_track[0];
    m_outBR.track[1] = m_track[1];
    m_outBR.chi2 = m_chi2;
    m_outBR.ndf = m_ndf;
}


bool CSIManager::Init() {
    m_isInit = false;


    // CSI location map
    std::string filename = "data/map_csi.txt";
    std::ifstream ifs(filename.c_str());
    if (!ifs) {
        std::cout << "Error : " << filename << " not found.\n";
        return false;
    }

    int lineID, locID, crysID, size;
    double posx, posy;
    while(ifs >> lineID >> locID >> crysID >> posx >> posy >> size) {
        m_CSI[locID] = new CSI(locID, posx, posy, size);
    }

    ifs.close();
    ifs.clear();


    // surface z position
    filename = "data/z_surface_crystals201808.txt";
    ifs.open(filename.c_str());
    if (!ifs) {
        std::cout << "Error : " << filename << " not found.\n";
        return false;
    }

    double z_surface_rel, z_surface;
    while(ifs >> locID >> z_surface_rel >> z_surface) {
        m_CSI[locID]->SetZsurface(z_surface);
    }

    ifs.close();
    ifs.clear();


    // MPPC map
    filename = "data/sumIDaslocID_run79.txt";
//    filename = "data/sumIDaslocID.txt";
    ifs.open(filename.c_str());
    if (!ifs) {
        std::cout << "Error : " << filename << " not found.\n";
        return false;
    }

    int hybID;
    while(ifs >> locID >> hybID) {
        m_CSI[locID]->SetHybridID(hybID);
        if(hybID == -1) {
            m_CSI[locID]->SetSumID(-1);
        } else {
            m_CSI[locID]->SetSumID(hybID / 4);
        }
    }

    ifs.close();
    ifs.clear();


    // Set data to each CSI
    for(int id = 0; id < nCSI; ++id) {
        m_CSI[id]->SetData(1, m_inBR.data[id]); // PMT
        m_CSI[id]->SetData(0, m_inBR.data[id+m_CSI[id]->GetHybridID()]); // MPPC
    }

    // Deposit energy
//    filename = Form("data/CSIEdep/cali_csiEdep_run%d.txt", runID_head);
//    std::ifstream ifs(filename.Data());
//    if(!ifs) {
//        std::cout << "Warning : " << filename << " not found\n";
//        for(int id = 0; id<nCSI; ++id) {
//            m_CSI[id]->SetEdepCalibConst(0);
//        }
//    } else {
//        std::cout << "\t" << filename << "\n";
//        int id;
//        double cc;
//        while(ifs >> id >> cc) {
//            m_CSI[id]->SetEdepCalibConst(cc);
//        }
//    }
//    ifs.close();
//    ifs.clear();
//
//    // Energy Threshold
//    filename = Form("data/CSIEdep/EdepThreshold_run%d.txt", runID_head);
//    ifs.open(filename.Data());
//    if(!ifs) {
//        std::cout << "Warning : " << filename << " not found\n";
//        for(int id = 0; id<nCSI; ++id) {
//            double MIP = 30;
//            if(id<2240) MIP *= 0.5;
//            m_CSI[id]->SetEdepThreshold(0.5*MIP);
//        }
//    } else {
//        std::cout << "\t" << filename << "\n";
//        int id;
//        double threshold;
//        while(ifs >> id >> threshold) {
//            m_CSI[id]->SetEdepThreshold(threshold);
//        }
//    }
//    ifs.close();
//    ifs.clear();


    // Hit z position
    filename = "data/csi_zhit.txt";
    ifs.open(filename.c_str());
    if(!ifs) {
        std::cout << "Error : " << filename << " not found\n";
        return false;
    }

    while(ifs >> locID) {
        double z[36];
        for(int i = 0; i<36; ++i) {
            ifs >> z[i];
        }
        m_CSI[locID]->SetZhitTable(z);
    }


    m_isInit = true;
    return true;
}



// Processes
void CSIManager::Process() {
    Tracking();
    RecZhit();
}

void CSIManager::Tracking() {

    m_nHit = 0;
    TGraphErrors g;

    for(int id = 0; id < nCSI; ++id) {
        m_CSI[id]->Process();
        if(m_CSI[id]->IsHit()) {
            ++m_nHit;
            m_cosmic_ray->AddHitPoint(m_CSI[id]->GetHitPos(), m_CSI[id]->GetPosres());
            int i_pnt = g.GetN();
            g.SetPoint(i_pnt, m_CSI[id]->GetPos()[0], m_CSI[id]->GetPos()[1]);
            g.SetPointError(i_pnt, 0, m_CSI[id]->GetPosres()[1]);
        }
    }

    m_track[0] = 0;
    m_track[1] = 0;
    m_chi2 = 0;
    m_ndf = 0;

    if(m_nHit<2) return;


    // Tracking in x-y plane
    // if vertical track
    if(TMath::MaxElement(m_nHit, g.GetX()) == TMath::MinElement(m_nHit, g.GetX())) {
        m_track[1] = 1e10;
        m_track[0] = g.GetY()[0] - m_track[1] * g.GetX()[0];
    } else {
        g.Fit("pol1", "Q");
        TF1* f = g.GetFunction("pol1");
        m_track[0] = f->GetParameter(0);
        m_track[1] = f->GetParameter(1);
        m_chi2 = f->GetChisquare();
        m_ndf = f->GetNDF();
    }

}

void CSIManager::RecZhit() {
    for(int id = 0; id < nCSI; ++id) {
        if(m_CSI[id]->IsHit()) {
            int i_track = m_cosmic_ray->GetTrackID() - 1;
            m_CSI[id]->SetHitpos(2, m_CSI[id]->GetGlobalZhit()[i_track]);
        } else {
            m_CSI[id]->SetHitpos(2, 0);
        }
    }
}


// Visualization
void CSIManager::Visualize() {
    for(int i=0; i<nCSI; ++i) {
        m_CSI[i]->Visualize();
    }
    m_isVis = 1;
}

void CSIManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }

    std::vector<CSI*> usedCSI;
    for(int id=0; id<nCSI; ++id) {
        if(m_CSI[id]->GetHybridID() == -1) {
            usedCSI.push_back(m_CSI[id]);
            continue;
        }
        m_CSI[id]->Display(plane);
    }

    for(std::vector<CSI*>::iterator itr=usedCSI.begin(); itr!=usedCSI.end(); ++itr) {
        (*itr)->Display(plane);
    }

    m_canv->Update();
    m_canv->Modified();
}
