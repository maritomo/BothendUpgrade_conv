//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_COSMICMEASSYSTEM_H
#define CONV_COSMICMEASSYSTEM_H

#include "TCanvas.h"
#include "TH1.h"
#include "TTree.h"

#include "TreeManager.h"
#include "TriggerManager.h"
#include "CsIManager.h"

struct SystemBranchContainer {
    UInt_t timestamp[3];
};

class CosmicMeasSystem : public TreeManager, public Visualization {
  public:
    CosmicMeasSystem(int nInputTree, TTree** tin, TTree* tout);
    ~CosmicMeasSystem();
    bool Init();
    void Branch();
    void Process();

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    int m_isInit;
    TriggerManager* m_trigMan;
    CsIManager* m_csiMan;
    SystemBranchContainer m_BRsys;
};


#endif //CONV_COSMICMEASSYSTEM_H
