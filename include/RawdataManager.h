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
    void CheckTimeStamp();

    void GetEntry(int entry);
    int GetEntries();

    static int Max(int N, const int* data);

  private:
    static int m_isSynchronized; // whether is the "Synchronize()" completed

    std::vector<TTree*> m_tree;
    std::vector<std::vector<int>> m_entry; // event# of synchronized events
    std::vector<int> m_timestamp_delta;
};


#endif //CONV_RAWDATAMANAGER_H
