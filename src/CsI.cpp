//
// Created by Tomoo MARI on 2018/09/24.
//

#include <fstream>
#include <iostream>

#include "CsI.h"
#include "CosmicTriggerSystem.h"

CsI::CsI(int locationID, int crystalID, int lineID, double posx, double posy, int size) {

  m_locationID = locationID;
  m_lineID = lineID;
  m_crystalID = crystalID;

  m_pos[0] = posx;
  m_pos[1] = posy;
  m_pos[2] = 0;

  m_size[0] = size;
  m_size[1] = size;
  m_size[2] = 500;

  m_posres[0] = m_size[0];
  m_posres[1] = m_size[1];
  m_posres[2] = m_size[2]/4;

  m_isHit = 0;
  m_isUsed = 0;

  for(int side=0; side < 2; ++side) {
    m_crate[side] = -1;
    m_slot[side] = -1;
    m_ch[side] = -1;
  }

  m_color = kBlack;
  m_isVis = 0;
}

CsI::~CsI() {}

void CsI::Process() {
  HitDecision();
}

void CsI::HitDecision() {
  m_isHit = 0;
  for(int axis=0; axis<3; ++axis) {
    m_hitpos[axis] = 0;
  }

  if(!m_isUsed) return;

  for(int side = 0; side < 2; ++side) {
    if(m_peak[side] > 50) {
      m_isHit = 1;
//      m_hitpos[0] = ????;
      m_hitpos[1] = m_pos[1];
      m_hitpos[2] = m_pos[2];
      break;
    }
  }
}

void CsI::SetHitPos(double* track) {
  //if(!m_isHit) return;
  m_hitpos[0] = m_pos[0];
  m_hitpos[1] = m_pos[1];
  m_hitpos[2] = (m_pos[1] - track[0]) / track[1];
}
