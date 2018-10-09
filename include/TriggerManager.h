//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_TRIGGERMANAGER_H
#define CONV_TRIGGERMANAGER_H

#include "TLine.h"

#include "TreeManager.h"
#include "Visualization.h"
#include "CosmicRayCounter.h"

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
    Float_t integ[12][2];
    Float_t pt[12][2];
    Float_t cft[12][2];

    Float_t MT[12];
    Float_t TD[12];

    Short_t isHit[12];
    Float_t posHit[12];
// Short_t data[2][6][2][64];
//    Float_t ped[2][6][2];
//    Float_t peak[2][6][2];
//    Float_t integ[2][6][2];
//    Float_t pt[2][6][2];
//    Float_t cft[2][6][2];
//
//    Float_t MT[2][6];
//    Float_t TD[2][6];
//
//    Short_t isHit[2][6];
//    Float_t posHit[2][6];
    Short_t nHit[2];
    Short_t hitID[2];

    Short_t trackID;
    Float_t track[3][2];
    Float_t TOF;

//    Short_t isOnlineHit[2][6][2][64];
    Short_t isOnlineHit[12][2][64];
    Short_t nOnlineHit[2][64];
    Short_t isOnlineTriggered[64];
};

class TriggerManager : public TreeManager, public Visualization {
  public:
    TriggerManager();
    ~TriggerManager();

//    static const int nLayer = 2;  // # of layer (top, bottom)
//    static const int nCRC = 6;  // # of cosmic ray counter in 1 layer
    static const int nCRC = 12;  // # of cosmic ray counters
    static const int nTrack = 36;

    // Initialize
    bool Init();
    bool Init_map();
    bool Init_DAQconfig();
    bool Init_calibConst();
    bool Init_hitCondition();

    void Branch();

    // Process
    void Process();
    void HitDecision();
    void Tracking();
    void OnlineHitDecision();
    void Fill();

//    bool GetLocationID(int scintiID, int& layerID, int& chID);
    int GetID(int scintiID);
    int GetID(int slot, int ch) { return 6 * slot + ch / 2; }
    int GetSide(int slot, int ch) { return ch % 2; }
    int GetLayer(int id) { return id/6; }

    double Distance(CosmicRayCounter* crc1, CosmicRayCounter* crc2);

    // Visualization
    void Visualize();
    void Display(int plane);
    void SetTrackLine(int plane);

  private:
//    CosmicRayCounter* m_crc[nLayer][nCRC];
    CosmicRayCounter* m_crc[nCRC];
    CosmicRayCounter* m_crcHit[2]; // Hit counters (top, bottom)
    TriggerBranchContainer m_BRout;

    int m_isInit;
    double m_commonThreshold;

    // Visualization
    TLine* m_lTrack[3];
};

#endif //CONV_TRIGGERMANAGER_H
