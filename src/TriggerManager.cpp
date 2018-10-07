//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include "TriggerManager.h"

TriggerManager::TriggerManager() {
    if(!Init()) {
        std::cout << "TriggerManager initialization failed\n";
        m_isInit = 0;
    } else {
        std::cout << "TriggerManager initialized\n";
        m_isInit = 1;
    }
}

TriggerManager::~TriggerManager() {
    for(int layer = 0; layer < nLayer; ++layer) {
        for(int ch = 0; ch < nCRC; ++ch) {
            delete m_crc[layer][ch];
        }
    }
}

bool TriggerManager::Init() {
    m_isInit = 0;
    std::cout << "\nInitializing TriggerManager...\n";

    if(!Init_map()) return false;
    if(!Init_channelDelay()) return false;
    if(!Init_calibConst()) return false;
    if(!Init_hitCondition()) return false;
    Branch();

    std::cout << "...fin\n";
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
    for (int layer = 0; layer < nLayer; ++layer) {
        for (int ch = 0; ch < nCRC; ++ch) {
            double pos[3];
            ifs >> scintiID >> dir >> pos[0] >> pos[1] >> pos[2];
            m_crc[layer][ch] = new CosmicRayCounter(layer, ch, scintiID, dir, pos);
        }
    }
    ifs.close();

    std::cout << "Trigger counter map             [OK]\n";
    return true;
}

bool TriggerManager::Init_channelDelay() {
    std::string filename = "./data/channel_delay.txt";
    std::ifstream ifs(filename.c_str());
    if(!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    int slot, adcch, delay;
    while(ifs >> slot >> adcch >> delay) {
        int layer = slot;
        int ch = adcch / 2;
        int side = adcch % 2;
        m_crc[layer][ch]->SetADCconfig(side, 3, slot, ch);
        m_crc[layer][ch]->SetDelay(side, delay);
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
        int layer, ch;
        if(GetLocationID(scintiID, layer, ch)) {
            m_crc[layer][ch]->SetCalibConst(cc);
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

    int slot, adcch;
    int layer, ch, side;
    while(ifs >> slot >> adcch >> m_commonThreshold) {
        layer = slot;
        ch = adcch / 2;
        side = adcch % 2;
        m_crc[layer][ch]->SetPeakThreshold(side, m_commonThreshold);
    }
    ifs.close();

    filename = "./data/coincidence_range.txt";
    std::ifstream ifs2(filename.c_str());
    if(!ifs2) {
        std::cout << filename << " not found\n";
        return false;
    }

    double coin_range[2];
    while(ifs2 >> slot >> adcch >> coin_range[0] >> coin_range[1]) {
        int layer = slot;
        int ch = adcch / 2;
        int side = adcch % 2;
        m_crc[layer][ch]->SetCoinRange(side, coin_range);
    }

    ifs2.close();

    std::cout << "Hit condition parameters        [OK]\n";
    return true;
}

void TriggerManager::Branch() {
    m_tout->Branch("trig.data", m_BRtrig.data, "trig.data[2][6][2][64]/S");
    m_tout->Branch("trig.ped", m_BRtrig.ped, "trig.ped[2][6][2]/F");
    m_tout->Branch("trig.peak", m_BRtrig.peak, "trig.peak[2][6][2]/F");
    m_tout->Branch("trig.integ", m_BRtrig.integ, "trig.integ[2][6][2]/F");
    m_tout->Branch("trig.pt", m_BRtrig.pt, "trig.pt[2][6][2]/F");
    m_tout->Branch("trig.cft", m_BRtrig.cft, "trig.cft[2][6][2]/F");

    m_tout->Branch("trig.TD", m_BRtrig.TD, "trig.TD[2][6]/F");
    m_tout->Branch("trig.MT", m_BRtrig.MT, "trig.MT[2][6]/F");

    m_tout->Branch("trig.isHit", m_BRtrig.isHit, "trig.isHit[2][6]/S");
    m_tout->Branch("trig.hitpos", m_BRtrig.hitpos, "trig.hitpos[2][6]/F");

    m_tout->Branch("trig.nHit", m_BRtrig.nHit, "trig.nHit[2]/S");
    m_tout->Branch("trig.hitpos", m_BRtrig.hitpos, "trig.hitpos[2][3]/F");
    m_tout->Branch("trig.TOF", &m_BRtrig.TOF, "TOF/F");

    m_tout->Branch("track", m_BRtrig.track, "track[3][2]/F");
    m_tout->Branch("trackID", &m_BRtrig.trackID, "trackID/S");

    m_tout->Branch("isOnlineHit", m_BRtrig.isOnlineHit, "isOnlineHit[2][6][2][64]/S");
    m_tout->Branch("nOnlineHit", m_BRtrig.nOnlineHit, "nOnlineHit[2][64]/S");
    m_tout->Branch("isOnlineTriggered", m_BRtrig.isOnlineTriggered, "isOnlineTriggered[64]/S");
}

// Process
void TriggerManager::Process() {
}

// Getter
bool TriggerManager::GetLocationID(int scintiID, int& layerID, int& chID) {
    for(int layer = 0; layer < nLayer; ++layer) {
        for(int ch = 0; ch < nCRC; ++ch) {
            if(m_crc[layer][ch]->GetScintiID()==scintiID) {
                layerID = layer;
                chID = ch;
                return true;
            }
        }
    }

    std::cout << "(scinti. " << scintiID << " not found)\n";
    return false;
}


// Visualization
void TriggerManager::Visualize() {
    for(int layer = 0; layer < nLayer; ++layer) {
        for(int ch = 0; ch < nCRC; ++ch) {
            m_crc[layer][ch]->Visualize();
        }
    }
    for(int plane=0; plane < 3; ++plane) {
        m_lTrack[plane] = new TLine();
        m_lTrack[plane]->SetLineColor(kRed);
    }
    m_isVis = 1;
}

void TriggerManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }

    for(int layer = 0; layer < nLayer; ++layer) {
        for(int ch = 0; ch < nCRC; ++ch) {
            m_crc[layer][ch]->Display(plane);
        }
    }
    m_canv->Update();
    m_canv->Modified();

    // Track
    if(m_BRtrig.trackID==0) return;

    SetTrackLine(plane);
    m_lTrack[plane]->Draw("same");
    m_canv->Update();
    m_canv->Modified();
}

void TriggerManager::SetTrackLine(int plane) {

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis
    GetVisAxis(plane, axis_h, axis_v);

    double h[2], v[2];
    for(int i = 0; i < 2; ++i) {
        h[i] = (m_world[axis_v][i] - m_BRtrig.track[plane][0]) / m_BRtrig.track[plane][1];

        if(h[i] < m_world[axis_h][0]) {
            h[i] = m_world[axis_h][0];
        }
        if(h[i] > m_world[axis_h][1]) {
            h[i] = m_world[axis_h][1];
        }

        v[i] = m_BRtrig.track[plane][1] * h[i] + m_BRtrig.track[plane][0];
    }

    m_lTrack[plane]->SetX1(h[0]);
    m_lTrack[plane]->SetX2(h[1]);
    m_lTrack[plane]->SetY1(v[0]);
    m_lTrack[plane]->SetY2(v[1]);
}
