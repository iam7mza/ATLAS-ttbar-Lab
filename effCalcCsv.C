#include "mini.h"
#include "fileHelper.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iomanip>
#include <TSystem.h>
#include <TString.h>
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
            return true;
        }
    }
    return false;
}

// Function to calculate efficiencies and write to CSV
void CalculateEfficiencies(vector<CutInfo>& cuts, int totalEvents, ofstream& csvFile) {
    cout << "\n===== CUT EFFICIENCIES =====\n";
    cout << left << setw(25) << "Cut" << setw(10) << "Passed" << setw(15) << "Absolute Eff (%)" << setw(15) << "Relative Eff (%)" << endl;
    cout << string(65, '-') << endl;

    // CSV header
    csvFile << "Cut,Passed,AbsoluteEff(%),RelativeEff(%)\n";
    
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
        
        // Print to console
        cout << left << setw(25) << cuts[i].name 
             << setw(10) << cuts[i].passed 
             << setw(15) << fixed << setprecision(2) << cuts[i].efficiency
             << setw(15) << fixed << setprecision(2) << relativeEff << endl;
        
        // Write to CSV
        csvFile << cuts[i].name << ","
                << cuts[i].passed << ","
                << fixed << setprecision(2) << cuts[i].efficiency << ","
                << fixed << setprecision(2) << relativeEff << "\n";
    }
    
    cout << "\nTotal events: " << totalEvents << endl;
}

int main(int argn, char *argv[]) {
    if (argn == 1) {
        cout << "Please start runSelection.exe with an input ROOT file as argument" << endl;
        return 1;
    }

    // Input file path
    string inpath = string(argv[1]);
    // Extract basename for output naming
    TString baseName = gSystem->BaseName(inpath.c_str()); // e.g. "tt_bar_example.root"
    
    // Create output ROOT file name
    TString outpath = TString("output_Selection/") + baseName;
    outpath.ReplaceAll(".root", "_selected.root");

    // Create CSV output file name
    TString csvName = baseName;
    csvName.ReplaceAll(".root", "_efficiencies.csv");

    // Open CSV output
    ofstream csvFile(csvName.Data());
    if (!csvFile.is_open()) {
        cerr << "ERROR: Unable to open output CSV file: " << csvName.Data() << endl;
        return 1;
    }

    cout << "Processing " << baseName.Data() << endl;

    mini* tree = fileHelper::GetMiniTree(inpath);
    if (tree == nullptr) {
        cerr << "ERROR: Tree is a null pointer" << endl;
        return 1;
    }

    int nEntries = tree->GetEntries();
    cout << "INFO: Tree contains " << nEntries << " events" << endl;
    if (nEntries == 0) {
        cerr << "ERROR: Tree contains zero events" << endl;
        return 1;
    }

    TFile* newFile = new TFile(outpath, "RECREATE");
    TTree* newTree = tree->CloneTree(0);

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

    vector<double> leptonPtCuts = {30000, 40000, 500000, 60000, 700000};
    vector<double> bJetThresholds = {0.11, 0.64, 0.83, 0.94};
    vector<double> metCuts = {20000, 30000, 40000, 50000, 60000};
    vector<double> jetPtCuts = {30000, 40000, 50000, 60000, 70000};

    map<double, int> leptonPtPassCounts;
    map<double, int> bTagPassCounts;
    map<double, int> metPassCounts;
    map<double, int> jetPtPassCounts;

    for (auto& pt : leptonPtCuts) leptonPtPassCounts[pt] = 0;
    for (auto& thresh : bJetThresholds) bTagPassCounts[thresh] = 0;
    for (auto& met : metCuts) metPassCounts[met] = 0;
    for (auto& pt : jetPtCuts) jetPtPassCounts[pt] = 0;

    for (int iEntry = 0; iEntry < nEntries; ++iEntry) {
        tree->GetEntry(iEntry);
        if ((iEntry + 1) % 100000 == 0) {
            cout << "INFO: Processing event " << (iEntry + 1) << endl;
        }

        bool passCriteria = true;
        bool passOneLepton = false;
        bool passLeptonPt = false;
        bool passFourJets = false;

        if (tree->lep_n == 1) {
            passOneLepton = true;
            cuts[1].passed++;
        } else {
            passCriteria = false;
        }

        TLorentzVector lep;
        if (passOneLepton && !tree->lep_pt->empty()) {
            lep.SetPtEtaPhiE(tree->lep_pt->at(0), tree->lep_eta->at(0), tree->lep_phi->at(0), static_cast<Double_t>(tree->lep_E->at(0)));
            double lepPt = lep.Pt();
            if (lepPt >= 25) {
                passLeptonPt = true;
                cuts[2].passed++;
            } else {
                passCriteria = false;
            }
            for (auto& ptCut : leptonPtCuts) if (lepPt >= ptCut) leptonPtPassCounts[ptCut]++;
            if (passLeptonPt && fabs(lep.Eta()) < 2.5) {
                cuts[3].passed++;
            } else {
                passCriteria = false;
            }
        } else {
            passCriteria = false;
        }
        // Actually between 2 and 6 jets
        if (static_cast<int>(tree->jet_n) > 2 && static_cast<int>(tree->jet_n) < 6) {
            passFourJets = true;
            cuts[4].passed++;
        } else {
            passCriteria = false;
        }

        if (passFourJets) { //should be independent of n of jets to see how it affects the data independently
            bool anyJetPassPt = false;
            for (int i = 0; i < static_cast<int>(tree->jet_n); i++) {
                if (tree->jet_pt->at(i) > 25) { anyJetPassPt = true; break; }
                
            }
            if (anyJetPassPt) {
                cuts[5].passed++;
            } else passCriteria = false;
        }


        //b tags
        double jetTags[static_cast<int32_t>(tree->jet_n)];
        for (int i = 0; i < static_cast<int>(tree->jet_n); i++){
            jetTags[i] = tree->jet_MV2c10->at(i);
        }

        if (TagCheck(jetTags, static_cast<int>(tree->jet_n),0.83 )){
            cuts[6].passed++;
        }else{
            passCriteria = false;
        }

        //b tag scan
        for (auto& thresh : bJetThresholds) if (TagCheck(jetTags, static_cast<int>(tree->jet_n), thresh)) bTagPassCounts[thresh]++;



        //jet pt scan 
        for (auto& ptCut : jetPtCuts) { //this is the jet pt scan
            bool anyjetpass = false;
            for (int i = 0; i < static_cast<int>(tree->jet_n); i++) {
                if (tree->jet_pt->at(i) > ptCut) { anyjetpass = true; break; }
            }
            if (anyjetpass) jetPtPassCounts[ptCut]++;
                }
        
        //b tagging and jet taggeting should be separate to minimize error

        if (tree->met_et >= 0) {
            cuts[7].passed++;
            for (auto& metCut : metCuts) if (tree->met_et >= metCut) metPassCounts[metCut]++;
        } else passCriteria = false;

        if (passCriteria) {
            cuts[8].passed++;
            newTree->Fill();
        }
    }

    // Write standard efficiencies to CSV
    CalculateEfficiencies(cuts, nEntries, csvFile);

    // Write parameter scans
    csvFile << "\nScanType,CutValue,PassedEvents,Efficiency(%)\n";
    for (auto& p : leptonPtPassCounts)  csvFile << "Lepton_pT," << p.first << "," << p.second << "," << fixed << setprecision(2) << (100.0*p.second/nEntries) << "\n";
    for (auto& p : jetPtPassCounts)     csvFile << "Jet_pT,"    << p.first << "," << p.second << "," << fixed << setprecision(2) << (100.0*p.second/nEntries) << "\n";
    for (auto& p : bTagPassCounts)      csvFile << "bTag_Thresh,"<< p.first << "," << p.second << "," << fixed << setprecision(2) << (100.0*p.second/nEntries) << "\n";
    for (auto& p : metPassCounts)       csvFile << "MET,"       << p.first << "," << p.second << "," << fixed << setprecision(2) << (100.0*p.second/nEntries) << "\n";

    cout << "\nINFO: Saving new tree with " << newTree->GetEntries() << " entries" << endl;
    newFile->Write();
    newFile->Close();
    csvFile.close();
    delete newFile;

    return 0;
}
