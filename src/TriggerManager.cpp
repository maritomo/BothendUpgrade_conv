//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include <TMath.h>
#include "TriggerManager.h"

TriggerManager::TriggerManager() {
    m_cosmi = CosmicRay::GetInstance();
    Branch();

    if(!Init()) {
        std::cout << "[Error] TriggerManager initialization failed\n";
        m_isInit = 0;
    } else {
        m_isInit = 1;
    }
}

TriggerManager::~TriggerManager() {
    for(int id=0; id<nTrig; ++id) {
        delete m_trig[id];
    }
}

void TriggerManager::Branch() {
    m_eventTree->Branch("trig.data", m_BRout.data, "trig.data[12][2][64]/S");
    m_eventTree->Branch("trig.ped", m_BRout.ped, "trig.ped[12][2]/F");
    m_eventTree->Branch("trig.peak", m_BRout.peak, "trig.peak[12][2]/F");
    m_eventTree->Branch("trig.sumADC", m_BRout.sumADC, "trig.sumADC[12][2]/F");
    m_eventTree->Branch("trig.pt", m_BRout.pt, "trig.pt[12][2]/F");
    m_eventTree->Branch("trig.cft", m_BRout.cft, "trig.cft[12][2]/F");

    m_eventTree->Branch("trig.MT", m_BRout.MT, "trig.MT[12]/F");
    m_eventTree->Branch("trig.TD", m_BRout.TD, "trig.TD[12]/F");

    m_eventTree->Branch("trig.isHit", m_BRout.isHit, "trig.isHit[12]/O");
    m_eventTree->Branch("trig.hitpos", m_BRout.hitpos, "trig.hitpos[12][3]/F");
    m_eventTree->Branch("trig.nHit", m_BRout.nHit, "trig.nHit[2]/S");
    m_eventTree->Branch("trig.hitID", m_BRout.hitID, "trig.hitID[2]/S");

    m_eventTree->Branch("trig.trackID", &m_BRout.trackID, "trig.trackID/S");

    // Online hit decision algorithm
//    m_eventTree->Branch("trig.isHit_online", m_BRout.isHit_online, "trig.isHit_online[12][2][64]/O");
//    m_eventTree->Branch("trig.isTriggered_online", m_BRout.isTriggered_online, "trig.isTriggered_online[64]/O");
//    m_eventTree->Branch("trig.nHit_online", m_BRout.nHit_online, "trig.nHit_online[2][64]/S");
}

void TriggerManager::Fill(){
    for(int id=0; id<nTrig; ++id) {
        for(int side=0; side<2; ++side) {
            for(int smpl=0; smpl<64; ++smpl) {
                m_BRout.data[id][side][smpl] = m_trig[id]->GetData(side)[smpl];
            }
            m_BRout.ped[id][side] = (Float_t) m_trig[id]->GetPedestal(side);
            m_BRout.peak[id][side] = (Float_t) m_trig[id]->GetPeak(side);
            m_BRout.sumADC[id][side] = (Float_t) m_trig[id]->GetIntegratedADC(side);
            m_BRout.pt[id][side] = (Float_t) m_trig[id]->GetPeakTime(side);
            m_BRout.cft[id][side] = (Float_t) m_trig[id]->GetCFTime(side);
        }

        m_BRout.MT[id] = (Float_t) m_trig[id]->GetMeanTime();
        m_BRout.TD[id] = (Float_t) m_trig[id]->GetTimeDiff();

        m_BRout.isHit[id] = m_trig[id]->IsHit();
        for(int axis=0; axis<3; ++axis) {
            m_BRout.hitpos[id][axis] = (Float_t) m_trig[id]->GetHitPos()[axis];
        }
        for(int layer=0; layer<2; ++layer) {
            m_BRout.nHit[layer] = (Short_t) m_nHit[layer];
            m_BRout.hitID[layer] =(Short_t)  m_hitID[layer];
        }

        m_BRout.trackID = (Short_t) m_trackID;

        for(int smpl=0; smpl<64; ++smpl) {
            m_BRout.isTriggered_online[smpl] = m_isTriggered_online[smpl];
            for(int side=0; side<2; ++side) {
                m_BRout.nHit_online[side][smpl] = (Short_t) m_nHit_online[side][smpl];
                for(int id=0; id<nTrig; ++id) {
                    m_BRout.isHit_online[id][side][smpl] = m_isHit_online[id][side][smpl];
                }
            }
        }

    }

    for(int layer=0; layer<2; ++layer) {
        m_BRout.nHit[layer] = (Short_t) m_nHit[layer];
        m_BRout.hitID[layer] = (Short_t) m_hitID[layer];
    }

}

// Initialization
bool TriggerManager::Init() {
    m_isInit = 0;
    std::cout << "Initializing TriggerManager ------------------------------------------\n";

    if(!Init_map()) return false;
    if(!Init_DAQconfig()) return false;
    if(!Init_calibration()) return false;
    if(!Init_hitCondition()) return false;

    std::cout << "----------------------------------------------------------------------\n";
    m_isInit = 1;
    return true;
}

bool TriggerManager::Init_map() {
    std::string filename = "./data/map_crc.txt";
    std::ifstream ifs(filename.c_str());
    if (!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    int scintiID, dir;
    double pos[3];
    for(int id=0; id<nTrig; ++id) {
        ifs >> scintiID >> dir >> pos[0] >> pos[1] >> pos[2];
        m_trig[id] = new TriggerCounter(scintiID, dir, pos);
    }
    ifs.close();

    std::cout << "* Location map                  [OK]\n";
    return true;
}

bool TriggerManager::Init_DAQconfig(){
    std::string filename = "./data/channel_delay.txt";
    std::ifstream ifs(filename.c_str());
    if(!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    int mod, ch, delay;
    while(ifs >> mod >> ch >> delay) {
        int id = GetID(mod, ch);
        int side = GetSide(mod, ch);
        m_trig[id]->SetData(side, 3, mod, ch);
        m_trig[id]->SetDelay(side, delay);
    }
    ifs.close();

    std::cout << "* Channel delays                [OK]\n";
    return true;
}

bool TriggerManager::Init_calibration() {
    // Hit x position
    std::string filename = "./data/cali_trigHitX.txt";
    std::ifstream ifs(filename.c_str());
    double cc[2] = {};
    if(!ifs) {
        std::cout << "\t[Warning] " <<  filename << " not found\n";
        for(int id=0; id<nTrig; ++id) {
            m_trig[id]->SetHitXparams(cc);
        }
    } else {
        int id;
        while(ifs >> id >> cc[0] >> cc[1]) {
            m_trig[id]->SetHitXparams(cc);
        }
    }
    ifs.close();
    ifs.clear();

    // Timing-zero of mean time
    filename = "data/trig_crossTiming.txt";
    ifs.open(filename.c_str());
    if(!ifs) {
        std::cout << "\t[Warning] " << filename << " not found\n";
        for(int id=0; id<nTrig; ++id) {
            m_trig[id]->SetMeanTimeZero(0);
        }
    } else {
        int id; double t0;
        while(ifs >> id >> t0) {
            m_trig[id]->SetMeanTimeZero(t0);
        }
    }

    std::cout << "* Calibration constants         [OK]\n";
    return true;
}

bool TriggerManager::Init_hitCondition() {
    std::string filename = "./data/common_threshold.txt";
    std::ifstream ifs(filename.c_str());
    if(!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    int mod, ch;
    while(ifs >> mod >> ch >> m_commonThreshold) {
        m_trig[GetID(mod, ch)]->SetPeakThreshold(GetSide(mod, ch), m_commonThreshold);
    }
    ifs.close();

    filename = "./data/coincidence_range.txt";
    std::ifstream ifs2(filename.c_str());
    if(!ifs2) {
        std::cout << filename << " not found\n";
        return false;
    }

    double coin_range[2];
    while(ifs2 >> mod >> ch >> coin_range[0] >> coin_range[1]) {
        m_trig[GetID(mod, ch)]->SetCoinRange(GetSide(mod, ch), coin_range);
    }
    ifs2.close();

    std::cout << "* Hit condition parameters      [OK]\n";
    return true;
}


// Process
void TriggerManager::Process() {
    for(int id=0; id<nTrig; ++id) {
        m_trig[id]->Process();
    }
    Tracking();
    OnlineHitDecision();
}

void TriggerManager::Tracking() {
    // Hit decision
    m_nHit[0] = 0;
    m_nHit[1] = 0;
    for(int id=0; id<nTrig; ++id) {
        if(m_trig[id]->IsHit()) {
            int layer = GetLayer(id);
            ++m_nHit[layer];
            m_hitID[layer] = (Short_t) id;
        }
    }

    // TrackID
    if(m_nHit[0]!=1 || m_nHit[1]!=1) {
        m_trackID = 0; // No track
    } else {
        int N = nTrig / 2; // = 6, # of trigger counters in each layer
        int i_top = m_hitID[1] - N; // 0-5, from upstream
        int i_btm = m_hitID[0];     // 0-5
        m_trackID = (Short_t) (i_top * N + i_btm + 1);
    }

    // Add hit points into cosmic ray
    m_cosmi->SetTrackID(m_trackID);
    for(int id=0; id<nTrig; ++id) {
        if(m_trig[id]->IsHit()) {
            m_cosmi->AddHitPoint(m_trig[id]->GetHitPos(), m_trig[id]->GetPosres());
        }
    }

}

void TriggerManager::OnlineHitDecision() {
    for(int smpl = 0; smpl < 64; ++smpl) {
        m_isTriggered_online[smpl] = 0;

        for(int layer = 0; layer < 2; ++layer) {
            m_nHit_online[layer][smpl] = 0;
            for(int ch = 0; ch < nTrig; ++ch) {
                int id = GetID(layer, ch);
                for(int side = 0; side < 2; ++side) {
                    if(m_trig[id]->IsOnlineHit(side)[smpl]) {
                        ++m_nHit_online[layer][smpl];
                    }
                }
            }
        }
        if(m_nHit_online[0][smpl] && m_nHit_online[1][smpl]) {
            m_isTriggered_online[smpl] = 1;
        }
    }
}

// Getter
int TriggerManager::GetID(int scintiID) {
    for(int id=0; id<nTrig; ++id) {
        if(m_trig[id]->GetScintiID()==scintiID) {
            return id;
        }
    }
    return -1;
}

double TriggerManager::Distance(TriggerCounter* crc1, TriggerCounter* crc2) {
    return sqrt(
            pow((crc1->GetHitPos()[0]-crc2->GetHitPos()[0]), 2) +
            pow((crc1->GetHitPos()[1]-crc2->GetHitPos()[1]), 2) +
            pow((crc1->GetHitPos()[2]-crc2->GetHitPos()[2]), 2)
    );
}

// Visualization
void TriggerManager::Visualize() {
    for(int id=0; id<nTrig; ++id) {
        m_trig[id]->Visualize();
    }
    m_isVis = 1;
}

void TriggerManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }

    for(int id=0; id<nTrig; ++id) {
        m_trig[id]->Display(plane);
    }
    m_canv->Update();
    m_canv->Modified();
}
