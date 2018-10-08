//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>

#include "TreeManager.h"

std::vector<TTree*> TreeManager::m_tin;
InputBranchContainer* TreeManager::m_BRin;
std::vector<std::vector<int>> TreeManager::m_entry;
std::vector<int> TreeManager::m_timestamp_delta;

TTree* TreeManager::m_tout = nullptr;

int TreeManager::m_isSynchronized = 0;

void TreeManager::SetInputTrees(int num, TTree** tree) {
    m_BRin = new InputBranchContainer[num];

    for(int i=0; i < num; ++i) {
        m_tin.push_back(tree[i]);
        m_tin[i]->SetBranchAddress("Data", m_BRin[i].Data);
        m_tin[i]->SetBranchAddress("Timestamp", &m_BRin[i].Timestamp);
    }

    if(!m_isSynchronized) CheckTimeStamp();
}

void TreeManager::CheckTimeStamp() {
    std::cout << "\nChecking timestamps...\n";

    const int nTree = m_tin.size();
    if(nTree < 2) {
        std::cout << "Error: # of trees should be >= 2\n";
        return;
    }

//    UInt_t* timestamp = new UInt_t[nTree];

    m_entry.resize(nTree);
    m_timestamp_delta.resize(nTree);

    int* entry = new int[nTree];
    int* tdif_delta = new int[nTree-1];

    // The first event in a run must be the delta triggers
    // and timestamp difference between crates can be references
    m_tin[nTree-1]->GetEntry(0);
    for(int k = 0; k < nTree; ++k) {
        m_tin[k]->GetEntry(0);
        m_timestamp_delta[k] = m_BRin[k].Timestamp;
//        m_timestamp_delta[k] = timestamp[k];
        if(k < nTree-1) {
//            tdif_delta[k] = timestamp[nTree-1] - timestamp[k];
            tdif_delta[k] = m_BRin[nTree-1].Timestamp - m_BRin[k].Timestamp;
        }
    }

    // Initialize entry#
    for(int k = 0; k < nTree; ++k) entry[k] = 1;

    /* Scan all events */
    while(1) {
        // end
        int end_flag = 0;
        for(int k = 0; k < nTree; ++k) {
            if(entry[k] >= m_tin[k]->GetEntries()) {
                end_flag = 1;
                break;
            }
        }
        if(end_flag) break;

        // skip some events
        int continue_flag = 0;

        // delta trigger events
        for(int k = 0; k < nTree; ++k) {
            m_tin[k]->GetEntry(entry[k]);
            if(m_BRin[k].Timestamp==m_timestamp_delta[k]) {
                ++entry[k];
                continue_flag = 1;
            }
        }
        if(continue_flag) continue;

        // Skip the last event in each Jay-spill
        // Timestamp does not change from one event before... I don't know why
        for(int k = 0; k < nTree; ++k) {
            if(entry[k] % 30==29) {
                ++entry[k];
                continue_flag = 1;
            }
        }
        if(continue_flag) continue;


        // If un-synchronization recovered
        int recover_flag = 1;
        int maxEntry = Max(nTree, entry);
        m_tin[nTree-1]->GetEntry(maxEntry);

        for(int k = 0; k < nTree-1; ++k) {
            if(maxEntry >= m_tin[k]->GetEntries()) {
                recover_flag = 0;
                break;
            }
            m_tin[k]->GetEntry(maxEntry);
//            if(timestamp[k]+tdif_delta[k]!=timestamp[nTree-1]) {
            if(m_BRin[k].Timestamp+tdif_delta[k]!=m_BRin[nTree-1].Timestamp) {
                recover_flag = 0;
                break;
            }
        }


        if(recover_flag) {
            for(int k = 0; k < nTree; ++k) {
                entry[k] = maxEntry;
                m_entry[k].push_back(entry[k]);
                ++entry[k];
            }

            int current_entry = m_entry[0].size();
            if(current_entry%1000==0)
                std::cout << current_entry << "th\n";

            continue;
        }

        // (nTree-1)th tree is used as reference
        int find_flag = 1;
        m_tin[nTree-1]->GetEntry(entry[nTree-1]);

        for(int k = 0; k < nTree-1; ++k) {
            for(int i = 0, maxloop = 300; i < maxloop; ++i) {
                int nextentry = entry[k] + i;

                // if not found
                if(i==maxloop - 1 || nextentry > m_tin[k]->GetEntries()-1) {
                    find_flag = 0;
                    break;
                }

                // skip the last event in each Jay-spill
                if(nextentry%30==29) {
                    continue;
                }

                m_tin[k]->GetEntry(nextentry);

                // skip delta trigger events
//                if(timestamp[k]==m_timestamp_delta[k]) {
                if(m_BRin[k].Timestamp==m_timestamp_delta[k]) {
                    continue;
                }

                // if found
//                if(timestamp[k] + tdif_delta[k]==timestamp[nTree-1]) {
                if(m_BRin[k].Timestamp + tdif_delta[k]==m_BRin[nTree-1].Timestamp) {
                    entry[k] = nextentry;
                    break;
                }

            }
        }

        // if found
        if(find_flag) {
            for(int k = 0; k < nTree; ++k) {
                m_entry[k].push_back(entry[k]);
                ++entry[k];
            }
            int current_entry = m_entry[0].size();
            if(current_entry%1000==0)
                std::cout << current_entry << "th\n";
        } else {
            ++entry[nTree-1];
            for(int k = 0; k < nTree-1; ++k) {
                if(entry[k] < entry[nTree-1]) {
                    entry[k] = entry[nTree-1];
                }
            }
        }

    } // while

    //        std::cout << "# of surviving events    "
//              << man->GetEntries() << " (" << std::setprecision(3)
//              << 100. * man->GetEntries() / tin[0]->GetEntries()
//              << "%)\n\n";

//    delete[] timestamp;
    delete[] entry;
    delete[] tdif_delta;

    m_isSynchronized = 1;
    std::cout << "...fin\n";
}

void TreeManager::GetEntry(int entry) {
    if(!m_isSynchronized) {
        std::cout << "Use CheckTimeStamp() function at first\n";
        return;
    }
    if(entry >= m_entry[0].size()) {
        std::cout << "Total # of events " << m_entry[0].size() << "\n";
        return;
    }
    for(int k = 0; k < m_tin.size(); ++k) {
        m_tin[k]->GetEntry(m_entry[k][entry]);
    }
}

int TreeManager::GetEntries() {
    if(!m_isSynchronized) {
        std::cout << "Use CheckTimeStamp() function at first\n";
        return 0;
    }
    return m_entry[0].size();
}

int TreeManager::Max(int N, const int* data) {
    int max = -1000000;
    for(int i = 0; i < N; ++i) {
        if(max < data[i]) max = data[i];
    }
    return max;
}