//
// Created by Tomoo MARI on 2018/09/19.
//

#include <iostream>
#include <sstream>
#include <cmath>

#include "COSMIC.h"

COSMIC::COSMIC(int ch_south, int ch_north, const double* pos) {

    m_ch_south = ch_south;
    m_ch_north = ch_north;
    for(int k = 0; k < 3; ++k) m_pos[k] = pos[k];

    m_size[0] = 2000;
    m_size[1] = 100;
    m_size[2] = 50;

    m_posres[0] = m_size[0] * 0.02; // resolution ~ 2% (4cm)
    m_posres[1] = m_size[1] / 2;
    m_posres[2] = m_size[2] / 2;

    m_isHit = false;

    m_color = kBlue;
    m_isVis = false;
}


void COSMIC::SetDeltaTtoX(const double* deltaT_to_x) {
    for(int i = 0; i < 2; ++i) {
        m_deltaT_to_x[i] = deltaT_to_x[i];
    }
}

void COSMIC::Process() {
    Reconstruct();
    m_meanT -= m_meanT0;
    HitDecision();
}

void COSMIC::HitDecision() {
    m_isHit = false;
    for(int axis = 0; axis < 3; ++axis) m_hitpos[axis] = 0;

    if(TMath::Max(m_peak[0], m_peak[1]) > 1000 && TMath::Min(m_peak[0], m_peak[1]) > 50) {
        m_isHit = true;
        m_hitpos[0] = m_deltaT_to_x[1] * m_deltaT + m_deltaT_to_x[0];
        m_hitpos[1] = m_pos[1];
        m_hitpos[2] = m_pos[2];
    }
}
