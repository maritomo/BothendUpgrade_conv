//
// Created by Tomoo MARI on 2018/09/24.
//

#ifndef CONV_CSI_H
#define CONV_CSI_H

#include "BothendReadoutDetector.h"
//#include "CosmicTriggerSystem.h"
class CsI : public BothendReadoutDetector {
public:
  CsI(int locID, int lineID, int posx, int posy, int size);

  void Process();
  void HitDecision();
  void ZTrack();
  void SetADC(int crate, int slot, int ch);
  void SetIsUsed(int isUsed);
  int GetIsUsed(){ return m_isUsed; }
  void SetHitPos(int plate, double* track); // trackID (yz plane) -> z hit position
  int GetLocID(){ return m_locationID; }
  int GetADC(int k){ return m_ADC[k];} //k=0 crate, k=1 slot, k=2 ch


private:
  static const int nCSI = 2716;
  static const int nCSI_S = 2240;
  static const int nCSI_L = 476;

  int m_locationID;
  int m_cristalID;
  int m_lineID;
  int m_ADC[3]; //crate,slot,ch
  //int m_slot;
  //int m_ch;
  int m_isUsed;


  double m_cEne; // Integrated ADC -> deposited energy
};


#endif //CONV_CSI_H
