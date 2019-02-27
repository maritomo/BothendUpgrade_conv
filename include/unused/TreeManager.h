//
// Created by Tomoo MARI on 2018/10/07.
//

#ifndef CONV_TreeManager_H
#define CONV_TreeManager_H

#include <vector>
#include "TTree.h"

struct InputBranchContainer {
    Short_t Data[24][64];
    UInt_t Timestamp;
};


class TreeManager {
  public:
    virtual ~TreeManager() {}
    static void SetRunID(int runID);
    static void AddInputTree(TTree* tree);
    static void SetInputTrees(int N, TTree** tree);
    static int GetHeadRunID(int runID);

    static TTree* GetEventTree();
    static TTree* GetStatusTree();

    static void CheckTimeStamp();
    static void GetEntry(int entry);
    static int GetEntries();
    static void Fill();

    static int Max(int N, const int* data);

  protected:
    static int m_runID;
    static int m_entry;
    static std::vector<TTree*> m_tin;
    static TTree* m_eventTree;
    static TTree* m_statusTree;
    static std::vector<InputBranchContainer> m_BRin;

    static std::vector<std::vector<int> > m_entries; // event# of synchronized events
    static std::vector<int> m_tDeltaTrigger;

  private:
    static bool m_isInit;
};


#endif //CONV_TreeManager_H
