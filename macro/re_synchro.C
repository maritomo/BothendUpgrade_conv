void re_synchro() {

    int runID = 1121;

    TFile* fin = new TFile(Form("../../rootfile/run%d.root", runID));

    TTree* tree[3];
    tree[0] = (TTree*) fin->Get("Crate3");
    tree[1] = (TTree*) fin->Get("Crate4");
    tree[2] = (TTree*) fin->Get("Crate5");

    int Ntot = tree[0]->GetEntries();

    UInt_t ts[3];
    for(int k = 0; k < 3; ++k) {
        tree[k]->SetBranchAddress("Timestamp", &ts[k]);
    }

    int spillNo[3] = {};
    int dif[3];
    int dif_delta[3];
    int flag[3];

    int ntry[3] = {};

    while(1) {
        if(spillNo[0]*30 > Ntot) break;
        std::cout << "SpillNo\t" << spillNo[0] << " " << spillNo[1] << " " << spillNo[2] << "\n";

        for(int entry = 0; entry < 30; ++entry) {

            // GetEntry
            std::cout << "\t" << entry << "\t|";
            for(int k = 0; k < 3; ++k) {
                tree[k]->GetEntry(30 * spillNo[k] + entry);
                std::cout << "\t" << ts[k];
            }
            std::cout << "\t|";
            // Dif
            for(int k = 0; k < 3; ++k) {
                dif[k] = ts[(k + 1) % 3] - ts[(k + 2) % 3];
                std::cout << "\t" << dif[k];
            }
            std::cout << "\t|";
            // Flag
            for(int k = 0; k < 3; ++k) {
                if(entry==0) dif_delta[k] = dif[k];
                flag[k] = 0;
                if(dif[k]!=dif_delta[k]) flag[k] = 1;
                std::cout << " " << flag[k];
            }
            std::cout << std::endl;

            // if unsynchronized
            if(flag[0] + flag[1] + flag[2] > 0) {
//                std::cout << "Ntry\t" << ntry[0] << " " << ntry[1] << " " << ntry[2] << std::endl;
                for(int k = 0; k < 3; ++k) {
                    if(flag[k]) continue; // synchronized channel
                    if(ntry[k] > 9) { // give up
                        spillNo[k] -= ntry[k];
                        ntry[k] = 0;
                        std::cout << ">>>>>>> Spill " << spillNo[k] << " of Crate " << k+3 << " ignored <<<<<<<\n";
                    } else if(dif[k]==dif_delta[k]) { // find
                        std::cout << "******* Crate " << k + 3 << " unsynchronized *******\n";
                        ++spillNo[k];
                        ++ntry[k];
                    }
                }
                break;
            } else if (entry>0) { // except delta trigger
//                std::cout << "NO unsynchronization\n";
                for(int k = 0; k < 3; ++k)
                    ntry[k] = 0;
            }

        } // entry loop

        if(!ntry[0] && !ntry[1] && !ntry[2]) {
            for(int k = 0; k < 3; ++k) {
                ++spillNo[k];
            }
        }
        std::cout << std::endl;

//        if(count==20) break;
    } // spill loop

}


