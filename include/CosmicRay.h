//
// Created by Tomoo MARI on 2018/10/11.
//

#ifndef CONV_COSMICRAY_H
#define CONV_COSMICRAY_H

#include "TLine.h"

#include "Visualizer.h"
#include "TreeManager.h"

struct CosmicRayBranchContainer {
    Short_t trackID;
    Float_t track[3][2];
    Float_t hitTime[2];
};

class CosmicRay : public Visualizer, public TreeManager {
  public:
    static CosmicRay* GetInstance();
    static void Delete();
    void Branch();
    void Fill();

    void SetTrackID(int trackID) { m_trackID = trackID; }
    void SetTrack(int plane, double* track) { for(int k=0; k<2; ++k) m_track[plane][k] = track[k]; }
    void SetHitTime(int layer, double hitTime) { m_hitTime[layer] = hitTime; }
    double* GetTrack(int plane) { return m_track[plane]; }
    double* GetHitTime(int layer) { return m_hitTime; }

    void Visualize();
    void Display(int plane);

    // Visualization

private:
    CosmicRay();
    ~CosmicRay();
    static CosmicRay* m_cosmi;
    static bool m_isInit;

    CosmicRayBranchContainer m_BRout;
    int m_trackID;
    double m_track[3][2];
    double m_hitTime[2];

    // Visualization
    TLine* m_trackLine[3];
};


#endif //CONV_COSMICRAY_H
