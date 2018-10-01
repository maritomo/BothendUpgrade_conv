#include "global.h"
#include "CosmicTriggerSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TText.h"


int main(int argc, char** argv) {

  if(argc < 2 || argc > 4) {
    std::cout << "./conv [runID]([comment])\n";
    return 1;
  }

  std::stringstream ss1;
  ss1 << argv[1];
  int runID;
  ss1 >> runID;

  std::string comment = "No comments";
  if(argc==3) {
    comment = argv[2];
  }


  /*
   * Trigger system
   */

  CosmicTriggerSystem* trg_sys = new CosmicTriggerSystem(runID);

#ifdef VISUALIZE
  trg_sys->Visualize();
#endif

  /*
   * Input tree
   */

  TFile* fin = new TFile(Form("./rootfile/run%d.root", runID));
  TTree* tin = (TTree*) fin->Get("Crate3");

  Short_t data[16][16][64];

  tin->SetBranchAddress("Data", data);


  /*
   * Output tree
   */

  const int nLayer = trg_sys->GetNLayer();
  const int nCRC = trg_sys->GetNCRC();
  const int nCsI = trg_sys->GetNCsI();
  const int nline = 4;
  const int num = 10;

  TFile* fout = new TFile(Form("./convfile/run%d_conv.root", runID), "recreate");
  TTree* tout = new TTree("tree", Form("run%d", runID));

  Float_t ped[nLayer][nCRC][2];
  Float_t peak[nLayer][nCRC][2];
  Float_t integ[nLayer][nCRC][2];
  Float_t pt[nLayer][nCRC][2];
  Float_t cft[nLayer][nCRC][2];

  Float_t MT[nLayer][nCRC];
  Float_t TD[nLayer][nCRC];
  Float_t recX[nLayer][nCRC];

  Short_t isHit[nLayer][nCRC];
  Short_t nHit[nLayer];
  Float_t hitpos[nLayer][3];
  Short_t trackID;

  tout->Branch("data", data, "data[16][16][64]/S");
  tout->Branch("ped", ped, Form("ped[%d][%d][2]/F", nLayer, nCRC));
  tout->Branch("peak", peak, Form("peak[%d][%d][2]/F", nLayer, nCRC));
  tout->Branch("integ", integ, Form("integ[%d][%d][2]/F", nLayer, nCRC));
  tout->Branch("pt", pt, Form("pt[%d][%d][2]/F", nLayer, nCRC));
  tout->Branch("cft", cft, Form("cft[%d][%d][2]/F", nLayer, nCRC));

  tout->Branch("TD", TD, Form("TD[%d][%d]/F", nLayer, nCRC));
  tout->Branch("MT", MT, Form("MT[%d][%d]/F", nLayer, nCRC));
  tout->Branch("recX", recX, Form("recX[%d][%d]/F", nLayer, nCRC));
  tout->Branch("isHit", isHit, Form("isHit[%d][%d]/S", nLayer, nCRC));

  tout->Branch("nHit", nHit, Form("nHit[%d]/S", nLayer));
  tout->Branch("hitpos", hitpos, Form("hitpos[%d][3]/F", nLayer));
  tout->Branch("trackID", &trackID, "trackID/S");

  /*
   * Input tree (CsI)
   */
  TTree* tCsI = new TTree("tCsI",Form("run%d_CsI",runID));

  Float_t ped_CsI[nCsI][2];
  Float_t peak_CsI[nCsI][2];
  Float_t integ_CsI[nCsI][2];
  Float_t pt_CsI[nCsI][2];
  Float_t cft_CsI[nCsI][2];

  Float_t MT_CsI[nCsI][2];

  Short_t isHit_CsI[nCsI];
  Short_t nHit_CsI[nline];
  //Float_t hitpos[nLayer][3];
  //Short_t trackID;

  tCsI->Branch("data", data, "data[16][16][64]/S");
  tCsI->Branch("ped", ped, Form("ped[%d][2]/F", nCsI));
  tCsI->Branch("peak", peak, Form("peak[%d][2]/F", nCsI));
  tCsI->Branch("integ", integ, Form("integ[%d][2]/F", nCsI));
  tCsI->Branch("pt", pt, Form("pt[%d][2]/F", nCsI));
  tCsI->Branch("cft", cft, Form("cft[%d][2]/F", nCsI));

  tCsI->Branch("TD", TD, Form("TD[%d]/F", nCsI));
  tCsI->Branch("MT", MT, Form("MT[%d]/F", nCsI));
  tCsI->Branch("recX", recX, Form("recX[%d]/F", nCsI));
  tCsI->Branch("isHit", isHit, Form("isHit[%d]/S", nCsI));

  tCsI->Branch("nHit", nHit, Form("nHit[%d]/S", nline));
  //tCsI->Branch("hitpos", hitpos, Form("hitpos[%d][3]/F", nLayer));
  //tCsI->Branch("trackID", &trackID, "trackID/S");
  /*
   * Scan events
   */

  for(int evt = 0, entry = tin->GetEntries(); evt < entry; ++evt) {
    //if(evt % 30==0) continue; // skip delta trigger events
    tin->GetEntry(evt);

    for(int slot = 0; slot < 16; ++slot) {
      for(int ch = 0; ch < 16; ++ch) {
        trg_sys->SetData(slot, ch, data[slot][ch]);
    }
    }

    trg_sys->Process();

    CosmicRayCounter* counter;
    CsI* csi;

    for(int layer = 0; layer < nLayer; ++layer) {
      for(int ch = 0; ch < nCRC; ++ch) {
        counter = trg_sys->GetCRC(layer, ch);

        for(int k = 0; k < 2; ++k) {
          ped[layer][ch][k] = counter->GetPed(k);
          peak[layer][ch][k] = counter->GetPeak(k);
          integ[layer][ch][k] = counter->GetInteg(k);
          pt[layer][ch][k] = counter->GetPT(k);
          cft[layer][ch][k] = counter->GetCFT(k);
        }

        TD[layer][ch] = counter->GetTD();
        MT[layer][ch] = counter->GetMT();
        recX[layer][ch] = counter->GetHitPos(0);
        isHit[layer][ch] = counter->GetIsHit();
      }

      nHit[layer] = trg_sys->GetNhit(layer);

      for(int axis = 0; axis < 3; ++axis) {
        hitpos[layer][axis] = counter->GetHitPos(axis);
      }

    }

    trackID = trg_sys->GetTrackID();

    tout->Fill();


    if(evt % 1000==0) std::cout << evt << "th\n";

#ifdef VISUALIZE
    TString filename;
    if(trackID>0) {
      trg_sys->Display();
      filename = Form("./test_%d.pdf", evt);
      //trg_sys->Print(filename);
    }
#endif

#ifdef DEBUG
    if(evt==0) break;
#endif

  }


  /*
   * Write
   */

  TString note_threshold;
#ifdef COMMON_THRESHOLD
  note_threshold = Form("%.1f (common)", trg_sys->GetCommonThreshold());
#else
  note_threshold = Form("%.1f * pedestal", trg_sys->GetfPedestalSigma());
#endif
  TText note2(0, 0, note_threshold);
  note2.SetName("Peak threshold");

  TText note1(0, 0, "comment");
  note1.SetName("Comments");


  fout->cd();
  tout->Write();
  note1.Write();
  note2.Write();


#ifdef DEBUG
  std::cout << "########## DEBUG mode ##########\n";
#endif

#ifdef COMMON_THRESHOLD
  std::cout << "######## COMMON_THRESHLD ########\n";
#endif


  fout->Close();
  return 0;
}
