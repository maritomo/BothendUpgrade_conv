//
// Created by Tomoo MARI on 2018/09/18.
//

#ifndef CONV_COSMICTRIGGERSYSTEM_H
#define CONV_COSMICTRIGGERSYSTEM_H

#include "CosmicRayCounter.h"
#include "CsI.h"

#include "TCanvas.h"
#include "TLine.h"
#include "TH1.h"

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


class CosmicTriggerSystem {
  public:
    CosmicTriggerSystem(int runID);
    ~CosmicTriggerSystem();

    bool Init();
    bool Init_map();
    bool Init_channelDelay();
    bool Init_calibConst();
    bool Init_hitCondition();
    bool Init_useCsI();

    void Process();
    void HitDecision();
    void Tracking();

    void ZCal();

    void SetData(int crate, int slot, int ch, const short* data);
    void SetData_CsI(int locID, int side, const short* data);
    void SetTrack();

    int GetNCRC() { return nCRC; }
    int GetNLayer() { return nLayer; }
    int GetNTrack() { return nTrack; }

    int GetNCsI() { return nCSI;}

    int IsInit() { return m_isInit; }
    int GetRunID() { return m_runID; }

    bool GetLocationID(int scintiID, int& layerID, int& ch);
    CosmicRayCounter* GetCRC(int layer, int ch) { return m_crc[layer][ch]; }
    CsI* GetCSI(int locID){return m_csi[locID]; }

    double GetCommonThreshold() { return m_comthr; }

    int GetNhit(int layer) { return m_nHit[layer]; }
    int GetHit(int layer) { return m_hitCh[layer]; }

    int GetTrackID() { return m_trackID; }
    const double* GetTrack(int plane) { return m_track[plane]; }
    double GetTOF() { return m_TOF; }

    // Imitation of the online trigger
    void OnlineHitDecision();
    int* GetNOnlineHit(int layer) { return m_nOnlineHit[layer]; }
    int* IsOnlineTriggered() { return m_isOnlineTriggered; }

    // Visualization
    void GetVisAxis(int plane, int& axis_h, int& axis_v);
    void Visualize();
    void SetTrackLine(int plane);
    void Display();
    void Print(const char* filename);

  private:
    static const int nLayer = 2;  // # of layer (top, bottom)
    static const int nCRC = 6;  // # of cosmic ray counter in 1 layer
    static const int nTrack = 36;
    static const int nCSI = 2716;

    CosmicRayCounter* m_crc[nLayer][nCRC];
    CsI* m_csi[nCSI];

    int m_isInit;
    int m_runID;

    double m_comthr; // common peak threshold

    int m_nHit[2];
    int m_hitCh[2];
    double m_hitpos[2][3];

    int m_trackID;
    double m_track[3][2]; // (xy, zy, xz) plane, (constant, slope)
    double m_TOF; // time of flight

    int m_isRead[2710];

    // Imitation of the online trigger
    int m_nOnlineHit[nLayer][64]; // # of fired channel in online at each sample
    int m_isOnlineTriggered[64]; // online trigger decision at each sample

    // Visualization
    int m_isVis[3];
    double m_world[3][2]; // (x, y, z)

    TCanvas* m_canv;
    TH1* m_frame[3];
    TLine* m_lTrack[3];
};

#endif //CONV_COSMITRIGGER_H
