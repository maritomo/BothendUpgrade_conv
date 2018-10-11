//
// Created by Tomoo MARI on 2018/10/11.
//

#include <iostream>
#include "CosmicRay.h"

CosmicRay* CosmicRay::m_cosmi;
bool CosmicRay::m_isInit = 0;

CosmicRay* CosmicRay::GetInstance(){
    if(!m_isInit) {
        m_cosmi = new CosmicRay();
    }
    return m_cosmi;
}

void CosmicRay::Delete(){
    delete m_cosmi;
//    std::cout << "##### Class \"CosmicRay\" instance was deleted #####\n";
}

CosmicRay::CosmicRay() {
    Branch();
    m_BRout.trackID = 0;
    for(int plane=0; plane<3; ++plane) {
        for(int par=0; par<2; ++par) {
            m_track[plane][par] = 0;
        }
    }
    for(int layer=0; layer<2; ++layer) {
        m_hitTime[layer] = 0;
    }
    m_isInit = 1;
}

CosmicRay::~CosmicRay() {
    m_isInit = 0;
}

void CosmicRay::Branch() {
    m_tout->Branch("cosmi.trackID", &m_BRout.trackID, "cosmi.trackID/S");
    m_tout->Branch("cosmi.track", m_BRout.track, "cosmi.track[3][2]/F");
    m_tout->Branch("cosmi.hitTime", m_BRout.hitTime, "cosmi.hitTime[2]/F");
}

void CosmicRay::Fill() {
    m_BRout.trackID = (Short_t) m_trackID;
    for(int plane=0; plane<3; ++plane) {
        for(int par=0; par<2; ++par) {
            m_BRout.track[plane][par] = (Float_t) m_track[plane][par];
        }
    }
    for(int layer=0; layer<2; ++layer) {
        m_BRout.hitTime[layer] = (Float_t) m_hitTime[layer];
    }
}


// Visualization
void CosmicRay::Visualize() {
    for(int plane=0; plane < 3; ++plane) {
        m_trackLine[plane] = new TLine();
        m_trackLine[plane]->SetLineColor(kRed);
        m_trackLine[plane]->SetLineWidth(2);
    }
    m_isVis = 1;
}

void CosmicRay::Display(int plane) {
    if(!m_trackID) {
        std::cout << "No track\n";
        return;
    }

    int axis_h, axis_v;
    GetVisAxis(plane, axis_h, axis_v);

    double h[2], v[2]; // end point of track lines in the canvas
    for(int i = 0; i < 2; ++i) {
        h[i] = (m_world[axis_v][i] - m_cosmi->GetTrack(plane)[0]) / m_cosmi->GetTrack(plane)[1];
        if(h[i] < m_world[axis_h][0]) {
            h[i] = m_world[axis_h][0];
        }
        if(h[i] > m_world[axis_h][1]) {
            h[i] = m_world[axis_h][1];
        }
        v[i] = m_cosmi->GetTrack(plane)[1] * h[i] + m_cosmi->GetTrack(plane)[0];
    }

    m_trackLine[plane]->SetX1(h[0]);
    m_trackLine[plane]->SetX2(h[1]);
    m_trackLine[plane]->SetY1(v[0]);
    m_trackLine[plane]->SetY2(v[1]);
    m_trackLine[plane]->Draw("same");

    m_canv->Update();
    m_canv->Modified();

}
