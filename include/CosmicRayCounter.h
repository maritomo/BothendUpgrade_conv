//
// Created by Tomoo MARI on 2018/09/19.
//

#ifndef CONV_COSMICRAYCOUNTER_H
#define CONV_COSMICRAYCOUNTER_H

#include "BothReadDetector.h"
class CosmicRayCounter : public BothReadDetector {
  public:
    CosmicRayCounter(int layer, int ch, int scintiID, int dir, const double* pos);

    void Process();
    void HitDecision();

    void SetCalibConst(const double* TD_x);
    void SetPeakThreshold(int side, double peak_thr) { m_peak_thr[side] = peak_thr; }
    void SetCoinRange(int side, double* coin_range);

    int GetScintiID() { return m_scintiID; }

    // Imitation of the online trigger
    void OnlineHitDecision(int side);
    const int* IsOnlineHit(int side) { return m_isOnlineHit[side]; }

  private:
    int m_layer;      // 0: bottom, 1: top
    int m_ch;         // 0 ~ 5
    int m_scintiID;   // scintillator ID
    int m_dir;        //    Directions of the counters along x-axis (+1: parallel, -1: unti-parallel) used in the pre-test at Tsukuba campus[2].
                      //    They are still used to calibrate Tdif to x-position,
                      //    because the calibration constants are same as that in the pre-test at Tsukuba.

    double m_ccX[2];     // calibration constant for TD->x, (constant, slope)

    double m_peak_thr[2];       // threshold of pulse height, (even, odd) ch
    double m_coin_range[2][2];  // coincidence window, (even, odd) ch, (min, max)

    int m_isOnlineHit[2][64];
};


#endif //CONV_COSMICRAYCOUNTER_H
