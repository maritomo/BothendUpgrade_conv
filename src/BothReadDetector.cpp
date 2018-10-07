//
// Created by Tomoo MARI on 2018/09/20.
//

#include <TString.h>
#include <iostream>
#include <cmath>

#include "BothReadDetector.h"

BothReadDetector::~BothReadDetector() {
    for(int plane = 0; plane < 3; ++plane) {
        delete m_box_det[plane];
        delete m_box_hit[plane];
    }
}

void BothReadDetector::SetADCconfig(int side, int crate, int slot, int ch) {
    int flag = 0;

    if(side!=0 && side!=1) flag = 1;
    if(crate<3 || crate > 5) flag = 1;
    if(slot<0 || slot > 15) flag = 1;
    if(ch<0 || ch>15) flag = 1;

    if(flag) {
        std::cout << "Error: FADC configuration is outside the range\n";
        m_isUsed = 0;
        return;
    }

    m_crate[side] = crate;
    m_slot[side] = slot;
    m_ch[side] = ch;
    m_pdata[side] = m_BRin[crate-3].Data[16][16];
    m_isUsed = 1;
}

void BothReadDetector::GetADCconfig(int side, int& crate, int& slot, int& ch) {
    crate = m_crate[side];
    slot = m_slot[side];
    ch = m_ch[side];
}

void BothReadDetector::SetData(int side) {
    for(int smpl = 0; smpl < 64; ++smpl)
        m_data[side][smpl] =m_BRin[m_crate[side]].Data[m_slot[side]][m_ch[side]][smpl];
    GetCFTime(side);

    m_pt[side] += 15 - m_delay[side];
    m_cft[side] += 15 - m_delay[side];
}

void BothReadDetector::GetCFTime(int side) {

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

            m_ped[side] += (float) m_data[side][i] / nPed;
        if(m_peak[side] < m_data[side][i]) {
            m_peak[side] = m_data[side][i];
            ipeak = i;
        }
        if(i < n - 1)
            m_integ[side] += m_data[side][i] * (time[i + 1] - time[i]);
    }

    m_integ[side] -= m_ped[side] * (time[n - 1] - time[0]);

    float yb = m_data[side][ipeak - 1];
    float y = m_data[side][ipeak];
    float yf = m_data[side][ipeak + 1];

    m_pt[side] = ipeak - (yf - yb) / 2 / (yf + yb - 2 * y);
    m_peak[side] = y - (yf - yb) * (yf - yb) / 8 / (yf + yb - 2 * y);
    m_peak[side] -= m_ped[side];

    if(ipeak==0 || ipeak==n - 1) {
        m_cft[side] = 404;
        return;
    }

    float threshold = m_peak[side] / 2 + m_ped[side];

    for(int i = ipeak; i > 0; --i) {
        if(m_data[side][i] > threshold && m_data[side][i - 1] < threshold) {
            m_cft[side] = time[i - 1] + (float) (time[i] - time[i - 1]) / (m_data[side][i] - m_data[side][i - 1]) * (threshold - m_data[side][i]);
            m_errflag[side] = 0;
            return;
        }
    }
}

short BothReadDetector::GetMax(int nSmpl, const short* data) {
    short max = -1 * (pow(2, 15) - 1);
    for(int i = 0; i < nSmpl; ++i) {
        if(max < data[i]) max = data[i];
    }
    return max;
}

/*
 * Visuallization
 */

void BothReadDetector::Visualize() {
    if(m_isVis) {
        std::cout << "Already visualized\n";
        return;
    }

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis
    for(int plane=0; plane < 3; ++plane) {
        GetVisAxis(plane, axis_h, axis_v);
        m_box_det[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                    m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
        m_box_det[plane]->SetLineColor(m_col);
        m_box_det[plane]->SetFillStyle(0);

        m_box_hit[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                    m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
        m_box_hit[plane]->SetFillColor(m_col);
    }

    m_isVis = 1;
}

void BothReadDetector::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
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