#define REDUCED_DATA
#define DEBUG
#define VISUALIZE

#include "CosmicMeasSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <TRint.h>

#include "TFile.h"
#include "TTree.h"
#include "TText.h"
#include "TApplication.h"
#include "TSystem.h"


int main(int argc, char** argv) {

#ifdef DEBUG
    std::cout << "###############################\n";
    std::cout << "######     DEBUG mode    ######\n";
    std::cout << "###############################\n";
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
     * Set trees into system
     */

    TString filename;

#ifdef REDUCED_DATA
    filename = Form("./rootfile/run%d.root_reduced", runID);
#else
    filename = Form("./rootfile/run%d.root", runID);
#endif

    // Input trees
    const int nCrate = 3;
    TFile* fin = new TFile(filename);
    TTree* tin[nCrate];
    TString treename[nCrate] = {"Crate3", "Crate4", "Crate5"};

    for(int k = 0; k < nCrate; ++k) {
        tin[k] = (TTree*) fin->Get(treename[k]);
    }

    // Output tree
    TFile* fout = new TFile(Form("./convfile/run%d_conv.root", runID), "recreate");
    TTree* tout = new TTree("tree", Form("run%d", runID));

    // system
    CosmicMeasSystem* sys = new CosmicMeasSystem(nCrate, tin, tout);

#ifdef VISUALIZE
    sys->Visualize();
//    TApplication app("app", &argc, argv);
#endif

    /*
     * Scan events
     */

    std::cout << "\nScan entries...\n";

    for(int entry = 0; entry < sys->GetEntries(); ++entry) {
        sys->GetEntry(entry);
        sys->Process();
        tout->Fill();
        if(entry % 1000==0) std::cout << entry << "th\n";

#ifdef VISUALIZE
        sys->Display(0); // xy plane
        sys->Display(1); // zy plane
//        gSystem->ProcessEvents();
//        gSystem->Sleep(1000);
        sys->Print(Form("./test_%d.pdf", entry));
#endif

#ifdef DEBUG
        if(entry==5) break;
#endif

    }

    std::cout << "...fin\n";


    /*
     * Write
     */

//    TString note_threshold = Form("%.1f (common)", trg_sys->GetCommonThreshold());
//    TText note2(0, 0, note_threshold);
//    note2.SetName("Peak threshold");

    TText note1(0, 0, comment.c_str());
    note1.SetName("Comments");

    fout->cd();
    tout->Write();
    note1.Write();
//    note2.Write();

    fout->Close();
    return 0;
}
