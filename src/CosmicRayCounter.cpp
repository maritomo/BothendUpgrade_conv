//
// Created by Tomoo MARI on 2018/09/19.
//

#include <iostream>
#include <sstream>
#include <cmath>

#include "global.h"
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

    for(int plane = 0; plane < 3; ++plane) {
        m_isVis[plane] = 0;
    }

    m_col = kRed;
}

/*
 * Setter
 */

void CosmicRayCounter::SetCalibConst(const double* TD_to_x) {
    for(int i = 0; i < 2; ++i) {
        m_TD_to_x[i] = TD_to_x[i] * m_dir;
    }
}

/*
 * Process
 */

void CosmicRayCounter::Process() {
    Reconstruct();
    HitDecision();
}

void CosmicRayCounter::Reconstruct() {
    m_TD = m_pt[0] - m_pt[1];
    m_MT = (m_pt[0] + m_pt[1]) / 2;
    m_hitpos[0] = (m_TD - m_TD_to_x[0]) / m_TD_to_x[1];
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
