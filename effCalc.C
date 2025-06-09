#include "mini.h"
#include "fileHelper.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <TSystem.h>
#include <TLorentzVector.h>

using namespace std;

// Structure to hold cut information
struct CutInfo {
    string name;
    int passed;
    double efficiency;
};

// Function to check for tag value
bool TagCheck(double arr[], int size, float threshold) {
  for (int i = 0; i < size; i++) {
      if (arr[i] > threshold) {
          return true;  // Found an element above threshold
      }
  }
  return false;  // No element above threshold found
}

// Function to calculate efficiencies
void CalculateEfficiencies(vector<CutInfo>& cuts, int totalEvents) {
    // No need for cumulativePass variable
    
    cout << "\n===== CUT EFFICIENCIES =====\n";
    cout << left << setw(25) << "Cut" << setw(10) << "Passed" << setw(15) << "Absolute Eff (%)" << setw(15) << "Relative Eff (%)" << endl;
    cout << string(65, '-') << endl;
    
    for (size_t i = 0; i < cuts.size(); i++) {
        // Calculate absolute efficiency (relative to total events)
        cuts[i].efficiency = 100.0 * cuts[i].passed / totalEvents;
        
        // Calculate relative efficiency (relative to previous cut)
        double relativeEff;
        if (i == 0) {
            relativeEff = cuts[i].efficiency;
        } else {
            relativeEff = (cuts[i-1].passed > 0) ? 100.0 * cuts[i].passed / cuts[i-1].passed : 0.0;
        }
        
        cout << left << setw(25) << cuts[i].name 
             << setw(10) << cuts[i].passed 
             << setw(15) << fixed << setprecision(2) << cuts[i].efficiency
             << setw(15) << fixed << setprecision(2) << relativeEff << endl;
    }
    cout << "\nTotal events: " << totalEvents << endl;
}

int main(int argn, char *argv[]) {

  // If you want to run this script for several input files, it is useful to call the name of the file when executing this program
  if(argn==1){
    cout << "Please start runSelection.exe with added argument of file to be processed" << endl;
    return 1;
  }
  // Path to the file to be studied + filename from argument called when executing file
  string inpath = string(argv[1]);
  // This is path for samples folder. This NEEDS to be adjusted if you use a different path in order to extract just the filename
  TString filename = TString(inpath).ReplaceAll("/ceph/groups/e4/users/bgocke/Zprime/Samples/", "");

  cout << "Processing " << filename << endl;

  // Retrieve the tree from the file
  mini * tree = fileHelper::GetMiniTree(inpath);
  if (tree == 0) {
    cout << "ERROR tree is null pointer" << endl;
    return 1;
  }

  // Check that the tree is not empty
  int nEntries = tree->GetEntries();
  cout << "INFO tree contains " << nEntries << " events" << endl;
  if (nEntries == 0) {
    cout << "ERROR tree contains zero events" << endl;
    return 1;
  }

  // Create file to which the selection is going to be saved to
  TString outpath = TString("output_Selection/") + gSystem->BaseName(filename);
  outpath.ReplaceAll(".root", "_selected.root");
  TFile * newFile = new TFile(outpath, "RECREATE");

  // Make a copy of the tree to be saved after selection
  TTree * newTree = tree->CloneTree(0); // Clone structure only, no entries

  // Initialize cut counters
  vector<CutInfo> cuts = {
    {"Initial", nEntries, 0.0},
    {"Exactly one lepton", 0, 0.0},
    {"Lepton pT cut", 0, 0.0},
    {"Lepton |η| < 2.5", 0, 0.0},
    {"Exactly four jets", 0, 0.0},
    {"Jet pT cut", 0, 0.0},
    {"At least one b-tagged jet", 0, 0.0},
    {"MET cut", 0, 0.0},
    {"All cuts", 0, 0.0}
  };

  // Additional vectors for testing different cut values
  vector<double> leptonPtCuts = {20000, 25000, 30000, 35000, 40000}; // Different lepton pT cuts to test
  vector<double> bJetThresholds = {0.75, 0.80, 0.83, 0.85, 0.90}; // Different b-tagging thresholds
  vector<double> metCuts = {20000, 25000, 30000, 35000, 40000}; // Different MET cuts
  
  // Create maps to store pass counts for different cut values
  map<double, int> leptonPtPassCounts;
  map<double, int> bTagPassCounts;
  map<double, int> metPassCounts;
  
  for (auto& pt : leptonPtCuts) leptonPtPassCounts[pt] = 0;
  for (auto& thresh : bJetThresholds) bTagPassCounts[thresh] = 0;
  for (auto& met : metCuts) metPassCounts[met] = 0;

  // Now loop over the events in the tree
  for (int iEntry = 0; iEntry < nEntries; ++iEntry) {
    // Get entry no. iEntry and tell every 100000th event
    tree->GetEntry(iEntry);
    if ((iEntry+1)%100000 == 0) {
      cout << "INFO processing the " << iEntry+1 << "th event" << endl;
    }

    bool passCriteria = true;
    // Track whether each individual cut is passed
    bool passOneLepton = false;
    bool passLeptonPt = false;
    bool passFourJets = false;

    // Lepton number check
    if (tree->lep_n == 1) {
      passOneLepton = true;
      cuts[1].passed++;
    } else {
      passCriteria = false;
    }

    // Lepton selection if there is one lepton
    TLorentzVector lep;
    if (passOneLepton && tree->lep_pt->size() > 0) {
      lep.SetPtEtaPhiE(
        (tree->lep_pt->at(0)),  
        (tree->lep_eta->at(0)), 
        (tree->lep_phi->at(0)),
        static_cast<Double_t>(tree->lep_E->at(0))
      );
      
      // Lepton pT cut check
      double lepPt = lep.Pt();
      if (lepPt >= 25) {
        passLeptonPt = true;
        cuts[2].passed++;
      } else {
        passCriteria = false;
      }
      
      // Check different lepton pT cut values
      for (auto& ptCut : leptonPtCuts) {
        if (lepPt >= ptCut) {
          leptonPtPassCounts[ptCut]++;
        }
      }
      
      // Lepton |η| < 2.5 check
      if (passLeptonPt && std::abs(lep.Eta()) < 2.5) {
        cuts[3].passed++;
      } else {
        passCriteria = false;
      }
    } else {
      passCriteria = false;
    }

    // Jet number check
    if (static_cast<int32_t>(tree->jet_n) == 4) {
      passFourJets = true;
      cuts[4].passed++;
    } else {
      passCriteria = false;
    }

    // Jet pT and b-tagging checks
    if (passFourJets) {
      bool allJetsPassPt = true;
      double jetTAGArray[static_cast<int32_t>(tree->jet_n)];
      
      for (int32_t i = 0; i < static_cast<int32_t>(tree->jet_n); i++) {
        // Check jet pT
        if (static_cast<double>(tree->jet_pt->at(i)) < 0) {
          allJetsPassPt = false;
          break;
        }
        
        // Store b-tag value
        jetTAGArray[i] = tree->jet_MV2c10->at(i);
      }
      
      if (allJetsPassPt) {
        cuts[5].passed++;
        
        // Check b-tagging with standard value
        if (TagCheck(jetTAGArray, static_cast<int32_t>(tree->jet_n), 0.83)) {
          cuts[6].passed++;
        } else {
          passCriteria = false;
        }
        
        // Check different b-tagging thresholds
        for (auto& thresh : bJetThresholds) {
          if (TagCheck(jetTAGArray, static_cast<int32_t>(tree->jet_n), thresh)) {
            bTagPassCounts[thresh]++;
          }
        }
      } else {
        passCriteria = false;
      }
    }

    // MET cut check
    if (tree->met_et >= 0) {
      cuts[7].passed++;
      
      // Check different MET cut values
      for (auto& metCut : metCuts) {
        if (tree->met_et >= metCut) {
          metPassCounts[metCut]++;
        }
      }
    } else {
      passCriteria = false;
    }

    // Check all selection criteria and only save the event to the new tree if all of them are true
    if (passCriteria) {
      cuts[8].passed++; // Count events passing all cuts
      newTree->Fill();
    }
  }

  // Calculate and print standard efficiencies
  CalculateEfficiencies(cuts, nEntries);
  
  // Print efficiencies for different cut values
  cout << "\n===== PARAMETER SCAN EFFICIENCIES =====\n";
  
  // Lepton pT scan
  cout << "\nLepton pT cut scan:\n";
  cout << left << setw(15) << "Cut Value (GeV)" << setw(15) << "Passed Events" << setw(15) << "Efficiency (%)" << endl;
  cout << string(45, '-') << endl;
  for (auto& pair : leptonPtPassCounts) {
    cout << left << setw(15) << pair.first 
         << setw(15) << pair.second 
         << setw(15) << fixed << setprecision(2) << (100.0 * pair.second / nEntries) << endl;
  }
  
  // b-tagging threshold scan
  // cout << "\nb-tagging threshold scan:\n";
  // cout << left << setw(15) << "Threshold" << setw(15) << "Passed Events" << setw(15) << "Efficiency (%)" << endl;
  // cout << string(45, '-') << endl;
  // for (auto& pair : bTagPassCounts) {
  //   cout << left << setw(15) << pair.first 
  //        << setw(15) << pair.second 
  //        << setw(15) << fixed << setprecision(2) << (100.0 * pair.second / nEntries) << endl;
  // }
  
  // MET cut scan
  cout << "\nMET cut scan:\n";
  cout << left << setw(15) << "Cut Value (GeV)" << setw(15) << "Passed Events" << setw(15) << "Efficiency (%)" << endl;
  cout << string(45, '-') << endl;
  for (auto& pair : metPassCounts) {
    cout << left << setw(15) << pair.first 
         << setw(15) << pair.second 
         << setw(15) << fixed << setprecision(2) << (100.0 * pair.second / nEntries) << endl;
  }

  // Save new tree to file
  cout << "\nINFO saving new tree with " << newTree->GetEntries() << " entries" << endl;
  newFile->Write();
  gDirectory->Purge();
  newFile->Close();
  
  // End program happily
  delete newFile;
  return 0;
}