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
    for(int plane=0; plane<3; ++plane) {
        for(int par=0; par<2; ++par) {
            m_track[plane][par] = 0;
        }
    }
    m_isSingleTrack = false;
    m_isInit = true;
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
    static int entry = -1;
    entry++;

    // Initiallization
    for(int plane=0; plane<3; ++plane) {
        for(int par = 0; par<2; ++par) {
            m_track[plane][par] = 0;
        }
        m_chi2[plane] = 0;
        m_ndf[plane] = 0;
    }

    if(!m_isSingleTrack) {
        Clear();
        return;
    }

    // Tracking
    for(int plane=0; plane<3; ++plane) {
        std::vector<double> h, v, dh, dv;

        int axis_h, axis_v;
        GetVisAxis(plane, axis_h, axis_v);

        std::vector<double>::iterator itr_h = m_hitpos[axis_h].begin();
        std::vector<double>::iterator itr_v = m_hitpos[axis_v].begin();
        std::vector<double>::iterator itr_dh = m_dhitpos[axis_h].begin();
        std::vector<double>::iterator itr_dv = m_dhitpos[axis_v].begin();

        for(; itr_h!=m_hitpos[axis_h].end(); ++itr_h, ++itr_v, ++itr_dh, ++itr_dv) {
            // Remove points outside the world
            if(*itr_h<m_world[axis_h][0] || *itr_h>m_world[axis_h][1] ||
               *itr_v<m_world[axis_v][0] || *itr_v>m_world[axis_v][1]) continue;
            // Remove CsI hit points in z-y plane
            if(plane==1 && *itr_v>-1000 && *itr_v<1000) continue;
            // Remove CsI hit points in x-z plane
            if(plane==2  && *itr_h>-1000 && *itr_h<1000) continue;

            h.push_back(*itr_h);
            v.push_back(*itr_v);
            dh.push_back(0);
            dv.push_back(*itr_dv);
        }


        if(h.size()<2) break;

        TGraphErrors g((int)h.size(), &h[0], &v[0], &dh[0], &dv[0]);
        TF1 ffit("ffit", "pol1");
        g.Fit("ffit", "Q");

        m_track[plane][0] = ffit.GetParameter(0);
        m_track[plane][1] = ffit.GetParameter(1);
        m_chi2[plane] = ffit.GetChisquare();
        m_ndf[plane] = ffit.GetNDF();
    }

    Clear();
}

void CosmicRay::Clear() {
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
