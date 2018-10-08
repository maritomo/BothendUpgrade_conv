//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>

#include "CosmicMeasSystem.h"

CosmicMeasSystem::CosmicMeasSystem(int nInputTree, TTree** tin, TTree* tout) {

    SetInputTrees(nInputTree, tin);
    SetOutputTree(tout);

    m_trigMan = new TriggerManager();
    m_csiMan = new CsIManager();

    if(!Init()) {
        std::cout << "CosmicMeasSystem initialization failed\n";
        m_isInit = 0;
    } else {
        std::cout << "CosmicMeasSystem initialized\n";
        m_isInit = 1;
    }

    m_isVis = 0;
}

CosmicMeasSystem::~CosmicMeasSystem() {
    delete m_trigMan;
    delete m_csiMan;
    delete m_canv;
}

bool CosmicMeasSystem::Init() {
    Branch();
    return true;
}

void CosmicMeasSystem::Branch() {
    m_tout->Branch("timestamp", m_BRout.timestamp, "timestamp[3]/i");
}

void CosmicMeasSystem::Process() {
    m_trigMan->Process();
    m_csiMan->Process();
}

void CosmicMeasSystem::Visualize(){
    if(!m_isVis) {
        m_canv = new TCanvas("m_canv", "Event Display", 900, 600);
        m_canv->Divide(2, 1);
    }
    m_trigMan->Visualize();
    m_csiMan->Visualize();
    m_isVis = 1;
}

void CosmicMeasSystem::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
    if(plane<0 || plane>1) {
        std::cout << "Plane should be 0 (xy) or 1 (zy)\n";
        return;
    }

    m_canv->cd(plane+1)->Clear();

    int axis_h; // index of horizontal axis
    int axis_v; // index of vertical axis
    GetVisAxis(plane, axis_h, axis_v);

    TString title[3] = {";x [mm];y [mm]", ";z [mm];y [mm]", ";x [mm];z [mm]"};
    m_canv->cd(plane + 1)->DrawFrame(m_world[axis_h][0], m_world[axis_v][0],
                                     m_world[axis_h][1], m_world[axis_v][1],
                                     title[plane]);
    m_trigMan->Display(plane);
    m_csiMan->Display(plane);
}