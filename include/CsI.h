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
  void SetHitPos(int trackID); // trackID (yz plane) -> z hit position
  int GetLocID(){ return m_locationID; }
  int GetCrate(){ return m_crate; }
  int GetSlot(){ return m_slot; }
  int GetCh() { return  m_ch; }
  int GetIsused(){ return  m_isUsed;}

private:
  static const int nCSI = 2716;
  static const int nCSI_S = 2240;
  static const int nCSI_L = 476;

  int m_locationID;
  int m_cristalID;
  int m_lineID;
  int m_crate;
  int m_slot;
  int m_ch;
  int m_isUsed;


  double m_cEne; // Integrated ADC -> deposited energy
};


#endif //CONV_CSI_H
