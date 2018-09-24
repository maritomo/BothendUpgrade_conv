//
// Created by Tomoo MARI on 2018/09/18.
//
#include "global.h"
#include "CosmicTriggerSystem.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <TMath.h>


CosmicTriggerSystem::CosmicTriggerSystem(int runID) : m_runID(runID) {

  if(!Init()) {
    std::cout << "Error: CosmicTriggerSystem instance was not initialized\n";
  }

  for(int plane = 0; plane < 3; ++plane) {
    m_isVis[plane] = 0;
  }

  double worldsize[3] = {1100, 1500, 300};
  for(int axis=0; axis<3; ++axis) {
    for(int i=0; i<2; ++i) {
      m_world[axis][i] = worldsize[axis];
      if(i==0) m_world[axis][i] *= -1;
    }
  }
  
}

CosmicTriggerSystem::~CosmicTriggerSystem() {
  for(int layer = 0; layer < m_nLayer; ++layer) {
    for(int ch = 0; ch < m_nCRC; ++ch) {
      delete m_crc[layer][ch];
    }
  }

  if(m_isVis[0]==1 || m_isVis[1]==1 || m_isVis[2]==1) {
    m_canv->Delete();
    for(int plane = 0; plane < 3; ++plane) {
      m_lTrack[plane]->Delete();
    }
  }
}

bool CosmicTriggerSystem::Init() {

  m_isInit = 0;

  std::cout << "---------------------------------------------\n";
  std::cout << "      Initialize cosmic trigger system       \n";
  std::cout << "---------------------------------------------\n";

  if(!Init_map()) return false;
  if(!Init_calibConst()) return false;
  if(!Init_hitCondition()) return false;

  std::cout << "---------------------------------------------\n";
  std::cout << "      Cosmic trigger system initialized      \n";
  std::cout << "---------------------------------------------\n";

  m_isInit = 1;
  return true;
}

/*
 * Initiallization
 */

bool CosmicTriggerSystem::Init_map() {

  std::string filename = "./data/map_crc.txt";
  std::ifstream ifs(filename.c_str());

  if(!ifs) {
    std::cout << filename << " not found !\n";
    return false;
  }

  int scintiID, dir;
  double pos[3] = {1000, 1000, 0};


  for(int layer = 0; layer < m_nLayer; ++layer) {
    for(int ch = 0; ch < m_nCRC; ++ch) {
      ifs >> scintiID >> dir >> pos[0] >> pos[1] >> pos[2];
      m_crc[layer][ch] = new CosmicRayCounter(layer, ch, scintiID, dir, pos);
    }
  }

  ifs.close();

  std::cout << "Trigger counter map             [OK]\n";
  return true;
}

bool CosmicTriggerSystem::Init_calibConst() {

  std::string filename = "./data/TDtoX.txt";
  std::ifstream ifs(filename.c_str());

  if(!ifs) {
    std::cout << filename << " not found !\n";
    return false;
  }

  for(int i = 0; i < 16; ++i) {
    int scintiID;
    double cc[2];
    ifs >> scintiID >> cc[0] >> cc[1];

    int layer, ch;
    if(GetLocationID(scintiID, layer, ch)) {
      m_crc[layer][ch]->SetCalibConst(cc);
    }
  }

  ifs.close();

  std::cout << "Calibration constants           [OK]\n";
  return true;
}

bool CosmicTriggerSystem::Init_hitCondition() {

  std::stringstream ss;
  ss << "./data/run" << m_runID << "/pedestal.txt";

  std::string filename;
  ss >> filename;

#ifdef COMMON_THRESHOLD
  filename = "./data/common_threshold.txt";
#endif

  std::ifstream ifs(filename.c_str());
  if(!ifs) {
    std::cout << filename << " not found !\n";
    return false;
  }

  double coin_range[2] = {20, 40};

  int layer;
  int pmtID;
  double mean[2], sigma[2];

  int ch;

  for(int k = 0; k < 24; ++k) {
    ifs >> layer >> pmtID >> mean[k % 2] >> sigma[k % 2];

#ifndef COMMON_THRESHOLD
    sigma[k % 2] *= 5;
#endif

    if(k % 2==1) {
      ch = pmtID / 2;
      m_crc[layer][ch]->SetHitCondition(sigma, coin_range);
    }

  }

  ifs.close();
  std::cout << "Hit condition parameters        [OK]\n";

  return true;
}

/*
 * Tracking process
 */

void CosmicTriggerSystem::Process() {
  HitDecision();
  Tracking();
}

void CosmicTriggerSystem::HitDecision() {
  m_nHit[0] = 0;
  m_nHit[1] = 0;

  for(int layer = 0; layer < m_nLayer; ++layer) {
    for(int ch = 0; ch < m_nCRC; ++ch) {
      m_crc[layer][ch]->Process();

      if(m_crc[layer][ch]->GetIsHit()) {
        ++m_nHit[layer];
        m_hitCh[layer] = ch;
      }
    }
  }

}

void CosmicTriggerSystem::Tracking() {

  if(m_nHit[0]!=1 || m_nHit[1]!=1) {
    m_trackID = 0;
    return;
  }

  for(int layer = 0; layer < m_nLayer; ++layer) {
    for(int axis = 0; axis < 3; ++axis) {
      m_hitpos[layer][axis] = m_crc[layer][m_hitCh[layer]]->GetHitPos(axis);
    }
  }

  m_trackID = m_hitCh[1] * m_nCRC + m_hitCh[0] + 1;

  TOF = sqrt((m_hitpos[1][0]-m_hitpos[0][0])*(m_hitpos[1][0]-m_hitpos[0][0])+
             (m_hitpos[1][1]-m_hitpos[0][1])*(m_hitpos[1][1]-m_hitpos[0][1])+
             (m_hitpos[1][2]-m_hitpos[0][2])*(m_hitpos[1][2]-m_hitpos[0][2])) / TMath::C();

  int axis_h; // index of horizontal axis
  int axis_v; // index of vertical axis

  for(int plane = 0; plane < 3; ++plane) {
    GetVisAxis(plane, axis_h, axis_v);
    m_track[plane][1] = (m_hitpos[1][axis_v] - m_hitpos[0][axis_v]) / (m_hitpos[1][axis_h] - m_hitpos[0][axis_h]);
    m_track[plane][0] = m_hitpos[1][axis_v] - m_track[plane][1] * m_hitpos[1][axis_h];
  }

}

/*
 * Setter
 */

void CosmicTriggerSystem::SetData(int slot, int ch, const short* data) {
  if(slot > 1) return;
  if(ch > 11) return;

  int layer = slot;
  int channel = ch / 2;
  int LR = ch % 2;

  m_crc[layer][channel]->SetData(LR, data);
}

/*
 * Getter
 */

bool CosmicTriggerSystem::GetLocationID(int scintiID, int& layerID, int& chID) {
  for(int layer = 0; layer < m_nLayer; ++layer) {
    for(int ch = 0; ch < m_nCRC; ++ch) {
      if(m_crc[layer][ch]->GetScintiID()==scintiID) {
        layerID = layer;
        chID = ch;
        return true;
      }
    }
  }

  std::cout << "(scinti. " << scintiID << " not found)\n";
  return false;
}

/*
 * Visuallization
 */

void CosmicTriggerSystem::GetVisAxis(int plane, int& axis_h, int& axis_v) {
  if(plane==0) {          // xy plane -> (x, y)
    axis_h = 0;
    axis_v = 1;
  } else if(plane==1) {   // yz plane -> (z, y)
    axis_h = 2;
    axis_v = 1;
  } else if(plane==2) {   // zx plane -> (x, z)
    axis_h = 0;
    axis_v = 2;
  } else {
    std::cout << "plane: 0 (xy), 1 (yz), 2 (zx)\n";
  }
}

void CosmicTriggerSystem::Visualize() {
  m_canv = new TCanvas("m_canv", "event display", 900, 600);
  m_canv->Divide(2, 1);

  // xy and yz plane
  for(int plane = 0; plane < 2; ++plane) {
    for(int layer = 0; layer < m_nLayer; ++layer) {
      for(int ch = 0; ch < m_nCRC; ++ch) {
        m_crc[layer][ch]->Visualize(plane);
      }
    }

    m_lTrack[plane] = new TLine();
    m_lTrack[plane]->SetLineColor(kRed);

    m_isVis[plane] = 1;
  }

}

void CosmicTriggerSystem::SetTrackLine(int plane) {

  int axis_h; // index of horizontal axis
  int axis_v; // index of vertical axis
  GetVisAxis(plane, axis_h, axis_v);

  double h[2], v[2];

  for(int i = 0; i < 2; ++i) {
    h[i] = (m_world[axis_v][i] - m_track[plane][0]) / m_track[plane][1];

    if(h[i] < m_world[axis_h][0]) {
      h[i] = m_world[axis_h][0];
    }
    if (h[i] > m_world[axis_h][1]) {
      h[i] = m_world[axis_h][1];
    }


    v[i] = m_track[plane][1] * h[i] + m_track[plane][0];
  }

  m_lTrack[plane]->SetX1(h[0]);
  m_lTrack[plane]->SetX2(h[1]);
  m_lTrack[plane]->SetY1(v[0]);
  m_lTrack[plane]->SetY2(v[1]);
}

void CosmicTriggerSystem::Display() {
  if(m_isVis[0]==0 && m_isVis[1]==0 && m_isVis[2]==0) {
    std::cout << "Visualization not ready\n";
    return;
  }

  int axis_h; // index of horizontal axis
  int axis_v; // index of vertical axis

  TString title[3] = {";x [mm];y [mm]", ";z [mm];y [mm]", ";x [mm];z [mm]"};

  // xy and yz plane
  for(int plane = 0; plane < 2; ++plane) {
    if(!m_isVis[plane]) continue;

    GetVisAxis(plane, axis_h, axis_v);

    m_canv->cd(plane + 1)->Clear();
    m_canv->cd(plane + 1)->DrawFrame(m_world[axis_h][0], m_world[axis_v][0],
                                     m_world[axis_h][1], m_world[axis_v][1], title[plane]);

    // Cosmic ray counters
    for(int layer = 0; layer < m_nLayer; ++layer) {
      for(int ch = 0; ch < m_nCRC; ++ch) {
        m_crc[layer][ch]->Display(plane);
      }
    }

    // Track
    if(m_trackID==0) continue;

    SetTrackLine(plane);
    m_lTrack[plane]->Draw("same");
  }

}

void CosmicTriggerSystem::Print(const char* filename) {
  if(m_isVis[0]==1 || m_isVis[1]==1 || m_isVis[2]==1)
    m_canv->Print(filename);
}
