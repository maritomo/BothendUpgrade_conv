//
// Created by Tomoo MARI on 2018/09/24.
//

#ifndef CONV_CSI_H
#define CONV_CSI_H

#include "BothendReadoutDetector.h"

class CsI : BothendReadoutDetector {
public:
  CsI(int locationID);

  void Process();
  void HitDecision();

  void SetHitPos(int trackID); // trackID (yz plane) -> z hit position

private:
  static const int nCSI = 2716;
  static const int nCSI_S = 2240;
  static const int nCSI_L = 476;

  int m_locationID;
  int m_lineID;

  double m_cEne; // Integrated ADC -> deposited energy
};


#endif //CONV_CSI_H
