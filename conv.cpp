#define IS_COMMON_THRESHOLD 1
//#define DEBUG
//#define VISUALIZE

#include "RawdataManager.h"
#include "CosmicTriggerSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <RawdataManager.h>

#include "TFile.h"
#include "TTree.h"
#include "TText.h"


int main(int argc, char** argv) {

#ifdef DEBUG
    std::cout << "########## DEBUG mode ##########\n";
#endif

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

    CosmicTriggerSystem* trg_sys = new CosmicTriggerSystem(runID, IS_COMMON_THRESHOLD);

#ifdef VISUALIZE
    trg_sys->Visualize();
#endif

    /*
     * Input tree
     */

    TFile* fin = new TFile(Form("./rootfile/run%d.root", runID));

    const int nCrate = 3;

//    TTree* Crate3 = (TTree*) fin->Get("Crate3");
//    UInt_t ts;
//    Crate3->SetBranchAddress("Timestamp", &ts);

    TTree* tin[nCrate];
    TString treename[nCrate] = {"Crate3", "Crate4", "Crate5"};

    RawdataManager* man = new RawdataManager();

    for(int k = 0; k < nCrate; ++k) {
        tin[k] = (TTree*) fin->Get(treename[k]);
        man->AddTree(tin[k]);
    }

    man->Synchronize();

    // "SetBranchAddress()" should be done AFTER "man->CheckTimeStamps()"
    Short_t Data[3][16][16][64]={{{}}};
    UInt_t Timestamp[nCrate]={};

    for(int k = 0; k < nCrate; ++k) {
        if(k==0) {
            tin[k]->SetBranchAddress("Data", &Data[3]);
        }
        tin[k]->SetBranchAddress("Timestamp", &Timestamp[k]);
    }


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

    Short_t data[nLayer][nCRC][2][64];
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

  /*
   * Input tree (CsI)
   */
    TTree* tCsI = new TTree("tCsI",Form("run%d_CsI",runID));

    Short_t data_CsI[nCsI][2][64];
    Float_t ped_CsI[nCsI][2];
    Float_t peak_CsI[nCsI][2];
    Float_t integ_CsI[nCsI][2];
    Float_t pt_CsI[nCsI][2];
    Float_t cft_CsI[nCsI][2];

    Float_t MT_CsI[nCsI][2];

    Short_t isHit_CsI[nCsI];
    Short_t isUsed_CsI[nCsI];
    Short_t nHit_CsI[nline];
    Float_t hitpos_CsI[nCsI][3];
    //Short_t trackID;

    tCsI->Branch("data_CsI", data_CsI, Form("data_CsI[%d][2][64]/S", nCsI));
    tCsI->Branch("ped_CsI", ped_CsI, Form("ped_CsI[%d][2]/F", nCsI));
    tCsI->Branch("peak_CsI", peak_CsI, Form("peak_CsI[%d][2]/F", nCsI));
    tCsI->Branch("integ_CsI", integ_CsI, Form("integ_CsI[%d][2]/F", nCsI));
    tCsI->Branch("pt_CsI", pt_CsI, Form("pt_CsI[%d][2]/F", nCsI));
    tCsI->Branch("cft_CsI", cft_CsI, Form("cft_CsI[%d][2]/F", nCsI));

    //tCsI->Branch("TD", TD, Form("TD[%d]/F", nCsI));
    tCsI->Branch("MT_CsI", MT_CsI, Form("MT_CsI[%d]/F", nCsI));
    //tCsI->Branch("recX", recX, Form("recX[%d]/F", nCsI));
    tCsI->Branch("isHit_CsI", isHit_CsI, Form("isHit_CsI[%d]/S", nCsI));
    tCsI->Branch("isUsed_CsI",isUsed_CsI, Form("isUsed_CsI[%d]/S", nCsI));

    tCsI->Branch("nHit_CsI", nHit_CsI, Form("nHit_CsI[%d]/S", nline));
    tCsI->Branch("hitpos_CsI", hitpos_CsI, Form("hitpos_CsI[%d][3]/F", nCsI));
    //tCsI->Branch("trackID", &trackID, "trackID/S");
    /*
     * Scan events
     */

    Float_t hitpos[nLayer][3];
    Short_t trackID;
    Float_t track[3][2];
    Float_t TOF;

    Short_t isOnlineHit[nLayer][nCRC][2][64];
    Short_t nOnlineHit[2][64];
    Short_t isOnlineTriggered[64];


    tout->Branch("data", data, Form("data[%d][%d][2][64]/S", nLayer, nCRC));
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

    tout->Branch("isOnlineHit", isOnlineHit, Form("isOnlineHit[%d][%d][2][64]/S", nLayer, nCRC));
    tout->Branch("nOnlineHit", nOnlineHit, "nOnlineHit[2][64]/S");
    tout->Branch("isOnlineTriggered", isOnlineTriggered, "isOnlineTriggered[64]/S");


    /*
     * Scan events
     */

    for(int entry = 0; entry < man->GetEntries(); ++entry) {
        man->GetEntry(entry);

        for(int slot = 0; slot < 16; ++slot) {
            for(int ch = 0; ch < 16; ++ch) {
                trg_sys->SetData(0, slot, ch, Data[0][slot][ch]);
            }
        }
        CsI* CsI;
        for(int locID = 0; locID < nCsI; ++locID){
            CsI = trg_sys -> GetCSI(locID);
            int use, ADC[3], crate;
            use = CsI -> GetIsUsed();
            if(use){
                for (int k = 0; k < 3 ; k++) {
                    ADC[k] = CsI -> GetADC(k); //0 crate, 1 slot, 2 ch,
                }
            }
            crate = ADC[0] - 3;
            trg_sys->SetData_CsI(locID, Data[crate][ADC[1]][ADC[2]]);

        }

        trg_sys->Process();

        CosmicRayCounter* counter;


        for(int layer = 0; layer < nLayer; ++layer) {
            for(int ch = 0; ch < nCRC; ++ch) {
                counter = trg_sys->GetCRC(layer, ch);
                for(int side = 0; side < 2; ++side) {
                    for(int smpl = 0; smpl < 64; ++smpl) {
                        data[layer][ch][side][smpl] = counter->GetData(side)[smpl];
                        isOnlineHit[layer][ch][side][smpl] = counter->GetIsOnlineHit(side)[smpl];
                    }
                    ped[layer][ch][side] = counter->GetPed(side);
                    peak[layer][ch][side] = counter->GetPeak(side);
                    integ[layer][ch][side] = counter->GetInteg(side);
                    pt[layer][ch][side] = counter->GetPT(side);
                    cft[layer][ch][side] = counter->GetCFT(side);
                }

                TD[layer][ch] = counter->GetTD();
                MT[layer][ch] = counter->GetMT();
                recX[layer][ch] = counter->GetHitPos(0);
                isHit[layer][ch] = counter->GetIsHit();
            } // ch loop

            nHit[layer] = trg_sys->GetNhit(layer);

            for(int axis = 0; axis < 3; ++axis) {
                hitpos[layer][axis] = counter->GetHitPos(axis);
            }


            for(int smpl = 0; smpl < 64; ++smpl) {
                nOnlineHit[layer][smpl] = trg_sys->GetNOnlineHit(layer)[smpl];
            }

        } // layer loop

        for(int plane = 0; plane < 3; ++plane) {
            for(int k = 0; k < 2; ++k) {
                track[plane][k] = trg_sys->GetTrack(plane)[k];
            }
        }

        trackID = trg_sys->GetTrackID();
        TOF = trg_sys->GetTOF();



        for(int smpl = 0; smpl < 64; ++smpl) {
            isOnlineTriggered[smpl] = trg_sys->GetIsOnlineTriggered()[smpl];
        }

        tout->Fill();

        if(entry % 1000==0) std::cout << entry << "th\n";

        for (int locID = 0; locID < nCsI; locID++){
            CsI = trg_sys -> GetCSI(locID);
            isUsed_CsI[locID] = CsI -> GetIsUsed();
            if(isUsed_CsI) {
                for (int side = 0; side < 1; side++) {

                    for (int smpl = 0; smpl < 64; smpl++) {
                        data_CsI[locID][side][smpl] = CsI->GetData(side)[smpl];

                    }
                    ped_CsI[locID][side] = CsI->GetPed(side);
                    peak_CsI[locID][side] = CsI->GetPeak(side);
                    integ_CsI[locID][side] = CsI->GetInteg(side);
                    pt_CsI[locID][side] = CsI->GetPT(side);
                    cft_CsI[locID][side] = CsI->GetCFT(side);
                }
            }


        }


#ifdef VISUALIZE
        TString filename;
        if(trackID>0) {
          trg_sys->Display();
          filename = Form("./test_%d.pdf", entry);
          //trg_sys->Print(filename);
        }
#endif

#ifdef DEBUG
        if(entry==0) break;
#endif

    }


    /*
     * Write
     */

    TString note_threshold;

    if(trg_sys->GetIsCommonThreshold())
        note_threshold = Form("%.1f (common)", trg_sys->GetCommonThreshold());
    else
        note_threshold = Form("%.1f * pedestal", trg_sys->GetfPedestalSigma());

    TText note2(0, 0, note_threshold);
    note2.SetName("Peak threshold");

    TText note1(0, 0, "comment");
    note1.SetName("Comments");

    fout->cd();
    tout->Write();
    note1.Write();
    note2.Write();
    fout->Close();

    if(trg_sys->GetIsCommonThreshold())
        std::cout << "************* COMMON_THRESHLD *************\n";

    return 0;
}
