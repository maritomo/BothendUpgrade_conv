#define REDUCED_DATA
//#define DEBUG
//#define VISUALIZE

#include "Converter.h"

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

bool GetFileState(TString filename);

int main(int argc, char** argv) {

    if(argc<2 || argc>4) {
        std::cout << "./conv [runID]([comment])\n";
        return 1;
    }

    std::stringstream ss1;
    ss1 << argv[1];
    int runID;
    ss1 >> runID;
    std::cout << "Run\t" << runID << " \n";

    std::string comment="No comments";
    if(argc == 3) {
        comment=argv[2];
        std::cout << comment << "\n";
    }



    /*
     * Set trees into TreeManager
     */

    TString filename = Form("./rootfile/run%d.root", runID);

#ifdef REDUCED_DATA
    filename=Form("./rootfile/run%d.root_reduced", runID);
#endif

    TreeManager::SetRunID(runID);

    // Input trees
    if(!GetFileState(filename)) {
        std::cout << filename << " not found\n";
        return 1;
    }

    TFile* fin = new TFile(filename);
    const int nCrate=3;
    TTree* tin[nCrate];
    TString treename[nCrate] = {"Crate3", "Crate4", "Crate5"};

    for(int k=0; k<nCrate; ++k) {
        tin[k] = (TTree*) fin->Get(treename[k]);
    }
//    TreeManager::SetInputTrees(nCrate, tin);
    for(int k=0; k<nCrate; ++k) {
        TreeManager::AddInputTree((TTree*) fin->Get(treename[k]));
    }

    // Output tree
    TFile* fout = new TFile(Form("./convfile/run%d_conv.root", runID), "recreate");
    TTree* tout = new TTree("tree", Form("run%d", runID));
    TreeManager::SetOutputTree(tout);
    TreeManager::CheckTimeStamp();


#ifdef VISUALIZE
    conv->Visualize();
//    TApplication app("app", &argc, argv);
#endif


    /*
     * Conversion
     */

    Converter* conv = new Converter();

    std::cout << "\n######################## Scanning entries ########################\n";
    for(int entry=0; entry<conv->GetEntries(); ++entry) {
        TreeManager::GetEntry(entry);
        conv->Convert();
        TreeManager::Fill();
        if(entry % 1000 == 0){
            std::cout << entry << "th\n";
        }

#ifdef VISUALIZE
        conv->Display(0); // xy plane
        conv->Display(1); // zy plane
//        gSystem->ProcessEvents();
//        gSystem->Sleep(1000);
        conv->Print(Form("./test_%d.pdf", entry));
#endif
#ifdef DEBUG
        if(entry==0) break;
#endif

    }
    std::cout << "############################## END ###############################\n";


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


#ifdef DEBUG
    std::cout << "###############################\n";
    std::cout << "######     DEBUG mode    ######\n";
    std::cout << "###############################\n";
#endif

    return 0;
}


bool GetFileState(TString filename) {
    FileStat_t info;
    if(gSystem->GetPathInfo(filename.Data(), info)) {
        std::cout << "Error: " << filename << " not found\n";
        return false;
    }
    return true;
}