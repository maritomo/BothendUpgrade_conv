//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_TreeManager_H
#define CONV_TreeManager_H

#include <vector>
#include "TTree.h"

struct InputBranchContainer {
    Short_t Data[16][16][64];
    UInt_t Timestamp;
};


class TreeManager {
  public:
    virtual ~TreeManager() {}
    static void SetInputTrees(int num, TTree** tree);
    static void SetOutputTree(TTree* tout) { m_tout = tout; }

    static void CheckTimeStamp();
    static void GetEntry(int entry);
    static int GetEntries();
    static int Max(int N, const int* data);

  protected:
    // Input tree
    static std::vector<TTree*> m_tin;
    static InputBranchContainer* m_BRin;
    static std::vector<std::vector<int>> m_entry; // event# of synchronized events
    static std::vector<int> m_timestamp_delta;
    // Output tree
    static TTree* m_tout;

  private:
    static int m_isSynchronized; // 1 if "TreeManager::CheckTimeStamp()" done

};


#endif //CONV_TreeManager_H
