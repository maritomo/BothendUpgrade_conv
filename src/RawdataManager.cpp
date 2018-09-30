//
// Created by Tomoo MARI on 2018/09/30.
//

#include "RawdataManager.h"

#include <iostream>


RawdataManager::RawdataManager() : m_isSynchronized(0) {}

RawdataManager::~RawdataManager() {}

void RawdataManager::Synchronize(){

    const int nTree = m_tree.size();
    if(nTree < 2) {
        std::cout << "# of trees should be >= 2\n";
        return;
    }

    m_entries.resize(nTree);
    m_entry.resize(nTree);
    m_lost.resize(nTree);

    for(int k = 0; k < nTree; ++k) {
        m_entries[k] = m_tree[k]->GetEntries();
    }

    UInt_t* timestamp = new UInt_t[nTree];
    for(int k = 0; k < nTree; ++k) {
        m_tree[k]->SetBranchAddress("Timestamp", &timestamp[k]);
    }


    int* spillNo = new int[nTree];
    int* dif = new int[nTree];
    int* dif_delta = new int[nTree];
    int* flag = new int[nTree];

    int endflag = 0;
    int nMAxRetry = 5;
    int* nRetry = new int[nTree];

    for(int k = 0; k < nTree; ++k) {
        spillNo[k] = 0;
        flag[k] = 0;
        nRetry[k] = 0;
    }

    int nEventPerSpill = 30;

    while(1) {
        for(int k = 0; k < nTree; ++k) {
            if(spillNo[k] * nEventPerSpill > m_entries[k])
                endflag = 1;
        }
        if(endflag) break; // the last spill are not used

//        std::cout << "SpillNo\t" << spillNo[0] << " " << spillNo[1] << " " << spillNo[2] << "\n";

        for(int entry = 0; entry < nEventPerSpill; ++entry) {
            // GetEntry
//            std::cout << "\t" << entry << "\t|";
            for(int k = 0; k < nTree; ++k) {
                m_tree[k]->GetEntry(nEventPerSpill * spillNo[k] + entry);
//                std::cout << "\t" << timestamp[k];
            }
//            std::cout << "\t|";

            // Dif
            for(int k = 0; k < nTree; ++k) {
                dif[k] = timestamp[(k + 1) % nTree] - timestamp[(k + 2) % nTree];
                if(entry==0) {
                    dif_delta[k] = dif[k]; // delta trigger
                }
//                std::cout << "\t" << dif[k];
            }

            if(entry==0) {  // delta trigger
//                std::cout << "\n";
                continue;
            }

//            std::cout << "\t|";

            // Flag
            for(int k = 0; k < nTree; ++k) {
                flag[k] = 0;
                int offset = nTree - 2;
                int index[2] = {(k + offset) % nTree, (k + 1 + offset) % nTree};

                // unsynchronization in k-th crate
                if(dif[index[0]]!=dif_delta[index[0]] && dif[index[1]]!=dif_delta[index[1]])
                    flag[k] = 1;
//                std::cout << " " << flag[k];
            }
//            std::cout << std::endl;

            // if unsynchronized
            if(Sum(nTree, flag) > 0) {
                for(int k = 0; k < nTree; ++k) {
                    // synchronized crate
                    if(!flag[k])
                        continue;
                    // unsynchronized crate
                    if(nRetry[k] > nMAxRetry) { // give up
                        spillNo[k] -= nRetry[k];
                        nRetry[k] = 0;
                        m_lost[k].push_back(spillNo[k]);
                        std::cout << ">>>>>>> Crate " << k+3 << ", Spill " << spillNo[k] << " skipped <<<<<<<\n";
                    } else { // find
//                        std::cout << "******* Crate " << k + 3 << " unsynchronized *******\n";
                        ++spillNo[k];
                        ++nRetry[k];
                    }
                }
                break;
            } else { // except delta trigger
                for(int k = 0; k < 3; ++k)
                    nRetry[k] = 0;
            }

        } // entry loop

        // if synchronized spills found
        if(nRetry[0] + nRetry[1] + nRetry[2]==0) {
            for(int k = 0; k < 3; ++k) {
                for(int entry = 0; entry < nEventPerSpill; ++entry) {
                    m_entry[k].push_back(spillNo[k]*nEventPerSpill + entry);
                }
                ++spillNo[k];
            }
        }
//        std::cout << std::endl;

    } // spill loop

    delete[] spillNo;
    delete[] timestamp;
    delete[] dif;
    delete[] dif_delta;
    delete[] flag;
    delete[] nRetry;

    m_isSynchronized = 1;

    std::cout << "---------------------------------------------\n";
    std::cout << "        Unsynchronized events removed        \n";
    std::cout << "---------------------------------------------\n";
}

void RawdataManager::GetEntry(int entry) {
    if(entry >= m_entry[0].size()) {
        std::cout << "Total # of events " << m_entry[0].size() << "\n";
        return;
    }
    for(int k = 0; k < m_tree.size(); ++k) {
        m_tree[k]->GetEntry(m_entry[k][entry]);
    }
}

void RawdataManager::GetLostEntry(int entry){
    if(entry >= m_lost[0].size()) {
        std::cout << "Total # of events " << m_lost[0].size() << "\n";
        return;
    }
    for(int k = 0; k < m_tree.size(); ++k) {
        m_tree[k]->GetEntry(m_lost[k][entry]);
    }
}

int RawdataManager::GetEntries() {
    return m_entry[0].size();
}

int RawdataManager::GetLostEntries(){
    return m_lost[0].size();
}

int RawdataManager::Sum(int N, const int* data) {
    int sum = 0;
    for(int i = 0; i < N; ++i) {
        sum += data[i];
    }
    return sum;
}