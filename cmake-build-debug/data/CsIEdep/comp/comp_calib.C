int g_runID[2716] = {};

void merge() {

  int runIDs[] = {1204, 1230, 1261, 1292, 1322, 1355, 1385, 1435, 1458, 1470, 1499, 1511, 1521, 1531};
  
  const int nCSI = 2716;
  double MIPadc[nCSI] = {};

  for(int i=0; i<sizeof(runIDs)/sizeof(runIDs[0]); ++i) {
    std::stringstream ss;
    ss << "../MIPadc_run" << runIDs[i] << ".txt";
    std::string filename = ss.str();

    std::ifstream ifs(filename.c_str());
    if(!ifs) {
      std::cout << filename << " not found\n";
      continue;
    }

    int id;
    double MIP;
    while(ifs >> id >> MIP) {
      if(MIP>0) {
	MIPadc[id] = MIP;
	g_runID[id] = runIDs[i];
	std::cout << "CsI# " << id << " : overwritten by run" << g_runID[id] << "\n";
      } 
    }
    ifs.close();
  }

  std::ofstream ofs("CsIPMT_MIPintegratedADC.txt");
  for(int id=0; id<nCSI; ++id) {
    ofs << id << "\t" << MIPadc[id] << "\n";
  }
  ofs.close();

}


void comp_calib() {

  merge();

  const int nCSI = 2716;
  double sumADC_wMPPC[nCSI];
  double sumADC_woMPPC[nCSI];


  // Data with MPPC
  std::ifstream ifs("CsIPMT_MIPintegratedADC.txt");
  if(!ifs) return;
  
  int id;
  while(ifs >> id >> sumADC_wMPPC[id]) {}
  ifs.close();
  ifs.clear();

  // Data without MPPC
  ifs.open("calib0629_result.dat");
  if(!ifs) return;
  
  double tmp;
  while(ifs >> id >> tmp >> tmp >> sumADC_woMPPC[id] >> tmp
	>> tmp >> tmp >> tmp >> tmp >> tmp) {
    if(sumADC_woMPPC[id]==-1) sumADC_woMPPC[id] = 1e10;
    if(id>=2240) sumADC_woMPPC[id] *= 2;
    if(id==nCSI-1) break;
  }


  
  double csiID[nCSI];
  for(int i=0; i<nCSI; ++i) csiID[i] = i;


  TFile fout("rootfile/MIPintegratedADC.root", "RECREATE");

  // Draw
  auto c1 = new TCanvas("c1", "", 1000, 500);
  c1->Divide(2, 1);
  c1->cd(1);

  auto gW = new TGraph(nCSI, csiID, sumADC_wMPPC);
  gW->SetMarkerColor(kRed);
  gW->SetMarkerStyle(4);
  gW->SetTitle("MIP value;ID;Integrated ADC [count]");
  gW->Draw("ap");

  auto gWO = new TGraph(nCSI, csiID, sumADC_woMPPC);
  gWO->SetMarkerColor(kBlue);
  gWO->SetMarkerStyle(4);
  gWO->Draw("p same");

  auto leg = new TLegend(0.1, 0.7, 0.4, 0.9);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->AddEntry(gW, "w/ MPPC", "p");
  leg->AddEntry(gWO, "w/o MPPC", "p");
  leg->Draw("same");
 

  c1->cd(2);//->SetLogy();
  auto hRatio = new TH1F("hRatio", "wMPPC / woMPPC;Ratio;", 100,0,2);
  double ratio[nCSI];
  for(int id=0; id<nCSI; ++id) {
    if(sumADC_wMPPC[id]>0) {
      hRatio->Fill(sumADC_wMPPC[id] / sumADC_woMPPC[id]);
    }
  }
  hRatio->Draw();

  c1->Print("pict/comp_csiMIP.pdf");

  fout.cd();
  gW->Write("wMPPC");
  gWO->Write("woMPPC");


}
