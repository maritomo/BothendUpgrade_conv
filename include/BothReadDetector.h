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

    virtual void Process() = 0;
    virtual void Reconstruct();

    void SetADCconfig(int side, int crate, int slot, int ch);
    void SetDelay(int side, int delay) { m_delay[side] = delay; }

    void GetADCconfig(int side, int& crate, int& slot, int& ch);
    short* GetData(int side) { if(!m_isUsed) return nullptr; return m_data[side]; }
    double GetPos(int axis) { if(!m_isUsed) return 0; return m_pos[axis]; }
    double GetPed(int side) { if(!m_isUsed) return 0; return m_ped[side]; }
    double GetPeak(int side) { if(!m_isUsed) return 0; return m_peak[side]; }
    double GetInteg(int side) { if(!m_isUsed) return 0; return m_integ[side]; }
    double GetPT(int side) { if(!m_isUsed) return 0; return m_pt[side]; }
    double GetCFT(int side) { if(!m_isUsed) return 0; return m_cft[side]; }
    double GetErrFlag(int side) { if(!m_isUsed) return 0; return m_errflag[side]; }
    double GetTD() { if(!m_isUsed) return 0; return m_TD; }
    double GetMT() { if(!m_isUsed) return 0; return m_MT; }
    int IsHit() { if(!m_isUsed) return 0; return m_isHit; }
    double GetHitPosition(int axis) { if(!m_isUsed) return 0; return m_hitpos[axis]; }

    void GetCFTime(int side); // delays are subtracted in this function
    short GetMax(int nSmpl, const short* data);

    // Visualization
    void Visualize();
    void Display(int plane);

  protected:
    double m_pos[3];   // location (x, y, z)
    double m_size[3];  // size (x, y, z)

    // FADC configuration
    int m_isUsed;
    int m_crate[2];
    int m_slot[2];
    int m_ch[2];
    int m_delay[2];   // Channel delay

    short* m_data[2];
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
    int m_color;          // detector color
    TBox* m_box_det[3]; // detector, (xy, yz, zx) plane
    TBox* m_box_hit[3]; // hit point (xy, yz, zx) plane

};


#endif //CONV_BOTHREADDETECTOR_H
