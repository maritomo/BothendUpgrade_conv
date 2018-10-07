//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>

#include "Visualization.h"

double Visualization::m_world[3][2] = {{-1100,1100}, {-1500,1500}, {-300,300}};
TCanvas* Visualization::m_canv = nullptr;
TH1* Visualization::m_frame[3] = {nullptr, nullptr, nullptr};

void Visualization::Print(const char* filename) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
    m_canv->Print(filename);
}

void Visualization::GetVisAxis(int plane, int& axis_h, int& axis_v){
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