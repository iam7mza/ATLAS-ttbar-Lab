#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include "physicsHelper.h" // Include the physicsHelper header

void CalculateObservablesAndSave(const std::string& inputFilePath, const std::string& outputFolderPath) {
    // Extract the base name of the input file (without path and extension)
    std::string baseName = std::filesystem::path(inputFilePath).stem().string();
    std::string outputFilePath = outputFolderPath + "/outobservable_" + baseName + ".root";

    // Open the input ROOT file
    TFile* inputFile = TFile::Open(inputFilePath.c_str(), "READ");
    if (!inputFile || inputFile->IsZombie()) {
        std::cerr << "Error: Unable to open input file!" << std::endl;
        return;
    }

    // Retrieve the input tree
    TTree* inputTree = dynamic_cast<TTree*>(inputFile->Get("mini"));
    if (!inputTree) {
        std::cerr << "Error: Input tree not found!" << std::endl;
        inputFile->Close();
        return;
    }


    // Variables to hold input data
    std::vector<float>* lep_pt = nullptr;
    std::vector<float>* lep_eta = nullptr;
    std::vector<float>* lep_phi = nullptr;
    std::vector<float>* lep_E = nullptr;

    std::vector<float>* jet_pt = nullptr;
    std::vector<float>* jet_eta = nullptr;
    std::vector<float>* jet_phi = nullptr;
    std::vector<float>* jet_E = nullptr;

    float met_et = 0.0;  // Missing transverse energy
    float met_phi = 0.0; // Missing transverse azimuthal angle
    float scaleFactor_COMBINED1 = 0.0;

    float XSection;
    float mcWeight;
    float SumWeights;

    inputTree->SetBranchAddress("lep_pt", &lep_pt);
    inputTree->SetBranchAddress("lep_eta", &lep_eta);
    inputTree->SetBranchAddress("lep_phi", &lep_phi);
    inputTree->SetBranchAddress("lep_E", &lep_E);
    

    inputTree->SetBranchAddress("jet_pt", &jet_pt);
    inputTree->SetBranchAddress("jet_eta", &jet_eta);
    inputTree->SetBranchAddress("jet_phi", &jet_phi);
    inputTree->SetBranchAddress("jet_E", &jet_E);

    inputTree->SetBranchAddress("met_et", &met_et);   // Set branch for missing transverse energy
    inputTree->SetBranchAddress("met_phi", &met_phi); // Set branch for missing transverse azimuthal angle
    inputTree->SetBranchAddress("scaleFactor_COMBINED", &scaleFactor_COMBINED1); //set branch for scale factor

    //parameter for w
    inputTree->SetBranchAddress("XSection", &XSection);
    inputTree->SetBranchAddress("mcWeight", &mcWeight);
    inputTree->SetBranchAddress("SumWeights", &SumWeights);

    float w;


    // Create the output folder if it doesn't exist
    std::filesystem::create_directories(outputFolderPath);

    // Create the output ROOT file and tree
    TFile* outputFile = TFile::Open(outputFilePath.c_str(), "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error: Unable to create output file!" << std::endl;
        inputFile->Close();
        return;
    }

    TTree* outputTree = new TTree("observables", "High-level observables");

    // Variables to hold calculated observables
    float Emiss_T = 0.0;
    float invariantMass3Jets = 0.0;
    float invariantMassSystem = 0.0;
    float pseudorapiditySystem = 0.0;
    float deltaPhiMetLepton = 0.0; // Difference in azimuthal angle between MET and lepton
    float scaleFactor_COMBINED = 0.0;

    outputTree->Branch("Emiss_T", &Emiss_T, "Emiss_T/F");
    outputTree->Branch("invariantMass3Jets", &invariantMass3Jets, "invariantMass3Jets/F");
    outputTree->Branch("invariantMassSystem", &invariantMassSystem, "invariantMassSystem/F");
    outputTree->Branch("pseudorapiditySystem", &pseudorapiditySystem, "pseudorapiditySystem/F");
    outputTree->Branch("deltaPhiMetLepton", &deltaPhiMetLepton, "deltaPhiMetLepton/F");
    //outputTree->Branch("scaleFactor_COMBINED", &scaleFactor_COMBINED, "scaleFactor_COMBINED/F");
    outputTree->Branch("Weights", &w, "Weights/F");

    // Loop over events in the input tree
    Long64_t nEntries = inputTree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        inputTree->GetEntry(i);

        // Skip events with no leptons or more than one lepton
        if (lep_pt->size() != 1) {
            continue;
        }

        // Skip events with missing transverse energy less than 1000
        if (met_et < 1000) {
            continue;
        }

        // Skip events with fewer than 4 jets
        if (jet_pt->size() < 4) {
            continue;
        }

        // Skip events where the leading lepton pT is less than 1000
        if (lep_pt->at(0) < 1000) {
            continue;
        }

        // Skip events where the leading jet pT is less than 1000
        if (jet_pt->at(0) < 1000) {
            continue;
        }

        //weights
        w = (scaleFactor_COMBINED * 10000. * XSection * mcWeight) / SumWeights;
        

        // Retrieve missing transverse energy
        Emiss_T = met_et;

        scaleFactor_COMBINED = scaleFactor_COMBINED1;

        // Calculate invariant mass of the system formed by the three jets with largest pT
        std::vector<TLorentzVector> jets;
        for (size_t j = 0; j < jet_pt->size(); ++j) {
            TLorentzVector jet;
            jet.SetPtEtaPhiE(jet_pt->at(j), jet_eta->at(j), jet_phi->at(j), jet_E->at(j));
            jets.push_back(jet);
        }
        std::sort(jets.begin(), jets.end(), [](const TLorentzVector& a, const TLorentzVector& b) {
            return a.Pt() > b.Pt();
        });
        invariantMass3Jets = (jets[0] + jets[1] + jets[2]).M();

        // Calculate the neutrino 4-vector using physicsHelper::Neutrino
        TLorentzVector lepton;
        lepton.SetPtEtaPhiE(lep_pt->at(0), lep_eta->at(0), lep_phi->at(0), lep_E->at(0));

        // Calculate the difference in azimuthal angle between MET and lepton
        deltaPhiMetLepton = std::abs(met_phi - lep_phi->at(0));
        if (deltaPhiMetLepton > M_PI) {
            deltaPhiMetLepton = 2 * M_PI - deltaPhiMetLepton;
        }

        TLorentzVector* neutrino = physicsHelper::Neutrino(TLorentzVector(met_et * cos(met_phi), met_et * sin(met_phi), 0, met_et), lepton);

        if (neutrino) {
            // Calculate invariant mass and pseudorapidity of the system
            TLorentzVector system = jets[0] + jets[1] + jets[2] + jets[3] + lepton + (*neutrino);
            invariantMassSystem = system.M();
            pseudorapiditySystem = system.Eta();

            delete neutrino; // Clean up dynamically allocated memory
        } else {
            std::cerr << "Neutrino reconstruction failed for event " << i << std::endl;
            invariantMassSystem = 0.0;
            pseudorapiditySystem = 0.0;
        }

        outputTree->Fill();
    }

    // Write the output tree to the file
    outputFile->cd();
    std::cout << "Writing tree to file with " << outputTree->GetEntries() << " entries. Version: " << outputTree->GetName() << std::endl;
    outputTree->Write();
    outputFile->Close();
    inputFile->Close();

    std::cout << "Output file created at: " << outputFilePath << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <inputFilePath>" << std::endl;
        return 1;
    }

    std::string inputFilePath = argv[1];
    std::string outputFolderPath = "output_observables";

    CalculateObservablesAndSave(inputFilePath, outputFolderPath);
    return 0;
}