//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_CONVERTER_H
#define CONV_CONVERTER_H

#include "TCanvas.h"
#include "TH1.h"
#include "TTree.h"

#include "TreeManager.h"
#include "TriggerManager.h"
#include "CsIManager.h"

struct SystemBranchContainer {
    UInt_t timestamp[3];
};

class Converter : public Visualizer, public TreeManager {
  public:
    Converter();
    ~Converter();

    void Branch();
    void Convert();
    void Fill();

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    TriggerManager* m_trigMan;
    CsIManager* m_csiMan;
    SystemBranchContainer m_BRout;
    CosmicRay* m_cosmi;
};


#endif //CONV_CONVERTER_H
