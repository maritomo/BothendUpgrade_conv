//
// Created by Tomoo MARI on 2018/09/24.
//

#include "CsI.h"

#include <fstream>
#include <iostream>
#include <CosmicTriggerSystem.h>

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

  m_isUsed = 0;
  m_isHit = 0;

  for(int side=0; side < 2; ++side) {
    m_crate[side] = -1;
    m_slot[side] = -1;
    m_ch[side] = -1;
  }

  for(int plane = 0; plane < 3; ++plane) {
    m_isVis[plane] = 0;
  }

  m_col = kRed;
}

void CsI::SetADCconfig(int side, int crate, int slot, int ch) {
  int errflag = 0;
  if(side!=0 && side!=1) errflag = 1;
  if(crate<3 || crate > 5) errflag = 1;
  if(slot<0 || slot > 15) errflag = 1;
  if(ch<0 || ch>15) errflag = 1;

  if(errflag) {
    std::cout << "Error: FADC configuration is outside the range\n";
    m_isUsed = 0;
    return;
  }

  m_crate[side] = crate;
  m_slot[side] = slot;
  m_ch[side] = ch;
  m_isUsed = 1;
}

void CsI::Process() {
  HitDecision();
  //SetHitPos();
}

void CsI::HitDecision() {
   // some conditions
    m_isHit = 1;
}


void CsI::SetHitPos(double* track) {
  if(m_isHit) {
    m_hitpos[0] = m_pos[0];
    m_hitpos[1] = m_pos[1];
    m_hitpos[2] = (m_pos[1] - track[0]) / track[1];
  } else {
    for(int axis=0; axis<3; ++axis) {
      m_hitpos[axis] = 0;
    }
  }
}