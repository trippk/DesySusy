#include <iostream>
#include "TROOT.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"

void PU_ratioer(TString mf="whole2011_RA6/SusyCAF_Tree_n11_3363707705_out.root", TString df="PU_data_160404-180252.root") {

  TFile* dataF = new TFile(df);
  TFile* mcF   = new TFile(mf);

  TH1D* dataH = (TH1D*) dataF->Get("pileup");
  TH1D* mcH   = (TH1D*) mcF  ->Get("control_plots/pu0_unweighted");

  dataH->Scale(1/dataH->Integral());
  mcH  ->Scale(1/mcH  ->Integral());

  cout<<dataH->Integral()<<"  "<<mcH  ->Integral()<<endl;

  TH1D* ratio = (TH1D*) dataH->Clone("ratio");

  ratio->Divide(mcH);

  TCanvas* can = new TCanvas("can","can",600,800);
  can->Divide(1,2);
  can->cd(1);
  dataH->Draw();
  mcH->Draw("sames");
  can->cd(2);
  ratio->Draw("HIST text");


  cout<<"PU_weightList = ";
  for(unsigned bin = 1; bin<ratio->GetXaxis()->GetXmax()+1; ++bin) {
    cout<<ratio->GetBinContent(bin)<<" ";

  }
  cout<<endl;
}

