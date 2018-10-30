//
// Created by Tomoo MARI on 2018/10/11.
//

#include <iostream>
#include <TGraphErrors.h>
#include <TF1.h>
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
    m_isSingleTrack = 0;
    for(int plane=0; plane<3; ++plane) {
        for(int par=0; par<2; ++par) {
            m_track[plane][par] = 0;
        }
    }
    m_isInit = 1;
}

CosmicRay::~CosmicRay() {
    m_isInit = 0;
}

void CosmicRay::Branch() {
    m_eventTree->Branch("cosmi.track", m_BRout.track, "cosmi.track[3][2]/F");
    m_eventTree->Branch("cosmi.chi2", m_BRout.chi2, "cosmi.chi2[3]/F");
    m_eventTree->Branch("cosmi.ndf", m_BRout.ndf, "cosmi.ndf[3]/S");
}

void CosmicRay::Fill() {
    for(int plane=0; plane<3; ++plane) {
        for(int par=0; par<2; ++par) {
            m_BRout.track[plane][par] = (Float_t) m_track[plane][par];
        }
        m_BRout.chi2[plane] = (Float_t) m_chi2[plane];
        m_BRout.ndf[plane] = (Short_t) m_ndf[plane];
    }
}

void CosmicRay::SetTrackID(int trackID) {
    m_trackID = trackID;
    if(m_trackID==0) {
        m_isSingleTrack = false;
    } else {
        m_isSingleTrack = true;
    }
}

void CosmicRay::AddHitPoint(const double* pos, const double* dpos) {
    for(int axis=0; axis<3; ++axis) {
        m_hitpos[axis].push_back(pos[axis]);
        m_dhitpos[axis].push_back(dpos[axis]);
    }
}

void CosmicRay::AddHitPoint(int axis, double pos, double dpos) {
    m_hitpos[axis].push_back(pos);
    m_dhitpos[axis].push_back(dpos);
}

void CosmicRay::Tracking() {
    // Initiallization
    for(int plane=0; plane<3; ++plane) {
        for(int par = 0; par<2; ++par) {
            m_track[plane][par] = 0;
        }
        m_chi2[plane] = 0;
        m_ndf[plane] = 0;
    }

    if(!m_isSingleTrack) return;

    // Tracking
    for(int plane=0; plane<3; ++plane) {
        int axis_h, axis_v;
        GetVisAxis(plane, axis_h, axis_v);
        if(m_hitpos[axis_h].size()<2) continue;
        if(m_hitpos[axis_v].size()<2) continue;

        int N = (int)m_hitpos->size();
        std::vector<double> zero;
        for(int i = 0; i<N; ++i) zero.push_back(0);

        TGraphErrors g(N, &m_hitpos[axis_h][0], &m_hitpos[axis_v][0], &zero[0], &m_dhitpos[axis_v][0]);
        TF1 ffit("ffit", "pol1");
        g.Fit("ffit", "Q");

        m_track[plane][0] = ffit.GetParameter(0);
        m_track[plane][1] = ffit.GetParameter(1);
        m_chi2[plane] = ffit.GetChisquare();
        m_ndf[plane] = ffit.GetNDF();
    }

    // Clear
    for(int axis=0; axis<3; ++axis) {
        m_hitpos[axis].clear();
        m_dhitpos[axis].clear();
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
    if(!m_isSingleTrack) {
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
