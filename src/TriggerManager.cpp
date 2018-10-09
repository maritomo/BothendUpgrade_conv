//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include <TMath.h>
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
    for(int id=0; id<nCRC; ++id) {
        delete m_crc[id];
    }
//   for(int layer = 0; layer < nLayer; ++layer) {
//        for(int ch = 0; ch < nCRC; ++ch) {
//            delete m_crc[layer][ch];
//        }
//    }
}

bool TriggerManager::Init() {
    m_isInit = 0;
    std::cout << "\nInitializing TriggerManager...\n";

    if(!Init_map()) return false;
    if(!Init_DAQconfig()) return false;
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
    double pos[3];
    for(int id=0; id<nCRC; ++id) {
        ifs >> scintiID >> dir >> pos[0] >> pos[1] >> pos[2];
        m_crc[id] = new CosmicRayCounter(scintiID, dir, pos);
    }
//    for (int layer = 0; layer < nLayer; ++layer) {
//        for (int ch = 0; ch < nCRC; ++ch) {
//            double pos[3];
//            ifs >> scintiID >> dir >> pos[0] >> pos[1] >> pos[2];
//            m_crc[layer][ch] = new CosmicRayCounter(layer, ch, scintiID, dir, pos);
//        }
//    }
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

//    int slot, adcch, delay;
    int slot, ch, delay;
//    while(ifs >> slot >> adcch >> delay) {
    while(ifs >> slot >> ch >> delay) {
//        int layer = slot;
//        int ch = adcch / 2;
//        int side = adcch % 2;
        int id = GetID(slot, ch);
        int side = GetSide(slot, ch);
//        m_crc[layer][ch]->SetADCconfig(side, 3, slot, adcch);
//        m_crc[layer][ch]->SetDelay(side, delay);
        m_crc[id]->SetADCconfig(side, 3, slot, ch);
        m_crc[id]->SetDelay(side, delay);
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
            m_crc[GetID(scintiID)]->SetCalibConst(cc);
        }

//        int layer, ch;
//        if(GetLocationID(scintiID, layer, ch)) {
//            m_crc[layer][ch]->SetCalibConst(cc);
//        }
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

    int slot, ch;
    while(ifs >> slot >> ch >> m_commonThreshold) {
        m_crc[GetID(slot, ch)]->SetPeakThreshold(GetSide(slot, ch), m_commonThreshold);
    }
//    int slot, adcch;
//    int layer, ch, side;
//    while(ifs >> slot >> adcch >> m_commonThreshold) {
//        layer = slot;
//        ch = adcch / 2;
//        side = adcch % 2;
//        m_crc[layer][ch]->SetPeakThreshold(side, m_commonThreshold);
//    }
    ifs.close();

    filename = "./data/coincidence_range.txt";
    std::ifstream ifs2(filename.c_str());
    if(!ifs2) {
        std::cout << filename << " not found\n";
        return false;
    }

    double coin_range[2];
    while(ifs2 >> slot >> ch >> coin_range[0] >> coin_range[1]) {
        m_crc[GetID(slot, ch)]->SetCoinRange(GetSide(slot, ch), coin_range);
    }
//    while(ifs2 >> slot >> adcch >> coin_range[0] >> coin_range[1]) {
//        int layer = slot;
//        int ch = adcch / 2;
//        int side = adcch % 2;
//        m_crc[layer][ch]->SetCoinRange(side, coin_range);
//    }

    ifs2.close();

    std::cout << "Hit condition parameters        [OK]\n";
    return true;
}

void TriggerManager::Branch() {
    m_tout->Branch("trig.data", m_BRout.data, "trig.data[12][2][64]/S");
    m_tout->Branch("trig.ped", m_BRout.ped, "trig.ped[12][2]/F");
    m_tout->Branch("trig.peak", m_BRout.peak, "trig.peak[12][2]/F");
    m_tout->Branch("trig.integ", m_BRout.integ, "trig.integ[12][2]/F");
    m_tout->Branch("trig.pt", m_BRout.pt, "trig.pt[12][2]/F");
    m_tout->Branch("trig.cft", m_BRout.cft, "trig.cft[12][2]/F");

    m_tout->Branch("trig.TD", m_BRout.TD, "trig.TD[12]/F");
    m_tout->Branch("trig.MT", m_BRout.MT, "trig.MT[12]/F");

    m_tout->Branch("trig.isHit", m_BRout.isHit, "trig.isHit[12]/S");
    m_tout->Branch("trig.posHit", m_BRout.posHit, "trig.posHit[12]/F");

//    m_tout->Branch("trig.data", m_BRout.data, "trig.data[2][6][2][64]/S");
//    m_tout->Branch("trig.ped", m_BRout.ped, "trig.ped[2][6][2]/F");
//    m_tout->Branch("trig.peak", m_BRout.peak, "trig.peak[2][6][2]/F");
//    m_tout->Branch("trig.integ", m_BRout.integ, "trig.integ[2][6][2]/F");
//    m_tout->Branch("trig.pt", m_BRout.pt, "trig.pt[2][6][2]/F");
//    m_tout->Branch("trig.cft", m_BRout.cft, "trig.cft[2][6][2]/F");
//
//    m_tout->Branch("trig.TD", m_BRout.TD, "trig.TD[2][6]/F");
//    m_tout->Branch("trig.MT", m_BRout.MT, "trig.MT[2][6]/F");
//
//    m_tout->Branch("trig.isHit", m_BRout.isHit, "trig.isHit[2][6]/S");
//    m_tout->Branch("trig.posHit", m_BRout.posHit, "trig.posHit[2][6]/F");
    m_tout->Branch("trig.nHit", m_BRout.nHit, "trig.nHit[2]/S");
    m_tout->Branch("trig.TOF", &m_BRout.TOF, "TOF/F");

    m_tout->Branch("track", m_BRout.track, "track[3][2]/F");
    m_tout->Branch("trackID", &m_BRout.trackID, "trackID/S");

    m_tout->Branch("isOnlineHit", m_BRout.isOnlineHit, "isOnlineHit[12][2][64]/S");
//    m_tout->Branch("isOnlineHit", m_BRout.isOnlineHit, "isOnlineHit[2][6][2][64]/S");
    m_tout->Branch("nOnlineHit", m_BRout.nOnlineHit, "nOnlineHit[2][64]/S");
    m_tout->Branch("isOnlineTriggered", m_BRout.isOnlineTriggered, "isOnlineTriggered[64]/S");
}

// Process
void TriggerManager::Process() {
    for(int id=0; id<nCRC; ++id) {
        m_crc[id]->Process();
    }
//    for(int layer = 0; layer < nLayer; ++layer) {
//        for(int ch = 0; ch < nCRC; ++ch) {
//            m_crc[layer][ch]->Process();
//        }
//    }
    Tracking();
    OnlineHitDecision();
    Fill();
}

void TriggerManager::Tracking() {

    // Hit decision
    m_BRout.nHit[0] = 0;
    m_BRout.nHit[1] = 0;
    for(int id=0; id<nCRC; ++id) {
        if(m_crc[id]->IsHit()) {
            int layer = GetLayer(id);
            ++m_BRout.nHit[layer];
            m_BRout.hitID[layer] = id;
            m_crcHit[layer] = m_crc[id];
        }
    }
//    for(int layer = 0; layer < nLayer; ++layer) {
//        for(int ch = 0; ch < nCRC; ++ch) {
//            if(m_crc[layer][ch]->IsHit()) {
//                ++m_BRout.nHit[layer];
//                m_BRout.hitID[layer] = ch;
//            }
//        }
//    }

    // Tracking
    if(m_BRout.nHit[0]!=1 || m_BRout.nHit[1]!=1) {
        m_BRout.trackID = 0;
        return;
    }

//    double posHit[nLayer][3];
//    for(int layer = 0; layer < nLayer; ++layer) {
//        for(int axis = 0; axis < 3; ++axis) {
//            posHit[layer][axis] = m_crc[layer][m_BRout.hitID[layer]]->GetHitPosition(axis);
//        }
//    }

    m_BRout.trackID = m_BRout.hitID[1] * nCRC + m_BRout.hitID[0] + 1;
    m_BRout.TOF = Distance(m_crcHit[0], m_crcHit[1]) / TMath::C();
//m_BRout.TOF = sqrt(
//            (posHit[1][0] - posHit[0][0])*(posHit[1][0] - posHit[0][0]) +
//            (posHit[1][1] - posHit[0][1])*(posHit[1][1] - posHit[0][1]) +
//            (posHit[1][2] - posHit[0][2])*(posHit[1][2] - posHit[0][2])
//    )/TMath::C();

    for(int plane = 0; plane < 3; ++plane) {
        int axis_h, axis_v; // horizontal, vertical
        GetVisAxis(plane, axis_h, axis_v);
        m_BRout.track[plane][1] = (m_crcHit[1]->GetHitPosition(axis_v)-m_crcHit[0]->GetHitPosition(axis_v))
                                  /(m_crcHit[1]->GetHitPosition(axis_h)-m_crcHit[0]->GetHitPosition(axis_h));
        m_BRout.track[plane][0] = m_crcHit[1]->GetHitPosition(axis_v)
                                  - m_BRout.track[plane][1] * (m_crcHit[1]->GetHitPosition(axis_h));
//        m_BRout.track[plane][1] = (posHit[1][axis_v] - posHit[0][axis_v])
//                                  /(posHit[1][axis_h] - posHit[0][axis_h]);
//        m_BRout.track[plane][0] = posHit[1][axis_v] - m_BRout.track[plane][1] * posHit[1][axis_h];
    }

}

void TriggerManager::OnlineHitDecision() {
    for(int smpl = 0; smpl < 64; ++smpl) {
        m_BRout.isOnlineTriggered[smpl] = 0;

        for(int layer = 0; layer < 2; ++layer) {
            m_BRout.nOnlineHit[layer][smpl] = 0;
            for(int ch = 0; ch < nCRC; ++ch) {
                int id = GetID(layer, ch);
                for(int side = 0; side < 2; ++side) {
                    if(m_crc[id]->IsOnlineHit(side)[smpl]) {
                        ++m_BRout.nOnlineHit[layer][smpl];
                    }
                }
            }
        }
//        for(int layer = 0; layer < nLayer; ++layer) {
//            m_BRout.nOnlineHit[layer][smpl] = 0;
//
//            for(int ch = 0; ch < nCRC; ++ch) {
//                for(int side = 0; side < 2; ++side) {
//                    if(m_crc[layer][ch]->IsOnlineHit(side)[smpl]) {
//                        ++m_BRout.nOnlineHit[layer][smpl];
//                    }
//                }
//            }
//        }
        if(m_BRout.nOnlineHit[0][smpl] && m_BRout.nOnlineHit[1][smpl]) {
            m_BRout.isOnlineTriggered[smpl] = 1;
        }
    }
}

void TriggerManager::Fill(){
    for(int id=0; id<nCRC; ++id) {
        for(int side=0; side<2; ++side) {
            for(int smpl=0; smpl<64; ++smpl) {
                m_BRout.data[id][side][smpl] = m_crc[id]->GetData(side)[smpl];
            }
        }
    }
//    for(int layer=0; layer<nLayer; ++layer) {
//        for(int ch=0; ch<nCRC; ++ch) {
//            for(int side=0; side<2; ++side) {
//                for(int smpl=0; smpl<64; ++smpl) {
//                    m_BRout.data[layer][ch][side][smpl] = m_crc[layer][ch]->GetData(side)[smpl];
//                }
//            }
//        }
//    }
}

int TriggerManager::GetID(int scintiID) {
    for(int id=0; id<nCRC; ++id) {
        if(m_crc[id]->GetScintiID() == scintiID) {
            return id;
        }
    }
    std::cout << "(scinti. " << scintiID << " not found)\n";
    return -1;
}

//bool TriggerManager::GetLocationID(int scintiID, int& layerID, int& chID) {
//    for(int layer = 0; layer < nLayer; ++layer) {
//        for(int ch = 0; ch < nCRC; ++ch) {
//            if(m_crc[layer][ch]->GetScintiID()==scintiID) {
//                layerID = layer;
//                chID = ch;
//                return true;
//            }
//        }
//    }
//
//    std::cout << "(scinti. " << scintiID << " not found)\n";
//    return false;
//}


double TriggerManager::Distance(CosmicRayCounter* crc1, CosmicRayCounter* crc2) {
    return sqrt(
            pow((crc1->GetHitPosition(0)-crc2->GetHitPosition(0)), 2) +
            pow((crc1->GetHitPosition(1)-crc2->GetHitPosition(1)), 2) +
            pow((crc1->GetHitPosition(2)-crc2->GetHitPosition(2)), 2)
    );
}


// Visualization
void TriggerManager::Visualize() {
    for(int id=0; id<nCRC; ++id) {
        m_crc[id]->Visualize();
    }
//    for(int layer = 0; layer < nLayer; ++layer) {
//        for(int ch = 0; ch < nCRC; ++ch) {
//            m_crc[layer][ch]->Visualize();
//        }
//    }
    for(int plane=0; plane < 3; ++plane) {
        m_lTrack[plane] = new TLine();
        m_lTrack[plane]->SetLineColor(kRed);
        m_lTrack[plane]->SetLineWidth(2);
    }
    m_isVis = 1;
}

void TriggerManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }

    for(int id=0; id<nCRC; ++id) {
        m_crc[id]->Display(plane);
    }
//    for(int layer = 0; layer < nLayer; ++layer) {
//        for(int ch = 0; ch < nCRC; ++ch) {
//            m_crc[layer][ch]->Display(plane);
//        }
//    }
    m_canv->Update();
    m_canv->Modified();

    // Track
    if(m_BRout.trackID==0) return;

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
        h[i] = (m_world[axis_v][i] - m_BRout.track[plane][0]) / m_BRout.track[plane][1];

        if(h[i] < m_world[axis_h][0]) {
            h[i] = m_world[axis_h][0];
        }
        if(h[i] > m_world[axis_h][1]) {
            h[i] = m_world[axis_h][1];
        }

        v[i] = m_BRout.track[plane][1] * h[i] + m_BRout.track[plane][0];
    }

    m_lTrack[plane]->SetX1(h[0]);
    m_lTrack[plane]->SetX2(h[1]);
    m_lTrack[plane]->SetY1(v[0]);
    m_lTrack[plane]->SetY2(v[1]);
}
