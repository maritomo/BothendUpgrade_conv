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
        std::cout << "TriggerManager initialization failed\n";
        m_isInit = 0;
    } else {
        std::cout << "TriggerManager initialized\n";
        m_isInit = 1;
    }
}

TriggerManager::~TriggerManager() {
    for(int id=0; id<nCRC; ++id) {
        delete m_trig[id];
    }
}

void TriggerManager::Branch() {
    m_tout->Branch("trig.data", m_BRout.data, "trig.data[12][2][64]/S");
    m_tout->Branch("trig.ped", m_BRout.ped, "trig.ped[12][2]/F");
    m_tout->Branch("trig.peak", m_BRout.peak, "trig.peak[12][2]/F");
    m_tout->Branch("trig.integ", m_BRout.integ, "trig.integ[12][2]/F");
    m_tout->Branch("trig.pt", m_BRout.pt, "trig.pt[12][2]/F");
    m_tout->Branch("trig.cft", m_BRout.cft, "trig.cft[12][2]/F");

    m_tout->Branch("trig.MT", m_BRout.MT, "trig.MT[12]/F");
    m_tout->Branch("trig.TD", m_BRout.TD, "trig.TD[12]/F");

    m_tout->Branch("trig.isHit", m_BRout.isHit, "trig.isHit[12]/O");
    m_tout->Branch("trig.hitpos", m_BRout.hitpos, "trig.hitpos[12]/F");
    m_tout->Branch("trig.nHit", m_BRout.nHit, "trig.nHit[2]/S");
    m_tout->Branch("trig.hitID", m_BRout.hitID, "trig.hitID[2]/S");

    m_tout->Branch("trig.TOF", &m_BRout.TOF, "trig.TOF/F");

//    m_tout->Branch("trig.isHit_online", m_BRout.isHit_online, "trig.isHit_online[12][2][64]/O");
//    m_tout->Branch("trig.isTriggered_online", m_BRout.isTriggered_online, "trig.isTriggered_online[64]/O");
//    m_tout->Branch("trig.nHit_online", m_BRout.nHit_online, "trig.nHit_online[2][64]/S");
}

void TriggerManager::Fill(){
    for(int id=0; id<nCRC; ++id) {
        for(int side=0; side<2; ++side) {
            for(int smpl=0; smpl<64; ++smpl) {
                m_BRout.data[id][side][smpl] = m_trig[id]->GetData(side)[smpl];
            }
            m_BRout.ped[id][side] = (Float_t) m_trig[id]->GetPedestal(side);
            m_BRout.peak[id][side] = (Float_t) m_trig[id]->GetPeak(side);
            m_BRout.integ[id][side] = (Float_t) m_trig[id]->GetIntegration(side);
            m_BRout.pt[id][side] = (Float_t) m_trig[id]->GetPeakTime(side);
            m_BRout.cft[id][side] = (Float_t) m_trig[id]->GetCFTime(side);
        }
        m_BRout.MT[id] = (Float_t) m_trig[id]->GetMeanTime();
        m_BRout.TD[id] = (Float_t) m_trig[id]->GetTimeDiff();

        m_BRout.isHit[id] = m_trig[id]->IsHit();
        for(int axis=0; axis<3; ++axis) {
            m_BRout.hitpos[id][axis] = (Float_t) m_trig[id]->GetHitPosition(axis);
        }

        for(int smpl=0; smpl<64; ++smpl) {
            m_BRout.isTriggered_online[smpl] = m_isTriggered_online[smpl];
            for(int side=0; side<2; ++side) {
                m_BRout.nHit_online[side][smpl] = m_nHit_online[side][smpl];
                for(int id=0; id<nCRC; ++id) {
                    m_BRout.isHit_online[id][side][smpl] = m_isHit_online[id][side][smpl];
                }
            }
        }
    }

    for(int layer=0; layer<2; ++layer) {
        m_BRout.nHit[layer] = (Short_t) m_nHit[layer];
        m_BRout.hitID[layer] = (Short_t) m_hitID[layer];
    }

    m_BRout.TOF = (Float_t) m_TOF;
}

// Initialization
bool TriggerManager::Init() {
    m_isInit = 0;
    std::cout << "Initializing TriggerManager ------------------------------------------\n";

    if(!Init_map()) return false;
    if(!Init_DAQconfig()) return false;
    if(!Init_calibConst()) return false;
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
    for(int id=0; id<nCRC; ++id) {
        ifs >> scintiID >> dir >> pos[0] >> pos[1] >> pos[2];
        m_trig[id] = new TriggerCounter(scintiID, dir, pos);
    }
    ifs.close();

    std::cout << "Trigger counter map             [OK]\n";
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
        m_trig[id]->SetADCconfig(side, 3, mod, ch);
        m_trig[id]->SetDelay(side, delay);
    }
    ifs.close();

    std::cout << "Channel delays                  [OK]\n";
    return true;
}

bool TriggerManager::Init_calibConst() {
    std::string filename = "./data/TDtoX.txt";
    std::ifstream ifs(filename.c_str());
    if(!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    for(int i = 0; i < 16; ++i) {
        int scintiID;
        double cc[2];
        ifs >> scintiID >> cc[0] >> cc[1];

        int id = GetID(scintiID);
        if(id >= 0) {
            m_trig[GetID(scintiID)]->SetCalibConst(cc);
        }
    }
    ifs.close();

    std::cout << "Calibration constants           [OK]\n";
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

    std::cout << "Hit condition parameters        [OK]\n";
    return true;
}


// Process
void TriggerManager::Process() {
    for(int id=0; id<nCRC; ++id) {
        m_trig[id]->Process();
    }
    Tracking();
    OnlineHitDecision();
}

void TriggerManager::Tracking() {
    // Hit decision
    m_nHit[0] = 0;
    m_nHit[1] = 0;
    for(int id=0; id<nCRC; ++id) {
        if(m_trig[id]->IsHit()) {
            int layer = GetLayer(id);
            ++m_nHit[layer];
            m_hitID[layer] = (Short_t) id;
            m_trigHit[layer] = m_trig[id];
        }
    }

    // Tracking
    // if no track
    if(m_nHit[0]!=1 || m_nHit[1]!=1) {
        m_cosmi->SetTrackID(0);
        double zero[2] = {};
        for(int plane=0; plane<3; ++plane) {
            m_cosmi->SetTrack(plane, zero);
        }
        return;
    }
    m_cosmi->SetTrackID((m_hitID[1]-nCRC/2) * nCRC/2 + m_hitID[0] + 1);
    m_TOF = (Float_t) (Distance(m_trigHit[0], m_trigHit[1]) / TMath::C());

    // if tracked
    for(int plane = 0; plane < 3; ++plane) {
        int axis_h, axis_v; // horizontal, vertical
        GetVisAxis(plane, axis_h, axis_v);
        double track[2];
        track[1] = (Float_t) ((m_trigHit[1]->GetHitPosition(axis_v)-m_trigHit[0]->GetHitPosition(axis_v))
                              / (m_trigHit[1]->GetHitPosition(axis_h)-m_trigHit[0]->GetHitPosition(axis_h)));
        track[0] = (Float_t) (m_trigHit[1]->GetHitPosition(axis_v)
                              - track[1] * (m_trigHit[1]->GetHitPosition(axis_h)));
        m_cosmi->SetTrack(plane, track);
    }

}

void TriggerManager::OnlineHitDecision() {
    for(int smpl = 0; smpl < 64; ++smpl) {
        m_isTriggered_online[smpl] = 0;

        for(int layer = 0; layer < 2; ++layer) {
            m_nHit_online[layer][smpl] = 0;
            for(int ch = 0; ch < nCRC; ++ch) {
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
    for(int id=0; id<nCRC; ++id) {
        if(m_trig[id]->GetScintiID()==scintiID) {
            return id;
        }
    }
    std::cout << "(scinti. " << scintiID << " not found)\n";
    return -1;
}

double TriggerManager::Distance(TriggerCounter* crc1, TriggerCounter* crc2) {
    return sqrt(
            pow((crc1->GetHitPosition(0)-crc2->GetHitPosition(0)), 2) +
            pow((crc1->GetHitPosition(1)-crc2->GetHitPosition(1)), 2) +
            pow((crc1->GetHitPosition(2)-crc2->GetHitPosition(2)), 2)
    );
}


// Visualization
void TriggerManager::Visualize() {
    for(int id=0; id<nCRC; ++id) {
        m_trig[id]->Visualize();
    }
}

void TriggerManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }

    for(int id=0; id<nCRC; ++id) {
        m_trig[id]->Display(plane);
    }
    m_canv->Update();
    m_canv->Modified();
}
