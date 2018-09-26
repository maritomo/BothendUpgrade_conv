//
// Created by Tomoo MARI on 2018/09/20.
//

#ifndef CONV_BOTHENDREADOUTDETECTOR_H
#define CONV_BOTHENDREADOUTDETECTOR_H

#include "TBox.h"

class BothendReadoutDetector {
public:
  virtual ~BothendReadoutDetector();

  virtual void SetData(int side, const short* data);
  virtual void Process() = 0;

  double GetPos(int axis) { return m_pos[axis]; }

  double GetPed(int side) { return m_ped[side]; }
  double GetPeak(int side) { return m_peak[side]; }
  double GetInteg(int side) { return m_integ[side]; }
  double GetPT(int side) { return m_pt[side]; }
  double GetCFT(int side) { return m_cft[side]; }
  double GetErrFlag(int side) { return m_errflag[side]; }

  double GetTD() { return m_TD; }
  double GetMT() { return m_MT; }

  int GetIsHit() { return m_isHit; }
  int GetHitPos(int axis) { return m_hitpos[axis]; }

  void SetDelay(int side, int delay) { m_delay[side] = delay; }

  void GetCFTime(int side, const short* data);

  void GetVisAxis(int plane, int& axis_h, int& axis_v);
  void Visualize(int plane);
  void Display(int plane);

protected:
  double m_pos[3];   // location (x, y, z)
  double m_size[3];  // size (x, y, z)

  int m_delay[2];   // Channel delay

  double m_ped[2];    // ch (even, odd)
  double m_peak[2];   // ch (even, odd)
  double m_integ[2];  // ch (even, odd)
  double m_pt[2];     // ch (even, odd)
  double m_cft[2];    // ch (even, odd)
  int m_errflag[2];   // ch (even, odd)

  double m_TD;  // timing difference
  double m_MT;  // mean time

  int m_isHit;
  double m_hitpos[3]; // hit position (x, y, z)
  double m_posres[3]; // position resolution (x, y, z)

  int m_isVis[3]; // (xy, yz, zx) plane

  TBox* m_box_det[3]; // detector, (xy, yz, zx) plane
  TBox* m_box_hit[3]; // hit point (xy, yz, zx) plane
  int m_col;          // detector color
};


#endif //CONV_BOTHENDREADOUTDETECTOR_H
