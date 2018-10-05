#include "global.h"
#include <cmath>
void draw(int entry) {

  std::cout << "runID\t" << g_runID << "\n";
  std::cout << "EventNo\t" << entry << "\n";

  TFile* fin = new TFile(Form("../run%d_conv.root", g_runID));
  TTree* tree = (TTree*) fin->Get("tree");


  TCut cut = "trackID == 1";


  // top layer
  TH2F* h1[12];
  int maxcount = pow(2, 14) - 1;

  tree->Draw(">>ls1", cut);
  TEventList* ls = (TEventList*) gDirectory->Get("ls1");


  TCanvas* c1 = new TCanvas("c1", "Top layer", 1200, 800);
  c1->Divide(4, 3);

  for(int ch=0; ch<12; ++ch) {
    c1->cd(ch+1);
    h1[ch] = new TH2F(Form("h1_%d",ch),"",64,0,64,maxcount,0,maxcount);
    tree->Draw(Form("data[1][%d]:Iteration$>>h1_%d",ch,ch),
	       Form("Entry$==%d",ls->GetEntry(entry)));
  }

  c1->Print(Form("./pict/wfm_run%d_%d_top.C", g_runID, entry));


  // bottom layer
  TH2F* h2[12];

  tree->Draw(">>ls2", cut);
  ls = (TEventList*) gDirectory->Get("ls2");

  TCanvas* c2 = new TCanvas("c2", "Bottom layer", 1200, 800);
  c2->Divide(4, 3);

  for(int ch=0; ch<12; ++ch) {
    c2->cd(ch+1);
    h2[ch] = new TH2F(Form("h2_%d",ch),"",64,0,64,maxcount,0,maxcount);
    tree->Draw(Form("data[0][%d]:Iteration$>>h2_  %d",ch,ch),
	       Form("Entry$==%d",ls->GetEntry(entry)));
  }

  c2->Print(Form("./pict/run%d_%d_btm.C", g_runID, entry));
}
