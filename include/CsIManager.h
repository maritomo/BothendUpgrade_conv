//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_CSIMANAGER_H
#define CONV_CSIMANAGER_H

#include "TreeManager.h"
#include "Visualization.h"
#include "CsI.h"

struct CsIBranchContainer {
    Short_t data[2716][2][64];
    Float_t ped[2716][2];
    Float_t peak[2716][2];
    Float_t integ[2716][2];
    Float_t pt[2716][2];
    Float_t cft[2716][2];

    Float_t TD[2716][2];
    Float_t MT[2716][2];

    Short_t isHit[2716];
    Short_t isUsed[2716];
    Short_t nHit[4]; // nline
    Float_t hitpos[2716][3];
};


class CsIManager : public TreeManager, public Visualization {
  public:
    CsIManager();
    ~CsIManager();

    static const int nCSI = 2716;
    static const int nCSI_S = 2240;
    static const int nCSI_L = 476;

    bool Init();
    bool Init_map();
    bool Init_ADCconfig();
    bool Init_channelDelay();
//    bool Init_calibConst();
//    bool Init_hitCondition();
    void Branch();

    void SetData();
    void Process();
    void HitDecision();
    void SetHitPos(double* track);
//    void Tracking();

    // Visualization
    void Visualize();
    void Display(int plane);


  private:
    int m_isInit;
    CsI* m_csi[nCSI];
    CsIBranchContainer m_BRcsi;
};

#endif //CONV_CSIMANAGER_H
