//
// Created by Tomoo MARI on 2018/09/24.
//

#include "CsI.h"

#include <fstream>

CsI::CsI(int locationID) : m_locationID(locationID) {
  std::ifstream ifs("./data/map_csi.txt");

  int locID, lineID, posx, posy, size;

  for(int id = 0; id < nCSI; ++id) {
    ifs >> locID >> lineID >> posx >> posy >> size;
    if(m_locationID==locID) break;
  }

  m_lineID = lineID;

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

  for(int plane = 0; plane < 3; ++plane) {
    m_isVis[plane] = 0;
  }

  m_col = kRed;
}

void CsI::Process() {
  HitDecision();
}

void CsI::HitDecision() {

  if(1) {         // some conditions
    m_isHit = 1;
    return;
  }

  m_isHit = 0;
}

void CsI::SetHitPos(int trackID) {
  if(!m_isHit) return;

  m_hitpos[0] = m_pos[0];
  m_hitpos[1] = m_pos[1];
  // m_hitpos[2] = ???

}