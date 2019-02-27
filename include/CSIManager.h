//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_CSIMANAGER_H
#define CONV_CSIMANAGER_H

#include "TChain.h"
#include "TTree.h"

#include "Visualizer.h"
#include "CSI.h"
#include "CosmicRay.h"


// Input branches of CSI PMT
struct inBR_CSI {
    Int_t timeStamp[3740];
    Short_t data[3740][64];
};


// Output branches of CSI PMT/MPPC
struct outBR_CSI {
    Int_t timeStamp[2716];

    Short_t data[2716][2][64];
    Float_t ped[2716][2];
    Float_t peak[2716][2];
    Float_t sumADC[2716][2];
    Float_t pt[2716][2];
    Float_t cft[2716][2];
    Float_t deltaT[2716];
    Float_t meanT[2716];

    Bool_t isHit[2716];
    Short_t nHit;
    Float_t hitpos[2716][3];
    Float_t localZ[2716];

    Float_t Edep[2716];
    Float_t range[2716];

    Float_t track[2];
    Float_t chi2;
    Int_t ndf;
};


class CSIManager : public Visualizer {
  public:
    CSIManager();
    ~CSIManager();

    static const int nCSI = 2716;
    static const int nCSI_S = 2240;
    static const int nCSI_L = 476;
    static const int nHyb = 1024;

    void SetBranchAddress(TChain* tin);
    void Branch(TTree* tout);
    void Fill();

    bool Init();
    void Process();
    void Tracking();
    void RecZhit();
    void RecRange();

    const double* GetTrack() { return m_track; }
    int GetNhit() { return m_nHit; }

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    bool m_isInit;

    CSI* m_CSI[nCSI];
    CosmicRay* m_cosmic_ray;

    inBR_CSI m_inBR;
    outBR_CSI m_outBR;

    int m_nHit;
    double m_track[2];
    double m_chi2;
    int m_ndf;
};

#endif //CONV_CSIMANAGER_H
