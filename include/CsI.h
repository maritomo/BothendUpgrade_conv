//
// Created by Tomoo MARI on 2018/09/24.
//

#ifndef CONV_CSI_H
#define CONV_CSI_H

#include "BothendReadoutDetector.h"

class CsI : public BothendReadoutDetector {
public:
  CsI(int locID, int lineID, double posx, double posy, int size);

  void Process();
  void HitDecision();
  void ZTrack();

  void SetADC(int side, int crate, int slot, int ch);
  void SetIsUsed(int isUsed);
  void SetHitPos(double* track); // trackID (yz plane)->z hit position

    int GetIsUsed(){ return m_isUsed; }
  int GetLocationID(){ return m_locationID; }

  int GetCrate(int side){ return m_crate[side]; }
  int GetSlot(int side){ return m_slot[side]; }
  int Getch(int side){ return m_ch[side]; }


private:
  static const int nCSI = 2716;
  static const int nCSI_S = 2240;
  static const int nCSI_L = 476;

  int m_locationID;
  int m_crystalID;
  int m_lineID;

  int m_crate[2]; // 0: MPPC, 1: PMT
  int m_slot[2];
  int m_ch[2];
  int m_isUsed;


  double m_cEne; // Integrated ADC->deposited energy
};


#endif //CONV_CSI_H
