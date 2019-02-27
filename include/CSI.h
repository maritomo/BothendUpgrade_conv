//
// Created by Tomoo MARI on 2018/09/24.
//

#ifndef CONV_CSI_H
#define CONV_CSI_H

#include "BothReadDetector.h"


class CSI : public BothReadDetector {
  public:
    CSI(int locID, double posx, double posy, int size);
    ~CSI();

    void Process();
    void Calibration();
    void HitDecision();

    bool IsCalibrated() { return m_isCalib; }

    void SetSumID(int sumID) { m_sumID = sumID; }
    void SetHybridID(int hybID) { m_hybID = hybID; }

    void SetEdepCalibConst(double cEne);
    void SetEdepThreshold(double thr) { m_EdepThreshold = thr; }

    void SetZhitTable(const double* z);
    void SetZsurface(double zSurface);

    int GetLocationID() { return m_locID; }
    int GetSumID() { return m_sumID; }
    int GetHybridID() { return m_hybID; }

    double GetEnergyDeposit() { return m_Edep; }
    const double* GetGlobalZhit() { return m_globalZhit; }
    double GetZsurface() { return m_zSurface; }

  private:
    static const int nCSI = 2716;
    static const int nCSI_S = 2240;
    static const int nCSI_L = 476;

    int m_locID; // same as location ID
    int m_sumID; // MPPC sum ID
    int m_hybID; // MPPC hybrid ID

    bool m_isCalib;

    double m_cEne;
    double m_Edep;
    double m_EdepThreshold;

    double m_zSurface;
    double m_globalZhit[36];
};


#endif //CONV_CSI_H
