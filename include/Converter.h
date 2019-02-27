//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_CONVERTER_H
#define CONV_CONVERTER_H

#include "TCanvas.h"
#include "TH1.h"
#include "TTree.h"
#include "TChain.h"

#include "COSMICManager.h"
#include "CSIManager.h"


class Converter : public Visualizer {
  public:
    Converter();
    ~Converter();

    void SetInputTree(TChain* tin);
    void SetOutputTree(TTree* tout);
    void Convert(int entry);

    CosmicRay* GetCosmicRay() { return m_cosmic_ray; }
    COSMICManager* GetCOSMICManager() { return m_COSMIC_man; }
    CSIManager* GetCSIManager() { return m_CSI_man; }

    // Visualization
    void Visualize();
    void Display(int plane);

  private:
    TChain* m_tin;
    TTree* m_tout;
    CosmicRay* m_cosmic_ray;
    COSMICManager* m_COSMIC_man;
    CSIManager* m_CSI_man;
};


#endif //CONV_CONVERTER_H
