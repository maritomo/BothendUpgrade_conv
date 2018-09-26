void pedestal() {

  int runID = 1030;

  TFile* fin = new TFile(Form("./rootfile/run%d_conv.root", runID));
  TTree* tin = (TTree*) fin->Get("tree");

  TH1F* h[2][6][2];
  TF1* fg = new TF1("fg", "gaus");

  TString filename = Form("../pedestal/pedestal_run%d.txt", runID);
  std::ofstream ofs(filename.Data());

  TCanvas* c1 = new TCanvas();
  c1->Print(Form("./pict/pedestal_run%d.pdf[", runID));

  for(int slot = 0; slot < 2; ++slot) {
    for(int adcch = 0; adcch < 12; ++adcch) {

      int layer = slot;
      int ch = adcch / 2;
      int side = adcch % 2;

      h[layer][ch][side] = new TH1F(Form("h%d_%d_%d", layer, ch, side),
                                    Form("layer%d, ch%d, side%d;[ADC count];", layer, ch, side),
                                    600, 200, 800);

      TCut cut;
      cut = Form("pt[%d][%d][%d]>35", layer, ch, side);
      cut = Form("!isHit[%d][%d]", layer, ch);

      tin->Draw(Form("ped[%d][%d][%d]>>h%d_%d_%d", layer, ch, side, layer, ch, side));

      double mean = h[layer][ch][side]->GetMean();
      double rms = h[layer][ch][side]->GetRMS();
      h[layer][ch][side]->Fit("fg", "", "", mean - 2 * rms, mean + 2 * rms);

      mean = fg->GetParameter(1);
      rms = fg->GetParameter(2);
      h[layer][ch][side]->Fit("fg", "", "", mean - 2 * rms, mean + 2 * rms);

      ofs << layer << " " << ch << " " << h[layer][ch][side]->GetMean() << " " << h[layer][ch][side]->GetRMS() << "\n";

      c1->Print(Form("./pict/pedestal_run%d.pdf", runID));
    }
  }

  c1->Print(Form("./pict/pedestal_run%d.pdf]", runID));

  ofs.close();

}
