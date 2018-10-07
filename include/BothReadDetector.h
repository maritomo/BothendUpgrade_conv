//
// Created by Tomoo MARI on 2018/09/20.
//

#ifndef CONV_BOTHREADDETECTOR_H
#define CONV_BOTHREADDETECTOR_H

#include "TBox.h"

#include "TreeManager.h"
#include "Visualization.h"

class BothReadDetector : public TreeManager, public Visualization {
  public:
    virtual ~BothReadDetector();

    void SetADCconfig(int side, int crate, int slot, int ch);
    void SetDelay(int side, int delay) { m_delay[side] = delay; }
    virtual void SetData(int side);

    void GetADCconfig(int side, int& crate, int& slot, int& ch);
    double GetPos(int axis) { return m_pos[axis]; }
    const int* GetData(int side) { return m_data[side]; }
    double GetPed(int side) { return m_ped[side]; }
    double GetPeak(int side) { return m_peak[side]; }
    double GetInteg(int side) { return m_integ[side]; }
    double GetPT(int side) { return m_pt[side]; }
    double GetCFT(int side) { return m_cft[side]; }
    double GetErrFlag(int side) { return m_errflag[side]; }

    double GetTD() { return m_TD; }
    double GetMT() { return m_MT; }

    int IsHit() { return m_isHit; }
    double GetHitPos(int axis) { return m_hitpos[axis]; }

    void GetCFTime(int side);
    short GetMax(int nSmpl, const short* data);

    // Visualization
    void Visualize();
    void Display(int plane);

  protected:
    double m_pos[3];   // location (x, y, z)
    double m_size[3];  // size (x, y, z)
    int m_delay[2];   // Channel delay

    // FADC configuration
    int m_crate[2];
    int m_slot[2];
    int m_ch[2];
    int m_isUsed;

    short* m_pdata[2];
    int m_data[2][64];
    double m_ped[2];
    double m_peak[2];
    double m_integ[2];
    double m_pt[2];
    double m_cft[2];
    int m_errflag[2];

    double m_TD;  // timing difference
    double m_MT;  // mean time

    int m_isHit;
    double m_hitpos[3]; // hit position (x, y, z)
    double m_posres[3]; // position resolution (x, y, z)

    // Visualization
    int m_col;          // detector color
    TBox* m_box_det[3]; // detector, (xy, yz, zx) plane
    TBox* m_box_hit[3]; // hit point (xy, yz, zx) plane

};


#endif //CONV_BOTHREADDETECTOR_H
