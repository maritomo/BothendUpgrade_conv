//
// Created by Tomoo MARI on 2018/09/20.
//

#ifndef CONV_BOTHREADDETECTOR_H
#define CONV_BOTHREADDETECTOR_H

#include "TF1.h"
#include "TBox.h"

#include "TreeManager.h"
#include "Visualizer.h"
#include "CosmicRay.h"

class BothReadDetector : public TreeManager, public Visualizer {
  public:
    virtual ~BothReadDetector();

    virtual void Process() = 0;
    virtual void Reconstruct();

    void SetData(int side, int crate, int mod, int ch);
    void SetDelay(int side, int delay) { m_delay[side] = delay; }
    void SetHitpos(double* hitpos) { for(int axis=0; axis<3; ++axis) {m_hitpos[axis] = hitpos[axis];} }
    void SetHitpos(int axis, double hitpos) { m_hitpos[axis] = hitpos; }

    void SetDeadChannel(int side) { m_isUsed[side] = false; }

    const double* GetPos() { return m_pos; }
    const double* GetPosres() { return m_posres; }
    void GetADCconfig(int side, int& crate, int& mod, int& ch);

    bool IsUsed(int side) { return m_isUsed[side]; }
    const short* GetData(int side) { return m_data[side]; }
    double GetPedestal(int side) { return m_ped[side]; }
    double GetPeak(int side) { return m_peak[side]; }
    double GetIntegratedADC(int side) { return m_sumADC[side]; }
    double GetPeakTime(int side) { return m_pt[side]; }
    double GetCFTime(int side) { return m_cft[side]; }
    bool GetErrorFlag(int side) { return m_eflag[side]; }
    bool IsDataComplemented(int side) { return m_isDataComplemented[side]; }
    double GetTimeDiff() { return m_TD; }
    double GetMeanTime() { return m_MT; }

    bool IsHit() { return m_isHit; }
    const double* GetHitPos() { return m_hitpos; }

    void CalculateCFTime(int side); // delays are subtracted in this function
    void Pedestal(int side);
    short Max(int nSmpl, const short* data);
    double Max(int nSmpl, std::vector<double>&  data, int& index);

    void RecRange();
    double GetRange() { return m_range; }

    void SetDataCompletionThreshold(int side, short val) { m_completion_peakThreshold[side] = val; }
    void AsymmetricGaussianEval(int side, std::vector<int>& removePoints);
    bool DataCompletion(int side);
    bool IsIncludeded(std::vector<int> v, int i);

    // Visualization
    void Visualize();
    void Display(int plane);

  protected:
    CosmicRay* m_cosmi;

    double m_pos[3];   // location (x, y, z)
    double m_size[3];  // size (x, y, z)

    // FADC configuration
    bool m_isUsed[2];
    int m_crate[2];
    int m_mod[2];
    int m_ch[2];
    int m_delay[2];   // Channel delay

    short* m_data[2];
    double m_ped[2];
    double m_peak[2];
    double m_sumADC[2];
    double m_pt[2];
    double m_cft[2];
    bool m_eflag[2];

    double m_TD;  // timing difference
    double m_MT;  // mean time

    bool m_isHit;
    double m_hitpos[3]; // hit position (x, y, z)
    double m_posres[3]; // position resolution (x, y, z)

    double m_range;

    double m_completion_peakThreshold[2];
    std::vector<int> m_badPoints[2];
    bool m_isDataComplemented[2];

    // Visualization
    int m_color;          // detector color
    TBox* m_box_det[3]; // detector, (xy, yz, zx) plane
    TBox* m_box_hit[3]; // hit point (xy, yz, zx) plane

};


#endif //CONV_BOTHREADDETECTOR_H
