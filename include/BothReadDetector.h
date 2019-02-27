//
// Created by Tomoo MARI on 2018/09/20.
//

#ifndef CONV_BOTHREADDETECTOR_H
#define CONV_BOTHREADDETECTOR_H

#include "TF1.h"
#include "TBox.h"

#include "Visualizer.h"
#include "CosmicRay.h"

class BothReadDetector : public Visualizer {
  public:
    virtual ~BothReadDetector();

    virtual void Process() = 0;
    virtual void Reconstruct();

    bool CalCFTime(int side);

    void SetData(int side, Short_t* data) { m_data[side] = data; };
    void SetDelay(int side, int delay) { m_delay[side] = delay; }
    void SetHitpos(double* hitpos) { for(int axis=0; axis<3; ++axis) {m_hitpos[axis] = hitpos[axis];} }
    void SetHitpos(int axis, double hitpos) { m_hitpos[axis] = hitpos; }

    const double* GetPos() { return m_pos; }
    const double* GetPosres() { return m_posres; }

    const short* GetData(int side) { return m_data[side]; }
    double GetPedestal(int side) { return m_ped[side]; }
    double GetPeak(int side) { return m_peak[side]; }
    double GetIntegratedADC(int side) { return m_sumADC[side]; }
    double GetPeakTime(int side) { return m_peakTime[side]; }
    double GetCFTime(int side) { return m_cft[side]; }

    double GetDeltaT() { return m_deltaT; }
    double GetMeanTime() { return m_meanT; }

    bool IsHit() { return m_isHit; }
    const double* GetHitPos() { return m_hitpos; }

    // Visualization
    void Visualize();
    void Display(int plane);

  protected:
    CosmicRay* m_cosmic_ray;

    double m_pos[3];   // location (x, y, z)
    double m_size[3];  // size (x, y, z)

    double m_delay[2];

    short* m_data[2];
    double m_ped[2];
    double m_peak[2];
    double m_sumADC[2];
    double m_peakTime[2];
    double m_cft[2];
    int m_ipeak[2];

    double m_deltaT;  // timing difference
    double m_meanT;  // mean time

    bool m_isHit;
    double m_hitpos[3]; // hit position (x, y, z)
    double m_posres[3]; // position resolution (x, y, z)

    // Visualization
    int m_color;          // detector color
    TBox* m_box_det[3]; // detector, (xy, yz, zx) plane
    TBox* m_box_hit[3]; // hit point (xy, yz, zx) plane

};


#endif //CONV_BOTHREADDETECTOR_H
