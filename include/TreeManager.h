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
    static int m_runID;
    static std::vector<TTree*> m_tin;
    static TTree* m_tout;
    static std::vector<InputBranchContainer> m_BRin;

    virtual ~TreeManager() {}

    static void SetRunID(int runID);
    static void AddInputTree(TTree* tree);
    static void SetInputTrees(int N, TTree** tree);
    static void SetOutputTree(TTree* tout);

    static void CheckTimeStamp();
    static void GetEntry(int entry);
    static int GetEntries();
    static void Fill();

    static int Max(int N, const int* data);

  protected:
    static std::vector<std::vector<int>> m_entry; // event# of synchronized events
    static std::vector<int> m_tDeltaTrigger;

  private:
    static bool m_isInit; // 1 if "TreeManager::CheckTimeStamp()" done

};


#endif //CONV_TreeManager_H
