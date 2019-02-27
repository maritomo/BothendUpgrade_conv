//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_VISUALIZATION_H
#define CONV_VISUALIZATION_H

#include "TCanvas.h"
#include "TH1.h"

class Visualizer {
  public:
    Visualizer() {}
    virtual ~Visualizer() {}

    virtual void Visualize() = 0;
    virtual void Display(int plane) = 0;

    void GenerateCanvas();
    void Print(const char* filename);
    void GetVisAxis(int plane, int& axis_h, int& axis_v);

    TCanvas* GetCanvas() { return m_canv; }

  protected:
    bool m_isVis;
    static double m_world[3][2]; // (x, y, z), (min, max)
    static TCanvas* m_canv;
};


#endif //CONV_VISUALIZATION_H
