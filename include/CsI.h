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
