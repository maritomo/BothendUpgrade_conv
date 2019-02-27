//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include <TMath.h>
#include "COSMICManager.h"

COSMICManager::COSMICManager() {
    m_cosmic_ray = CosmicRay::GetInstance();
    if(!Init()) {
        std::cerr << "Error : COSMICManager initialization failed\n";
        m_isInit = 0;
    } else {
        std::cout << "COSMICManager initialized.\n";
        m_isInit = 1;
    }
}

COSMICManager::~COSMICManager() {
    for(int id = 0; id < nCOSMIC; ++id) {
        delete m_COSMIC[id];
    }
}

void COSMICManager::SetBranchAddress(TTree* tin) {
    tin->SetBranchAddress("COSMIC.timeStamp", m_inBR.timeStamp);
    tin->SetBranchAddress("COSMIC.data", m_inBR.data);
}

void COSMICManager::Branch(TTree* tout) {
    tout->Branch("COSMIC.data", m_outBR.data, "COSMIC.data[12][2][64]/S");
    tout->Branch("COSMIC.ped", m_outBR.ped, "COSMIC.ped[12][2]/F");
    tout->Branch("COSMIC.peak", m_outBR.peak, "COSMIC.peak[12][2]/F");
    tout->Branch("COSMIC.sumADC", m_outBR.sumADC, "COSMIC.sumADC[12][2]/F");
    tout->Branch("COSMIC.pt", m_outBR.pt, "COSMIC.pt[12][2]/F");
    tout->Branch("COSMIC.cft", m_outBR.cft, "COSMIC.cft[12][2]/F");
    tout->Branch("COSMIC.meanT", m_outBR.meanT, "COSMIC.meanT[12]/F");
    tout->Branch("COSMIC.deltaT", m_outBR.deltaT, "COSMIC.deltaT[12]/F");

    tout->Branch("COSMIC.isHit", m_outBR.isHit, "COSMIC.isHit[12]/O");
    tout->Branch("COSMIC.hitpos", m_outBR.hitpos, "COSMIC.hitpos[12][3]/F");
    tout->Branch("COSMIC.nHit", m_outBR.nHit, "COSMIC.nHit[2]/S");
    tout->Branch("COSMIC.hitID", m_outBR.hitID, "COSMIC.hitID[2]/S");

    tout->Branch("COSMIC.trackID", &m_outBR.trackID, "COSMIC.trackID/S");
    tout->Branch("COSMIC.track", m_outBR.track, "COSMIC.track[2]/F");
}

void COSMICManager::Fill() {
    for(int id = 0; id < nCOSMIC; ++id) {
        for(int side = 0; side < 2; ++side) {
            for(int smpl = 0; smpl < 64; ++smpl) {
                m_outBR.data[id][side][smpl] = m_COSMIC[id]->GetData(side)[smpl];
            }
            m_outBR.ped[id][side] = m_COSMIC[id]->GetPedestal(side);
            m_outBR.peak[id][side] = m_COSMIC[id]->GetPeak(side);
            m_outBR.sumADC[id][side] = m_COSMIC[id]->GetIntegratedADC(side);
            m_outBR.pt[id][side] = m_COSMIC[id]->GetPeakTime(side);
            m_outBR.cft[id][side] = m_COSMIC[id]->GetCFTime(side);
        }

        m_outBR.meanT[id] = m_COSMIC[id]->GetMeanTime();
        m_outBR.deltaT[id] = m_COSMIC[id]->GetDeltaT();

        m_outBR.isHit[id] = m_COSMIC[id]->IsHit();
        for(int axis = 0; axis < 3; ++axis) {
            m_outBR.hitpos[id][axis] = m_COSMIC[id]->GetHitPos()[axis];
        }
        for(int layer = 0; layer < 2; ++layer) {
            m_outBR.nHit[layer] = m_nHit[layer];
            m_outBR.hitID[layer] = m_hitID[layer];
        }

    }

    for(int layer = 0; layer < 2; ++layer) {
        m_outBR.nHit[layer] = m_nHit[layer];
        m_outBR.hitID[layer] = m_hitID[layer];
    }

    m_outBR.trackID = m_trackID;
    m_outBR.track[0] = m_track[0];
    m_outBR.track[1] = m_track[1];

}


bool COSMICManager::Init() {

    m_isInit = false;

    // Location map
    std::string filename = "data/map_cosmic.txt";
    std::ifstream ifs(filename.c_str());
    if(!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    int id, ch[2];
    double pos[3];
    while(ifs >> id >> ch[0] >> ch[1] >> pos[0] >> pos[1] >> pos[2]) {
        m_COSMIC[id] = new COSMIC(ch[0], ch[1], pos);
        m_COSMIC[id]->SetData(0, m_inBR.data[m_COSMIC[id]->GetChannel_south()]); // 0 : south
        m_COSMIC[id]->SetData(1, m_inBR.data[m_COSMIC[id]->GetChannel_north()]); // 1 : north
    }

    ifs.close();
    ifs.clear();


    // Hit x position
    filename = "data/cali_trigHitX.txt";
    ifs.open(filename.c_str());
    double cc[2] = {};
    if(!ifs) {
        std::cout << "Warning : " << filename << " not found\n";
        for(int id = 0; id < nCOSMIC; ++id) {
            m_COSMIC[id]->SetDeltaTtoX(cc);
        }
    } else {
        int id;
        while(ifs >> id >> cc[0] >> cc[1]) {
            m_COSMIC[id]->SetDeltaTtoX(cc);
        }
    }
    ifs.close();
    ifs.clear();



    // Mean time zero (cross timing)
    filename = "data/trig_crossTiming.txt";
    ifs.open(filename.c_str());
    if(!ifs) {
        std::cout << "Warning : " << filename << " not found\n";
        for(int id = 0; id < nCOSMIC; ++id) {
            m_COSMIC[id]->SetMeanTimeZero(0);
        }
    } else {
        int id;
        double t0;
        while(ifs >> id >> t0) {
            m_COSMIC[id]->SetMeanTimeZero(t0);
        }
    }
    ifs.close();
    ifs.clear();


    m_isInit = true;
    return true;
}


void COSMICManager::Process() {
    for(int id = 0; id < nCOSMIC; ++id) {
        m_COSMIC[id]->Process();
    }
    Tracking();
}

void COSMICManager::Tracking() {

    m_nHit[0] = 0;
    m_nHit[1] = 0;

    // Collect hit counters
    for(int id = 0; id < nCOSMIC; ++id) {
        if(m_COSMIC[id]->IsHit()) {
            int layerID = m_COSMIC[id]->GetLayerID();
            ++m_nHit[layerID];
            m_hitID[layerID] = id;
        }
    }

    if(m_nHit[0] != 1 || m_nHit[1] != 1) {
        // Tracking failed
        m_trackID = -1;
        m_track[0] = 0, m_track[1] = 0;
    } else {
        int i_top = m_hitID[1] - 6;
        int i_btm = m_hitID[0];
        m_trackID = 6 * i_top + i_btm;

        const double* hitpos_top = m_COSMIC[m_hitID[1]]->GetHitPos();
        const double* hitpos_btm = m_COSMIC[m_hitID[0]]->GetHitPos();
        m_track[1] = (hitpos_top[1] - hitpos_btm[1]) / (hitpos_top[0] - hitpos_btm[0]);
        m_track[0] = hitpos_top[1] - m_track[1] * hitpos_top[0];
    }

    // Add hit points into cosmic ray
    m_cosmic_ray->SetTrackID(m_trackID);
    for(int id = 0; id < nCOSMIC; ++id) {
        if(m_COSMIC[id]->IsHit()) {
            m_cosmic_ray->AddHitPoint(m_COSMIC[id]->GetHitPos(), m_COSMIC[id]->GetPosres());
        }
    }

}


double COSMICManager::Distance(COSMIC* c1, COSMIC* c2) {
    return sqrt(
            pow((c1->GetHitPos()[0] - c2->GetHitPos()[0]), 2) +
            pow((c1->GetHitPos()[1] - c2->GetHitPos()[1]), 2) +
            pow((c1->GetHitPos()[2] - c2->GetHitPos()[2]), 2)
    );
}

// Visualization
void COSMICManager::Visualize() {
    for(int id = 0; id < nCOSMIC; ++id) {
        m_COSMIC[id]->Visualize();
    }
    m_isVis = 1;
}

void COSMICManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }

    for(int id = 0; id < nCOSMIC; ++id) {
        m_COSMIC[id]->Display(plane);
    }
    m_canv->Update();
    m_canv->Modified();
}
