//
// Created by Tomoo MARI on 2018/09/18.
//

#ifndef CONV_COSMICTRIGGERSYSTEM_H
#define CONV_COSMICTRIGGERSYSTEM_H

#include "CosmicRayCounter.h"

#include "TCanvas.h"
#include "TLine.h"
#include "TH1.h"

/* new ID based on their location
 * --> z
 *
 *  6  7  8  9 10 11 : top
 *  ----------------
 *        CSI
 *  ----------------
 *  0  1  2  3  4  5 : btm
 *
 */


class CosmicTriggerSystem {

  public:
    CosmicTriggerSystem(int runID);
    ~CosmicTriggerSystem();

    bool Init();
    bool Init_map();
    bool Init_channelDelay();
    bool Init_calibConst();
    bool Init_hitCondition();

    void Process();
    void HitDecision();
    void Tracking();

    void SetData(int slot, int ch, const short* data);

    int GetNCRC() { return m_nCRC; }
    int GetNLayer() { return m_nLayer; }
    int GetNTrack() { return m_nTrack; }

    int GetIsInit() { return m_isInit; }
    int GetRunID() { return m_runID; }

    bool GetLocationID(int scintiID, int& layerID, int& ch);
    CosmicRayCounter* GetCRC(int layer, int ch) { return m_crc[layer][ch]; }

    double GetCommonThreshold() { return m_comthr; }
    double GetfPedestalSigma() { return m_fPedSig; }

    int GetNhit(int layer) { return m_nHit[layer]; }
    int GetHit(int layer) { return m_hitCh[layer]; }
    double GetHitPos(int layer, int axis) { return m_hitpos[layer][axis]; }

    int GetTrackID() { return m_trackID; }

    void GetVisAxis(int plane, int& axis_h, int& axis_v);
    void Visualize();
    void SetTrackLine(int plane);
    void Display();
    void Print(const char* filename);

  private:
    static const int m_nLayer = 2;  // # of layer (top, bottom)
    static const int m_nCRC = 6;  // # of cosmic ray counter in 1 layer
    static const int m_nTrack = 36;

    int m_isInit;
    int m_runID;

    CosmicRayCounter* m_crc[m_nLayer][m_nCRC];

    double m_comthr; // common peak threshold
    double m_fPedSig; // peak threshold = fPedSig * pedestalRMS

    int m_nHit[2];
    int m_hitCh[2];
    double m_hitpos[2][3];

    int m_trackID;
    double m_track[3][2]; // (xy, zy, xz) plane, (constant, slope)
    double TOF; // time of flight

    int m_isVis[3];
    double m_world[3][2]; // (x, y, z)

    TCanvas* m_canv;
    TH1* m_frame[3];
    TLine* m_lTrack[3];
};

#endif //CONV_COSMITRIGGER_H
