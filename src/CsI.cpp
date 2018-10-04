//
// Created by Tomoo MARI on 2018/09/24.
//

#include "CsI.h"

#include <fstream>
#include <CosmicTriggerSystem.h>

CsI::CsI(int locID, int lineID, double posx, double posy, int size) : m_locationID(locID) {


  m_lineID = lineID;
  m_cristalID = 0;

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

  if(1) {         // some conditions
    m_isHit = 1;
    return;
  }

  m_isHit = 0;
}


void CsI::SetHitPos(double* track) {
  if(!m_isHit) return;

  m_hitpos[0] = m_pos[0];
  m_hitpos[1] = m_pos[1];
  m_hitpos[2] = *track + (1 / m_pos[1] ) * *(track + 1);

}

void CsI::SetADC(int side, int crate, int slot, int ch) {
  m_crate[side] = crate;
  m_slot[side] = slot;
  m_ch[side] = ch;
}

void CsI::SetIsUsed(int IsUsed) {
    m_isUsed = IsUsed;
}