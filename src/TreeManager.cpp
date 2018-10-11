//
// Created by Tomoo MARI on 2018/10/07.
//

#include <iostream>

#include "TreeManager.h"

int TreeManager::m_runID;
std::vector<TTree*> TreeManager::m_tin;
TTree* TreeManager::m_tout;
std::vector<InputBranchContainer> TreeManager::m_BRin;
std::vector<std::vector<int>> TreeManager::m_entry;
std::vector<int> TreeManager::m_tDeltaTrigger;


bool TreeManager::m_isInit = 0;

void TreeManager::SetRunID(int runID) {
    m_runID = runID;
}

void TreeManager::SetInputTrees(int N, TTree** tree) {
    m_tin.resize(N);
    m_BRin.resize(N);
    for(int k=0; k<N; ++k) {
        m_tin[k] = tree[k];
        m_tin[k]->SetBranchAddress("Data", m_BRin[k].Data);
        m_tin[k]->SetBranchAddress("Timestamp", &m_BRin[k].Timestamp);
    }
}

void TreeManager::AddInputTree(TTree* tree) {
    m_tin.push_back(tree);
    if(m_BRin.size()==0) m_BRin.resize(16);

    int index = m_tin.size()-1;
    m_tin[index]->SetBranchAddress("Data", m_BRin[index].Data);
    m_tin[index]->SetBranchAddress("Timestamp", &m_BRin[index].Timestamp);
}

void TreeManager::SetOutputTree(TTree* tout) {
    m_tout = tout;
}

void TreeManager::CheckTimeStamp() {
    std::cout << "Checking timestamps --------------------------------------\n";

    const int nTree = m_tin.size();
    if(nTree < 2) {
        std::cout << "Error: # of trees should be >= 2\n";
        return;
    }

    m_entry.resize(nTree);
    m_tDeltaTrigger.resize(nTree);

    int* entry = new int[nTree];
    int* tdif_delta = new int[nTree-1];

    // The first event in a run must be the delta triggers
    // and timestamp difference between crates can be references
    int first_entry = 30;
    m_tin[nTree-1]->GetEntry(first_entry);
    for(int k = 0; k < nTree; ++k) {
        m_tin[k]->GetEntry(first_entry);
        m_tDeltaTrigger[k] = m_BRin[k].Timestamp;
        if(k < nTree-1) {
            tdif_delta[k] = m_BRin[nTree-1].Timestamp - m_BRin[k].Timestamp;
        }
    }

    for(int k = 0; k < nTree; ++k) {
        entry[k] = first_entry + 1;
    }


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
            if(m_BRin[k].Timestamp==m_tDeltaTrigger[k]) {
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
                if(m_BRin[k].Timestamp==m_tDeltaTrigger[k]) {
                    continue;
                }

                // if found
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

    std::cout << "# of surviving events     " << m_entry[0].size() << " ("
              << (double)m_entry[0].size()/m_tin[0]->GetEntries()*100 << "%)\n";

    delete[] entry;
    delete[] tdif_delta;

    std::cout << "----------------------------------------------------------\n";
    m_isInit = 1;
}

void TreeManager::GetEntry(int entry) {
    if(!m_isInit) {
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
    if(!m_isInit) {
        std::cout << "Use CheckTimeStamp() function at first\n";
        return 0;
    }
    return m_entry[0].size();
}

void TreeManager::Fill() {
    m_tout->Fill();
}

int TreeManager::Max(int N, const int* data) {
    int max = -1000000;
    for(int i = 0; i < N; ++i) {
        if(max < data[i]) max = data[i];
    }
    return max;
}