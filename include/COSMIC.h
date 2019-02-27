//
// Created by Tomoo MARI on 2018/09/19.
//

#ifndef CONV_TRIGGERCOUNTER_H
#define CONV_TRIGGERCOUNTER_H

#include "BothReadDetector.h"


class COSMIC : public BothReadDetector {
  public:
    COSMIC(int ch_south, int ch_north, const double* pos);

    void Process();
    void HitDecision();

    void SetDeltaTtoX(const double* deltaT_to_x);
    void SetMeanTimeZero(double MT0) { m_meanT0 = MT0; }

    int GetChannel_south() { return m_ch_south; }
    int GetChannel_north() { return m_ch_north; }

    int GetLayerID() { return m_pos[1] < 0 ? 0 : 1; }

  private:
    int m_ch_south;
    int m_ch_north;

    double m_deltaT_to_x[2]; // deltaT -> x (constant, slope)
    double m_meanT0;         // Timing zero (= cross timing)
};


#endif //CONV_TRIGGERCOUNTER_H
