//
// Created by Tomoo MARI on 2018/09/30.
//
//#define DEBUG


#include "RawdataManager.h"

#include <iostream>


RawdataManager::RawdataManager() : m_isSynchronized(0) {}

RawdataManager::~RawdataManager() {}


// Assume un-synchronization occurs only in 1 crate
void RawdataManager::CheckTimeStamp() {

    const int nTree = m_tree.size();
    if(nTree < 2) {
        std::cout << "Error: # of trees should be >= 2\n";
        return;
    }

    std::cout << "Checking timestamps -------------------------\n";

    UInt_t* timestamp = new UInt_t[nTree];
    for(int k = 0; k < nTree; ++k) {
        m_tree[k]->SetBranchAddress("Timestamp", &timestamp[k]);
    }

    m_entry.resize(nTree);
    m_timestamp_delta.resize(nTree);

    int* entry = new int[nTree];
    int* tdif_delta = new int[nTree - 1];

    // The first event in a run must be the delta triggers
    // and timestamp difference between crates can be references
    m_tree[nTree - 1]->GetEntry(0);
    for(int k = 0; k < nTree; ++k) {
        m_tree[k]->GetEntry(0);
        m_timestamp_delta[k] = timestamp[k];
        if(k < nTree - 1) {
            tdif_delta[k] = timestamp[nTree - 1] - timestamp[k];
        }
    }

    // Initialize entry#
    for(int k = 0; k < nTree; ++k)
        entry[k] = 1;


    /* Scan all entries */

#ifdef DEBUG
    for(int k = 0; k < nTree; ++k) {
        std::cout << m_tree[k]->GetName() << " / ";
    }
    std::cout << "\n";
#endif
    
    int count = 0;

    while(1) {
        // end
        int end_flag = 0;
        for(int k = 0; k < nTree; ++k) {
            if(entry[k] >= m_tree[k]->GetEntries()) {
                end_flag = 1;
                break;
            }
        }
        if(end_flag) break;

        // skip some events
        int continue_flag = 0;
        
        // delta trigger events
        for(int k = 0; k < nTree; ++k) {
            m_tree[k]->GetEntry(entry[k]);
            if(timestamp[k]==m_timestamp_delta[k]) {
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
        m_tree[nTree-1]->GetEntry(maxEntry);

        for(int k = 0; k < nTree-1; ++k) {
            if(maxEntry >= m_tree[k]->GetEntries()) {
                recover_flag = 0;
                break;
            }

            m_tree[k]->GetEntry(maxEntry);
            if(timestamp[k]+tdif_delta[k]!=timestamp[nTree-1]) {
                recover_flag = 0;
                break;
            }
        }

        if(recover_flag) {
            for(int k = 0; k < nTree; ++k) {
                entry[k] = maxEntry;
                m_entry[k].push_back(entry[k]);

#ifdef DEBUG
                std::cout << k << ":" << entry[k] << "\t";
#endif
                ++entry[k];
            }
#ifdef DEBUG
            std::cout << "\n";
#endif
            int current_entry = m_entry[0].size();
            if(current_entry%1000==0)
                std::cout << current_entry << "th\n";

            continue;
        }

        // (nTree-1)th tree is used as reference
        int find_flag = 1;
        m_tree[nTree - 1]->GetEntry(entry[nTree - 1]);

        for(int k = 0; k < nTree - 1; ++k) {
            for(int i = 0, maxloop = 300; i < maxloop; ++i) {
                int nextentry = entry[k] + i;

                // if not found
                if(i==maxloop - 1 || nextentry > m_tree[k]->GetEntries()-1) {
                    find_flag = 0;
                    break;
                }

                // skip the last event in each Jay-spill
                if(nextentry%30==29) {
                    continue;
                }

                m_tree[k]->GetEntry(nextentry);

                // skip delta trigger events
                if(timestamp[k]==m_timestamp_delta[k]) {
                    continue;
                }

                // if found
                if(timestamp[k] + tdif_delta[k]==timestamp[nTree - 1]) {
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
#ifdef DEBUG
                std::cout << k << "::" << entry[k] << "\t";
#endif
            }
            int current_entry = m_entry[0].size();
            if(current_entry%1000==0)
                std::cout << current_entry << "th\n";
#ifdef DEBUG
            std::cout << std::endl;
#endif

        } else {

#ifdef DEBUG
            for(int k = 0; k < nTree; ++k) {
                std::cout << k << "->" << entry[k] << "\t";
            }
            std::cout << std::endl;
#endif
            ++entry[nTree - 1];
            for(int k = 0; k < nTree - 1; ++k) {
                if(entry[k] < entry[nTree-1]) {
                    entry[k] = entry[nTree-1];
                }
            }
        }


    } // while

    delete[] timestamp;
    delete[] entry;
    delete[] tdif_delta;

    m_isSynchronized = 1;

    std::cout << "------------------------- Synchronization fin\n\n";
}

void RawdataManager::GetEntry(int spill) {
    if(!m_isSynchronized) {
        std::cout << "Use CheckTimeStamp() function at first\n";
        return;
    }
    if(spill >= m_entry[0].size()) {
        std::cout << "Total # of events " << m_entry[0].size() << "\n";
        return;
    }
    for(int k = 0; k < m_tree.size(); ++k) {
        m_tree[k]->GetEntry(m_entry[k][spill]);
    }
}

int RawdataManager::GetEntries() {
    if(!m_isSynchronized) {
        std::cout << "Use CheckTimeStamp() function at first\n";
        return 0;
    }
    return m_entry[0].size();
}

int RawdataManager::Max(int N, const int* data) {
    int max = -1000000;
    for(int i = 0; i < N; ++i) {
        if(max < data[i]) max = data[i];
    }
    return max;
}
