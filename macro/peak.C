#include "global.h"

void peak() {

  std::cout << "runID\t" << g_runID << "\n";

  TFile* fin = new TFile(Form("../run%d_conv.root", g_runID));
  TTree* tree = (TTree*) fin->Get("tree");

  // Bottom layer
  TH1F* h1[12];

  TCanvas* c1 = new TCanvas("c1", "Bottom layer", 1200, 800);
  c1->Divide(4, 3);

  for(int ch = 0; ch < 6; ++ch) {
    for(int lr = 0; lr < 2; ++lr) {
      int id = 2 * ch + lr;

      c1->cd(id + 1);//->SetLogy();
      h1[id] = new TH1F(Form("h1_%d", id), "", 70, 0, 14000);
      tree->Draw(Form("peak[0][%d][%d]>>h1_%d", ch, lr, id), Form("isHit[0][%d]", ch));
    }
  }
  c1->Print(Form("./pict/run%d/peak_btm.pdf", g_runID));


  // Top layer
  TH1F* h2[12];

  TCanvas* c2 = new TCanvas("c2", "Top layer", 1200, 800);
  c2->Divide(4, 3);

  for(int ch = 0; ch < 6; ++ch) {
    for(int lr = 0; lr < 2; ++lr) {
      int id = 2 * ch + lr;

      c2->cd(id + 1);//->SetLogy();
      h2[id] = new TH1F(Form("h2_%d", id), "", 70, 0, 14000);
      tree->Draw(Form("peak[1][%d][%d]>>h2_%d", ch, lr, id), Form("isHit[1][%d]", ch));
    }
  }

  c2->Print(Form("./pict/run%d/peak_top.pdf", g_runID));
}
