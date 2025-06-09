#include "fileHelper.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include "string.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TLorentzVector.h"
#include "TLegend.h"
#include <vector>
#include "physicsHelper.h"
#include <fstream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// These functions might be useful. They can be found at the end of this script and don't have to be but can be altered. //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//max index finder

int findLargestIndex(double arr[], int size) {
    if (size <= 0) {
        return -1; // Return -1 if the array is empty
    }
    int maxIndex = 0;
    int maxValue = arr[0];
    for (int i = 1; i < size; ++i) {
        if (arr[i] > maxValue) {
            maxValue = arr[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

//btag checker
bool bJetCounter(double arr[], int size, float threshold) {
  int n = 0;
  for (int i = 0; i < size; i++) {
      if (arr[i] > threshold) {
          n++;  // Found an element above than threshold
      }
  }
  return n;  // number of jets above threshold
}


void SetStyle();
// Should be called before plotting to get a decent looking plot

TH1F * InitHist(TString varName,TString varUnit, int numberBins, float minBin, float maxBin, bool isData); 
// Helps you initialize a histogram so that it already has the correct labels when plotted.
// varName is the variable name in the tuple or the name you pick for a derived physical variable. You should stick to it for the rest of the analysis. (e.g. lep_eta)

void PlotHist(TString filename, TH1F * hist);
// Creates a file in with path name (e.g. "text.pdf") and plots histogram in it

int main(int argn, char *argv[]) {

  //if you want to run this script for several input files, it is useful to call the name of the file when executing this program
  if(argn==1){
    cout << "Please start plotDistribution.exe with added argument of file to be processed" << endl;
    return 1;
  }

  // path to the file to be studied, e.g.
  string path = string(argv[1]);
  // is the file a data file or not? setting this variable now might be useful
  bool isdata = false;
  if (path.find("data_") != std::string::npos){ // wenn data im Dateinamen ist, dann gehen davon aus dass Data drin ist ;)
     isdata=true;
  }
    
  // retrieve the tree from the file
  mini * tree = fileHelper::GetMiniTree(path);
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
  
  /////////////////////////////////////////////////////////////////////////////////////////////
  // To do: initialize histograms to be made
  // pT, eta (pseudorapidity), phi (azimuthal angle), energy, ...
  /////////////////////////////////////////////////////////////////////////////////////////////
  //leptons
  TH1F* h_met_et = InitHist("met_et", "[GeV]", 50, 0, 5000000, isdata);
  TH1F* h_invariantMassSystem = InitHist("invariantMassSystem", "[GeV]", 50, 0, 5000000, isdata);
  TH1F* h_invariantMass3Jets = InitHist("invariantMass3Jets", "[GeV]", 50, 0, 5000000, isdata);
  TH1F* h_pseudorapiditySystem = InitHist("pseudorapiditySystem", "[angle]", 50, -3.14, 3.14, isdata);
  TH1F* h_deltaPhiMetLepton = InitHist("deltaPhiMetLepton", "[angle]", 50, -3.14, 3.14, isdata);
  
  /// IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111111
  /// FIX THE BIN NUMBERS FOR THE ABOVE HISTOGRAMS!!!!!!!!!!!!!!!!!!!!!!!!!
 




  float deltaPhiMetLepton = 0;
  float invariantMass3Jets = 0;
  float invariantMassSystem = 0;
  float pseudorapiditySystem = 0;

  // now loop over the events in the tree
  for (int iEntry = 0; iEntry < nEntries; ++iEntry) {

    // get entry no. iEntry and tell every 100th event
    tree->GetEntry(iEntry);
    if ((iEntry+1)%10000 == 0) {
      cout << "INFO processing the " << iEntry+1 << "th event" << endl;
    }

    // For Monte Carlo, each event has to have a scale factor. 
    // The scale factors necessary can be found separately in the samples, but they have also been combined in the variable scaleFactor_COMBINED.
    float w = 1.;
    if (!isdata)
      w = (tree->scaleFactor_COMBINED * 10000. * tree->XSection * tree->mcWeight) / tree->SumWeights;

    /////////////////////////////////////////////////////////////////////////////////////////
    // To do: Get variable or calculate it (s. instructions) and fill histograms
    ///////////////////////////////////////////////////////////////////////////////////////////

  // Skip events with no leptons or more than one lepton
      if (tree->lep_pt->size() != 1) {
          continue;
      }

      // Skip events with missing transverse energy less than 1000
      if (tree->met_et < 1000) {
          continue;
      }

      // Skip events with fewer than 4 jets
      if (tree->jet_pt->size() < 4) {
          continue;
      }

      // Skip events where the leading lepton pT is less than 1000
      if (tree->lep_pt->at(0) < 1000) {
          continue;
      }

      // Skip events where the leading jet pT is less than 1000
      if (tree->jet_pt->at(0) < 1000) {
          continue;
      }

    

    // Calculate invariant mass of the system formed by the three jets with largest pT
        std::vector<TLorentzVector> jets;
        for (size_t j = 0; j < tree->jet_pt->size(); ++j) {
            TLorentzVector jet;
            jet.SetPtEtaPhiE(tree->jet_pt->at(j), tree->jet_eta->at(j), tree->jet_phi->at(j), tree->jet_E->at(j));
            jets.push_back(jet);
        }
        std::sort(jets.begin(), jets.end(), [](const TLorentzVector& a, const TLorentzVector& b) {
            return a.Pt() > b.Pt();
        });
        invariantMass3Jets = (jets[0] + jets[1] + jets[2]).M();
    
    TLorentzVector lepton;
        lepton.SetPtEtaPhiE(tree->lep_pt->at(0), tree->lep_eta->at(0), tree->lep_phi->at(0), tree->lep_E->at(0));

        // Calculate the difference in azimuthal angle between MET and lepton
        deltaPhiMetLepton = std::abs(tree->met_phi - tree->lep_phi->at(0));
        if (deltaPhiMetLepton > M_PI) {
            deltaPhiMetLepton = 2 * M_PI - deltaPhiMetLepton;
        }

        TLorentzVector* neutrino = physicsHelper::Neutrino(TLorentzVector(tree->met_et * cos(tree->met_phi), tree->met_et * sin(tree->met_phi), 0, tree->met_et), lepton);

        if (neutrino) {
            // Calculate invariant mass and pseudorapidity of the system
            TLorentzVector system = jets[0] + jets[1] + jets[2] + jets[3] + lepton + (*neutrino);
            invariantMassSystem = system.M();
            pseudorapiditySystem = system.Eta();

            delete neutrino; // Clean up dynamically allocated memory
        } else {
            std::cerr << "Neutrino reconstruction failed for event " << iEntry << std::endl;
            invariantMassSystem = 0.0;
            pseudorapiditySystem = 0.0;
        }

        h_invariantMassSystem->Fill(invariantMassSystem,w);
        h_invariantMass3Jets->Fill(invariantMass3Jets,w);
        h_pseudorapiditySystem->Fill(pseudorapiditySystem,w);
        h_met_et->Fill(tree->met_et);
        h_deltaPhiMetLepton->Fill(deltaPhiMetLepton);
        
        
  }

  SetStyle();

  ///////////////////////////////////////////////////////////////////////////////////////////
  // To do: Use PlotHist to plot and save the plots
  /////////////////////////////////////////////////////////////////////////////////////////////

  //lep
  PlotHist("invariantMass3Jets.pdf", h_invariantMass3Jets);
  PlotHist("invariantMassSystem.pdf", h_invariantMassSystem);
  PlotHist("pseudorapiditySystem.pdf", h_pseudorapiditySystem);
  PlotHist("deltaPhiMetLepton.pdf", h_deltaPhiMetLepton);
  PlotHist("met_et.pdf", h_met_et);
  

  /////////////////////////////////////////////////////////////////////////////////////////////
  // To do: Use fileHelper::SaveNewHist to save histograms
  /////////////////////////////////////////////////////////////////////////////////////////////

  //lep
  fileHelper::SaveNewHist("invariantMassSystem", h_invariantMassSystem,true);
  fileHelper::SaveNewHist("invariantMass3Jets", h_invariantMass3Jets,true);
  fileHelper::SaveNewHist("pseudorapiditySystem", h_pseudorapiditySystem,true);
  fileHelper::SaveNewHist("deltaPhiMetLepton", h_deltaPhiMetLepton,true);
  fileHelper::SaveNewHist("met_et", h_met_et,true);

  /////////////////////////////////////////////////////////////////////////////////////////////
  // To do: delete all dynamic instances to end the program properly
  /////////////////////////////////////////////////////////////////////////////////////////////
  delete h_invariantMassSystem;
  delete h_invariantMass3Jets;
  delete h_met_et;
  delete h_pseudorapiditySystem;
  delete h_deltaPhiMetLepton;

  delete tree;
  
  cout << "All dynamic objects deleted! Success!" << endl;

  return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////
////// Functions that can but do not have to be uses: ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



TH1F * InitHist(TString varName,TString varUnit, int numberBins, float minBin, float maxBin, bool isData){
  TH1F *hist = new TH1F(varName,varName,numberBins,minBin,maxBin);
  hist->SetTitle(";"+varUnit+";Events");
  hist->GetYaxis()->SetTitleOffset(1.3);
  if(!isData){
    std::cout << "MC Histogram" << std::endl;  
    hist->Sumw2(true);
  }
  else{
    std::cout << "Data Histogram" << std::endl;  
    hist->Sumw2(false);
  }
  return hist;
}


void PlotHist(TString filename, TH1F * hist){
    TCanvas * canv = new TCanvas("canv","Canvas for histogram",1);
    canv->SetLeftMargin(0.15);

    hist->Draw("HIST");

    // Calculate statistics
    double mean = hist->GetMean();
    double stddev = hist->GetStdDev();
    double entries = hist->GetEntries();

    // Create legend
    TLegend *legend = new TLegend(0.6, 0.7, 0.88, 0.88);
    legend->SetFillColor(0);
    legend->SetBorderSize(1);
    legend->SetTextSize(0.035);
   
    TString stats;
    stats.Form("Entries = %.0f", entries);
    legend->AddEntry((TObject*)0, stats, "");

    stats.Form("Mean = %.2f", mean);
    legend->AddEntry((TObject*)0, stats, "");

    stats.Form("Std Dev = %.2f", stddev);
    legend->AddEntry((TObject*)0, stats, "");

    legend->Draw();

    canv->Print("output_plotDistribution/" + filename);
    cout << "INFO: " << filename << " has been created" << endl;

    delete legend;
    delete canv;
}

void Plot2Hist(TString filename, TString varUnit, TH1F * hist1, TH1F * hist2) {
  TCanvas * canv = new TCanvas("canv","Canvas for histograms",1);
  canv->SetLeftMargin(.12);
  canv->SetRightMargin(.1);
  
  hist1->Draw("HIST");

  hist1->SetTitle(";"+varUnit+";Events");
  hist1->GetYaxis()->SetTitleOffset(1);

  hist2->Draw("HIST SAME");

  TLegend * l = new TLegend(0.5, 0.75, 0.86, 0.9, "");
  l->SetFillColor(0);
  l->SetBorderSize(1);
  l->SetTextSize(0.04);
  l->SetTextAlign(12);
  l->AddEntry(hist1, "Add description", "l");
  l->AddEntry(hist2, "Add description here", "l");
  l->Draw();

  ///////////////////////////////////////////
  // Histograms can be normalized to unit area by calling 
  // hist->Scale(1./hist->Integral()) before plotting
  // In case you decide to do that, you can use the following lines to label your plots
  ////////////////////////////////////////////////////////////////////////////////////////
  //TLatex * t = new TLatex(0.93,0.55,"#bf{Normalized to unit area}");
  //t->SetTextAngle(90);
  //t->SetNDC();
  //t->SetTextSize(.04);
  //t->DrawClone();

  canv->Print(filename);
  cout << "INFO: " << filename << " has been created" << endl;
  delete canv;
}

void SetStyle() {
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetTitleXOffset(1.3);
  gStyle->SetTitleYOffset(1.3);
  gStyle->SetTextSize(0.05);
  gStyle->SetLabelSize(0.05,"x");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetLabelSize(0.05,"y");
  gStyle->SetTitleSize(0.05,"y");
  gStyle->SetLabelSize(0.05,"z");
  gStyle->SetTitleSize(0.05,"z");
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(1.2);
  gStyle->SetHistLineWidth(2);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
}


