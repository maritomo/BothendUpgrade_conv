#include "global.h"

void TD() {

  std::cout << "runID\t" << g_runID << "\n";

  TFile* fin = new TFile(Form("../run%d_conv.root", g_runID));
  TTree* tree = (TTree*) fin->Get("tree");

  TH1F* h1[12];

  TCanvas* c1 = new TCanvas("c1", "Timing difference", 1200, 800);
  c1->Divide(4, 3);

  for(int layer = 0; layer < 2; ++layer) {
    for(int ch = 0; ch < 6; ++ch) {
      int id = 6 * layer + ch;
      c1->cd(id + 1);//->SetLogy();
      h1[id] = new TH1F(Form("h1_%d", id), Form("layer%d, ch%d;[ns]",layer,ch), 80, -40, 40);
      tree->Draw(Form("TD[%d][%d]*8>>h1_%d", layer, ch, id), Form("isHit[%d][%d]", layer, ch));
    }
  }
  c1->Print(Form("./pict/run%d/TD.pdf", g_runID));

}
