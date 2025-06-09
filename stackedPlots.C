#include "fileHelper.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>
#include "string.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "atlasstyle-00-04-02/AtlasStyle.h"
#include "TROOT.h"

using namespace std;

void SetStyle();
void PlotStack(TString path, TString varUnit, THStack * mc, TH1F * data, TLegend * legend);
void PlotStackWithRatio(TString path, TString varUnit, THStack *mc, TH1F *data, TLegend *legend, bool log=false);
TH1F* CombineStack(THStack* mc);

int main() {
  ////////////////////////////////////////////////////////////////////////////////
  // This file can guide you through the process of combining monte carlo and data
  ///////////////////////////////////////////////////////////////////////////////
    
  std::string basePath = "./output_plotDistribution/";  
    
  // Let's say you want to have all the relevant plots in one file, this is how you create it
  TFile * analysis = new TFile("analysis.root", "RECREATE");

  // now you have to get the histograms you want to use. If you have saved them in another file beforehand, you can get them this way:
  // ttbar histogramme (top paare)
  TFile * file_ttbar = new TFile((basePath + "histograms_ttbar_hist.root").c_str(),"READ");
  TH1F * h_ttbar_lep_pt = (TH1F*)file_ttbar->Get("lep_pt");
  TH1F * h_ttbar_lep_eta = (TH1F*)file_ttbar->Get("lep_eta");
  TH1F * h_ttbar_lep_phi = (TH1F*)file_ttbar->Get("lep_phi");
  TH1F * h_ttbar_lep_E = (TH1F*)file_ttbar->Get("lep_E");
  TH1F * h_ttbar_jet_pt = (TH1F*)file_ttbar->Get("jet_pt");
  TH1F * h_ttbar_jet_eta = (TH1F*)file_ttbar->Get("jet_eta");
  TH1F * h_ttbar_jet_phi = (TH1F*)file_ttbar->Get("jet_phi");
  TH1F * h_ttbar_jet_E = (TH1F*)file_ttbar->Get("jet_E");
  TH1F * h_ttbar_ljet_pt = (TH1F*)file_ttbar->Get("ljet_pt");
  TH1F * h_ttbar_ljet_eta = (TH1F*)file_ttbar->Get("ljet_eta");
  TH1F * h_ttbar_ljet_phi = (TH1F*)file_ttbar->Get("ljet_phi");
  TH1F * h_ttbar_ljet_E = (TH1F*)file_ttbar->Get("ljet_E");
  TH1F * h_ttbar_njet = (TH1F*)file_ttbar->Get("njet");
  TH1F * h_ttbar_nbjet = (TH1F*)file_ttbar->Get("nbjet");
  TH1F * h_ttbar_met = (TH1F*)file_ttbar->Get("met");
  TH1F * h_ttbar_invM3jet = (TH1F*)file_ttbar->Get("invMasse3jets");
  TH1F * h_ttbar_invM4jetlv = (TH1F*)file_ttbar->Get("invMasse4jetslv");
    
  // diboson histogramme (WW, WZ, ZZ, WW+quarks, ...)
  TFile * file_diboson = new TFile((basePath + "histograms_diboson_hist.root").c_str(),"READ");
  TH1F * h_diboson_lep_pt = (TH1F*)file_diboson->Get("lep_pt");
  TH1F * h_diboson_lep_eta = (TH1F*)file_diboson->Get("lep_eta");
  TH1F * h_diboson_lep_phi = (TH1F*)file_diboson->Get("lep_phi");
  TH1F * h_diboson_lep_E = (TH1F*)file_diboson->Get("lep_E");
  TH1F * h_diboson_jet_pt = (TH1F*)file_diboson->Get("jet_pt");
  TH1F * h_diboson_jet_eta = (TH1F*)file_diboson->Get("jet_eta");
  TH1F * h_diboson_jet_phi = (TH1F*)file_diboson->Get("jet_phi");
  TH1F * h_diboson_jet_E = (TH1F*)file_diboson->Get("jet_E");
  TH1F * h_diboson_ljet_pt = (TH1F*)file_diboson->Get("ljet_pt");
  TH1F * h_diboson_ljet_eta = (TH1F*)file_diboson->Get("ljet_eta");
  TH1F * h_diboson_ljet_phi = (TH1F*)file_diboson->Get("ljet_phi");
  TH1F * h_diboson_ljet_E = (TH1F*)file_diboson->Get("ljet_E");
  TH1F * h_diboson_njet = (TH1F*)file_diboson->Get("njet");
  TH1F * h_diboson_nbjet = (TH1F*)file_diboson->Get("nbjet");
  TH1F * h_diboson_met = (TH1F*)file_diboson->Get("met");
  TH1F * h_diboson_invM3jet = (TH1F*)file_diboson->Get("invMasse3jets");
  TH1F * h_diboson_invM4jetlv = (TH1F*)file_diboson->Get("invMasse4jetslv");
    
  // singleTop histogramme  
  TFile * file_singleTop = new TFile((basePath + "histograms_singleTop_hist.root").c_str(),"READ");
  TH1F * h_singleTop_lep_pt = (TH1F*)file_singleTop->Get("lep_pt");
  TH1F * h_singleTop_lep_eta = (TH1F*)file_singleTop->Get("lep_eta");
  TH1F * h_singleTop_lep_phi = (TH1F*)file_singleTop->Get("lep_phi");
  TH1F * h_singleTop_lep_E = (TH1F*)file_singleTop->Get("lep_E");
  TH1F * h_singleTop_jet_pt = (TH1F*)file_singleTop->Get("jet_pt");
  TH1F * h_singleTop_jet_eta = (TH1F*)file_singleTop->Get("jet_eta");
  TH1F * h_singleTop_jet_phi = (TH1F*)file_singleTop->Get("jet_phi");
  TH1F * h_singleTop_jet_E = (TH1F*)file_singleTop->Get("jet_E");
  TH1F * h_singleTop_ljet_pt = (TH1F*)file_singleTop->Get("ljet_pt");
  TH1F * h_singleTop_ljet_eta = (TH1F*)file_singleTop->Get("ljet_eta");
  TH1F * h_singleTop_ljet_phi = (TH1F*)file_singleTop->Get("ljet_phi");
  TH1F * h_singleTop_ljet_E = (TH1F*)file_singleTop->Get("ljet_E");
  TH1F * h_singleTop_njet = (TH1F*)file_singleTop->Get("njet");
  TH1F * h_singleTop_nbjet = (TH1F*)file_singleTop->Get("nbjet");
  TH1F * h_singleTop_met = (TH1F*)file_singleTop->Get("met");
  TH1F * h_singleTop_invM3jet = (TH1F*)file_singleTop->Get("invMasse3jets");
  TH1F * h_singleTop_invM4jetlv   = (TH1F*)file_singleTop->Get("invMasse4jetslv");
  
  //Wjets histogramme
  TFile * file_Wjets = new TFile((basePath + "histograms_Wjets_hist.root").c_str(),"READ");
  TH1F * h_Wjets_lep_pt = (TH1F*)file_Wjets->Get("lep_pt");
  TH1F * h_Wjets_lep_eta = (TH1F*)file_Wjets->Get("lep_eta");
  TH1F * h_Wjets_lep_phi = (TH1F*)file_Wjets->Get("lep_phi");
  TH1F * h_Wjets_lep_E = (TH1F*)file_Wjets->Get("lep_E");
  TH1F * h_Wjets_jet_pt = (TH1F*)file_Wjets->Get("jet_pt");
  TH1F * h_Wjets_jet_eta = (TH1F*)file_Wjets->Get("jet_eta");
  TH1F * h_Wjets_jet_phi = (TH1F*)file_Wjets->Get("jet_phi");
  TH1F * h_Wjets_jet_E = (TH1F*)file_Wjets->Get("jet_E");
  TH1F * h_Wjets_ljet_pt = (TH1F*)file_Wjets->Get("ljet_pt");
  TH1F * h_Wjets_ljet_eta = (TH1F*)file_Wjets->Get("ljet_eta");
  TH1F * h_Wjets_ljet_phi = (TH1F*)file_Wjets->Get("ljet_phi");
  TH1F * h_Wjets_ljet_E = (TH1F*)file_Wjets->Get("ljet_E");
  TH1F * h_Wjets_njet = (TH1F*)file_Wjets->Get("njet");
  TH1F * h_Wjets_nbjet = (TH1F*)file_Wjets->Get("nbjet");
  TH1F * h_Wjets_met = (TH1F*)file_Wjets->Get("met");
  TH1F * h_Wjets_invM3jet = (TH1F*)file_Wjets->Get("invMasse3jets");
  TH1F * h_Wjets_invM4jetlv   = (TH1F*)file_Wjets->Get("invMasse4jetslv");
  
  //Zjets histogramme
  TFile * file_Zjets = new TFile((basePath + "histograms_Zjets_hist.root").c_str(),"READ");
  TH1F * h_Zjets_lep_pt = (TH1F*)file_Zjets->Get("lep_pt");
  TH1F * h_Zjets_lep_eta = (TH1F*)file_Zjets->Get("lep_eta");
  TH1F * h_Zjets_lep_phi = (TH1F*)file_Zjets->Get("lep_phi");
  TH1F * h_Zjets_lep_E = (TH1F*)file_Zjets->Get("lep_E");
  TH1F * h_Zjets_jet_pt = (TH1F*)file_Zjets->Get("jet_pt");
  TH1F * h_Zjets_jet_eta = (TH1F*)file_Zjets->Get("jet_eta");
  TH1F * h_Zjets_jet_phi = (TH1F*)file_Zjets->Get("jet_phi");
  TH1F * h_Zjets_jet_E = (TH1F*)file_Zjets->Get("jet_E");
  TH1F * h_Zjets_ljet_pt = (TH1F*)file_Zjets->Get("ljet_pt");
  TH1F * h_Zjets_ljet_eta = (TH1F*)file_Zjets->Get("ljet_eta");
  TH1F * h_Zjets_ljet_phi = (TH1F*)file_Zjets->Get("ljet_phi");
  TH1F * h_Zjets_ljet_E = (TH1F*)file_Zjets->Get("ljet_E");
  TH1F * h_Zjets_njet = (TH1F*)file_Zjets->Get("njet");
  TH1F * h_Zjets_nbjet = (TH1F*)file_Zjets->Get("nbjet");
  TH1F * h_Zjets_met = (TH1F*)file_Zjets->Get("met");
  TH1F * h_Zjets_invM3jet = (TH1F*)file_Zjets->Get("invMasse3jets");
  TH1F * h_Zjets_invM4jetlv   = (TH1F*)file_Zjets->Get("invMasse4jetslv");
    
  // Daten Histogramme
  TFile * file_data = new TFile((basePath + "histograms_data_hist.root").c_str(),"READ");
  TH1F * h_data_lep_pt = (TH1F*)file_data->Get("lep_pt");
  TH1F * h_data_lep_eta = (TH1F*)file_data->Get("lep_eta");
  TH1F * h_data_lep_phi = (TH1F*)file_data->Get("lep_phi");
  TH1F * h_data_lep_E = (TH1F*)file_data->Get("lep_E");
  TH1F * h_data_jet_pt = (TH1F*)file_data->Get("jet_pt");
  TH1F * h_data_jet_eta = (TH1F*)file_data->Get("jet_eta");
  TH1F * h_data_jet_phi = (TH1F*)file_data->Get("jet_phi");
  TH1F * h_data_jet_E = (TH1F*)file_data->Get("jet_E");
  TH1F * h_data_ljet_pt = (TH1F*)file_data->Get("ljet_pt");
  TH1F * h_data_ljet_eta = (TH1F*)file_data->Get("ljet_eta");
  TH1F * h_data_ljet_phi = (TH1F*)file_data->Get("ljet_phi");
  TH1F * h_data_ljet_E = (TH1F*)file_data->Get("ljet_E");
  TH1F * h_data_njet = (TH1F*)file_data->Get("njet");
  TH1F * h_data_nbjet = (TH1F*)file_data->Get("nbjet");
  TH1F * h_data_met = (TH1F*)file_data->Get("met");
  TH1F * h_data_invM3jet = (TH1F*)file_data->Get("invMasse3jets");
  TH1F * h_data_invM4jetlv = (TH1F*)file_data->Get("invMasse4jetslv");

  // ZPrime histogramme (erst mal nur 1TeV Masse!)
  TFile * file_zprime1000 = new TFile((basePath + "histograms_Zprime1000_hist.root").c_str(),"READ");
  TH1F * h_zprime1000_lep_pt = (TH1F*)file_zprime1000->Get("lep_pt");
  TH1F * h_zprime1000_lep_eta = (TH1F*)file_zprime1000->Get("lep_eta");
  TH1F * h_zprime1000_lep_phi = (TH1F*)file_zprime1000->Get("lep_phi");
  TH1F * h_zprime1000_lep_E = (TH1F*)file_zprime1000->Get("lep_E");
  TH1F * h_zprime1000_jet_pt = (TH1F*)file_zprime1000->Get("jet_pt");
  TH1F * h_zprime1000_jet_eta = (TH1F*)file_zprime1000->Get("jet_eta");
  TH1F * h_zprime1000_jet_phi = (TH1F*)file_zprime1000->Get("jet_phi");
  TH1F * h_zprime1000_jet_E = (TH1F*)file_zprime1000->Get("jet_E");
  TH1F * h_zprime1000_ljet_pt = (TH1F*)file_zprime1000->Get("ljet_pt");
  TH1F * h_zprime1000_ljet_eta = (TH1F*)file_zprime1000->Get("ljet_eta");
  TH1F * h_zprime1000_ljet_phi = (TH1F*)file_zprime1000->Get("ljet_phi");
  TH1F * h_zprime1000_ljet_E = (TH1F*)file_zprime1000->Get("ljet_E");
  TH1F * h_zprime1000_njet = (TH1F*)file_zprime1000->Get("njet");
  TH1F * h_zprime1000_nbjet = (TH1F*)file_zprime1000->Get("nbjet");
  TH1F * h_zprime1000_met = (TH1F*)file_zprime1000->Get("met");
  TH1F * h_zprime1000_invM3jet = (TH1F*)file_zprime1000->Get("invMasse3jets");
  TH1F * h_zprime1000_invM4jetlv = (TH1F*)file_zprime1000->Get("invMasse4jetslv");
    
  /////////////////////////////////////////////////////////////////////////////////////////
  //If you want to adjust the bin width, use Rebin(int number_of_bins_to_be_merged)
   
  //You should set a different fill color for each process using SetFillColor(Color_t fcolor); examples for fcolor are kRed, kGreen, kYellow etc.
  //  You can also add integer like kRed+2 to change the shade
  cout << "Set fill color 1" << endl;
  h_diboson_lep_pt->SetFillColor(kYellow);
  h_diboson_lep_eta->SetFillColor(kYellow);
  h_diboson_lep_phi->SetFillColor(kYellow);
  h_diboson_lep_E->SetFillColor(kYellow);
  h_diboson_jet_pt->SetFillColor(kYellow);
  h_diboson_jet_eta->SetFillColor(kYellow);
  h_diboson_jet_phi->SetFillColor(kYellow);
  h_diboson_jet_E->SetFillColor(kYellow);
  h_diboson_ljet_pt->SetFillColor(kYellow);
  h_diboson_ljet_eta->SetFillColor(kYellow);
  h_diboson_ljet_phi->SetFillColor(kYellow);
  h_diboson_ljet_E->SetFillColor(kYellow);
  h_diboson_njet->SetFillColor(kYellow);
  h_diboson_nbjet->SetFillColor(kYellow);
  h_diboson_met->SetFillColor(kYellow);
  h_diboson_invM3jet->SetFillColor(kYellow);
  h_diboson_invM4jetlv->SetFillColor(kYellow);
  cout << "Set fill color 2" << endl;
  h_singleTop_lep_pt->SetFillColor(kGreen);
  h_singleTop_lep_eta->SetFillColor(kGreen);
  h_singleTop_lep_phi->SetFillColor(kGreen);
  h_singleTop_lep_E->SetFillColor(kGreen);
  h_singleTop_jet_pt->SetFillColor(kGreen);
  h_singleTop_jet_eta->SetFillColor(kGreen);
  h_singleTop_jet_phi->SetFillColor(kGreen);
  h_singleTop_jet_E->SetFillColor(kGreen);
  h_singleTop_ljet_pt->SetFillColor(kGreen);
  h_singleTop_ljet_eta->SetFillColor(kGreen);
  h_singleTop_ljet_phi->SetFillColor(kGreen);
  h_singleTop_ljet_E->SetFillColor(kGreen);
  h_singleTop_njet->SetFillColor(kGreen);
  h_singleTop_nbjet->SetFillColor(kGreen);
  h_singleTop_met->SetFillColor(kGreen);
  h_singleTop_invM3jet->SetFillColor(kGreen);
  h_singleTop_invM4jetlv->SetFillColor(kGreen);
  cout << "Set fill color 3" << endl;
  h_ttbar_lep_pt->SetFillColor(kRed);
  h_ttbar_lep_eta->SetFillColor(kRed);
  h_ttbar_lep_phi->SetFillColor(kRed);
  h_ttbar_lep_E->SetFillColor(kRed);
  h_ttbar_jet_pt->SetFillColor(kRed);
  h_ttbar_jet_eta->SetFillColor(kRed);
  h_ttbar_jet_phi->SetFillColor(kRed);
  h_ttbar_jet_E->SetFillColor(kRed);
  h_ttbar_ljet_pt->SetFillColor(kRed);
  h_ttbar_ljet_eta->SetFillColor(kRed);
  h_ttbar_ljet_phi->SetFillColor(kRed);
  h_ttbar_ljet_E->SetFillColor(kRed);
  h_ttbar_njet->SetFillColor(kRed);
  h_ttbar_nbjet->SetFillColor(kRed);
  h_ttbar_met->SetFillColor(kRed);
  h_ttbar_invM3jet->SetFillColor(kRed);
  h_ttbar_invM4jetlv->SetFillColor(kRed);
  cout << "Set fill color 4" << endl;
  h_Wjets_lep_pt->SetFillColor(kBlue);
  h_Wjets_lep_eta->SetFillColor(kBlue);
  h_Wjets_lep_phi->SetFillColor(kBlue);
  h_Wjets_lep_E->SetFillColor(kBlue);
  h_Wjets_jet_pt->SetFillColor(kBlue);
  h_Wjets_jet_eta->SetFillColor(kBlue);
  h_Wjets_jet_phi->SetFillColor(kBlue);
  h_Wjets_jet_E->SetFillColor(kBlue);
  h_Wjets_ljet_pt->SetFillColor(kBlue);
  h_Wjets_ljet_eta->SetFillColor(kBlue);
  h_Wjets_ljet_phi->SetFillColor(kBlue);
  h_Wjets_ljet_E->SetFillColor(kBlue);
  h_Wjets_njet->SetFillColor(kBlue);
  h_Wjets_nbjet->SetFillColor(kBlue);
  h_Wjets_met->SetFillColor(kBlue);
  h_Wjets_invM3jet->SetFillColor(kBlue);
  h_Wjets_invM4jetlv->SetFillColor(kBlue);
  cout << "Set fill color 5" << endl;
  h_Zjets_lep_pt->SetFillColor(kOrange);
  h_Zjets_lep_eta->SetFillColor(kOrange);
  h_Zjets_lep_phi->SetFillColor(kOrange);
  h_Zjets_lep_E->SetFillColor(kOrange);
  h_Zjets_jet_pt->SetFillColor(kOrange);
  h_Zjets_jet_eta->SetFillColor(kOrange);
  h_Zjets_jet_phi->SetFillColor(kOrange);
  h_Zjets_jet_E->SetFillColor(kOrange);
  h_Zjets_ljet_pt->SetFillColor(kOrange);
  h_Zjets_ljet_eta->SetFillColor(kOrange);
  h_Zjets_ljet_phi->SetFillColor(kOrange);
  h_Zjets_ljet_E->SetFillColor(kOrange);
  h_Zjets_njet->SetFillColor(kOrange);
  h_Zjets_nbjet->SetFillColor(kOrange);
  h_Zjets_met->SetFillColor(kOrange);
  h_Zjets_invM3jet->SetFillColor(kOrange);
  h_Zjets_invM4jetlv->SetFillColor(kOrange);
  cout << "Set fill color 6" << endl;
  h_zprime1000_lep_pt->SetFillColor(kCyan);
  h_zprime1000_lep_eta->SetFillColor(kCyan);
  h_zprime1000_lep_phi->SetFillColor(kCyan);
  h_zprime1000_lep_E->SetFillColor(kCyan);
  h_zprime1000_jet_pt->SetFillColor(kCyan);
  h_zprime1000_jet_eta->SetFillColor(kCyan);
  h_zprime1000_jet_phi->SetFillColor(kCyan);
  h_zprime1000_jet_E->SetFillColor(kCyan);
  h_zprime1000_ljet_pt->SetFillColor(kCyan);
  h_zprime1000_ljet_eta->SetFillColor(kCyan);
  h_zprime1000_ljet_phi->SetFillColor(kCyan);
  h_zprime1000_ljet_E->SetFillColor(kCyan);
  h_zprime1000_njet->SetFillColor(kCyan);
  h_zprime1000_nbjet->SetFillColor(kCyan);
  h_zprime1000_met->SetFillColor(kCyan);
  h_zprime1000_invM3jet->SetFillColor(kCyan);
  h_zprime1000_invM4jetlv->SetFillColor(kCyan);
 

  
  //You might also ant to change the line color using e.g. SetLineColor(kBlack)
  cout << "Set line color" << endl;
  h_diboson_lep_pt->SetLineColor(kYellow);
  h_diboson_lep_eta->SetLineColor(kYellow);
  h_diboson_lep_phi->SetLineColor(kYellow);
  h_diboson_lep_E->SetLineColor(kYellow);
  h_diboson_jet_pt->SetLineColor(kYellow);
  h_diboson_jet_eta->SetLineColor(kYellow);
  h_diboson_jet_phi->SetLineColor(kYellow);
  h_diboson_jet_E->SetLineColor(kYellow);
  h_diboson_ljet_pt->SetLineColor(kYellow);
  h_diboson_ljet_eta->SetLineColor(kYellow);
  h_diboson_ljet_phi->SetLineColor(kYellow);
  h_diboson_ljet_E->SetLineColor(kYellow);
  h_diboson_njet->SetLineColor(kYellow);
  h_diboson_nbjet->SetLineColor(kYellow);
  h_diboson_met->SetLineColor(kYellow);
  h_diboson_invM3jet->SetLineColor(kYellow);
  h_diboson_invM4jetlv->SetLineColor(kYellow);
  
  h_singleTop_lep_pt->SetLineColor(kGreen);
  h_singleTop_lep_eta->SetLineColor(kGreen);
  h_singleTop_lep_phi->SetLineColor(kGreen);
  h_singleTop_lep_E->SetLineColor(kGreen);
  h_singleTop_jet_pt->SetLineColor(kGreen);
  h_singleTop_jet_eta->SetLineColor(kGreen);
  h_singleTop_jet_phi->SetLineColor(kGreen);
  h_singleTop_jet_E->SetLineColor(kGreen);
  h_singleTop_ljet_pt->SetLineColor(kGreen);
  h_singleTop_ljet_eta->SetLineColor(kGreen);
  h_singleTop_ljet_phi->SetLineColor(kGreen);
  h_singleTop_ljet_E->SetLineColor(kGreen);
  h_singleTop_njet->SetLineColor(kGreen);
  h_singleTop_nbjet->SetLineColor(kGreen);
  h_singleTop_met->SetLineColor(kGreen);
  h_singleTop_invM3jet->SetLineColor(kGreen);
  h_singleTop_invM4jetlv->SetLineColor(kGreen);
  
  h_ttbar_lep_pt->SetLineColor(kRed);
  h_ttbar_lep_eta->SetLineColor(kRed);
  h_ttbar_lep_phi->SetLineColor(kRed);
  h_ttbar_lep_E->SetLineColor(kRed);
  h_ttbar_jet_pt->SetLineColor(kRed);
  h_ttbar_jet_eta->SetLineColor(kRed);
  h_ttbar_jet_phi->SetLineColor(kRed);
  h_ttbar_jet_E->SetLineColor(kRed);
  h_ttbar_ljet_pt->SetLineColor(kRed);
  h_ttbar_ljet_eta->SetLineColor(kRed);
  h_ttbar_ljet_phi->SetLineColor(kRed);
  h_ttbar_ljet_E->SetLineColor(kRed);
  h_ttbar_njet->SetLineColor(kRed);
  h_ttbar_nbjet->SetLineColor(kRed);
  h_ttbar_met->SetLineColor(kRed);
  h_ttbar_invM3jet->SetLineColor(kRed);
  h_ttbar_invM4jetlv->SetLineColor(kRed);
  
  h_Wjets_lep_pt->SetLineColor(kBlue);
  h_Wjets_lep_eta->SetLineColor(kBlue);
  h_Wjets_lep_phi->SetLineColor(kBlue);
  h_Wjets_lep_E->SetLineColor(kBlue);
  h_Wjets_jet_pt->SetLineColor(kBlue);
  h_Wjets_jet_eta->SetLineColor(kBlue);
  h_Wjets_jet_phi->SetLineColor(kBlue);
  h_Wjets_jet_E->SetLineColor(kBlue);
  h_Wjets_ljet_pt->SetLineColor(kBlue);
  h_Wjets_ljet_eta->SetLineColor(kBlue);
  h_Wjets_ljet_phi->SetLineColor(kBlue);
  h_Wjets_ljet_E->SetLineColor(kBlue);
  h_Wjets_njet->SetLineColor(kBlue);
  h_Wjets_nbjet->SetLineColor(kBlue);
  h_Wjets_met->SetLineColor(kBlue);
  h_Wjets_invM3jet->SetLineColor(kBlue);
  h_Wjets_invM4jetlv->SetLineColor(kBlue);
  
  h_Zjets_lep_pt->SetLineColor(kOrange);
  h_Zjets_lep_eta->SetLineColor(kOrange);
  h_Zjets_lep_phi->SetLineColor(kOrange);
  h_Zjets_lep_E->SetLineColor(kOrange);
  h_Zjets_jet_pt->SetLineColor(kOrange);
  h_Zjets_jet_eta->SetLineColor(kOrange);
  h_Zjets_jet_phi->SetLineColor(kOrange);
  h_Zjets_jet_E->SetFillColor(kOrange);
  h_Zjets_ljet_pt->SetLineColor(kOrange);
  h_Zjets_ljet_eta->SetLineColor(kOrange);
  h_Zjets_ljet_phi->SetLineColor(kOrange);
  h_Zjets_ljet_E->SetLineColor(kOrange);
  h_Zjets_njet->SetLineColor(kOrange);
  h_Zjets_nbjet->SetLineColor(kOrange);
  h_Zjets_met->SetFillColor(kOrange);
  h_Zjets_invM3jet->SetLineColor(kOrange);
  h_Zjets_invM4jetlv->SetLineColor(kOrange);
  
  h_zprime1000_lep_pt->SetLineColor(kCyan);
  h_zprime1000_lep_eta->SetLineColor(kCyan);
  h_zprime1000_lep_phi->SetLineColor(kCyan);
  h_zprime1000_lep_E->SetLineColor(kCyan);
  h_zprime1000_jet_pt->SetLineColor(kCyan);
  h_zprime1000_jet_eta->SetLineColor(kCyan);
  h_zprime1000_jet_phi->SetLineColor(kCyan);
  h_zprime1000_jet_E->SetFillColor(kCyan);
  h_zprime1000_ljet_pt->SetLineColor(kCyan);
  h_zprime1000_ljet_eta->SetLineColor(kCyan);
  h_zprime1000_ljet_phi->SetLineColor(kCyan);
  h_zprime1000_ljet_E->SetLineColor(kCyan);
  h_zprime1000_njet->SetLineColor(kCyan);
  h_zprime1000_nbjet->SetLineColor(kCyan);
  h_zprime1000_met->SetFillColor(kCyan);
  h_zprime1000_invM3jet->SetLineColor(kCyan);
  h_zprime1000_invM4jetlv->SetLineColor(kCyan);
    
  //You should add a legend to be able to distinguish the different processes
  cout << "Create legend" << endl;
  TLegend *leg = new TLegend(0.7,0.6,0.85,0.9);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->AddEntry(h_diboson_invM4jetlv, "Diboson", "f");
  leg->AddEntry(h_singleTop_invM4jetlv, "single top", "f");
  leg->AddEntry(h_Wjets_invM4jetlv, "W+jet", "f");
  leg->AddEntry(h_Zjets_invM4jetlv, "Z+jet", "f");
  leg->AddEntry(h_ttbar_invM4jetlv, "tt", "f");
  //leg->AddEntry(h_zprime1000_invM4jetlv, "Z' (1 TeV)", "f");
  leg->AddEntry(h_data_invM4jetlv,  "Data", "p");

  //To plot several MC histograms, use THStack. At this point you should be able to figure out its use by looking it up online. 
  cout << "Doing stack now" << endl;
  THStack *h_mc_invM4jetlv_stacked = new THStack("h_mc_invM4jetlv_stacked","Stacked invM4jetlv mc histograms");
  h_mc_invM4jetlv_stacked->Add(h_diboson_invM4jetlv);
  h_mc_invM4jetlv_stacked->Add(h_singleTop_invM4jetlv);
  h_mc_invM4jetlv_stacked->Add(h_ttbar_invM4jetlv);
  h_mc_invM4jetlv_stacked->Add(h_Wjets_invM4jetlv);
  h_mc_invM4jetlv_stacked->Add(h_Zjets_invM4jetlv);
  //h_mc_invM4jetlv_stacked->Add(h_zprime1000_invM4jetlv);
    
  THStack *h_mc_invM3jet_stacked = new THStack("h_mc_invM3jet_stacked","Stacked invM3jet mc histograms");
  h_mc_invM3jet_stacked->Add(h_diboson_invM3jet);
  h_mc_invM3jet_stacked->Add(h_singleTop_invM3jet);
  h_mc_invM3jet_stacked->Add(h_ttbar_invM3jet);
  h_mc_invM3jet_stacked->Add(h_Wjets_invM3jet);
  h_mc_invM3jet_stacked->Add(h_Zjets_invM3jet);
  //h_mc_lep_pt_stacked->Add(h_zprime1000_lep_pt);
  
  THStack *h_mc_lep_pt_stacked = new THStack("h_mc_lep_pt_stacked","Stacked lep pt mc histograms");
  h_mc_lep_pt_stacked->Add(h_diboson_lep_pt);
  h_mc_lep_pt_stacked->Add(h_singleTop_lep_pt);
  h_mc_lep_pt_stacked->Add(h_ttbar_lep_pt);
  h_mc_lep_pt_stacked->Add(h_Wjets_lep_pt);
  h_mc_lep_pt_stacked->Add(h_Zjets_lep_pt);
    
  THStack *h_mc_lep_phi_stacked = new THStack("h_mc_lep_phi_stacked","Stacked lep phi mc histograms");
  h_mc_lep_phi_stacked->Add(h_diboson_lep_phi);
  h_mc_lep_phi_stacked->Add(h_singleTop_lep_phi);
  h_mc_lep_phi_stacked->Add(h_ttbar_lep_phi);
  h_mc_lep_phi_stacked->Add(h_Wjets_lep_phi);
  h_mc_lep_phi_stacked->Add(h_Zjets_lep_phi);
    
  THStack *h_mc_lep_eta_stacked = new THStack("h_mc_lep_eta_stacked","Stacked lep eta mc histograms");
  h_mc_lep_eta_stacked->Add(h_diboson_lep_eta);
  h_mc_lep_eta_stacked->Add(h_singleTop_lep_eta);
  h_mc_lep_eta_stacked->Add(h_ttbar_lep_eta);
  h_mc_lep_eta_stacked->Add(h_Wjets_lep_eta);
  h_mc_lep_eta_stacked->Add(h_Zjets_lep_eta);
    
  THStack *h_mc_lep_E_stacked = new THStack("h_mc_lep_E_stacked","Stacked lep E mc histograms");
  h_mc_lep_E_stacked->Add(h_diboson_lep_E);
  h_mc_lep_E_stacked->Add(h_singleTop_lep_E);
  h_mc_lep_E_stacked->Add(h_ttbar_lep_E);
  h_mc_lep_E_stacked->Add(h_Wjets_lep_E);
  h_mc_lep_E_stacked->Add(h_Zjets_lep_E);
    
    
  THStack *h_mc_jet_pt_stacked = new THStack("h_mc_jet_pt_stacked","Stacked jet pt mc histograms");
  h_mc_jet_pt_stacked->Add(h_diboson_jet_pt);
  h_mc_jet_pt_stacked->Add(h_singleTop_jet_pt);
  h_mc_jet_pt_stacked->Add(h_ttbar_jet_pt);
  h_mc_jet_pt_stacked->Add(h_Wjets_jet_pt);
  h_mc_jet_pt_stacked->Add(h_Zjets_jet_pt);
    
  THStack *h_mc_jet_phi_stacked = new THStack("h_mc_jet_phi_stacked","Stacked jet phi mc histograms");
  h_mc_jet_phi_stacked->Add(h_diboson_jet_phi);
  h_mc_jet_phi_stacked->Add(h_singleTop_jet_phi);
  h_mc_jet_phi_stacked->Add(h_ttbar_jet_phi);
  h_mc_jet_phi_stacked->Add(h_Wjets_jet_phi);
  h_mc_jet_phi_stacked->Add(h_Zjets_jet_phi);
    
  THStack *h_mc_jet_eta_stacked = new THStack("h_mc_jet_eta_stacked","Stacked jet eta mc histograms");
  h_mc_jet_eta_stacked->Add(h_diboson_jet_eta);
  h_mc_jet_eta_stacked->Add(h_singleTop_jet_eta);
  h_mc_jet_eta_stacked->Add(h_ttbar_jet_eta);
  h_mc_jet_eta_stacked->Add(h_Wjets_jet_eta);
  h_mc_jet_eta_stacked->Add(h_Zjets_jet_eta);
    
  THStack *h_mc_jet_E_stacked = new THStack("h_mc_jet_E_stacked","Stacked jet E mc histograms");
  h_mc_jet_E_stacked->Add(h_diboson_jet_E);
  h_mc_jet_E_stacked->Add(h_singleTop_jet_E);
  h_mc_jet_E_stacked->Add(h_ttbar_jet_E);
  h_mc_jet_E_stacked->Add(h_Wjets_jet_E);
  h_mc_jet_E_stacked->Add(h_Zjets_jet_E);
    
  
  THStack *h_mc_ljet_pt_stacked = new THStack("h_mc_ljet_pt_stacked","Stacked ljet pt mc histograms");
  h_mc_ljet_pt_stacked->Add(h_diboson_ljet_pt);
  h_mc_ljet_pt_stacked->Add(h_singleTop_ljet_pt);
  h_mc_ljet_pt_stacked->Add(h_ttbar_ljet_pt);
  h_mc_ljet_pt_stacked->Add(h_Wjets_ljet_pt);
  h_mc_ljet_pt_stacked->Add(h_Zjets_ljet_pt);
    
  THStack *h_mc_ljet_phi_stacked = new THStack("h_mc_ljet_phi_stacked","Stacked ljet phi mc histograms");
  h_mc_ljet_phi_stacked->Add(h_diboson_ljet_phi);
  h_mc_ljet_phi_stacked->Add(h_singleTop_ljet_phi);
  h_mc_ljet_phi_stacked->Add(h_ttbar_ljet_phi);
  h_mc_ljet_phi_stacked->Add(h_Wjets_ljet_phi);
  h_mc_ljet_phi_stacked->Add(h_Zjets_ljet_phi);
    
  THStack *h_mc_ljet_eta_stacked = new THStack("h_mc_ljet_eta_stacked","Stacked ljet eta mc histograms");
  h_mc_ljet_eta_stacked->Add(h_diboson_ljet_eta);
  h_mc_ljet_eta_stacked->Add(h_singleTop_ljet_eta);
  h_mc_ljet_eta_stacked->Add(h_ttbar_ljet_eta);
  h_mc_ljet_eta_stacked->Add(h_Wjets_ljet_eta);
  h_mc_ljet_eta_stacked->Add(h_Zjets_ljet_eta);
    
  THStack *h_mc_ljet_E_stacked = new THStack("h_mc_ljet_E_stacked","Stacked ljet E mc histograms");
  h_mc_ljet_E_stacked->Add(h_diboson_ljet_E);
  h_mc_ljet_E_stacked->Add(h_singleTop_ljet_E);
  h_mc_ljet_E_stacked->Add(h_ttbar_ljet_E);
  h_mc_ljet_E_stacked->Add(h_Wjets_ljet_E);
  h_mc_ljet_E_stacked->Add(h_Zjets_ljet_E);
    
  
  THStack *h_mc_njet_stacked = new THStack("h_mc_njet_stacked","Stacked njet mc histograms");
  h_mc_njet_stacked->Add(h_diboson_njet);
  h_mc_njet_stacked->Add(h_singleTop_njet);
  h_mc_njet_stacked->Add(h_ttbar_njet);
  h_mc_njet_stacked->Add(h_Wjets_njet);
  h_mc_njet_stacked->Add(h_Zjets_njet);
    
  THStack *h_mc_nbjet_stacked = new THStack("h_mc_nbjet_stacked","Stacked nbjet mc histograms");
  h_mc_nbjet_stacked->Add(h_diboson_nbjet);
  h_mc_nbjet_stacked->Add(h_singleTop_nbjet);
  h_mc_nbjet_stacked->Add(h_ttbar_nbjet);
  h_mc_nbjet_stacked->Add(h_Wjets_nbjet);
  h_mc_nbjet_stacked->Add(h_Zjets_nbjet);
    
  THStack *h_mc_met_stacked = new THStack("h_mc_met_stacked","Stacked met mc histograms");
  h_mc_met_stacked->Add(h_diboson_met);
  h_mc_met_stacked->Add(h_singleTop_met);
  h_mc_met_stacked->Add(h_ttbar_met);
  h_mc_met_stacked->Add(h_Wjets_met);
  h_mc_met_stacked->Add(h_Zjets_met);
   
  //For further analysis, you should however combine them to a new TH1F
  // ** we only need the invM4jetlv histogram for later !! **
  TH1F* h_background = (TH1F*)h_diboson_invM4jetlv->Clone();
  h_background->Add(h_singleTop_invM4jetlv);
  h_background->Add(h_ttbar_invM4jetlv);
  h_background->Add(h_Wjets_invM4jetlv);
  h_background->Add(h_Zjets_invM4jetlv);
  h_background->SetTitle("background");
  h_background->SetName("background");
    
  //For histograms of data, you can use the following commands to change the attributes of the histobramm
  cout << "Setting data layout" << endl;
  //h_data_invM4jetlv->SetLineWidth(0);
  h_data_invM4jetlv->SetLineColor(kBlack);
  h_data_invM4jetlv->SetLineStyle(1);
  h_data_invM4jetlv->SetMarkerStyle(20);
  h_data_invM4jetlv->SetMarkerSize(1.0);
  h_data_invM4jetlv->SetMarkerColor(kBlack);
    
  //h_data_invM3jet->SetLineWidth(0);
  h_data_invM3jet->SetLineColor(kBlack);
  h_data_invM3jet->SetLineStyle(1);
  h_data_invM3jet->SetMarkerStyle(20);
  h_data_invM3jet->SetMarkerSize(1.0);
  h_data_invM3jet->SetMarkerColor(kBlack);
    
  
  //h_data_lep_pt->SetLineWidth(0);
  h_data_lep_pt->SetLineColor(kBlack);
  h_data_lep_pt->SetLineStyle(1);
  h_data_lep_pt->SetMarkerStyle(20);
  h_data_lep_pt->SetMarkerSize(1.0);
  h_data_lep_pt->SetMarkerColor(kBlack);
    
  //h_data_lep_phi->SetLineWidth(0);
  h_data_lep_phi->SetLineColor(kBlack);
  h_data_lep_phi->SetLineStyle(1);
  h_data_lep_phi->SetMarkerStyle(20);
  h_data_lep_phi->SetMarkerSize(1.0);
  h_data_lep_phi->SetMarkerColor(kBlack);
    
  //h_data_lep_eta->SetLineWidth(0);
  h_data_lep_eta->SetLineColor(kBlack);
  h_data_lep_eta->SetLineStyle(1);
  h_data_lep_eta->SetMarkerStyle(20);
  h_data_lep_eta->SetMarkerSize(1.0);
  h_data_lep_eta->SetMarkerColor(kBlack);
    
  //h_data_lep_E->SetLineWidth(0);
  h_data_lep_E->SetLineColor(kBlack);
  h_data_lep_E->SetLineStyle(1);
  h_data_lep_E->SetMarkerStyle(20);
  h_data_lep_E->SetMarkerSize(1.0);
  h_data_lep_E->SetMarkerColor(kBlack);
    
  
  //h_data_jet_pt->SetLineWidth(0);
  h_data_jet_pt->SetLineColor(kBlack);
  h_data_jet_pt->SetLineStyle(1);
  h_data_jet_pt->SetMarkerStyle(20);
  h_data_jet_pt->SetMarkerSize(1.0);
  h_data_jet_pt->SetMarkerColor(kBlack);
    
  //h_data_jet_phi->SetLineWidth(0);
  h_data_jet_phi->SetLineColor(kBlack);
  h_data_jet_phi->SetLineStyle(1);
  h_data_jet_phi->SetMarkerStyle(20);
  h_data_jet_phi->SetMarkerSize(1.0);
  h_data_jet_phi->SetMarkerColor(kBlack);
    
  //h_data_jet_eta->SetLineWidth(0);
  h_data_jet_eta->SetLineColor(kBlack);
  h_data_jet_eta->SetLineStyle(1);
  h_data_jet_eta->SetMarkerStyle(20);
  h_data_jet_eta->SetMarkerSize(1.0);
  h_data_jet_eta->SetMarkerColor(kBlack);
    
  //h_data_jet_E->SetLineWidth(0);
  h_data_jet_E->SetLineColor(kBlack);
  h_data_jet_E->SetLineStyle(1);
  h_data_jet_E->SetMarkerStyle(20);
  h_data_jet_E->SetMarkerSize(1.0);
  h_data_jet_E->SetMarkerColor(kBlack);
    
  
  //h_data_ljet_pt->SetLineWidth(0);
  h_data_ljet_pt->SetLineColor(kBlack);
  h_data_ljet_pt->SetLineStyle(1);
  h_data_ljet_pt->SetMarkerStyle(20);
  h_data_ljet_pt->SetMarkerSize(1.0);
  h_data_ljet_pt->SetMarkerColor(kBlack);
    
  //h_data_ljet_phi->SetLineWidth(0);
  h_data_ljet_phi->SetLineColor(kBlack);
  h_data_ljet_phi->SetLineStyle(1);
  h_data_ljet_phi->SetMarkerStyle(20);
  h_data_ljet_phi->SetMarkerSize(1.0);
  h_data_ljet_phi->SetMarkerColor(kBlack);
    
  //h_data_ljet_eta->SetLineWidth(0);
  h_data_ljet_eta->SetLineColor(kBlack);
  h_data_ljet_eta->SetLineStyle(1);
  h_data_ljet_eta->SetMarkerStyle(20);
  h_data_ljet_eta->SetMarkerSize(1.0);
  h_data_ljet_eta->SetMarkerColor(kBlack);
    
  //h_data_ljet_E->SetLineWidth(0);
  h_data_ljet_E->SetLineColor(kBlack);
  h_data_ljet_E->SetLineStyle(1);
  h_data_ljet_E->SetMarkerStyle(20);
  h_data_ljet_E->SetMarkerSize(1.0);
  h_data_ljet_E->SetMarkerColor(kBlack);
     
  //h_data_njet->SetLineWidth(0);
  h_data_njet->SetLineColor(kBlack);
  h_data_njet->SetLineStyle(1);
  h_data_njet->SetMarkerStyle(20);
  h_data_njet->SetMarkerSize(1.0);   
  h_data_njet->SetMarkerColor(kBlack);
      
  //h_data_nbjet->SetLineWidth(0);
  h_data_nbjet->SetLineColor(kBlack);
  h_data_nbjet->SetLineStyle(1);
  h_data_nbjet->SetMarkerStyle(20);    
  h_data_nbjet->SetMarkerSize(1.0);
  h_data_nbjet->SetMarkerColor(kBlack);
      
  //h_data_met->SetLineWidth(0);
  h_data_met->SetLineColor(kBlack);
  h_data_met->SetLineStyle(1);
  h_data_met->SetMarkerStyle(20);
  h_data_met->SetMarkerSize(1.0);
  h_data_met->SetMarkerColor(kBlack);
  
  //For plotting data and stacked MC, you can use the function PlotStack at the end of this file 
  TString output_invM4jetlv = "./output_stackedPlots/h_four_jet_lep_neutrino_inv_mass.pdf"; 
  TString output_invM3jet = "./output_stackedPlots/h_mc_invM3jet_stacked.pdf";
  TString output_lep_pt = "./output_stackedPlots/h_mc_lep_pt_stacked.pdf";
  TString output_lep_phi = "./output_stackedPlots/h_mc_lep_phi_stacked.pdf";
  TString output_lep_eta = "./output_stackedPlots/h_mc_lep_eta_stacked.pdf";
  TString output_lep_E = "./output_stackedPlots/h_mc_lep_E_stacked.pdf";
  TString output_jet_pt = "./output_stackedPlots/h_mc_jet_pt_stacked.pdf";
  TString output_jet_phi = "./output_stackedPlots/h_mc_jet_phi_stacked.pdf";
  TString output_jet_eta = "./output_stackedPlots/h_mc_jet_eta_stacked.pdf";
  TString output_jet_E = "./output_stackedPlots/h_mc_jet_E_stacked.pdf";
  TString output_ljet_pt = "./output_stackedPlots/h_mc_ljet_pt_stacked.pdf";
  TString output_ljet_phi = "./output_stackedPlots/h_mc_ljet_phi_stacked.pdf";
  TString output_ljet_eta = "./output_stackedPlots/h_mc_ljet_eta_stacked.pdf";
  TString output_ljet_E = "./output_stackedPlots/h_mc_ljet_E_stacked.pdf";
  TString output_njet = "./output_stackedPlots/h_mc_njet_stacked.pdf";
  TString output_nbjet = "./output_stackedPlots/h_mc_nbjet_stacked.pdf";
  TString output_met = "./output_stackedPlots/h_mc_met_stacked.pdf";
    
  cout << "Plotting Stacked ..." << endl;
  PlotStack(output_invM4jetlv, "m_{4j,l,v} [GeV]", h_mc_invM4jetlv_stacked, h_data_invM4jetlv, leg);
  PlotStack(output_invM3jet, "m_{3j} [GeV]", h_mc_invM3jet_stacked, h_data_invM3jet, leg);
  
  PlotStack(output_lep_pt, "p_{T} (l) [GeV]", h_mc_lep_pt_stacked, h_data_lep_pt, leg);
  PlotStack(output_lep_eta, "#eta (l)", h_mc_lep_eta_stacked, h_data_lep_eta, leg);
  PlotStack(output_lep_phi, "#phi (l)", h_mc_lep_phi_stacked, h_data_lep_phi, leg);
  PlotStack(output_lep_E, "E (l) [GeV]", h_mc_lep_E_stacked, h_data_lep_E, leg);
    
  PlotStack(output_jet_pt, "p_{T} (jets) [GeV]", h_mc_jet_pt_stacked, h_data_jet_pt, leg);
  PlotStack(output_jet_eta, "#eta (jets)", h_mc_jet_eta_stacked, h_data_jet_eta, leg);
  PlotStack(output_jet_phi, "#phi (jets)", h_mc_jet_phi_stacked, h_data_jet_phi, leg);
  PlotStack(output_jet_E, "E (jets) [GeV]", h_mc_jet_E_stacked, h_data_jet_E, leg);  
    
  PlotStack(output_ljet_pt, "p_{T} (ljet) [GeV]", h_mc_ljet_pt_stacked, h_data_ljet_pt, leg);
  PlotStack(output_ljet_eta, "#eta (ljet)", h_mc_ljet_eta_stacked, h_data_ljet_eta, leg);
  PlotStack(output_ljet_phi, "#phi (ljet)", h_mc_ljet_phi_stacked, h_data_ljet_phi, leg);
  PlotStack(output_ljet_E, "E (ljet) [GeV]", h_mc_ljet_E_stacked, h_data_ljet_E, leg);  
    
  PlotStack(output_njet, "Number of jets", h_mc_njet_stacked, h_data_njet, leg);
  PlotStack(output_nbjet, "Number of b-tagged jets", h_mc_nbjet_stacked, h_data_nbjet, leg);
  PlotStack(output_met, "E_{T}^{miss} [GeV]", h_mc_met_stacked, h_data_met, leg);
  
  ///////////////////////////////////////////////////////////////////////////////////
  /////// To do: Rerun with PlotStackWithRatio() 
  ///////////////////////////////////////////////////////////////////////////////////

  analysis->cd();
  //For all objects you want to write to the analysis file, call Write(), e.gl
  //h_histogram1->Write();
  h_background->Write();
  h_data_invM4jetlv->SetTitle("data");
  h_data_invM4jetlv->SetName("data");
  h_data_invM4jetlv->Write();

  //End the programm properly by deleting all dynamic instances
  //file_histogram1->Close();
  //delete file_histogram1;
  delete leg;
  delete h_diboson_lep_pt;
  delete h_diboson_lep_eta;
  delete h_diboson_lep_phi;
  delete h_diboson_lep_E;
  delete h_diboson_jet_pt;
  delete h_diboson_jet_eta;
  delete h_diboson_jet_phi;
  delete h_diboson_jet_E;
  delete h_diboson_ljet_pt;
  delete h_diboson_ljet_eta;
  delete h_diboson_ljet_phi;
  delete h_diboson_ljet_E;
  delete h_diboson_njet;
  delete h_diboson_nbjet;
  delete h_diboson_met;
  delete h_diboson_invM3jet;
  delete h_diboson_invM4jetlv;
  
  delete h_singleTop_lep_pt;
  delete h_singleTop_lep_eta;
  delete h_singleTop_lep_phi;
  delete h_singleTop_lep_E;
  delete h_singleTop_jet_pt;
  delete h_singleTop_jet_eta;
  delete h_singleTop_jet_phi;
  delete h_singleTop_jet_E;
  delete h_singleTop_ljet_pt;
  delete h_singleTop_ljet_eta;
  delete h_singleTop_ljet_phi;
  delete h_singleTop_ljet_E;
  delete h_singleTop_njet;
  delete h_singleTop_nbjet;
  delete h_singleTop_met;
  delete h_singleTop_invM3jet;
  delete h_singleTop_invM4jetlv;
  
  delete h_ttbar_lep_pt;
  delete h_ttbar_lep_eta;
  delete h_ttbar_lep_phi;
  delete h_ttbar_lep_E;
  delete h_ttbar_jet_pt;
  delete h_ttbar_jet_eta;
  delete h_ttbar_jet_phi;
  delete h_ttbar_jet_E;
  delete h_ttbar_ljet_pt;
  delete h_ttbar_ljet_eta;
  delete h_ttbar_ljet_phi;
  delete h_ttbar_ljet_E;
  delete h_ttbar_njet;
  delete h_ttbar_nbjet;
  delete h_ttbar_met;
  delete h_ttbar_invM3jet;
  delete h_ttbar_invM4jetlv;
  
  delete h_Wjets_lep_pt;
  delete h_Wjets_lep_eta;
  delete h_Wjets_lep_phi;
  delete h_Wjets_lep_E;
  delete h_Wjets_jet_pt;
  delete h_Wjets_jet_eta;
  delete h_Wjets_jet_phi;
  delete h_Wjets_jet_E;
  delete h_Wjets_ljet_pt;
  delete h_Wjets_ljet_eta;
  delete h_Wjets_ljet_phi;
  delete h_Wjets_ljet_E;
  delete h_Wjets_njet;
  delete h_Wjets_nbjet;
  delete h_Wjets_met;
  delete h_Wjets_invM3jet;
  delete h_Wjets_invM4jetlv;
  
  delete h_Zjets_lep_pt;
  delete h_Zjets_lep_eta;
  delete h_Zjets_lep_phi;
  delete h_Zjets_lep_E;
  delete h_Zjets_jet_pt;
  delete h_Zjets_jet_eta;
  delete h_Zjets_jet_phi;
  delete h_Zjets_jet_E;
  delete h_Zjets_ljet_pt;
  delete h_Zjets_ljet_eta;
  delete h_Zjets_ljet_phi;
  delete h_Zjets_ljet_E;
  delete h_Zjets_njet;
  delete h_Zjets_nbjet;
  delete h_Zjets_met;
  delete h_Zjets_invM3jet;
  delete h_Zjets_invM4jetlv;
  
  delete h_zprime1000_lep_pt;
  delete h_zprime1000_lep_eta;
  delete h_zprime1000_lep_phi;
  delete h_zprime1000_lep_E;
  delete h_zprime1000_jet_pt;
  delete h_zprime1000_jet_eta;
  delete h_zprime1000_jet_phi;
  delete h_zprime1000_jet_E;
  delete h_zprime1000_ljet_pt;
  delete h_zprime1000_ljet_eta;
  delete h_zprime1000_ljet_phi;
  delete h_zprime1000_ljet_E;
  delete h_zprime1000_njet;
  delete h_zprime1000_nbjet;
  delete h_zprime1000_met;
  delete h_zprime1000_invM3jet;
  delete h_zprime1000_invM4jetlv;
  
  delete h_mc_invM4jetlv_stacked;
  delete h_mc_invM3jet_stacked;
  delete h_mc_lep_pt_stacked;
  delete h_mc_lep_phi_stacked;
  delete h_mc_lep_eta_stacked;
  delete h_mc_lep_E_stacked;
  delete h_mc_jet_pt_stacked;
  delete h_mc_jet_phi_stacked;
  delete h_mc_jet_eta_stacked;
  delete h_mc_jet_E_stacked;
  delete h_mc_ljet_pt_stacked;
  delete h_mc_ljet_phi_stacked;
  delete h_mc_ljet_eta_stacked;
  delete h_mc_ljet_E_stacked;
  delete h_mc_njet_stacked;
  delete h_mc_nbjet_stacked;
  delete h_mc_met_stacked;
    
  delete h_background;
    
  analysis->Close();



  return 0;
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
  gStyle->SetHistLineWidth(1);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
}


void PlotStack(TString filename, TString varUnit, THStack* mc, TH1F * data, TLegend * legend) {
  SetStyle();
	TCanvas * canv = new TCanvas("canv","Canvas for histogram",1);
  canv->SetLeftMargin(.12);
  canv->SetRightMargin(.1);
  data->Draw("E1");
  data->SetMinimum(0);
  mc->Draw("hist SAME");
  data->Draw("E1 SAME");
  mc->SetTitle(";"+varUnit+";Events");
  mc->GetYaxis()->SetTitleOffset(1);
  data->Draw("E1 SAME");
  legend->Draw();
  canv->Print(filename);
  delete canv;
}

TH1F *CombineStack(THStack *mc)
{
  // Get the list of histograms from the THStack
  TList *histos = mc->GetHists();
  if (!histos)
    return nullptr;

  // Create a base histogram from the first histogram in the stack
  TH1F *firstHist = (TH1F *)histos->At(0);
  TH1F *h_combined = (TH1F *)firstHist->Clone("h_combined");
  h_combined->Reset(); // Ensure it starts empty

  // Iterate over each histogram and add to the base histogram
  TIter next(histos);
  TH1F *h;
  while ((h = (TH1F *)next()))
  {
    h_combined->Add(h);
  }

  return h_combined;
}

void PlotStackWithRatio(TString filename, TString varUnit, THStack *mc, TH1F *data, TLegend *legend, bool log)
{
  cout << "Setting global style" << endl;
  //SetStyle();
  SetAtlasStyle();
  cout << "Creating canvas" << endl;
  TCanvas *canv = new TCanvas("canv", "Canvas for histogram", 800, 600);
  //canv->SetLeftMargin(.12);
  //canv->SetRightMargin(.1);

  // Pads
  cout << "Creating pads" << endl;
  TPad *pad1 = new TPad("pad1", "Pad for stacked histograms", 0.0, 0.31, 1.0, 1.00);
  TPad *pad2 = new TPad("pad2", "Pad for data/MC ratio", 0.0, 0.01, 1.0, 0.30);
  pad1->SetBottomMargin(0.001);
  pad1->SetBorderMode(0);
  pad2->SetBottomMargin(0.5);
  pad1->SetTicks(1,1);
  pad2->SetTicks(1,1);
  if(log){
      pad1->SetLogy(true);
  }
  pad1->Draw();
  pad2->Draw();
  
  //double max_data = data->GetMaximum();
  cout << "Start plotting" << endl;
  pad1->cd();
  //data->SetMinimum(0 - max_data * 0.001); // Setting minimum y-value
    
  mc->Draw("hist");
  data->Draw("PE1 SAME");
  mc->SetTitle(";" + varUnit + ";Events");
  mc->GetYaxis()->SetLabelSize(0.05);
  mc->GetYaxis()->SetLabelFont(42);
  mc->GetYaxis()->SetTitleFont(42);
  mc->GetYaxis()->SetTitleOffset(1.1);
  mc->GetXaxis()->SetNoExponent(kTRUE);
  legend->Draw();

  canv->cd();
  pad2->cd();
  cout << "Start plotting ratio" << endl;
  TH1F *mc_hist = CombineStack(mc);
  TH1F *mc_ratio = (TH1F *)mc_hist->Clone("mc_ratio");
  TH1F *data_ratio = (TH1F *)data->Clone("data_ratio");
  data_ratio->Divide(mc_ratio);
  
  data_ratio->GetXaxis()->SetLabelFont(42);
  data_ratio->GetXaxis()->SetLabelSize(0.12);
  data_ratio->GetXaxis()->SetLabelOffset(0.01); // Adjust this value if needed
  data_ratio->GetXaxis()->SetTitleFont(42);
  data_ratio->GetXaxis()->SetTitleSize(0.20);
  data_ratio->GetXaxis()->SetTitleOffset(0.9);  // Adjust this to move the title closer or farther from the axis
  data_ratio->GetXaxis()->SetNdivisions(505);  
  data_ratio->GetXaxis()->SetTitle(varUnit);
  
  data_ratio->GetYaxis()->SetRangeUser(0.7, 1.3);
  data_ratio->GetYaxis()->SetLabelFont(42);
  data_ratio->GetYaxis()->SetLabelSize(0.15);
  data_ratio->GetYaxis()->SetLabelOffset(0.03);
  data_ratio->GetYaxis()->SetTitleFont(42);
  data_ratio->GetYaxis()->SetTitleSize(0.15);
  data_ratio->GetYaxis()->SetTitleOffset(0.5);
  data_ratio->GetYaxis()->SetNdivisions(505);
  data_ratio->GetYaxis()->SetTitle("Data/MC");
  data_ratio->Draw("PE1 SAME");

  const int min = data_ratio->GetXaxis()->GetBinLowEdge(1);
  const int max = data_ratio->GetXaxis()->GetBinUpEdge(data_ratio->GetNbinsX());
  TLine *line = new TLine(min, 1, max, 1);
  line->SetLineColor(kRed);
  line->SetLineStyle(2);
  line->SetLineWidth(3);
  line->Draw("SAME");

  canv->Print(filename);

  delete pad1;
  delete pad2;
  delete canv;
  delete line;
}

// Code stolen from original ATLAS Style
void SetAtlasStyle ()
{
  static TStyle* atlasStyle = 0;
  std::cout << "\nApplying ATLAS style settings...\n" << std::endl ;
  if ( atlasStyle==0 ) atlasStyle = AtlasStyle();
  gROOT->SetStyle("ATLAS");
  gROOT->ForceStyle();
}

TStyle* AtlasStyle()
{
  TStyle *atlasStyle = new TStyle("ATLAS","Atlas style");

  // use plain black on white colors
  Int_t icol=0; // WHITE
  atlasStyle->SetFrameBorderMode(icol);
  atlasStyle->SetFrameFillColor(icol);
  atlasStyle->SetCanvasBorderMode(icol);
  atlasStyle->SetCanvasColor(icol);
  atlasStyle->SetPadBorderMode(icol);
  atlasStyle->SetPadColor(icol);
  atlasStyle->SetStatColor(icol);
  //atlasStyle->SetFillColor(icol); // don't use: white fill color for *all* objects

  // set the paper & margin sizes
  atlasStyle->SetPaperSize(20,26);

  // set margin sizes
  atlasStyle->SetPadTopMargin(0.05);
  atlasStyle->SetPadRightMargin(0.05);
  atlasStyle->SetPadBottomMargin(0.16);
  atlasStyle->SetPadLeftMargin(0.16);

  // set title offsets (for axis label)
  atlasStyle->SetTitleXOffset(1.4);
  atlasStyle->SetTitleYOffset(1.4);

  // use large fonts
  //Int_t font=72; // Helvetica italics
  //Int_t font=42; // Helvetica
  //Double_t tsize=0.05;
  // --- test
  Int_t font=43;
  Double_t tsize=21;
  // --- test end
  atlasStyle->SetTextFont(font);

  atlasStyle->SetTextSize(tsize);
  atlasStyle->SetLabelFont(font,"x");
  atlasStyle->SetTitleFont(font,"x");
  atlasStyle->SetLabelFont(font,"y");
  atlasStyle->SetTitleFont(font,"y");
  atlasStyle->SetLabelFont(font,"z");
  atlasStyle->SetTitleFont(font,"z");

  atlasStyle->SetLabelSize(tsize,"x");
  atlasStyle->SetTitleSize(tsize,"x");
  atlasStyle->SetLabelSize(tsize,"y");
  atlasStyle->SetTitleSize(tsize,"y");
  atlasStyle->SetLabelSize(tsize,"z");
  atlasStyle->SetTitleSize(tsize,"z");

  // use bold lines and markers
  atlasStyle->SetMarkerStyle(20);
  atlasStyle->SetMarkerSize(1.0);
  atlasStyle->SetHistLineWidth(2.);
  atlasStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // get rid of X error bars (as recommended in ATLAS figure guidelines)
  atlasStyle->SetErrorX(0.0001);
  // get rid of error bar caps
  atlasStyle->SetEndErrorSize(0.);

  // do not display any of the standard histogram decorations
  atlasStyle->SetOptTitle(0);
  //atlasStyle->SetOptStat(1111);
  atlasStyle->SetOptStat(0);
  //atlasStyle->SetOptFit(1111);
  atlasStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  atlasStyle->SetPadTickX(1);
  atlasStyle->SetPadTickY(1);

  return atlasStyle;

}

