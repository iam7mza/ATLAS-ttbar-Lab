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
  // Extract base name (without directory)
  size_t last_slash = path.find_last_of("/\\");
  string filename = (last_slash == string::npos) ? path : path.substr(last_slash + 1);
  // Remove extension
  size_t last_dot = filename.find_last_of(".");
  string base = (last_dot == string::npos) ? filename : filename.substr(0, last_dot);
  // Remove "_selected" if present
  size_t sel_pos = base.find("_selected");
  if (sel_pos != string::npos) {
      base = base.substr(0, sel_pos);
  }
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
 TH1F* h_lep_pt = InitHist("lep_pt", "p_{T} [GeV]", 50, 0, 500000, isdata);
 TH1F* h_lep_eta = InitHist("lep_eta", "eta [angle]", 50, -2.5, 2.5, isdata);
 TH1F* h_lep_phi = InitHist("lep_phi", "phi [angle]", 50, -3.14, 3.14, isdata);
 TH1F* h_lep_E = InitHist("lep_E", "E [GeV]", 50, 0, 1000000, isdata);
 //jets 
 TH1F* h_jet_pt = InitHist("jet_pt", "p_{t} [GeV]", 50, 0, 1000000, isdata);
 TH1F* h_jet_eta = InitHist("jet_eta", "eta [angle]", 50, -2.5, 2.5, isdata);
 TH1F* h_jet_phi = InitHist("jet_phi", "phi [angle]", 50, -3.14, 3.14, isdata);
 TH1F* h_jet_E = InitHist("jet_E", "E [GeV]", 50, 0, 1000000, isdata);

  //max pt jet
 TH1F* h_maxjet_pt = InitHist("ljet_pt", "p_{t} [GeV]", 50, 0, 1000000, isdata);
 TH1F* h_maxjet_eta = InitHist("ljet_eta", "eta [angle]", 50, -2.5, 2.5, isdata);
 TH1F* h_maxjet_phi = InitHist("ljet_phi", "phi [angle]", 50, -3.14, 3.14, isdata);
 TH1F* h_maxjet_E = InitHist("ljet_E", "E [GeV]", 50, 0, 1000000, isdata);
//n jets
 TH1F* h_jet_n = InitHist("njet", "count", 15, 0, 15, isdata);
//btag jet n
 TH1F* h_bjet_n = InitHist("nbjet", "count", 15, 0, 15, isdata);
// met pt
 TH1F* h_met_et = InitHist("met", "E [GeV]", 50, 0, 1000000, isdata);

 TH1F* h_inv_M3jet = InitHist("invMasse3jets", "E [GeV]", 50, 0, 1000000, isdata);
 TH1F* h_inv_M4jetlv = InitHist("invMasse4jetslv", "E [GeV]", 50, 0, 1000000, isdata);




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

    //lep
    h_lep_pt->Fill(static_cast<double>(tree->lep_pt->at(0)),w);
    h_lep_eta->Fill(static_cast<double>(tree->lep_eta->at(0)),w);
    h_lep_phi->Fill(static_cast<double>(tree->lep_phi->at(0)),w);
    h_lep_E->Fill(static_cast<double>(tree->lep_E->at(0)),w);


    //jets
    double jetPtArr[static_cast<int32_t>(tree->jet_n)]; //for max jet pt 
    double jetTAGArray[static_cast<int32_t>(tree->jet_n)]; //define array to keep the values of the jet tag in 
    for (int i = 0; i < static_cast<int>(tree->jet_n); i++){
      h_jet_pt->Fill(static_cast<double>(tree->jet_pt->at(i)),w);
      h_jet_eta->Fill(static_cast<double>(tree->jet_eta->at(i)),w);
      h_jet_phi->Fill(static_cast<double>(tree->jet_phi->at(i)),w);
      h_jet_E->Fill(static_cast<double>(tree->jet_E->at(i)),w);

      jetPtArr[i] = static_cast<double>(tree->jet_pt->at(i));
      jetTAGArray[i] = tree->jet_MV2c10->at(i);
    }

    //max jet
    int maxindex = findLargestIndex(jetPtArr, static_cast<int32_t>(tree->jet_n));
    if (maxindex != -1){
    h_maxjet_pt->Fill(static_cast<double>(tree->jet_pt->at(maxindex)),w);
    h_maxjet_eta->Fill(static_cast<double>(tree->jet_eta->at(maxindex)),w);
    h_maxjet_phi->Fill(static_cast<double>(tree->jet_phi->at(maxindex)),w);
    h_maxjet_E->Fill(static_cast<double>(tree->jet_E->at(maxindex)),w);
    }

    //jet n 
    h_jet_n->Fill(static_cast<double>(tree->jet_n),w);

    //b jet n
    int bjet_n = bJetCounter(jetTAGArray, static_cast<int32_t>(tree->jet_n), 0.83);
    h_bjet_n->Fill(bjet_n);

    //met et
    h_met_et->Fill(static_cast<double>(tree->met_et),w);

    // ...existing code...

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
    double invMass3Jets = 0.0;
    double invMass4JetsLV = 0.0;
    if (jets.size() >= 3) {
        invMass3Jets = (jets[0] + jets[1] + jets[2]).M();
        h_inv_M3jet->Fill(invMass3Jets, w);
    }
    if (jets.size() >= 4) {
        // 4 jets + lepton + neutrino
        TLorentzVector lepton;
        lepton.SetPtEtaPhiE(tree->lep_pt->at(0), tree->lep_eta->at(0), tree->lep_phi->at(0), tree->lep_E->at(0));
        TLorentzVector* neutrino = physicsHelper::Neutrino(
            TLorentzVector(tree->met_et * cos(tree->met_phi), tree->met_et * sin(tree->met_phi), 0, tree->met_et),
            lepton);
        if (neutrino) {
            invMass4JetsLV = (jets[0] + jets[1] + jets[2] + jets[3] + lepton + (*neutrino)).M();
            h_inv_M4jetlv->Fill(invMass4JetsLV, w);
            delete neutrino;
        }

    } 

  }

  SetStyle();

  ///////////////////////////////////////////////////////////////////////////////////////////
  // To do: Use PlotHist to plot and save the plots
  /////////////////////////////////////////////////////////////////////////////////////////////

  //lep
  // PlotHist("lep_pt", h_lep_pt);
  // PlotHist("lep_eta", h_lep_eta);
  // PlotHist("lep_phi", h_lep_phi);
  // PlotHist("lep_E", h_lep_E);


  // //jets
  // PlotHist("jet_pt", h_jet_pt);
  // PlotHist("jet_eta", h_jet_eta);
  // PlotHist("jet_phi", h_jet_phi);
  // PlotHist("jet_E", h_jet_E);

  
  // //maxJet
  // PlotHist("maxjet_pt", h_maxjet_pt);
  // PlotHist("maxjet_eta", h_maxjet_eta);
  // PlotHist("maxjet_phi", h_maxjet_phi);
  // PlotHist("maxjet_E", h_maxjet_E);

  // // n jets
  // PlotHist("jet_n", h_jet_n);

  // // bjet n
  // PlotHist("bjet_n", h_bjet_n);

  // //met et
  // PlotHist("met_et", h_met_et);

  // PlotHist("invMasse3jets", h_inv_M3jet);
  // PlotHist("invMasse4jetslv", h_inv_M4jetlv);


  /////////////////////////////////////////////////////////////////////////////////////////////
  // To do: Use fileHelper::SaveNewHist to save histograms
  /////////////////////////////////////////////////////////////////////////////////////////////

  //lep
  // fileHelper::SaveNewHist("lep_pt", h_lep_pt,true);
  // fileHelper::SaveNewHist("lep_eta", h_lep_eta,true);
  // fileHelper::SaveNewHist("lep_phi", h_lep_phi,true);
  // fileHelper::SaveNewHist("lep_E", h_lep_E,true);


  // //jet
  // fileHelper::SaveNewHist("jet_pt", h_jet_pt,true);
  // fileHelper::SaveNewHist("jet_eta", h_jet_eta,true);
  // fileHelper::SaveNewHist("jet_phi", h_jet_phi,true);
  // fileHelper::SaveNewHist("jet_E", h_jet_E,true);

  // //maxjet
  // fileHelper::SaveNewHist("maxjet_pt", h_maxjet_pt,true);
  // fileHelper::SaveNewHist("maxjet_eta", h_maxjet_eta,true);
  // fileHelper::SaveNewHist("maxjet_phi", h_maxjet_phi,true);
  // fileHelper::SaveNewHist("maxjet_E", h_maxjet_E,true);

  // fileHelper::SaveNewHist("jet_n", h_jet_n,true);
  // fileHelper::SaveNewHist("bjet_n", h_bjet_n,true);
  // fileHelper::SaveNewHist("met_et", h_met_et,true);

  /////////////////////////////////////////////////////////////////////////////////////////////
  // Writing into a root file /////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////
    TFile *outFile = new TFile((std::string("output_plotDistribution/") + "histograms_" + base + "_hist.root").c_str(), "RECREATE"); /// to do: change directory, and name of file and observables, also add high level observables
      h_lep_pt->Write();
      h_lep_eta->Write();
      h_lep_phi->Write();
      h_lep_E->Write();
      h_jet_pt->Write();
      h_jet_eta->Write();
      h_jet_phi->Write();
      h_jet_E->Write();
      h_maxjet_pt->Write();
      h_maxjet_eta->Write();
      h_maxjet_phi->Write();
      h_maxjet_E->Write();
      h_jet_n->Write();
      h_bjet_n->Write();
      h_met_et->Write();
      h_inv_M3jet->Write();
      h_inv_M4jetlv->Write(); 
      outFile->Close();
      delete outFile;


  /////////////////////////////////////////////////////////////////////////////////////////////
  // To do: delete all dynamic instances to end the program properly
  /////////////////////////////////////////////////////////////////////////////////////////////
  delete h_lep_pt;
  delete h_lep_eta;
  delete h_lep_phi;
  delete h_lep_E;

  delete h_jet_pt;
  delete h_jet_eta;
  delete h_jet_phi;
  delete h_jet_E;

  delete h_maxjet_pt;
  delete h_maxjet_eta;
  delete h_maxjet_phi;
  delete h_maxjet_E;

  delete h_jet_n;
  delete h_bjet_n;
  delete h_met_et;

  delete h_inv_M3jet;
  delete h_inv_M4jetlv;

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
  hist->Draw("hist");
  canv->Print("output_plotDistribution/"+filename);
  cout << "INFO: " << filename << " has been created" << endl;
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


