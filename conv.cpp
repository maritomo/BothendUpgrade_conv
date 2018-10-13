#define REDUCED_DATA
//#define DEBUG
//#define VISUALIZE

#include "TreeManager.h"
#include "Converter.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

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

    TreeManager::SetRunID(runID);

    // Input files
    TString filename = Form("./rootfile/run%d.root", runID);
#ifdef REDUCED_DATA
    filename = Form("./rootfile/run%d.root_reduced", runID);
#endif
    if(!GetFileState(filename)) {
        std::cout << "[Error] " << filename << " not found\n";
        return 1;
    }
    TFile* fin = new TFile(filename);

    // Output file
    TFile* fout = new TFile(Form("./convfile/run%d_conv.root", runID), "recreate");


    /*
     * Set trees into converter
     */

    TString treename[] = {"Crate3", "Crate4", "Crate5"};
    for(int k=0; k<sizeof(treename)/sizeof(TString); ++k) {
        TreeManager::AddInputTree((TTree*) fin->Get(treename[k]));
    }

    /*
     * Conversion
     */

    Converter* conv = new Converter();

#ifdef VISUALIZE
    conv->Visualize();
//    TApplication app("app", &argc, argv);
#endif

    std::cout << "\n######################## Scanning entries ########################\n";

    TreeManager::CheckTimeStamp();

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
        conv->Print(Form("./pict/test_%d.pdf", entry));
#endif
#ifdef DEBUG
        if(entry==100) break;
#endif

    }
    std::cout << "############################## END ###############################\n";


    /*
     * Write
     */

    TText note1(0, 0, comment.c_str());
    note1.SetName("Comments");

    fout->cd();
    TreeManager::GetEventTree()->Write();
    TreeManager::GetStatusTree()->Write();
    note1.Write();

    fout->Close();

#ifdef DEBUG
    std::cout << "\n";
    std::cout << "*-*-*-*-*-*-*-* DEBUG *-*-*-*-*-*-*-*\n";
#endif
#ifdef REDUCED_DATA
    std::cout << "\n";
    std::cout << "*-*-*-*-*-*-*-* REDUCED_DATA *-*-*-*-*-*-*-*\n";
#endif

    return 0;
}


bool GetFileState(TString filename) {
    FileStat_t info;
    if(gSystem->GetPathInfo(filename.Data(), info)) {
        return false;
    }
    return true;
}
