#define REDUCED_DATA
//#define DEBUG
//#define VISUALIZE

#include "RawdataManager.h"
#include "CosmicTriggerSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

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

    CosmicTriggerSystem* trg_sys = new CosmicTriggerSystem(runID);

#ifdef VISUALIZE
    trg_sys->Visualize();
#endif

    /*
     * Input tree
     */

    TString filename;

#ifdef REDUCED_DATA
    filename = Form("./rootfile/run%d.root_reduced", runID);
#else
    filename = Form("./rootfile/run%d.root", runID);
#endif

    TFile* fin = new TFile(filename);

    const int nCrate = 3;
    TTree* tin[nCrate];
    TString treename[nCrate] = {"Crate3", "Crate4", "Crate5"};

    // Check timestamps of input trees
    // TTree::SetBranchAddress() should be done AFTER RawdataManager::CheckTimeStamps()
    RawdataManager* man = new RawdataManager();
    for(int k = 0; k < nCrate; ++k) {
        tin[k] = (TTree*) fin->Get(treename[k]);
        man->AddTree(tin[k]);
    }
    man->CheckTimeStamp();
    std::cout << "# of surviving events    "
              << man->GetEntries() << " (" << std::setprecision(3)
              << 100. * man->GetEntries() / tin[0]->GetEntries()
              << "%)\n\n";

    Short_t Data[3][16][16][64];
    UInt_t Timestamp[nCrate];

    for(int k = 0; k < nCrate; ++k) {
        tin[k]->SetBranchAddress("Data", Data[k]);
        tin[k]->SetBranchAddress("Timestamp", &Timestamp[k]);
    }


    /*
     * Output tree
     */

    TFile* fout = new TFile(Form("./convfile/run%d_conv.root", runID), "recreate");
    TTree* tout = new TTree("tree", Form("run%d", runID));

    // CsI branches
    const int nCsI = trg_sys->GetNCsI();
    const int nline = 4;

    Short_t data_CsI[nCsI][2][64];
    Float_t ped_CsI[nCsI][2];
    Float_t peak_CsI[nCsI][2];
    Float_t integ_CsI[nCsI][2];
    Float_t pt_CsI[nCsI][2];
    Float_t cft_CsI[nCsI][2];

    Float_t TD_CsI[nCsI][2];
    Float_t MT_CsI[nCsI][2];

    Short_t isHit_CsI[nCsI];
    Short_t isUsed_CsI[nCsI];
    Short_t nHit_CsI[nline];
    Float_t hitpos_CsI[nCsI][3];

    tout->Branch("csi.data", data_CsI, Form("csi.data[%d][2][64]/S", nCsI));
    tout->Branch("csi.ped", ped_CsI, Form("csi.ped[%d][2]/F", nCsI));
    tout->Branch("csi.peak", peak_CsI, Form("csi.peakI[%d][2]/F", nCsI));
    tout->Branch("csi.integ", integ_CsI, Form("csi.integ[%d][2]/F", nCsI));
    tout->Branch("csi.pt", pt_CsI, Form("csi.pt[%d][2]/F", nCsI));
    tout->Branch("csi.cft", cft_CsI, Form("csi.cft[%d][2]/F", nCsI));

    tout->Branch("csi.TD", TD_CsI, Form("csi.TD[%d]/F", nCsI));
    tout->Branch("csi.MT", MT_CsI, Form("csi.MT[%d]/F", nCsI));
    tout->Branch("csi.isHit", isHit_CsI, Form("csi.isHit[%d]/S", nCsI));
    tout->Branch("csi.isUsed",isUsed_CsI, Form("csi.isUsed[%d]/S", nCsI));

    tout->Branch("csi.nHit", nHit_CsI, Form("csi.nHit[%d]/S", nline));
    tout->Branch("csi.hitpos", hitpos_CsI, Form("csi.hitpos[%d][3]/F", nCsI));

    // Trigger branches
    const int nLayer = trg_sys->GetNLayer();
    const int nCRC = trg_sys->GetNCRC();

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

    Float_t hitpos[nLayer][3];
    Short_t trackID;
    Float_t track[3][2];
    Float_t TOF;

    Short_t isOnlineHit[nLayer][nCRC][2][64];
    Short_t nOnlineHit[2][64];
    Short_t isOnlineTriggered[64];

    tout->Branch("trig.data", data, Form("trig.data[%d][%d][2][64]/S", nLayer, nCRC));
    tout->Branch("trig.ped", ped, Form("trig.ped[%d][%d][2]/F", nLayer, nCRC));
    tout->Branch("trig.peak", peak, Form("trig.peak[%d][%d][2]/F", nLayer, nCRC));
    tout->Branch("trig.integ", integ, Form("trig.integ[%d][%d][2]/F", nLayer, nCRC));
    tout->Branch("trig.pt", pt, Form("trig.pt[%d][%d][2]/F", nLayer, nCRC));
    tout->Branch("trig.cft", cft, Form("trig.cft[%d][%d][2]/F", nLayer, nCRC));

    tout->Branch("trig.TD", TD, Form("trig.TD[%d][%d]/F", nLayer, nCRC));
    tout->Branch("trig.MT", MT, Form("trig.MT[%d][%d]/F", nLayer, nCRC));
    tout->Branch("trig.recX", recX, Form("trig.recX[%d][%d]/F", nLayer, nCRC));

    tout->Branch("trig.isHit", isHit, Form("trig.isHit[%d][%d]/S", nLayer, nCRC));
    tout->Branch("trig.nHit", nHit, Form("trig.nHit[%d]/S", nLayer));
    tout->Branch("trig.hitpos", hitpos, Form("trig.hitpos[%d][3]/F", nLayer));
    tout->Branch("trig.TOF", &TOF, "TOF/F");

    tout->Branch("track", track, "track[3][2]/F");
    tout->Branch("trackID", &trackID, "trackID/S");

    tout->Branch("isOnlineHit", isOnlineHit, Form("isOnlineHit[%d][%d][2][64]/S", nLayer, nCRC));
    tout->Branch("nOnlineHit", nOnlineHit, "nOnlineHit[2][64]/S");
    tout->Branch("isOnlineTriggered", isOnlineTriggered, "isOnlineTriggered[64]/S");

    // Timestamp branch
    tout->Branch("timestamp", Timestamp, Form("timestamp[%d]/i", nCrate));


    /*
     * Scan events
     */

    std::cout << "Scan entries --------------------------------\n";

    for(int entry = 0; entry < man->GetEntries(); ++entry) {
        man->GetEntry(entry);

        // Set trigger counter data
        for(int slot = 0; slot < 16; ++slot) {
            for(int ch = 0; ch < 16; ++ch) {
                trg_sys->SetData(0, slot, ch, Data[0][slot][ch]);
            }
        }

        // Set CsI data
        CsI* CsI;
        for(int locID = 0; locID < nCsI; ++locID){
            CsI = trg_sys->GetCSI(locID);
            if(CsI->IsUsed()) {
                for(int side = 0; side < 2; side++) {
                    int crate = CsI->GetCrate(side) - 3;
                    int slot = CsI->GetSlot(side);
                    int ch = CsI->Getch(side);
                    trg_sys->SetData_CsI(locID, side, Data[crate][slot][ch]);
                }
            }
        }

        trg_sys->Process();

        // Fill data
        // Trigger
        CosmicRayCounter* counter;

        for(int layer = 0; layer < nLayer; ++layer) {
            for(int ch = 0; ch < nCRC; ++ch) {
                counter = trg_sys->GetCRC(layer, ch);
                for(int side = 0; side < 2; ++side) {
                    for(int smpl = 0; smpl < 64; ++smpl) {
                        data[layer][ch][side][smpl] = counter->GetData(side)[smpl];
                        isOnlineHit[layer][ch][side][smpl] = counter->IsOnlineHit(side)[smpl];
                    }
                    ped[layer][ch][side] = counter->GetPed(side);
                    peak[layer][ch][side] = counter->GetPeak(side);
                    integ[layer][ch][side] = counter->GetInteg(side);
                    pt[layer][ch][side] = counter->GetPT(side);
                    cft[layer][ch][side] = counter->GetCFT(side);
                }

                TD[layer][ch] = counter->GetTD();
                MT[layer][ch] = counter->GetMT();
                recX[layer][ch] = counter->GetHitPosition(0);
                isHit[layer][ch] = counter->IsHit();
            } // ch loop

            nHit[layer] = trg_sys->GetNhit(layer);

            for(int axis = 0; axis < 3; ++axis) {
                hitpos[layer][axis] = counter->GetHitPosition(axis);
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
            isOnlineTriggered[smpl] = trg_sys->IsOnlineTriggered()[smpl];
        }

        // CsI
        for (int locID = 0; locID < nCsI; locID++) {
            CsI = trg_sys->GetCSI(locID);
            isUsed_CsI[locID] = CsI->IsUsed();
            if (isUsed_CsI[locID]) {
                for (int side = 0; side < 2; side++) {
                    for (int smpl = 0; smpl < 64; smpl++) {
                        data_CsI[locID][side][smpl] = CsI->GetData(side)[smpl];

                    }
                    ped_CsI[locID][side] = CsI->GetPed(side);
                    peak_CsI[locID][side] = CsI->GetPeak(side);
                    integ_CsI[locID][side] = CsI->GetInteg(side);
                    pt_CsI[locID][side] = CsI->GetPT(side);
                    cft_CsI[locID][side] = CsI->GetCFT(side);
                }
                for(int axis = 0; axis < 3; ++axis) {
                    hitpos_CsI[locID][axis] = CsI ->GetHitPosition(axis);
                }
            }

        }
        tout->Fill();

        if(entry % 1000==0) std::cout << entry << "th\n";

#ifdef VISUALIZE
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

    std::cout << "------------------------------------------fin\n";


    /*
     * Write
     */

    TString note_threshold = Form("%.1f (common)", trg_sys->GetCommonThreshold());
    TText note2(0, 0, note_threshold);
    note2.SetName("Peak threshold");

    TText note1(0, 0, comment.c_str());
    note1.SetName("Comments");

    fout->cd();
    tout->Write();
    note1.Write();
    note2.Write();

    fout->Close();
    return 0;
}
