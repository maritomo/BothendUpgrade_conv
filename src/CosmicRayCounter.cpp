//
// Created by Tomoo MARI on 2018/09/19.
//

#include <iostream>
#include <sstream>
#include <cmath>

#include "CosmicRayCounter.h"

CosmicRayCounter::CosmicRayCounter(int layer, int ch, int scintiID, int dir, const double* pos) :
        m_layer(layer), m_ch(ch), m_scintiID(scintiID), m_dir(dir) {

    for(int k = 0; k < 3; ++k) {
        m_pos[k] = pos[k];
    }

    m_size[0] = 2000;
    m_size[1] = 100;
    m_size[2] = 50;

    m_posres[0] = m_size[0] / 20;
    m_posres[1] = m_size[1];
    m_posres[2] = m_size[2];

    m_isHit = 0;

    m_col = kBlue;
    m_isVis = 0;
}

/*
 * Setter
 */

void CosmicRayCounter::SetCalibConst(const double* TD_to_x) {
    for(int i = 0; i < 2; ++i) {
        m_ccX[i] = TD_to_x[i] * m_dir;
    }
}

void CosmicRayCounter::SetCoinRange(int side, double* coin_range) {
    for(int i = 0; i < 2; ++i) {
        m_coin_range[side][i] = coin_range[i];
    }
}


/*
 * Process
 */

void CosmicRayCounter::Process() {
    Reconstruct();
    HitDecision();
    for(int side = 0; side < 2; ++side) {
        OnlineHitDecision(side);
    }
}

void CosmicRayCounter::Reconstruct() {
    m_TD = m_pt[0] - m_pt[1];
    m_MT = (m_pt[0] + m_pt[1]) / 2;
    m_hitpos[0] = (m_TD - m_ccX[0]) / m_ccX[1];
    m_hitpos[1] = m_pos[1];
    m_hitpos[2] = m_pos[2];
}

void CosmicRayCounter::HitDecision() {

    double f_far = 0; // arbitrary factor for peak threshold in far side

    for(int k = 0; k < 2; ++k) {
        int k1 = k % 2;        // index of one side
        int k2 = (k + 1) % 2;  // index of another side

        // hit condition (near side)
        if(m_peak[k1] > m_peak_thr[k1] &&
           m_pt[k1] > m_coin_range[k1][0] &&
           m_pt[k1] < m_coin_range[k1][1]) {

            // hit condition (far side)
            /*
            if(m_peak[k2] > f_far * m_peak_thr[k2] &&
               m_pt[k2] > m_coin_range[k2][0] &&
               m_pt[k2] < m_coin_range[k2][1]) {
            */
            if(1) {
                m_isHit = 1;
                return;
            }
        }
    }

    m_isHit = 0;
}

// Imitation of the online trigger
void CosmicRayCounter::OnlineHitDecision(int side) {

    // local time window
    int window_l[2] = {-2, 2};
    const int nSmpl_l = window_l[1] - window_l[0] + 1;

    // global time window
    int window_g[2] = {-2, 3};
    const int nSmpl_g = window_g[1] - window_g[0] + 1;

    short data_g[nSmpl_g];
    short data_l[nSmpl_l];

    for(int smpl = 0; smpl < 64; ++smpl) {
        m_isOnlineHit[side][smpl] = 0;

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
            m_isOnlineHit[side][smpl] = 1;
        }
    }

}
