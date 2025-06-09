#include "mini.h"
#include "fileHelper.h"
#include <iostream>
#include <string>
#include <fstream>
#include <TSystem.h>
#include <TLorentzVector.h>

using namespace std;


//checking for tag value function
bool TagCheck(double arr[], int size, float threshold) {
  for (int i = 0; i < size; i++) {
      if (arr[i] > threshold) {
          return true;  // Found an element above than threshold
      }
  }
  return false;  // No element above than threshold found
}


int main(int argn, char *argv[]) {

  //if you want to run this script for several input files, it is useful to call the name of the file when executing this program
  if(argn==1){
    cout << "Please start runSelection.exe with added argument of file to be processed" << endl;
    return 1;
  }
  // path to the file to be studied + filename from argument called when executing file
  string inpath = string(argv[1]);
  // This is path for samples folder. This NEEDS to be adjusted if you use a different path in order to extract just the filename
  TString filename = TString(inpath).ReplaceAll("/ceph/groups/e4/users/bgocke/Zprime/Samples/", "");

  cout << "Processing " << filename << endl;

  // retrieve the tree from the file
  mini * tree = fileHelper::GetMiniTree(inpath);
  if (tree == 0) {
    cout << "ERROR tree is null pointer" << endl;
    return 1;
  }

  // check that the tree is not empty
  int nEntries = tree->GetEntries();
  cout << "INFO tree contains " << nEntries << " events" << endl;
  if (nEntries == 0) {
    cout << "ERROR tree contains zero events" << endl;
    return 1;
  }

  // create file to which the selection is going to be saved to
  TString outpath = TString("output_Selection/") + gSystem->BaseName(filename);
  outpath.ReplaceAll(".root", "_selected.root");
  TFile * newFile = new TFile(outpath, "RECREATE");

  // make a copy of the tree to be saved after selection
  TTree * newTree = tree->CloneTree();

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // To do: initalize Variables for the effiencies
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  // now loop over the events in the tree
  for (int iEntry = 0; iEntry < nEntries; ++iEntry) {

    // get entry no. iEntry and tell every 100000th event
    tree->GetEntry(iEntry);
    if ((iEntry+1)%100000 == 0) {
      cout << "INFO processing the " << iEntry+1 << "th event" << endl;
    }

    bool passCriteria = true;

    /////////////////////////////////////////////////////////////////////////////////////////////
    // To do: Implement all required selection criteria //
    /////////////////////////////////////////////////////////////////////////////////////////////
    // you can use continue if one of the cuts is not fullfilled
    /////////////////////////////////////////////////////////////////////////////////////////////


    //lepton selection:
    TLorentzVector lep;
    // Access the elements correctly from the RVec pointers
    if (tree->lep_pt->size() > 0) {
      lep.SetPtEtaPhiE(
        (tree->lep_pt->at(0)),  
        (tree->lep_eta->at(0)), 
        (tree->lep_phi->at(0)),
        static_cast<Double_t>(tree->lep_E->at(0))
      );
    } else {
      // Handle case where there are no leptons
      passCriteria = false;
      continue;
    }
    // 1. Lepton pT > 25 GeV
    if (lep.Pt() < 0) {
      passCriteria = false;
      continue;
    }
    // 2. Lepton |η| < 2.5
    if (std::abs(lep.Eta()) >= 2.5) {
      passCriteria = false;
      continue;
    }  

    //lepton number
    if (tree->lep_n != 1) {
      passCriteria = false;
      continue;
    }

    //jet number
    if (static_cast<Double_t>(tree->jet_n) > 2 && static_cast<Double_t>(tree->jet_n) < 6){
      passCriteria = false;
      continue;
     }

    // jet pt cut
    double jetTAGArray[static_cast<int32_t>(tree->jet_n)]; //define array to keep the values of the jet tag in 

    bool anyJetPassedPt = false;
    for (int32_t i = 0; i < static_cast<int32_t>(tree->jet_n); i++){ //check number of jets; i do not know how this shit works

      if(static_cast<double_t>(tree->jet_pt->at(i)) >= 30000){ //checking if any jet passes minimum pt
        anyJetPassedPt = true;
      }
    
    //appending tag value to array
    jetTAGArray[i] = tree->jet_MV2c10->at(i);
    } //will check if one of the jets satisfies the b tagging in the next line
    if (!(TagCheck(jetTAGArray, static_cast<int32_t>(tree->jet_n), 0.83))){// the function TagCheck return true if an element is above the threashold; else false
      passCriteria = false;
      continue;
    }

    if(!anyJetPassedPt){ //checking if any jet passed min pt
      passCriteria = false;
      continue;
    }




    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////// MAYBE ADD JET_ETA CUT IF IT TURNS OUT TO BE IMPORTANT//////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    //missing Energy/Momentun cut
    if (tree->met_et < 0){
      passCriteria = false;
      continue;
    }


    // check all selection criteria and only save the event to the new
    // tree if all of them are true
    if (passCriteria) {
      newTree->Fill();
    }
    
  }

  // save new tree to file
  cout << "INFO saving new tree with " << newTree->GetEntries() << " entries" << endl;
  newFile->Write();
  gDirectory->Purge();
  newFile->Close();
  
  // end program happily
  delete newFile;

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // To do: Write code here to print the efficenies
  ////////////////////////////////////////////////////////////////////////////////////////////////////

  return 0;
}