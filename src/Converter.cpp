//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>

#include "Converter.h"

Converter::Converter() {
    m_cosmi = CosmicRay::GetInstance();
    m_csiMan = new CsIManager();
    m_trigMan = new TriggerManager();
    Branch();
    m_isVis = 0;
}

Converter::~Converter() {
    CosmicRay::Delete();
    delete m_trigMan;
    delete m_csiMan;
    delete m_canv;
}

void Converter::Branch() {
    m_statusTree->Branch("timestamp", m_BRout.timestamp, "timestamp[3]/i");
}

void Converter::Fill(){
    for(int crate=0; crate<3; ++crate) {
        m_BRout.timestamp[crate] = m_BRin[crate].Timestamp;
    }
}

void Converter::Convert(){
    m_csiMan->Process();
    m_trigMan->Process();
    m_csiMan->RecHitPosition();

    m_trigMan->Fill();
    m_csiMan->Fill();
    m_cosmi->Fill();
    Fill();
}

void Converter::Visualize(){
    if(!m_isVis) {
        m_canv = new TCanvas("m_canv", "Event Display", 900, 600);
        m_canv->Divide(2, 1);
    }
    m_trigMan->Visualize();
    m_csiMan->Visualize();
    m_cosmi->Visualize();
    m_isVis = 1;
}

void Converter::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
    if(plane<0 || plane>1) {
        std::cout << "plane should be 0 (xy) or 1 (zy)\n";
        return;
    }
    m_canv->cd(plane+1)->Clear();

    int axis_h, axis_v;
    GetVisAxis(plane, axis_h, axis_v);

    TString title[3] = {";x [mm];y [mm]", ";z [mm];y [mm]", ";x [mm];z [mm]"};
    m_canv->cd(plane + 1)->DrawFrame(m_world[axis_h][0], m_world[axis_v][0],
                                     m_world[axis_h][1], m_world[axis_v][1],
                                     title[plane]);
    m_trigMan->Display(plane);
    m_csiMan->Display(plane);
    m_cosmi->Display(plane);
}