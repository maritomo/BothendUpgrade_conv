//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_CSIMANAGER_H
#define CONV_CSIMANAGER_H

#include "TreeManager.h"
#include "Visualizer.h"
#include "CsI.h"
#include "CosmicRay.h"

struct CsIBranchContainer {
    Bool_t isUsed[2716][2];

    Short_t data[2716][2][64];
    Float_t ped[2716][2];
    Float_t peak[2716][2];
    Float_t integ[2716][2];
    Float_t pt[2716][2];
    Float_t cft[2716][2];
    Bool_t eflag[2716][2];

    Float_t TD[2716];
    Float_t MT[2716];

    Bool_t isHit[2716];
    Short_t nHit;
    Float_t hitpos[2716][3];
};


class CsIManager : public TreeManager, public Visualizer {
  public:
    CsIManager();
    ~CsIManager();
    void Branch();
    void Fill();

    static const int nCSI = 2716;
    static const int nCSI_S = 2240;
    static const int nCSI_L = 476;

    bool Init();
    bool Init_map();
    bool Init_DAQconfig();
//    bool Init_calibConst();

    void Process();
    void Tracking();

    void RecHitZpos();

    const double* GetTrack() { return m_track; }

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    bool m_isInit;
    CsI* m_csi[nCSI];
    CsIBranchContainer m_BRout;
    CosmicRay* m_cosmi;

    int m_nHit;
    double m_track[2];
};

#endif //CONV_CSIMANAGER_H
