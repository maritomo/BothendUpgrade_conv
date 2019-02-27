//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>

#include "Visualizer.h"

double Visualizer::m_world[3][2] = {{-1100,1100}, {-2500,2500}, {-300,300}};
TCanvas* Visualizer::m_canv = nullptr;

void Visualizer::GenerateCanvas() {
    if(m_canv == nullptr) {
        m_canv =  m_canv = new TCanvas("m_canv", "Event Display", 1100, 1200);
        m_canv->Divide(2, 1);
    }
}

void Visualizer::Print(const char* filename) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
    m_canv->Print(filename);
}

void Visualizer::GetVisAxis(int plane, int& axis_h, int& axis_v){
    if(plane==0) {          // xy plane->(x, y)
        axis_h = 0;
        axis_v = 1;
    } else if(plane==1) {   // yz plane->(z, y)
        axis_h = 2;
        axis_v = 1;
    } else if(plane==2) {   // zx plane->(x, z)
        axis_h = 0;
        axis_v = 2;
    } else {
        std::cout << "plane 0: xy, 1: zy, 2: xz\n";
    }
}