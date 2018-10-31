//
// Created by Tomoo MARI on 2018/09/19.
//

#include <iostream>
#include <sstream>
#include <cmath>

#include "TriggerCounter.h"

//TriggerCounter::TriggerCounter(int layer, int ch, int scintiID, int dir, const double* pos) :
//        m_layer(layer), m_ch(ch), m_scintiID(scintiID), m_dir(dir) {
TriggerCounter::TriggerCounter(int scintiID, int dir, const double* pos) :
        m_scintiID(scintiID), m_dir(dir) {

    for(int k = 0; k < 3; ++k) {
        m_pos[k] = pos[k];
    }

    m_size[0] = 2000;
    m_size[1] = 100;
    m_size[2] = 50;

    m_posres[0] = m_size[0] * 0.02; // resolution ~ 2% (4cm)
    m_posres[1] = m_size[1] * 0.5;
    m_posres[2] = m_size[2] * 0.5;

    m_isHit = 0;

    m_color = kBlue;
    m_isVis = 0;
}

/*
 * Setter
 */

void TriggerCounter::SetHitXparams(const double* TD_to_x) {
    for(int i = 0; i < 2; ++i) {
        m_cHitX[i] = TD_to_x[i];
    }
}

void TriggerCounter::SetCoinRange(int side, double* coin_range) {
    for(int i = 0; i < 2; ++i) {
        m_coin_range[side][i] = coin_range[i];
    }
}


/*
 * Process
 */

void TriggerCounter::Process() {
    Reconstruct();
    m_MT -= m_MT0;
    HitDecision();
    for(int side = 0; side < 2; ++side) {
        OnlineHitDecision(side);
    }
}

void TriggerCounter::HitDecision() {
    m_isHit = false;
    for(int axis=0; axis<3; ++axis) {
        m_hitpos[axis] = 0;
    }

    for(int side = 0; side < 2; ++side) {
        if(m_peak[side] > m_peak_thr[side] &&
           m_pt[side]/8 + m_delay[side] > m_coin_range[side][0] &&
           m_pt[side]/8 + m_delay[side] < m_coin_range[side][1])
        {
            m_isHit = true;
            m_hitpos[0] = m_cHitX[1] * m_TD + m_cHitX[0];
            m_hitpos[1] = m_pos[1];
            m_hitpos[2] = m_pos[2];
            break;
        }
    }
}

// Imitation of the online trigger
void TriggerCounter::OnlineHitDecision(int side) {

    // local time window
    int window_l[2] = {-2, 2};
    const int nSmpl_l = window_l[1] - window_l[0] + 1;

    // global time window
    int window_g[2] = {-2, 3};
    const int nSmpl_g = window_g[1] - window_g[0] + 1;

    short data_g[nSmpl_g];
    short data_l[nSmpl_l];

    for(int smpl = 0; smpl < 64; ++smpl) {
        m_isHit_online[side][smpl] = 0;

        if(smpl < 0 - window_g[0] || smpl >= 64 - window_g[1])
            continue;

        for(int k = 0; k < nSmpl_g; ++k) {
            int index = k + window_g[0];
            data_g[k] = m_data[side][smpl + index];
            if(k < nSmpl_l) data_l[k] = m_data[side][smpl + index];
        }

        short max_l = GetMax(nSmpl_l, data_l);
        short max_g = GetMax(nSmpl_g, data_g);

        if(max_l == max_g && max_l-m_ped[side] > m_peak_thr[side]) {
            m_isHit_online[side][smpl] = 1;
        }
    }

}
