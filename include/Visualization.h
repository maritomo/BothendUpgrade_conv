//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_VISUALIZATION_H
#define CONV_VISUALIZATION_H

#include "TCanvas.h"
#include "TH1.h"

class Visualization {
  public:
    Visualization() {}
    virtual ~Visualization() {}

    virtual void Visualize() = 0;
    virtual void Display(int plane) = 0;

    void Print(const char* filename);
    void GetVisAxis(int plane, int& axis_h, int& axis_v);

  protected:
    int m_isVis;
    static double m_world[3][2]; // (x, y, z), (min, max)
    static TCanvas* m_canv;
    static TH1* m_frame[3]; // (xy, zy, xz)
};


#endif //CONV_VISUALIZATION_H
