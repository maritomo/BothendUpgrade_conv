//#define VISUALIZE
//#define DEBUG


#include <iostream>
#include <string>
#include <sstream>

#include <dirent.h>

#include "TFile.h"
#include "TTree.h"
#include <TChain.h>


#include "Converter.h"


bool GetFileNames(const char* directory, std::vector<std::string>& filenames) {
    filenames.clear();
    DIR* dir = opendir(directory);
    if(dir == NULL) return false;
    while(1) {
        dirent* entry = readdir(dir);
        if(entry == NULL) break;
        std::string filename = entry->d_name;
        if(filename.length() < 5) continue;
        if(filename.substr(filename.length()-5, 5) != ".root") continue;
        filename.insert(0, directory);
        filenames.push_back(filename);
    }
    return true;
}


int main(int argc, char** argv) {

    if(argc != 2) {
        std::cerr << "./" << argv[0] << " [runID]\n";
        return 1;
    }

    int runID;
    std::stringstream ss1;
    ss1 << argv[1];
    ss1 >> runID;
    std::cout << "run\t" << runID << "\n";


    // Get input file names
    auto tin = new TChain("eventTree");
    std::string dir = Form("build_in/run%d/", runID);
    std::vector<std::string> fin_names;

    if(GetFileNames(dir.c_str(), fin_names) == false) {
        std::cerr << "Error : Directory \"./build_in/run" << runID << "\" not found.\n";
        return 1;
    }

    if(fin_names.size()==0) {
        std::cerr << "Error : There is no file in \"build_in/run" << runID << "\".\n";
        return 1;
    }


    // Generate convereter
    Converter* conv = new Converter();


    // Input tree
    for(auto fname : fin_names) {
        std::cout << fname << "\n";
        tin->Add(fname.c_str());
    }
    conv->SetInputTree(tin);


    // Generate output tree
    auto fout = new TFile(Form("conv_file/run%d_conv.root", runID), "RECREATE");
    auto tout = new TTree("eventTree", Form("run%d", runID));
    conv->SetOutputTree(tout);


#ifdef VISUALIZE
    conv->Visualize();
#endif

    std::cout << "\n######################## Conversion started ########################\n";


    for(int entry = 0; entry < tin->GetEntries(); ++entry) {
        conv->Convert(entry);
        if(entry % 10 == 0) std::cout << entry << "th\n";

#ifdef VISUALIZE
        conv->Display(0); // xy plane
        conv->Display(1); // zy plane
        conv->Print(Form("pict/run%d_%d.png", runID, entry));
#endif

#ifdef DEBUG
        break;
#endif

    }


    std::cout << "############################## END ###############################\n";


    fout->cd();
    tout->Write();
    fout->Close();


    return 0;
}
