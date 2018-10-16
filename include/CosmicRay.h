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
    Float_t csiTrack[2];
};

class CosmicRay : public Visualizer, public TreeManager {
  public:
    static CosmicRay* GetInstance();
    static void Delete();
    void Branch();
    void Fill();

    void SetIsTracked(bool isTracked) { m_isTracked = isTracked; }
    void SetTrack(int plane, double* track) { for(int k=0; k<2; ++k) m_track[plane][k] = track[k]; }

    void SetIsTracked_CsI(bool isTracked) { m_isTracked_CsI = isTracked; }
    void SetCsITrack(double* track) { for(int k=0; k<2; ++k) m_csiTrack[k] = track[k]; }

    bool IsTracked() { return m_isTracked; }
    bool IsTracked_CsI() { return m_isTracked_CsI; }
    const double* GetTrack(int plane) { return m_track[plane]; }
    const double* GetCsITrack() { return m_csiTrack; }

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
    double m_track[3][2];

    bool m_isTracked_CsI;
    double m_csiTrack[2];

    // Visualization
    TLine* m_trackLine[3];
};


#endif //CONV_COSMICRAY_H
