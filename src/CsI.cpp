//
// Created by Tomoo MARI on 2018/09/24.
//

#include <fstream>
#include <iostream>

#include "CsI.h"

CsI::CsI(int locationID, int crystalID, int lineID, double posx, double posy, int size) {

    m_cosmi = CosmicRay::GetInstance();

    m_locationID = locationID;
    m_lineID = lineID;
    m_crystalID = crystalID;

    m_pos[0] = posx;
    m_pos[1] = posy;
    m_pos[2] = 0;

    m_size[0] = size;
    m_size[1] = size;
    m_size[2] = 500;

    m_posres[0] = m_size[0] * 0.5;
    m_posres[1] = m_size[1] * 0.5;
    m_posres[2] = m_size[2] * 0.1;

    for(int side = 0; side<2; ++side) {
        m_isUsed[side] = 0;
        m_crate[side] = -1;
        m_mod[side] = -1;
        m_ch[side] = -1;
    }

    m_completion_peakThreshold[0] = 50;
    m_completion_peakThreshold[1] = 50;

    m_isHit = false;
    m_isCalibrated = false;

    m_color = kBlack;
    m_isVis = 0;
}

CsI::~CsI() {}

void CsI::Process() {
    Reconstruct();
    Calibration();
    HitDecision();
}

void CsI::Calibration() {
    m_Edep = m_sumADC[1] * m_cEne;
}

void CsI::HitDecision() {
    m_isHit = false;
    for(int axis = 0; axis<3; ++axis) {
        m_hitpos[axis] = 0;
    }

    if(m_eflag[1]) {
      m_isHit = false;
      return;
    }

    double mip = 30;
    if(m_size[0]==25) mip *= 0.5;

    if(m_isCalibrated) {
        if(m_Edep>0.25*mip) {
            m_isHit = true;
        }
    } else {
        if(m_peak[1]>100) {
            m_isHit = true;
        }
    }

    if(m_isHit) {
        m_hitpos[0] = m_pos[0];
        m_hitpos[1] = m_pos[1];
    }
}

void CsI::SetEdepCalibConst(double cEne) {
    // if negative
    if(cEne <= 0) {
        m_isCalibrated = false;
        return;
    }
    m_cEne = cEne;
    m_isCalibrated = true;
}

void CsI::SetZhitTable(const double* z) {
    for(int i=0; i<36; ++i) {
        m_zhit[i] = z[i];
    }
}
