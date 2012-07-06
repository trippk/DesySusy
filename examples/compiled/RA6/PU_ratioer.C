/*
usage:

   root -l PU_ratioer.C+\(\"MCfileWithPUhistogram.root\",\"DataFileWithPUhistogram.root\"\)

*/

#include <iostream>
#include "TROOT.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLatex.h"

void PU_ratioer(TString mf="whole2011_RA6/SusyCAF_Tree_n11_3363707705_out.root", TString df="PU_data_160404-180252.root") {

  TFile* dataF = new TFile(df);
  TFile* mcF   = new TFile(mf);

  if(!dataF->IsOpen()) {
    cout<<"data file can not be opened."<<endl;
    return;
  }
  if(!mcF->IsOpen()) {
    cout<<"mc file can not be opened."<<endl;
    return;
  }

  TH1D* dataH = (TH1D*) dataF->Get("pileup");
  TH1D* mcH   = (TH1D*) mcF  ->Get("control_plots/pu0_unweighted");


  dataH->Scale(1/dataH->Integral());
  mcH  ->Scale(1/mcH  ->Integral());
  dataH->SetLineColor(kBlack);
  dataH->SetLineWidth(2);
  mcH->SetLineColor(kRed);
  mcH->SetLineWidth(2);

  TH1D* ratio = (TH1D*) dataH->Clone("ratio");

  ratio->Divide(mcH);

  TLegend *leg = new TLegend(0.6,0.3,0.9,0.6);
  leg->AddEntry(dataH,"data","l");
  leg->AddEntry(mcH, "MC","l");

  TCanvas* can = new TCanvas("can","can",600,900);
  can->Divide(1,2);
  can->cd(1);
  dataH->Draw("HIST");
  mcH->Draw("sames HIST");
  leg->Draw();
  can->cd(2);
  gPad->SetBottomMargin(.4);
  ratio->SetTitle("ratio: data/MC");
  ratio->Draw("HIST");
  float maxax=ratio->GetMaximum();

  TLatex *tex= new TLatex();
  tex->SetTextSize(.035);
  tex->SetTextFont(42);
  tex->DrawLatex(-5,-maxax/3,"Input:");
  tex->DrawLatex(-5,-maxax/2,"Data: "+df);
  tex->DrawLatex(-5,-maxax/1.7,"MC: "+mf);

  cout<<"PU_weightList = ";
  for(unsigned bin = 1; bin<ratio->GetXaxis()->GetXmax()+1; ++bin)
    cout<<ratio->GetBinContent(bin)<<" ";
  cout<<endl;
}

