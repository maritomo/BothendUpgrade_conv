//
// Created by Tomoo MARI on 2018/09/30.
//

#ifndef CONV_RAWDATAMANAGER_H
#define CONV_RAWDATAMANAGER_H

#include <vector>

#include "TTree.h"

class RawdataManager {
  public:
    RawdataManager();
    ~RawdataManager();

    void AddTree(TTree* tree) { m_tree.push_back(tree); }
    void Synchronize();

    void GetEntry(int entry);
    void GetLostEntry(int entry);
    int GetEntries();
    int GetLostEntries();

    int Sum(int N, const int* data);

  private:
    std::vector<TTree*> m_tree;
    std::vector<int> m_entries;
    std::vector<std::vector<int>> m_entry; // event# of synchronized events
    std::vector<std::vector<int>> m_lost;  // event# of un-synchronized events

    int m_isSynchronized;
};


#endif //CONV_RAWDATAMANAGER_H
