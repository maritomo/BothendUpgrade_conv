//
// Created by Tomoo MARI on 2018/09/20.
//

#include <TString.h>
#include <iostream>
#include "BothendReadoutDetector.h"

BothendReadoutDetector::~BothendReadoutDetector() {
    for(int plane = 0; plane < 3; ++plane) {
        if(!m_isVis[plane]) continue;
        delete m_box_det[plane];
        delete m_box_hit[plane];
    }
}

void BothendReadoutDetector::SetData(int side, const short* data) {
    if(side!=0 && side!=1) {
        std::cout << "Channel should be 0 or 1 ! Data were not set\n";
        return;
    }
    GetCFTime(side, data);
}

void BothendReadoutDetector::GetCFTime(int side, const short* data) {

    m_ped[side] = 0;
    m_peak[side] = 0;
    m_integ[side] = 0;
    m_errflag[side] = 1;

    int nPed = 10;
    int ipeak = 0;

    int n = 64;

    int time[64];
    for(int i = 0; i < 64; ++i) time[i] = i;

    for(int i = 0; i < n; ++i) {
        if(i < nPed)
            m_ped[side] += (float) data[i] / nPed;
        if(m_peak[side] < data[i]) {
            m_peak[side] = data[i];
            ipeak = i;
        }
        if(i < n - 1)
            m_integ[side] += data[i] * (time[i + 1] - time[i]);
    }

    m_integ[side] -= m_ped[side] * (time[n - 1] - time[0]);

    float yb = data[ipeak - 1];
    float y = data[ipeak];
    float yf = data[ipeak + 1];

    m_pt[side] = ipeak - (yf - yb) / 2 / (yf + yb - 2 * y);
    m_peak[side] = y - (yf - yb) * (yf - yb) / 8 / (yf + yb - 2 * y);
    m_peak[side] -= m_ped[side];

    if(ipeak==0 || ipeak==n - 1) {
        m_cft[side] = 404;
        return;
    }

    float threshold = m_peak[side] / 2 + m_ped[side];

    for(int i = ipeak; i > 0; --i) {
        if(data[i] > threshold && data[i - 1] < threshold) {
            m_cft[side] = time[i - 1] + (float) (time[i] - time[i - 1]) / (data[i] - data[i - 1]) * (threshold - data[i]);
            m_errflag[side] = 0;
            return;
        }
    }

}

/*
 * Visuallization
 */

void BothendReadoutDetector::GetVisAxis(int plane, int& axis_h, int& axis_v) {
    if(plane==0) {          // xy plane -> (x, y)
        axis_h = 0;
        axis_v = 1;
    } else if(plane==1) {   // yz plane -> (z, y)
        axis_h = 2;
        axis_v = 1;
    } else if(plane==2) {   // zx plane -> (x, z)
        axis_h = 0;
        axis_v = 2;
    } else {
        std::cout << "plane: 0 (xy), 1 (yz), 2 (zx)\n";
    }
}

void BothendReadoutDetector::Visualize(int plane) {

    if(plane!=0 && plane!=1 && plane!=2) {
        std::cout << "plane: 0 (xy), 1 (yz), 2 (zx)\n";
        m_isVis[plane] = 0;
    }

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis

    GetVisAxis(plane, axis_h, axis_v);

    m_box_det[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
    m_box_det[plane]->SetLineColor(m_col);
    m_box_det[plane]->SetFillStyle(0);

    m_box_hit[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
    m_box_hit[plane]->SetFillColor(m_col);

    m_isVis[plane] = 1;
}

void BothendReadoutDetector::Display(int plane) {

    if(!m_isVis[plane]) return;

    m_box_det[plane]->Draw("same");

    if(!m_isHit) return;

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis

    GetVisAxis(plane, axis_h, axis_v);

    m_box_hit[plane]->SetX1(m_hitpos[axis_h] - 0.5 * m_posres[axis_h]);
    m_box_hit[plane]->SetX2(m_hitpos[axis_h] + 0.5 * m_posres[axis_h]);
    m_box_hit[plane]->SetY1(m_hitpos[axis_v] - 0.5 * m_posres[axis_v]);
    m_box_hit[plane]->SetY2(m_hitpos[axis_v] + 0.5 * m_posres[axis_v]);

    m_box_hit[plane]->Draw("same");
}