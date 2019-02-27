//
// Created by Tomoo MARI on 2018/10/11.
//

#ifndef CONV_COSMICRAY_H
#define CONV_COSMICRAY_H

#include "TTree.h"
#include "TLine.h"

#include "Visualizer.h"

struct outBR_CosmicRay {
    Float_t track[3][2];
    Float_t chi2[3];
    Short_t ndf[3];
};

class CosmicRay : public Visualizer {
  public:
    static CosmicRay* GetInstance();
    static void Delete();

    void Branch(TTree* tout);
    void Fill();

    void SetTrackID(int trackID) { m_trackID = trackID; };
    void AddHitPoint(const double* pos, const double* dpos);
    void AddHitPoint(int axis, double pos, double dpos);
    void Tracking();
    void Clear();

    const double* GetTrack(int plane) { return m_track[plane]; }
    int GetTrackID() { return m_trackID; }

    double GetVerticalCoordinate(int plane, double coord_h) {
        return m_track[plane][1] * coord_h + m_track[plane][0];
    }
    double GetHorizontalCoordinate(int plane, double coord_v) {
        return (coord_v - m_track[plane][0]) / m_track[plane][1];
    }

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    CosmicRay();
    ~CosmicRay();

    static CosmicRay* m_cosmic_ray;
    static bool m_isInit;

    outBR_CosmicRay m_outBR;

    std::vector<double> m_hitpos[3], m_dhitpos[3];

    int m_trackID;
    double m_track[3][2];
    double m_chi2[3];
    int m_ndf[3];

    // Visualization
    TLine* m_trackLine[3];
};


#endif //CONV_COSMICRAY_H
