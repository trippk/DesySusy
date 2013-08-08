#include "NtupleTools3.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>

using namespace std;

void readerSingleS(TString list, TString outname,bool useW=true){

	TObjArray* arr = list.Tokenize(" ");
	int size=arr->GetEntries();
	if(size%2!=0) {
        	cout<<"unbalance file/weight list: "<<list<<endl;
        	exit(0);
	}
	vector<TString> files;
	vector<double> weights;
	for(int i=0;i<size;i+=2){
        	files.push_back( arr->At(i)->GetName() );
        	weights.push_back( atof( arr->At(i+1)->GetName() ) );
	}

	TH1F* hMET   = new TH1F("hMET","MET",100,0,3000);
	TH1F* hMT    = new TH1F("hMT","MT",100,0,300);
	TH1F* hMT2Wpre  = new TH1F("hMT2Wpre","MT2W w/o MET,MT req.",100,0,450);
	TH1F* hMT2W  = new TH1F("hMT2W","MT2W",100,0,450);
	TH1F* hMETMeff  = new TH1F("hMETMeff","MET/(MET+HT)",100,0,1);
	TH1F* hHT    = new TH1F("hHT","HT40",100,0,5000);
	TH1F* hDphi  = new TH1F("hDphi","#Delta#phi",100,0,4);
	TH1F* hAllHT = new TH1F("hAllHT","before cuts HT40",100,0,5000);
	TH1F* hAllLepPt = new TH1F("hAllLepPt","singlw lep Pt - all",100,0,2000);
	TH1F* hLepPtJb = new TH1F("hLepPtJ","single lep Pt - jet req",100,0,2000);
	TH1F* hLepPtM = new TH1F("hLepPtM","single lep Pt - jet + MET",100,0,2000);
	TH1F* hLepPtMM = new TH1F("hLepPtMM","single lep Pt - jet + MET +Mt+MT2W",100,0,2000);

	TH1I* hNel = new TH1I("hNel","N good(30) el",10,0,10);
	TH1I* hNmu = new TH1I("hNmu","N good(30) mu",10,0,10);
	TH1I* hNlep = new TH1I("hNlep","N good(30) mu+el",10,0,10);
	TH1I* hNtj = new TH1I("hNtj","N good(40) jets",15,0,15);
	TH1I* hNbjet = new TH1I("hNbjet","N b jets",15,0,15);

	EasyChain* tree = new EasyChain("delphTree");
	
	for(unsigned i=0;i<files.size();i++){
       		tree->AddSmartW(files[i],weights[i]);
		cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
	}

	int Nevents=tree->GetEntries();
	cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

	// CutFlow variables
	const int CutNumb = 14;
	const char * CutList[CutNumb] = {"Presel","sngl. lep.","nJets >= 4",
                                         "bjets == 1/2","MET>500","MT>120","MT2W>250",
                                         "dphi12 > 0.5","   HT>750","   HT>1000","   HT>1500",
                                         "or Meff>0.2","   Meff>0.3","   Meff>0.4"};
	double CFCounter[CutNumb];
	int   iCFCounter[CutNumb];

	for (int i=0;i < CutNumb; i++){
	  CFCounter[i] = 0;
	 iCFCounter[i] = 0;
	}
	TH1D* CutFlow= new TH1D("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
	// label bins
        for(int cj = 0; cj < CutNumb; cj++)
              CutFlow->GetXaxis()->SetBinLabel(cj+1,CutList[cj]);
              
	//	double EvWeight = 1;

	for(int entry=0; entry < Nevents; entry+=1){
//		progress();

		progressT();
		double fw = tree->GetEntryW(entry); // the applied with AddSmartW for the current file/dir

		double EvWeight = 1;
		if(useW) EvWeight = tree->Get(EvWeight,"EventWeight");
		EvWeight *= fw * 300000;

//		double HT   = tree->Get(HT,"HT"); 
		double HT   = tree->Get(HT,"HT40"); 
		hAllHT->Fill(HT,EvWeight);

		// 0. CF presel
		 CFCounter[0]+= EvWeight;
		iCFCounter[0]++;

		std::vector<TLorentzVector> &Electrons = tree->Get(&Electrons,"Electrons");
		std::vector<TLorentzVector> &Muons = tree->Get(&Muons,"Muons");

		// 1. Lepton veto
		// etamin in ntupler 2.5, ptmin 10
		int    Nel_tight = 0;
		int    Nmu_tight = 0;
		int    Nel_loose = Electrons.size();
		int    Nmu_loose = Muons.size();
		// 
		double lepPt=0;
		for(unsigned i=0;i<Nmu_loose;++i) 
			if(fabs(Muons[i].Eta())<=2.4&&Muons[i].Pt()>30 ){
				Nmu_tight++;
				lepPt=Muons[0].Pt();
			}
		for(unsigned i=0;i<Nel_loose;++i) 
			if(fabs(Electrons[i].Eta())<=2.4&&Electrons[i].Pt()>30 ){
				Nel_tight++;
				lepPt=Electrons[0].Pt();
			}
		
		hNmu->Fill(Nmu_tight,EvWeight);
		hNel->Fill(Nel_tight,EvWeight);
		hNlep->Fill(Nel_tight+Nmu_tight,EvWeight);
		// exactly 1 hard lepton, no other loose
		if(Nel_tight+Nmu_tight != 1) continue;
		if(Nel_loose+Nmu_loose > 1) continue;
		 CFCounter[1]+= EvWeight;
		iCFCounter[1]++;
		hAllLepPt->Fill(lepPt,EvWeight);

		// 2. nJets >= 3
		vector<TLorentzVector> &Jets = tree->Get(&Jets,"Jets");
		int Njet_loose = tree->Get(Njet_loose,"Njet");
		int Njet_tight = 0;
		for(int i = 0;i  < Njet_loose; i++)
			if(Jets[i].Pt() > 40) Njet_tight++;

		hNtj->Fill(Njet_tight,EvWeight);
		if(Njet_tight < 4) continue;
		 CFCounter[2]+= EvWeight;
		iCFCounter[2]++;

		// 3. Btag cut
		int Nbjet = 0;
		vector<int> &JetB = tree->Get(&JetB,"JetB");		
		for(unsigned i=0;i<Jets.size();i++) 
			if(JetB[i]>0&&Jets[i].Pt() > 40) Nbjet++;
		hNbjet->Fill(Nbjet,EvWeight);
		if(Nbjet<1||Nbjet>2) continue;		
		 CFCounter[3]+= EvWeight;
		iCFCounter[3]++;

		hLepPtJb->Fill(lepPt,EvWeight);

		double MET  = tree->Get(MET,"MET");
		double MT2W  = tree->Get(MT2W,"MT2W");
		if(MT2W>0)hMT2Wpre->Fill(MT2W,EvWeight);

		//  MET/Meff
		double Meff = MET + HT;
		if(MET>0)hMETMeff->Fill(MET/Meff,EvWeight);

		// 4. MET cut
		hMET->Fill(MET,EvWeight);
		if(MET<500) continue;
		 CFCounter[4]+= EvWeight;
		iCFCounter[4]++;

		hLepPtM->Fill(lepPt,EvWeight);

		// 5. MT cut
		double MT  = tree->Get(MT,"MT");
		hMT->Fill(MT,EvWeight);
		if(MT<120) continue;
		 CFCounter[5]+= EvWeight;
		iCFCounter[5]++;

		// 5. MT2W cut
		if(MT2W>0)hMT2W->Fill(MT2W,EvWeight);
		if(MT2W<250) continue;
		 CFCounter[6]+= EvWeight;
		iCFCounter[6]++;

		hLepPtMM->Fill(lepPt,EvWeight);

		// 6. dPhi
		vector<double> &JetMETdPhi = tree->Get(&JetMETdPhi,"JetMETdPhi");		
		double dPhi = TMath::Min(JetMETdPhi[0],JetMETdPhi[1]);
		hDphi->Fill(dPhi,EvWeight);
		if(dPhi > 0.8) continue;
		 CFCounter[7]+= EvWeight;
		iCFCounter[7]++;


		// 8. HT cut
		hHT->Fill(HT,EvWeight);
//		if(HT < 500) continue;
		if(HT > 750) {
		 CFCounter[8]+= EvWeight;
		iCFCounter[8]++;
		}
		if(HT > 1000) {
		 CFCounter[9]+= EvWeight;
		iCFCounter[9]++;
		}
		if(HT > 1500) {
		 CFCounter[10]+= EvWeight;
		iCFCounter[10]++;
		}
//		if(MET/Meff < 0.2) continue;
		if(MET/Meff > 0.2) {
		 CFCounter[11]+= EvWeight;
		iCFCounter[11]++;
		}
		if(MET/Meff > 0.3) {
		 CFCounter[12]+= EvWeight;
		iCFCounter[12]++;
		}
		if(MET/Meff > 0.4) {
		 CFCounter[13]+= EvWeight;
		iCFCounter[13]++;
		}

	}
	// ^loop end^



	ofstream tfile;
	tfile.open("SingleS_"+outname+".txt");
	tfile << "########################################" << endl;
	tfile << "Cut efficiency numbers:" << endl;
	for(int ci = 0; ci < CutNumb; ci++)
	{
		tfile << "After cut " << CutList[ci] <<  "\t\t\t"
		      << CFCounter[ci]  << "\t events left\t"<< iCFCounter[ci] <<" cnt\t"<< endl;
		CutFlow->SetBinContent(1+ci,CFCounter[ci]);
	}

	TFile * outf = new TFile("SingleS_"+outname+"_his.root","RECREATE");

	hDphi->Write();
	hHT->Write();
	hAllHT->Write();
	hMETMeff->Write();
	hMET->Write();
	hMT->Write();
	hMT2W->Write();
	hNel->Write();
	hNmu->Write();
	hNtj->Write();
	hNel->Write();
	hNmu->Write();
	hNlep->Write();
	hNbjet->Write();

	hAllLepPt->Write();
	hLepPtJb->Write();
	hLepPtM->Write();
	hLepPtMM->Write();
	hMT2Wpre->Write();
}
