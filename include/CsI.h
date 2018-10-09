//
// Created by Tomoo MARI on 2018/09/24.
//

#ifndef CONV_CSI_H
#define CONV_CSI_H

#include "BothReadDetector.h"

class CsI : public BothReadDetector {
public:
  CsI(int locationID, int crystalID, int lineID, double posx, double posy, int size);
  ~CsI();

  void Process();
  void HitDecision();
  void SetHitPos(double* track); // trackID -> z hit position

  int IsUsed() { return m_isUsed; }
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

  double m_cEne; // Integrated ADC->deposited energy
};


#endif //CONV_CSI_H
