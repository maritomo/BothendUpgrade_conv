//
// Created by Tomoo MARI on 2018/09/30.
//

#include "RawdataManager.h"

#include <iostream>


RawdataManager::RawdataManager() : m_isSynchronized(0) {}

RawdataManager::~RawdataManager() {}


// Assume un-synchronization occurs only in 1 crate
void RawdataManager::Synchronize(){

    std::cout << "Checking timestamps -------------------------\n";

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

    int skipflag = 0;
    int nMaxTrial = 5;
    int* nRetry = new int[nTree];

    for(int k = 0; k < nTree; ++k) {
        spillNo[k] = 0;
        flag[k] = 0;
        nRetry[k] = 0;
    }

    std::cout <<"TTree\t";
    for(int k=0; k<nTree; ++k) {
        std::cout << m_tree[k]->GetName() << " / ";
    }
    std::cout << "\n";

    while(1) {
        int endflag = 0;
        // if the last spill
        for(int k = 0; k < nTree; ++k) {
            if(spillNo[k] * 30 > m_entries[k])
                endflag = 1;
        }
        if(endflag) break; // ignore the last spill

        // scan 30 events (Jay_spill)
        for(int entry = 0; entry < 30; ++entry) {
            for(int k = 0; k < nTree; ++k) {
                m_tree[k]->GetEntry(30 * spillNo[k] + entry);
            }

            // timestamp difference between crates
            for(int k = 0; k < nTree; ++k) {
                dif[k] = timestamp[(k + 1) % nTree] - timestamp[(k + 2) % nTree];
                // if delta trigger
                if(entry==0) dif_delta[k] = dif[k];
            }

            // delta trigger
            if(entry==0) continue;

            // flag if un-synchronized
            for(int k = 0; k < nTree; ++k) {
                flag[k] = 0;
                int offset = nTree - 2;
                int index[2] = {(k + offset) % nTree, (k + 1 + offset) % nTree};
                // if k-th crate un-synchronized
                if(dif[index[0]]!=dif_delta[index[0]] && dif[index[1]]!=dif_delta[index[1]])
                    flag[k] = 1;
            }

            // if this spill un-synchronized
            if(Sum(nTree, flag)) {
                for(int k = 0; k < nTree; ++k) {
                    // if healthy crate
                    if(!flag[k]) continue;
                    // if un-synchronized crate
                    if(nRetry[k] < nMaxTrial) { // search for the correct spill
                        ++spillNo[k];
                        ++nRetry[k];
                        skipflag = 0;
                    } else { // give up to find
                        for(int i=0; i<nTree; ++i) {
                            spillNo[i] -= nRetry[i];
                            nRetry[i] = 0;
                            m_lost[i].push_back(spillNo[i]);
                        }
                        skipflag = 1;
                        break;
                    }
                }
                // messages
                if(skipflag) {
                    std::cout <<"Spill\t";
                    for(int k=0; k<nTree; ++k) {
                        std::cout << spillNo[k] << " / ";
                    }
                    std::cout << "\tskipped\n";
                }
                break;

            } else {
                for(int k = 0; k < 3; ++k) nRetry[k] = 0;
                skipflag = 0;
            }

        } // entry loop

        // if synchronized spills found
        if(!Sum(nTree, nRetry)) {
            for(int k = 0; k < 3; ++k) {
                if(!skipflag) {
                    for(int entry = 0; entry < 30; ++entry) {
                        m_entry[k].push_back(spillNo[k]*30 + entry);
                    }
                }
                ++spillNo[k];
            }
        }

    } // spill loop

    delete[] spillNo;
    delete[] timestamp;
    delete[] dif;
    delete[] dif_delta;
    delete[] flag;
    delete[] nRetry;

    m_isSynchronized = 1;

    std::cout << "------------------------- Synchronization fin\n\n";

}

void RawdataManager::GetEntry(int entry) {
    if(!m_isSynchronized) {
        std::cout << "Use Synchronize() function at first\n";
        return;
    }
    if(entry >= m_entry[0].size()) {
        std::cout << "Total # of events " << m_entry[0].size() << "\n";
        return;
    }
    for(int k = 0; k < m_tree.size(); ++k) {
        m_tree[k]->GetEntry(m_entry[k][entry]);
    }
}

void RawdataManager::GetLostEntry(int entry){
    if(!m_isSynchronized) {
        std::cout << "Use Synchronize() function at first\n";
        return;
    }
    if(entry >= m_lost[0].size()) {
        std::cout << "Total # of events " << m_lost[0].size() << "\n";
        return;
    }
    for(int k = 0; k < m_tree.size(); ++k) {
        m_tree[k]->GetEntry(m_lost[k][entry]);
    }
}

int RawdataManager::GetEntries() {
    if(!m_isSynchronized) {
        std::cout << "Use Synchronize() function at first\n";
        return 0;
    }
    return m_entry[0].size();
}

int RawdataManager::GetLostEntries(){
    if(!m_isSynchronized) {
        std::cout << "Use Synchronize() function at first\n";
        return 0;
    }
    return m_lost[0].size();
}

int RawdataManager::Sum(int N, const int* data) {
    int sum = 0;
    for(int i = 0; i < N; ++i) {
        sum += data[i];
    }
    return sum;
}