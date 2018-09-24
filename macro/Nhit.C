#include "global.h"

void Nhit() {

  std::cout << "g_runID\t" << g_runID << "\n";

  TFile* fin = new TFile(Form("../run%d_conv.root", g_runID));
  TTree* tree = (TTree*) fin->Get("tree");

  double ID[2][6];
  double Nhit[2][6];

  double max = 0;
  
  for(int layer=0; layer<2; ++layer) {
    for(int id=0; id<6; ++id) {
      ID[layer][id] = id;
      TCut cut = Form("isHit[%d][%d]", layer, id);
      Nhit[layer][id] = tree->GetEntries(cut);
      if(max < Nhit[layer][id]) max = Nhit[layer][id];
    }
  }
  
  TCanvas* c1 = new TCanvas("c1", "", 1000, 450);
  c1->Divide(2, 1);

  c1->cd(1)->DrawFrame(-0.5, 0, 5.5, max*1.5, ";ID;Nhit");

  TGraph* g[2];
  int color[2] = {kBlue, kRed};

  TString name[2] = {"Bottom", "Top"};
  TLegend* leg = new TLegend(0.6, 0.7, 0.88, 0.88);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);

  for(int layer=0; layer<2; ++layer) {
    g[layer] = new TGraph(6, ID[layer], Nhit[layer]);
    g[layer]->SetTitle(";ID;Nhit");
    g[layer]->SetMarkerStyle(8);
    g[layer]->SetMarkerColor(color[layer]);
    g[layer]->SetLineColor(color[layer]);
    g[layer]->SetLineWidth(2);
    g[layer]->Draw("pl same");
    leg->AddEntry(g[layer], name[layer], "pl");
  }

  leg->Draw();

  c1->cd(2);
  tree->Draw("trackID", "trackID==1||trackID==8||trackID==15||trackID==22||trackID==29||trackID==36");

  gStyle->SetOptStat(0);
  c1->Print(Form("./pict/Nhit_run%d.C", g_runID));

  int nTrackedEvent = tree->GetEntries("trackID>0");
  std::cout << "# of successfully tracked event\t" << nTrackedEvent << Form(" (%.1f%%)\n", (double)nTrackedEvent/tree->GetEntries());
}
