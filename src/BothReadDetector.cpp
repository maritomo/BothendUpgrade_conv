//
// Created by Tomoo MARI on 2018/09/20.
//

#include <TString.h>
#include <iostream>
#include <cmath>
#include <TGraph.h>

#include "BothReadDetector.h"

BothReadDetector::~BothReadDetector() {
    for(int plane = 0; plane<3; ++plane) {
        delete m_box_det[plane];
        delete m_box_hit[plane];
    }
}

void BothReadDetector::Reconstruct() {
    for(int side = 0; side<2; ++side) {
        CalculateCFTime(side);
        if(DataCompletion(side)) CalculateCFTime(side); // Retry
    }
    m_TD = m_cft[0] - m_cft[1];
    m_MT = 0.5 * (m_cft[0] + m_cft[1]);
}

void BothReadDetector::SetData(int side, int crate, int mod, int ch) {
    if((side<0 || side>1) || (crate<3 || crate>5) ||
       (mod<0 || mod>15) || (ch<0 || ch>15)) {
        m_data[side] = new short[64];
        for(int smpl = 0; smpl<64; ++smpl) {
            m_data[side][smpl] = 0;
        }
        m_isUsed[side] = 0;
        return;
    }

    m_crate[side] = crate;
    m_mod[side] = mod;
    m_ch[side] = ch;

    m_data[side] = m_BRin[crate - 3].Data[mod][ch];
    m_isUsed[side] = 1;
}

void BothReadDetector::GetADCconfig(int side, int& crate, int& mod, int& ch) {
    crate = m_crate[side];
    mod = m_mod[side];
    ch = m_ch[side];
}

void BothReadDetector::CalculateCFTime(int side) {
    m_peak[side] = 0;
    m_sumADC[side] = 0;
    int ipeak = 0;
    int n = 64;

    int time[64];
    for(int i = 0; i<64; ++i) time[i] = i;

    Pedestal(side);

    // Skip first bin
    for(int i = 1; i<n; ++i) {
        if(m_peak[side]<m_data[side][i]) {
            m_peak[side] = m_data[side][i];
            ipeak = i;
        }
        m_sumADC[side] += m_data[side][i] - m_ped[side];
    }

    float yb = m_data[side][ipeak - 1];
    float y = m_data[side][ipeak];
    float yf = m_data[side][ipeak + 1];

    m_pt[side] = ipeak - (yf - yb) / 2 / (yf + yb - 2 * y);
    m_pt[side] -= m_delay[side];
    m_pt[side] *= 8;

    m_peak[side] = y - (yf - yb) * (yf - yb) / 8 / (yf + yb - 2 * y);
    m_peak[side] -= m_ped[side];

    if(ipeak==0 || ipeak==n - 1) {
        m_cft[side] = 0;
        m_eflag[side] = 1;
        return;
    }

    float threshold = m_peak[side] / 2 + m_ped[side];
    for(int i = ipeak; i>0; --i) {
        if(m_data[side][i]>threshold && m_data[side][i - 1]<threshold) {
            m_cft[side] = time[i - 1] + (float) (time[i] - time[i - 1]) /
                                        (m_data[side][i] - m_data[side][i - 1]) *
                                        (threshold - m_data[side][i]);
            m_cft[side] -= m_delay[side];
            m_cft[side] *= 8;
            m_eflag[side] = 0;
            return;
        }
    }

    m_eflag[side] = 2;
}

void BothReadDetector::Pedestal(int side) {
    m_ped[side] = 0;
    int nPed = 10;
    // Skip first bin
    for(int smpl = 1; smpl<1 + nPed; ++smpl) {
        m_ped[side] += (double) m_data[side][smpl] / nPed;
    }
}

short BothReadDetector::Max(int nSmpl, const short* data) {
    short max = -10000;
    for(int i = 0; i<nSmpl; ++i) {
        if(max<data[i]) max = data[i];
    }
    return max;
}

double BothReadDetector::Max(int nSmpl, std::vector<double>&  data, int& index) {
    double max = -1.e10;
    for(int i = 0; i<nSmpl; ++i) {
        if(max<data[i]) {
            max = data[i];
            index = i;
        }
    }
    return max;
}

void BothReadDetector::RecRange() {

    if(!m_cosmi->IsSingleTrack()) {
        m_range = 0;
        return;
    }

    double bound_x[2] = {m_pos[0] - 0.5 * m_size[0], m_pos[0] + 0.5 * m_size[0]};
    double bound_y[2] = {m_pos[1] - 0.5 * m_size[1], m_pos[1] + 0.5 * m_size[1]};
    double bound_z[2] = {m_pos[2] - 0.5 * m_size[2], m_pos[2] + 0.5 * m_size[2]};
    std::vector<double> x, y, z;

    // search hit points from x-y view
    for(int side = 0; side<2; ++side) {
        double coord_y = m_cosmi->GetVerticalCoordinate(0, bound_x[side]);
        if(coord_y>bound_y[0] && coord_y<bound_y[1]) {
            x.push_back(bound_x[side]);
            y.push_back(coord_y);
        }
        double coord_x = m_cosmi->GetHorizontalCoordinate(0, bound_y[side]);
        if(coord_x>bound_x[0] && coord_x<bound_x[1]) {
            x.push_back(coord_x);
            y.push_back(bound_y[side]);
        }
    }

    if((int) x.size()!=2) return;

    // search hit points from z-y view
    for(int i = 0; i<2; ++i) {
        double coord_z = m_cosmi->GetHorizontalCoordinate(1, y[i]);
        if(coord_z<bound_z[0] || coord_z>bound_z[1]) return;
        z.push_back(coord_z);
    }

    m_range = sqrt(
            (x[0] - x[1]) * (x[0] - x[1]) +
            (y[0] - y[1]) * (y[0] - y[1]) +
            (z[0] - z[1]) * (z[0] - z[1])
    );
}


////////////////////////////////////////////////
/// Data completion
////////////////////////////////////////////////

bool BothReadDetector::DataCompletion(int side) {
    m_isDataComplemented[side] = false;
    m_badPoints[side].clear();
    if(m_peak[side]<m_completion_peakThreshold[side]) return false;

    for(int i=0; i<63; ++i) {
        double dif = m_data[side][i+1]-m_data[side][i];
        // fall
        if(fabs(dif) > 0.5*m_peak[side]) {
            if(i>9 && i<54) {
                m_badPoints[side].push_back(i+1);
            } else {
                m_data[side][i+1] = (short) m_ped[side];
            }
            m_isDataComplemented[side] = true;
            ++i;
        }
    }
    AsymmetricGaussianEval(side, m_badPoints[side]);
    return m_isDataComplemented[side];
}

void BothReadDetector::AsymmetricGaussianEval(int side, std::vector<int>& removePoints) {

    if(removePoints.size()==0) return;

    std::vector<double> x, y;
    for(int i=0; i<64; ++i) {
        if(IsIncludeded(removePoints, i)) continue;
        x.push_back(i);
        y.push_back((double)m_data[side][i]);
    }

    TGraph wfm((int)x.size(), &x[0], &y[0]);
    wfm.SetMarkerStyle(8);
    TF1 fG("fG", "gaus");

    int i_peak;
    fG.SetParameters(Max(y.size(), y, i_peak), m_pt[side], m_pt[side]-m_cft[side]);
    wfm.Fit("fG", "Q", "", i_peak-2, i_peak+2);
    double mean = fG.GetParameter(1);
    double sigma = fG.GetParameter(2);

    TF1 fAG("fAG", "[0]*exp(-(x-[1])*(x-[1])/2/[2]/[2]/(1+[3]*(x-[1]))/(1+[3]*(x-[1])))");
    fAG.SetParameters(fG.GetParameter(0), mean, sigma, 0);
    wfm.Fit("fAG", "Q", "", i_peak-4, i_peak+4);

    for(std::vector<int>::iterator itr = removePoints.begin(); itr!=removePoints.end(); ++itr) {
        m_data[side][*itr]=(short) fAG.Eval(*itr);
    }
}

bool BothReadDetector::IsIncludeded(std::vector<int> v, int i){
    int index = 0;
    for(std::vector<int>::iterator itr = v.begin(); itr!=v.end(); ++itr) {
        if(*itr==i) return true;
        ++index;
    }
    return false;
}

////////////////////////////////////////////////
/// Visuallization
////////////////////////////////////////////////

void BothReadDetector::Visualize() {
    if(m_isVis) {
        std::cout << "Already visualized\n";
        return;
    }

    if(!m_isUsed[0] && !m_isUsed[1]) m_color = kGray;

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis
    for(int plane = 0; plane<3; ++plane) {
        GetVisAxis(plane, axis_h, axis_v);
        m_box_det[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                    m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
        m_box_det[plane]->SetLineColor(m_color);
        m_box_det[plane]->SetFillStyle(0);

        m_box_hit[plane] = new TBox(m_pos[axis_h] - 0.5 * m_size[axis_h], m_pos[axis_v] - 0.5 * m_size[axis_v],
                                    m_pos[axis_h] + 0.5 * m_size[axis_h], m_pos[axis_v] + 0.5 * m_size[axis_v]);
        m_box_hit[plane]->SetFillColor(m_color);
    }

    m_isVis = 1;
}

void BothReadDetector::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
    m_box_det[plane]->Draw("same");
    if(!m_isHit) return;

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis
    GetVisAxis(plane, axis_h, axis_v);

    m_box_hit[plane]->SetX1(m_hitpos[axis_h] - m_posres[axis_h]);
    m_box_hit[plane]->SetX2(m_hitpos[axis_h] + m_posres[axis_h]);
    m_box_hit[plane]->SetY1(m_hitpos[axis_v] - m_posres[axis_v]);
    m_box_hit[plane]->SetY2(m_hitpos[axis_v] + m_posres[axis_v]);
    m_box_hit[plane]->Draw("same");
}
