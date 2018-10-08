//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>
#include <fstream>
#include "CsIManager.h"

CsIManager::CsIManager() {
    if(!Init()) {
        std::cout << "CsIManager initialization failed\n";
        m_isInit = 0;
    } else {
        std::cout << "CsIManager initialized\n";
        m_isInit = 1;
    }
    m_isVis = 0;
}

CsIManager::~CsIManager() {
    for(int id=0; id < nCSI; ++id) {
        delete m_csi[id];
    }
}

bool CsIManager::Init() {
    m_isInit = 0;
    std::cout << "\nInitializing CsI...\n";

    if(!Init_map()) return false;
    if(!Init_ADCconfig()) return false;
    Branch();

    std::cout << "...fin\n";
    m_isInit = 1;
    return true;
}

bool CsIManager::Init_map() {

    std::string filename = "./data/map_csi.txt";
    std::ifstream ifs(filename.c_str());
    if (!ifs) {
        std::cout << filename << " not found\n";
        return false;
    }

    int locID, lineID, crysID, size;
    double posx, posy;
    while(ifs >> lineID >> locID >> crysID >> posx >> posy >> size) {
        m_csi[locID] = new CsI(locID, crysID, lineID, posx, posy, size);
    }
    ifs.close();

    std::cout << "CsI map                         [OK]\n";
    return true;
}

bool CsIManager::Init_ADCconfig() {

    // ADC channels
    for(int side = 0; side < 2; side++) {
        std::string filename;
        if(side == 0)
            filename = "./data/use_csi_mppc.txt";
        if(side == 1)
            filename = "./data/use_csi.txt";

        std::ifstream ifs(filename.c_str());
        if (!ifs) {
            std::cout << " not found\n";
            return false;
        }

        int locID, crate, slot, ch;
        while (ifs >> locID >> crate >> slot >> ch) {
            m_csi[locID]->SetADCconfig(side, crate, slot, ch);
        }
    }

    // Digital delays
    for(int id=0; id < nCSI; ++id) {
        for(int side=0; side < 2; ++side) {
            int delay = 15;
            if(m_csi[id]->IsUsed())
                m_csi[id]->SetDelay(side, delay);
        }
    }

    return true;
}

//    bool Init_calibConst();
//    bool Init_hitCondition();

void CsIManager::Branch() {
    m_tout->Branch("csi.isUsed", m_BRout.isUsed, "csi.isUsed[2716]/S");

    m_tout->Branch("csi.data", m_BRout.data, "csi.data[2716][2][64]/S");
    m_tout->Branch("csi.ped", m_BRout.ped, "csi.ped[2716][2]/F");
    m_tout->Branch("csi.peak", m_BRout.peak, "csi.peak[2716][2]/F");
    m_tout->Branch("csi.integ", m_BRout.integ, "csi.integ[2716][2]/F");
    m_tout->Branch("csi.pt", m_BRout.pt, "csi.pt[2716][2]/F");
    m_tout->Branch("csi.cft", m_BRout.cft, "csi.cft[2716][2]/F");

    m_tout->Branch("csi.TD", m_BRout.TD, "csi.TD[2716]/F");
    m_tout->Branch("csi.MT", m_BRout.MT, "csi.MT[2716]/F");

    int nline = 4; // what's this ?
    m_tout->Branch("csi.isHit", m_BRout.isHit, "csi.isHit[2716]/S");
    m_tout->Branch("csi.nHit", m_BRout.nHit, Form("csi.nHit[%d]/S", nline));
    m_tout->Branch("csi.hitpos", m_BRout.hitpos, "csi.hitpos[2716][3]/F");
}

void CsIManager::Process() {
    HitDecision();
    Fill();
}

void CsIManager::HitDecision() {
    for(int id = 0; id < nCSI; ++id){
        m_csi[id]->HitDecision();
    }
}

void CsIManager::Fill(){
    for(int id=0; id<nCSI; ++id) {
        for(int side=0; side<2; ++side) {
            for(int smpl=0; smpl<64; ++smpl) {
//                m_BRout.data[id][side][smpl] = m_csi[id]->GetData(side)[smpl];
            }
        }
    }
}

void CsIManager::Visualize() {
    for(int i=0; i<nCSI; ++i) {
        m_csi[i]->Visualize();
    }
    m_isVis = 1;
}

void CsIManager::Display(int plane) {
    if(!m_isVis) {
        std::cout << "Visualization not ready\n";
        return;
    }
    for(int i=0; i<nCSI; ++i) {
        m_csi[i]->Display(plane);
    }
    m_canv->Update();
    m_canv->Modified();
}
