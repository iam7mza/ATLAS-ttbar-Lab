//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Sep 24 11:04:45 2023 by ROOT version 6.26/04
// from TTree mini/mini
// found on file: ../Samples/ttbar_410000.root
//////////////////////////////////////////////////////////

#ifndef mini_h
#define mini_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "ROOT/RVec.hxx"
#include "ROOT/RVec.hxx"
#include "ROOT/RVec.hxx"
#include "ROOT/RVec.hxx"

class mini {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         SumWeights;
   Float_t         XSection;
   Int_t           channelNumber;
   Int_t           eventNumber;
   ROOT::VecOps::RVec<float> *jet_E;
   ROOT::VecOps::RVec<float> *jet_MV2c10;
   ROOT::VecOps::RVec<float> *jet_eta;
   ROOT::VecOps::RVec<float> *jet_jvt;
   UInt_t          jet_n;
   ROOT::VecOps::RVec<float> *jet_phi;
   ROOT::VecOps::RVec<float> *jet_pt;
   ROOT::VecOps::RVec<int> *jet_trueflav;
   ROOT::VecOps::RVec<bool> *jet_truthMatched;
   ROOT::VecOps::RVec<float> *lep_E;
   ROOT::VecOps::RVec<int> *lep_charge;
   ROOT::VecOps::RVec<float> *lep_eta;
   ROOT::VecOps::RVec<float> *lep_etcone20;
   ROOT::VecOps::RVec<bool> *lep_isTightID;
   UInt_t          lep_n;
   ROOT::VecOps::RVec<float> *lep_phi;
   ROOT::VecOps::RVec<float> *lep_pt;
   ROOT::VecOps::RVec<float> *lep_ptcone30;
   ROOT::VecOps::RVec<float> *lep_trackd0pvunbiased;
   ROOT::VecOps::RVec<float> *lep_tracksigd0pvunbiased;
   ROOT::VecOps::RVec<bool> *lep_trigMatched;
   ROOT::VecOps::RVec<bool> *lep_truthMatched;
   ROOT::VecOps::RVec<unsigned int> *lep_type;
   ROOT::VecOps::RVec<float> *lep_z0;
   Float_t         mcWeight;
   Float_t         met_et;
   Float_t         met_phi;
   Int_t           runNumber;
   Float_t         scaleFactor_BTAG;
   Float_t         scaleFactor_COMBINED;
   Float_t         scaleFactor_ELE;
   Float_t         scaleFactor_LepTRIGGER;
   Float_t         scaleFactor_MUON;
   Float_t         scaleFactor_PILEUP;
   Float_t         scaleFactor_PhotonTRIGGER;
   Bool_t          trigE;
   Bool_t          trigM;

   // List of branches
   TBranch        *b_SumWeights;   //!
   TBranch        *b_XSection;   //!
   TBranch        *b_channelNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_jet_E;   //!
   TBranch        *b_jet_MV2c10;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_jvt;   //!
   TBranch        *b_jet_n;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_trueflav;   //!
   TBranch        *b_jet_truthMatched;   //!
   TBranch        *b_lep_E;   //!
   TBranch        *b_lep_charge;   //!
   TBranch        *b_lep_eta;   //!
   TBranch        *b_lep_etcone20;   //!
   TBranch        *b_lep_isTightID;   //!
   TBranch        *b_lep_n;   //!
   TBranch        *b_lep_phi;   //!
   TBranch        *b_lep_pt;   //!
   TBranch        *b_lep_ptcone30;   //!
   TBranch        *b_lep_trackd0pvunbiased;   //!
   TBranch        *b_lep_tracksigd0pvunbiased;   //!
   TBranch        *b_lep_trigMatched;   //!
   TBranch        *b_lep_truthMatched;   //!
   TBranch        *b_lep_type;   //!
   TBranch        *b_lep_z0;   //!
   TBranch        *b_mcWeight;   //!
   TBranch        *b_met_et;   //!
   TBranch        *b_met_phi;   //!
   TBranch        *b_runNumber;   //!
   TBranch        *b_scaleFactor_BTAG;   //!
   TBranch        *b_scaleFactor_COMBINED;   //!
   TBranch        *b_scaleFactor_ELE;   //!
   TBranch        *b_scaleFactor_LepTRIGGER;   //!
   TBranch        *b_scaleFactor_MUON;   //!
   TBranch        *b_scaleFactor_PILEUP;   //!
   TBranch        *b_scaleFactor_PhotonTRIGGER;   //!
   TBranch        *b_trigE;   //!
   TBranch        *b_trigM;   //!

   mini(TTree *tree=0);
   virtual ~mini();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Int_t GetEntries() { return fChain->GetEntries(); }
   virtual Int_t GetEntries(const char *sel) { return fChain->GetEntries(sel); }
   virtual TTree *CloneTree() { return fChain->CloneTree(0); }
   virtual TTree *CloneTree(int a) { return fChain->CloneTree(a); }
   virtual void CopyAddresses(TTree *tree) { fChain->CopyAddresses(tree, true); }
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   virtual Double_t GetMinimum(TString varName);
   virtual Double_t GetMaximum(TString varName);
};

#endif

#ifdef mini_cxx
mini::mini(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../Samples/ttbar_410000.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../Samples/ttbar_410000.root");
      }
      f->GetObject("mini",tree);

   }
   Init(tree);
}

mini::~mini()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mini::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mini::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void mini::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jet_E = 0;
   jet_MV2c10 = 0;
   jet_eta = 0;
   jet_jvt = 0;
   jet_phi = 0;
   jet_pt = 0;
   jet_trueflav = 0;
   jet_truthMatched = 0;
   lep_E = 0;
   lep_charge = 0;
   lep_eta = 0;
   lep_etcone20 = 0;
   lep_isTightID = 0;
   lep_phi = 0;
   lep_pt = 0;
   lep_ptcone30 = 0;
   lep_trackd0pvunbiased = 0;
   lep_tracksigd0pvunbiased = 0;
   lep_trigMatched = 0;
   lep_truthMatched = 0;
   lep_type = 0;
   lep_z0 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("SumWeights", &SumWeights, &b_SumWeights);
   fChain->SetBranchAddress("XSection", &XSection, &b_XSection);
   fChain->SetBranchAddress("channelNumber", &channelNumber, &b_channelNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("jet_E", &jet_E, &b_jet_E);
   fChain->SetBranchAddress("jet_MV2c10", &jet_MV2c10, &b_jet_MV2c10);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_jvt", &jet_jvt, &b_jet_jvt);
   fChain->SetBranchAddress("jet_n", &jet_n, &b_jet_n);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_trueflav", &jet_trueflav, &b_jet_trueflav);
   fChain->SetBranchAddress("jet_truthMatched", &jet_truthMatched, &b_jet_truthMatched);
   fChain->SetBranchAddress("lep_E", &lep_E, &b_lep_E);
   fChain->SetBranchAddress("lep_charge", &lep_charge, &b_lep_charge);
   fChain->SetBranchAddress("lep_eta", &lep_eta, &b_lep_eta);
   fChain->SetBranchAddress("lep_etcone20", &lep_etcone20, &b_lep_etcone20);
   fChain->SetBranchAddress("lep_isTightID", &lep_isTightID, &b_lep_isTightID);
   fChain->SetBranchAddress("lep_n", &lep_n, &b_lep_n);
   fChain->SetBranchAddress("lep_phi", &lep_phi, &b_lep_phi);
   fChain->SetBranchAddress("lep_pt", &lep_pt, &b_lep_pt);
   fChain->SetBranchAddress("lep_ptcone30", &lep_ptcone30, &b_lep_ptcone30);
   fChain->SetBranchAddress("lep_trackd0pvunbiased", &lep_trackd0pvunbiased, &b_lep_trackd0pvunbiased);
   fChain->SetBranchAddress("lep_tracksigd0pvunbiased", &lep_tracksigd0pvunbiased, &b_lep_tracksigd0pvunbiased);
   fChain->SetBranchAddress("lep_trigMatched", &lep_trigMatched, &b_lep_trigMatched);
   fChain->SetBranchAddress("lep_truthMatched", &lep_truthMatched, &b_lep_truthMatched);
   fChain->SetBranchAddress("lep_type", &lep_type, &b_lep_type);
   fChain->SetBranchAddress("lep_z0", &lep_z0, &b_lep_z0);
   fChain->SetBranchAddress("mcWeight", &mcWeight, &b_mcWeight);
   fChain->SetBranchAddress("met_et", &met_et, &b_met_et);
   fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("scaleFactor_BTAG", &scaleFactor_BTAG, &b_scaleFactor_BTAG);
   fChain->SetBranchAddress("scaleFactor_COMBINED", &scaleFactor_COMBINED, &b_scaleFactor_COMBINED);
   fChain->SetBranchAddress("scaleFactor_ELE", &scaleFactor_ELE, &b_scaleFactor_ELE);
   fChain->SetBranchAddress("scaleFactor_LepTRIGGER", &scaleFactor_LepTRIGGER, &b_scaleFactor_LepTRIGGER);
   fChain->SetBranchAddress("scaleFactor_MUON", &scaleFactor_MUON, &b_scaleFactor_MUON);
   fChain->SetBranchAddress("scaleFactor_PILEUP", &scaleFactor_PILEUP, &b_scaleFactor_PILEUP);
   fChain->SetBranchAddress("scaleFactor_PhotonTRIGGER", &scaleFactor_PhotonTRIGGER, &b_scaleFactor_PhotonTRIGGER);
   fChain->SetBranchAddress("trigE", &trigE, &b_trigE);
   fChain->SetBranchAddress("trigM", &trigM, &b_trigM);
   Notify();
}

Bool_t mini::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void mini::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t mini::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

Double_t mini::GetMinimum(TString varName)
{
   if (!fChain)
      return 0.0;
   return fChain->GetMinimum(varName);
}

Double_t mini::GetMaximum(TString varName)
{
   if (!fChain)
      return 0.0;
   return fChain->GetMaximum(varName);
}

#endif // #ifdef mini_cxx
