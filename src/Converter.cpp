//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>

#include "Converter.h"

Converter::Converter() {
    m_cosmic_ray = CosmicRay::GetInstance();
    m_CSI_man = new CSIManager();
    m_COSMIC_man = new COSMICManager();
    m_isVis = false;
}

Converter::~Converter() {
    CosmicRay::Delete();
    delete m_COSMIC_man;
    delete m_CSI_man;
    delete m_canv;
}

void Converter::SetInputTree(TChain* tin) {
    m_tin = tin;
    m_CSI_man->SetBranchAddress(m_tin);
    m_COSMIC_man->SetBranchAddress(m_tin);
}

void Converter::SetOutputTree(TTree* tout) {
    m_tout = tout;
    m_CSI_man->Branch(m_tout);
    m_COSMIC_man->Branch(m_tout);
    m_cosmic_ray->Branch(m_tout);
}

void Converter::Convert(int entry) {
    m_tin->GetEntry(entry);

    m_COSMIC_man->Process();
    m_CSI_man->Process();
    m_cosmic_ray->Tracking();

//    m_CSI_man->RecRange();

    m_COSMIC_man->Fill();
    m_CSI_man->Fill();
    m_cosmic_ray->Fill();

    m_tout->Fill();
}

void Converter::Visualize() {
    if(!m_isVis) {
        GenerateCanvas();
        m_COSMIC_man->Visualize();
        m_CSI_man->Visualize();
        m_cosmic_ray->Visualize();
    }
    m_isVis = true;
}

void Converter::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
    if(plane < 0 || plane > 1) {
        std::cout << "plane should be 0 (xy) or 1 (zy)\n";
        return;
    }
    m_canv->cd(plane + 1)->Clear();

    int axis_h, axis_v;
    GetVisAxis(plane, axis_h, axis_v);

    TString title[3] = {";x [mm];y [mm]", ";z [mm];y [mm]", ";x [mm];z [mm]"};
    m_canv->cd(plane + 1)->DrawFrame(m_world[axis_h][0], m_world[axis_v][0],
                                     m_world[axis_h][1], m_world[axis_v][1],
                                     title[plane]);
    m_COSMIC_man->Display(plane);
    m_CSI_man->Display(plane);
    m_cosmic_ray->Display(plane);
}