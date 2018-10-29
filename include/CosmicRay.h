//
// Created by Tomoo MARI on 2018/10/11.
//

#ifndef CONV_COSMICRAY_H
#define CONV_COSMICRAY_H

#include "TLine.h"

#include "Visualizer.h"
#include "TreeManager.h"

struct CosmicRayBranchContainer {
    Float_t track[3][2];
    Float_t chi2[3];
    Short_t ndf[3];
};

class CosmicRay : public Visualizer, public TreeManager {
  public:
    static CosmicRay* GetInstance();
    static void Delete();
    void Branch();
    void Fill();

    void AddHitPoint(const double* pos, const double* dpos);
    void Tracking();

    void SetIsTracked(bool isTracked) { m_isTracked = isTracked; }
    void SetTrack(int plane, double* track) { for(int k=0; k<2; ++k) m_track[plane][k] = track[k]; }
    void SetTrackID(int trackID) { m_trackID = trackID; }

    bool IsTracked() { return m_isTracked; }
    const double* GetTrack(int plane) { return m_track[plane]; }
    int GetTrackID() { return m_trackID; }

    // Visualization
    void Visualize();
    void Display(int plane);

private:
    CosmicRay();
    ~CosmicRay();
    static CosmicRay* m_cosmi;
    static bool m_isInit;

    CosmicRayBranchContainer m_BRout;

    bool m_isTracked;
    std::vector<double> m_hitpos[3], m_dhitpos[3];
    double m_track[3][2];
    double m_chi2[3];
    int m_ndf[3];

    int m_trackID;

    // Visualization
    TLine* m_trackLine[3];
};


#endif //CONV_COSMICRAY_H
