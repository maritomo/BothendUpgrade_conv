//
// Created by Tomoo MARI on 2018/09/19.
//

#ifndef CONV_COSMICRAYCOUNTER_H
#define CONV_COSMICRAYCOUNTER_H

#include "BothendReadoutDetector.h"

class CosmicRayCounter : public BothendReadoutDetector {
public:
  CosmicRayCounter(int layer, int ch, int scintiID, int dir, const double* pos);

  void Process();
  void Reconstruct();
  void HitDecision();

  bool SetCalibConst(const double* TD_x);
  bool SetHitCondition(const double* peak_thr, const double* coin_range);

  int GetScintiID() { return m_scintiID; }

private:
  int m_layer;    // 0: bottom, 1: top
  int m_ch;         // 0 to 5
  int m_scintiID;   // scintillator ID
  int m_dir;        //    Directions of the CRCs along x-axis (+1: parallel, -1: unti-parallel) used in the pre-test at Tsukuba campus.
                    //    They are still used to calibrate Tdif to x-position,
                    //    because the calibration constants are same as that in the pre-test at Tsukuba.

  double m_TD_to_x[2];     // calibration constant for TD -> x, (constant, slope)

  double m_peak_thr[2];   // threshold of pulse height, ch (even, odd)
  double m_coin_range[2]; // coincidence time window, ch (even, odd)
};


#endif //CONV_COSMICRAYCOUNTER_H
