#include "NtupleTools2.h"
#include<iostream>
#include<vector>
#include<TLorentzVector.h>
#include<TCanvas.h>
#include<TH1F.h>
#include<TFile.h>
using namespace std;

void reader(const TString& dir=""){

	TCanvas* c1 = new TCanvas("c1","",800,600);
	c1->Divide(3,2);

	TH1F* hMET  = new TH1F("hMET","MET",50,0,2000);
	TH1F* hMT   = new TH1F("hMT","MT",50,0,300);
	TH1F* hHT   = new TH1F("hHT","HT",50,0,3000);
	TH1F* hMT0  = new TH1F("hMT0","MT0",50,0,300);
	TH1F* hMT2W = new TH1F("hMT2W","MT2W",50,0,450);

	EasyChain* tree = new EasyChain("delphTree");
//	tree->AddSmart(dir);
	tree->AddSmartW("Output/ttbar/0-600/",    513./1806906); // there must be just one root file
	tree->AddSmartW("Output/ttbar/600-1100/",  42.5/11270097);
	tree->AddSmartW("Output/ttbar/1100-1700/",  4.48/10777402);
	tree->AddSmartW("Output/ttbar/1700-2500/",  0.528/10618078);
	tree->AddSmartW("Output/ttbar/2500-100000/",0.0545/10853534);
	
	int N=tree->GetEntries();
	cout<<"total number of events: "<<N<<endl;

	int ol=0;
	for(int i=0;i<N;i++){
//	for(int i=0;i<N;i+=1000){
//		progress();
		progressT();
		double fw = tree->GetEntryW(i); // the applied with AddSmartW for the current file/dir
		int    Nel = tree->Get(   Nel,   "Nel");
		int    Nmu = tree->Get(   Nmu,   "Nmu");
		if(Nel+Nmu!=1) continue;
		int looNel = tree->Get(looNel,"looNel");
		if(looNel>0) continue;
		int looNmu = tree->Get(looNmu,"looNmu");
		if(looNmu>0) continue;
		
		int Nbjet = tree->Get(Nbjet,"Nbjet");
		if(Nbjet==0||Nbjet>2) continue;
		int Njet = tree->Get(Njet,"Njet");
		if(Njet<4) continue;
		ol++;
		double MET  = tree->Get(MET,"MET"); 
		double MT   = tree->Get(MT,"MT"); 
		double HT   = tree->Get(HT,"HT"); 
		double MT0  = tree->Get(MT0,"MT0"); 
		double MT2W = tree->Get(MT2W,"MT2W"); 
		double w  = fw*tree->Get(w,"EventWeight"); 
		hHT->Fill(HT,w);
		hMET->Fill(MET,w);
		hMT->Fill(MT,w);
		hMT0->Fill(MT0,w);
		hMT2W->Fill(MT2W,w);
		
	}
	cout<<ol<<"/"<<N<<endl;
	c1->cd(1);
	hMT0->SetLineColor(kBlue);
	hMT0->Draw();
	hMT->Draw("same");
	c1->cd(2);
	hMT2W->Draw();
	c1->cd(3);
	hMET->Draw();
	c1->cd(4);
	hHT->Draw();
	c1->SaveAs("histos.pdf");
	c1->SaveAs("histos.C");
}
