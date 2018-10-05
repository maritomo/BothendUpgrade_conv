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
    CosmicTriggerSystem(int runID, int isCommonThreshold);
    ~CosmicTriggerSystem();

    bool Init();
    bool Init_map();
    bool Init_channelDelay();
    bool Init_calibConst();
    bool Init_hitCondition();
    bool Init_useCsI();

    void HitDecision();
    void Process();
    void Tracking();
    void ZCal();


    void SetData(int crate, int slot, int ch, const short* data);
    void SetData_CsI(int locID, int side, const short* data);
    void SetTrack();

    int GetNCRC() { return m_nCRC; }
    int GetNLayer() { return m_nLayer; }
    int GetNTrack() { return m_nTrack; }
    int GetNCsI() { return m_nCsI;}

    int GetIsInit() { return m_isInit; }
    int GetRunID() { return m_runID; }
    int GetIsCommonThreshold() { return m_isCommonThreshold; }

    bool GetLocationID(int scintiID, int& layerID, int& ch);
    CosmicRayCounter* GetCRC(int layer, int ch) { return m_crc[layer][ch]; }
    CsI* GetCSI(int locID){return m_csi[locID]; }

    double GetCommonThreshold() { return m_comthr; }
    double GetfPedestalSigma() { return m_fPedSig; }

    int GetNhit(int layer) { return m_nHit[layer]; }
    int GetHit(int layer) { return m_hitCh[layer]; }

    int GetTrackID() { return m_trackID; }
    const double* GetTrack(int plane) { return m_track[plane]; }
    double GetTOF() { return m_TOF; }

    // Imitation of the online trigger
    void OnlineHitDecision();
    int* GetNOnlineHit(int layer) { return m_nOnlineHit[layer]; }
    int* GetIsOnlineTriggered() { return m_isOnlineTriggered; }

    // Visualization
    void GetVisAxis(int plane, int& axis_h, int& axis_v);
    void Visualize();
    void SetTrackLine(int plane);
    void Display();
    void Print(const char* filename);

  private:
    static const int m_nLayer = 2;  // # of layer (top, bottom)
    static const int m_nCRC = 6;  // # of cosmic ray counter in 1 layer
    static const int m_nTrack = 36;
    static const int m_nCsI = 2716;

    int m_isInit;
    int m_runID;
    int m_isCommonThreshold;

    CosmicRayCounter* m_crc[m_nLayer][m_nCRC];
    CsI* m_csi[m_nCsI];
    double m_comthr; // common peak threshold
    double m_fPedSig; // peak threshold = fPedSig * pedestalRMS

    int m_nHit[2];
    int m_hitCh[2];
    double m_hitpos[2][3];

    int m_trackID;
    double m_track[3][2]; // (xy, zy, xz) plane, (constant, slope)
    double m_TOF; // time of flight

    int m_isRead[2710];

    // Imitation of the online trigger
    int m_nOnlineHit[m_nLayer][64]; // # of fired channel in online at each sample
    int m_isOnlineTriggered[64]; // online trigger decision at each sample


    // Visualization
    int m_isVis[3];
    double m_world[3][2]; // (x, y, z)

    TCanvas* m_canv;
    TH1* m_frame[3];
    TLine* m_lTrack[3];
};

#endif //CONV_COSMITRIGGER_H
