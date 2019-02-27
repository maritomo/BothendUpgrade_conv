//
// Created by Tomoo MARI on 2018/09/24.
//

#include <fstream>
#include <iostream>

#include "CSI.h"

CSI::CSI(int locID, double posx, double posy, int size) {

    m_cosmic_ray = CosmicRay::GetInstance();

    m_locID = locID;

    m_pos[0] = posx;
    m_pos[1] = posy;
    m_pos[2] = 0;

    m_size[0] = size;
    m_size[1] = size;
    m_size[2] = 500;

    m_posres[0] = m_size[0] / 2;
    m_posres[1] = m_size[1] / 2;
    m_posres[2] = m_size[2] * 0.25; // ~1.5ns = 25%

    m_delay[0] = 0, m_delay[1] = 0;

    m_isHit = false;
    m_isCalib = false;

    m_color = kBlack;
    m_isVis = 0;
}

CSI::~CSI() {}

void CSI::SetZsurface(double zSurface) {
    m_zSurface = zSurface;
    m_pos[2] += m_zSurface;
}

void CSI::Process() {
    Reconstruct();
    Calibration();
    HitDecision();
}

void CSI::Calibration() {
    m_Edep = m_sumADC[1] * m_cEne;
}

void CSI::HitDecision() {
    m_isHit = false;
    for(int axis = 0; axis<3; ++axis) m_hitpos[axis] = 0;

    double mip = 30;
    if(m_size[0]==25) mip *= 0.5;

    if(m_isCalib) {
        if(m_Edep > m_EdepThreshold) m_isHit = true; // MeV
    } else {
        if(m_peak[1] > 100) m_isHit = true; // ADC count
    }

    if(m_isHit) {
        m_hitpos[0] = m_pos[0];
        m_hitpos[1] = m_pos[1];
    }
}

void CSI::SetEdepCalibConst(double cEne) {
    if(cEne <= 0) {
        m_isCalib = false;
        return;
    }
    m_cEne = cEne;
    m_isCalib = true;
}

void CSI::SetZhitTable(const double* z) {
    for(int i=0; i<36; ++i) {
        m_globalZhit[i] = z[i];
    }
}
