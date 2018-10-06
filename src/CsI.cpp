//
// Created by Tomoo MARI on 2018/09/24.
//

#include "CsI.h"

#include <fstream>
#include <iostream>
#include <CosmicTriggerSystem.h>

CsI::CsI(int locID, int lineID, double posx, double posy, int size) {

  m_locationID = locID;
  m_lineID = lineID;
  m_crystalID = 0;

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
  m_crate[0] = 0;
  m_crate[1] = 0;
  m_slot[0] = 0;
  m_slot[1] = 0;
  m_ch[0] = 0;
  m_ch[1] = 0;


  for(int plane = 0; plane < 3; ++plane) {
    m_isVis[plane] = 0;
  }

  m_col = kRed;
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
  //if(!m_isHit) return;

  m_hitpos[0] = m_pos[0];
  m_hitpos[1] = m_pos[1];
  m_hitpos[2] = (m_pos[1] - (*track)) / (*(track + 1));
//  std::cout << m_hitpos[2] << std::endl;
//  std::cout<< m_isHit << std::endl;
//  std::cout << *track << std::endl;

}

void CsI::SetADC(int side, int crate, int slot, int ch) {
  m_crate[side] = crate;
  m_slot[side] = slot;
  m_ch[side] = ch;
}

void CsI::SetIsUsed(int IsUsed) {
    m_isUsed = IsUsed;
}