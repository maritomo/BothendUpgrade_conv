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

void BothReadDetector::Reconstruct() {
    for(int side=0; side<2; ++side) {
        CalculateCFTime(side);
    }
    m_TD = m_cft[0] - m_cft[1];
    m_MT = (m_cft[0] + m_cft[1]) / 2;
}

void BothReadDetector::SetData(int side, int crate, int mod, int ch) {
    if( (side<0||side>1) || (crate<3||crate>5) ||
        (mod<0||mod>15) || (ch<0||ch>15) )
    {
        m_data[side] = new short[64];
        for(int smpl=0; smpl<64; ++smpl) {
            m_data[side][smpl] = 0;
        }
        m_isUsed[side] = 0;
        return;
    }

    m_crate[side] = crate;
    m_mod[side] = mod;
    m_ch[side] = ch;

    m_data[side] = m_BRin[crate-3].Data[mod][ch];
    m_isUsed[side] = 1;
}

void BothReadDetector::GetADCconfig(int side, int& crate, int& mod, int& ch) {
    crate = m_crate[side];
    mod = m_mod[side];
    ch = m_ch[side];
}

void BothReadDetector::CalculateCFTime(int side){
    m_ped[side] = 0;
    m_peak[side] = 0;
    m_integ[side] = 0;

    int nPed = 10;
    int ipeak = 0;

    int n = 64;

    int time[64];
    for(int i = 0; i < 64; ++i) time[i] = i;

    for(int i = 0; i < n; ++i) {
        if(i < nPed) {
            m_ped[side] += (float) m_data[side][i] / nPed;
        }
        if(m_peak[side] < m_data[side][i]) {
            m_peak[side] = m_data[side][i];
            ipeak = i;
        }
        if(i < n - 1) {
            m_integ[side] += m_data[side][i] * (time[i + 1] - time[i]);
        }
    }

    m_integ[side] -= m_ped[side] * (time[n - 1] - time[0]);

    float yb = m_data[side][ipeak - 1];
    float y = m_data[side][ipeak];
    float yf = m_data[side][ipeak + 1];

    m_pt[side] = ipeak - (yf - yb) / 2 / (yf + yb - 2 * y);
    m_pt[side] -= m_delay[side];
    m_pt[side] *= 8;

    m_peak[side] = y - (yf - yb) * (yf - yb) / 8 / (yf + yb - 2 * y);
    m_peak[side] -= m_ped[side];

    if(ipeak==0 || ipeak==n - 1) {
        m_cft[side] = 0;
        m_eflag[side] = 1;
        return;
    }

    float threshold = m_peak[side] / 2 + m_ped[side];
    for(int i = ipeak; i > 0; --i) {
        if(m_data[side][i] > threshold && m_data[side][i - 1] < threshold) {
            m_cft[side] = time[i - 1] + (float) (time[i] - time[i - 1]) /
                                        (m_data[side][i] - m_data[side][i - 1]) *
                                        (threshold - m_data[side][i]);
            m_cft[side] -= m_delay[side];
            m_cft[side] *= 8;
            m_eflag[side] = 0;
            return;
        }
    }

    m_eflag[side] = 2;
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

    if(!m_isUsed[0] && !m_isUsed[1]) m_color = kGray;

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis
    for(int plane=0; plane < 3; ++plane) {
        GetVisAxis(plane, axis_h, axis_v);
        m_box_det[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                    m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
        m_box_det[plane]->SetLineColor(m_color);
        m_box_det[plane]->SetFillStyle(0);

        m_box_hit[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                    m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
        m_box_hit[plane]->SetFillColor(m_color);
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