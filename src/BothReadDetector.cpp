//
// Created by Tomoo MARI on 2018/09/20.
//

#include <iostream>
#include <cmath>

#include <TGraph.h>
#include <TString.h>

#include "BothReadDetector.h"


BothReadDetector::~BothReadDetector() {
    for(int plane = 0; plane < 3; ++plane) {
        delete m_box_det[plane];
        delete m_box_hit[plane];
    }
}

void BothReadDetector::Reconstruct() {
    for(int side = 0; side < 2; ++side) {
        CalCFTime(side);
    }
    m_deltaT = m_cft[0] - m_cft[1];
    m_meanT = 0.5 * (m_cft[0] + m_cft[1]);
}


bool BothReadDetector::CalCFTime(int side) {

    m_ped[side] = 0;
    m_peak[side] = 0;
    m_sumADC[side] = 0;
    m_peak[side] = 0;
    m_peakTime[side] = 0;
    m_cft[side] = 0;

    int i_peak = 0;

    const int N = 64;
    const int Nped = 5;

    for(int i = 1; i < N; ++i) {
        if(i < 1+Nped) m_ped[side] += (Float_t)m_data[side][i] / Nped;
        if(m_peak[side] < m_data[side][i]) {
            m_peak[side] = m_data[side][i];
            i_peak = i;
        }
        m_sumADC[side] += m_data[side][i];
    }
    m_sumADC[side] -= m_ped[side] * (N-1);

    if(i_peak == 0 || i_peak == N - 1) {
        m_peak[side] -= m_ped[side];
        m_peakTime[side] = i_peak * 8;
//        std::cout << "Error : " << i_peak << "th sample has the largest ADC count\n";
        return false;
    }

    float yb = m_data[side][i_peak - 1];
    float y = m_data[side][i_peak];
    float yf = m_data[side][i_peak + 1];

    m_peakTime[side] = i_peak - (yf - yb) / 2 / (yf + yb - 2 * y);
    m_peakTime[side] *= 8;

    m_peak[side] = y - (yf - yb) * (yf - yb) / 8 / (yf + yb - 2 * y);
    m_peak[side] -= m_ped[side];

    float threshold = m_peak[side] * 0.5 + m_ped[side];
    for(int i = i_peak; i > 0; --i) {
        if(m_data[side][i] > threshold && m_data[side][i - 1] < threshold) {
            m_cft[side] = i - 1 + (threshold - m_data[side][i]) / (m_data[side][i] - m_data[side][i - 1]);
            m_cft[side] *= 8;
            return true;
        }
    }

//    std::cout << "Error : Could not find CFTime\n";
    return false;

}

// Visuallization
void BothReadDetector::Visualize() {
    if(m_isVis) {
        std::cout << "Already visualized\n";
        return;
    }

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis
    for(int plane = 0; plane < 3; ++plane) {
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

    m_box_hit[plane]->SetX1(m_hitpos[axis_h] - m_posres[axis_h]);
    m_box_hit[plane]->SetX2(m_hitpos[axis_h] + m_posres[axis_h]);
    m_box_hit[plane]->SetY1(m_hitpos[axis_v] - m_posres[axis_v]);
    m_box_hit[plane]->SetY2(m_hitpos[axis_v] + m_posres[axis_v]);
    m_box_hit[plane]->Draw("same");
}
