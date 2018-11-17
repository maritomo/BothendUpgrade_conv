//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_TRIGGERMANAGER_H
#define CONV_TRIGGERMANAGER_H

#include "TLine.h"

#include "TriggerCounter.h"
#include "Visualizer.h"
#include "TreeManager.h"
#include "CosmicRay.h"

/*
 * --> z
 *
 *  6  7  8  9 10 11 : top
 *  ----------------
 *        CSI
 *  ----------------
 *  0  1  2  3  4  5 : btm
 *
 */

struct TriggerBranchContainer {
    Short_t data[12][2][64];
    Float_t ped[12][2];
    Float_t peak[12][2];
    Float_t sumADC[12][2];
    Float_t pt[12][2];
    Float_t cft[12][2];
    Bool_t isDataComplemented[12][2];
    Float_t MT[12];
    Float_t TD[12];

    Bool_t isHit[12];
    Float_t hitpos[12][3];
    Short_t nHit[2];
    Short_t hitID[2];

    Short_t trackID;

    Bool_t isHit_online[12][2][64];
    Short_t nHit_online[2][64];
    Bool_t isTriggered_online[64];
};

class TriggerManager : public Visualizer, public TreeManager {
  public:
    TriggerManager();
    ~TriggerManager();
    void Branch();
    void Fill();

    static const int nTrig = 12;  // # of cosmic ray counters
    static const int nTrack = 36;

    // Initialization
    bool Init();
    bool Init_map();
    bool Init_DAQconfig();
    bool Init_calibration();
    bool Init_hitCondition();

    // Process
    void Process();
    void HitDecision();
    void Tracking();
    void OnlineHitDecision();

    int GetID(int scintiID);
    int GetID(int mod, int ch) { return 6 * mod + ch / 2; }
    int GetSide(int mod, int ch) { return ch % 2; }
    int GetLayer(int id) { return id/6; }

    double Distance(TriggerCounter* crc1, TriggerCounter* crc2);

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    TriggerCounter* m_trig[nTrig];
    TriggerBranchContainer m_BRout;
    CosmicRay* m_cosmi;

    bool m_isInit;
    double m_commonThreshold;
    double m_MT0[nTrig]; // timing-zero of mean time
    
    int m_nHit[2];
    int m_hitID[2];
    int m_trackID;
    
    bool m_isHit_online[nTrig][2][64];
    bool m_isTriggered_online[64];
    double m_nHit_online[2][64];
};

#endif //CONV_TRIGGERMANAGER_H
