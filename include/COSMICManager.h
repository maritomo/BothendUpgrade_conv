//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_TRIGGERMANAGER_H
#define CONV_TRIGGERMANAGER_H

#include "TTree.h"
#include "TLine.h"

#include "COSMIC.h"
#include "Visualizer.h"
#include "CosmicRay.h"


/* PMT channel map
 *
 * south (north)
 * -> z
 *
 *  12(13) 14(15) 16(17) 18(19) 20(21) 22(23) : top
 *  -----------------------------------------
 *                    CSI
 *  -----------------------------------------
 *  0(1)   2(3)   4(5)   6(7)   8(9)   10(11) : btm
 *
 */

struct inBR_COSMIC {
    Int_t timeStamp[24];
    Short_t data[24][64];
};

struct outBR_COSMIC {
    Short_t data[12][2][64];
    Float_t ped[12][2];
    Float_t peak[12][2];
    Float_t sumADC[12][2];
    Float_t pt[12][2];
    Float_t cft[12][2];
    Float_t meanT[12];
    Float_t deltaT[12];

    Bool_t isHit[12];
    Float_t hitpos[12][3];
    Short_t nHit[2];
    Short_t hitID[2];

    Short_t trackID;
    Float_t track[2];
};

class COSMICManager : public Visualizer {
  public:
    COSMICManager();
    ~COSMICManager();

    static const int nCOSMIC = 12;
    static const int nTrack = 36;

    void SetBranchAddress(TTree* tin);
    void Branch(TTree* tout);
    void Fill();

    bool Init();
    void Process();
    void HitDecision();
    void Tracking();

    double Distance(COSMIC* crc1, COSMIC* crc2);

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    COSMIC* m_COSMIC[nCOSMIC];
    CosmicRay* m_cosmic_ray;

    inBR_COSMIC m_inBR;
    outBR_COSMIC m_outBR;

    bool m_isInit;
    double m_commonThreshold;
    double m_meanT0[nCOSMIC]; // timing-zero of mean time
    
    int m_nHit[2];
    int m_hitID[2];

    int m_trackID;
    double m_track[2];
};

#endif //CONV_TRIGGERMANAGER_H
