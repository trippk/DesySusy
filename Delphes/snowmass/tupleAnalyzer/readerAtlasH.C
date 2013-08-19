
// used to be readerAtlasH2min.C

#include "NtupleTools3.h"
#include<fstream>
#include<iostream>
#include<vector>
#include<TLorentzVector.h>
#include<TCanvas.h>
#include<TH1F.h>
#include<TFile.h>

using namespace std;

void readerAtlasH(TString list, TString outname,bool useW=true){

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

	TH1F* hMET   = new TH1F("hMET","MET",50,0,2000);
	TH1F* hMETMeff  = new TH1F("hMETMeff","MET/(MET+HT)",50,0,1);
	TH1F* hMeff  = new TH1F("hMeff","MET+HT",50,0,5000);
	TH1F* hHT    = new TH1F("hHT","HT",50,0,3000);
	TH1F* hHT2    = new TH1F("hHT2","HT",50,0,5000);
	TH1F* hDphi  = new TH1F("hDphi","#Delta#phi",100,0,4);
	TH1F* hAllHT = new TH1F("hAllHT","all HT",50,0,3000);

	TH1I* hNel = new TH1I("hNel","N good el",10,0,10);
	TH1I* hNmu = new TH1I("hNmu","N good mu",10,0,10);
	TH1I* hNtj = new TH1I("hNtj","N tight jets",15,0,15);
	TH1I* hNbjet = new TH1I("hNbjet","N b jets",15,0,15);

	TH1F* hJ0Pt =  new TH1F("hJ0Pt","jet1 pt",50,0,500);
	TH1F* hJ1Pt =  new TH1F("hJ1Pt","jet2 pt",50,0,500);
	TH1F* hJ2Pt =  new TH1F("hJ2Pt","jet2 pt",50,0,500);

	TH1F* hiso =  new TH1F("hiso","lep iso",50,0,1);

	EasyChain* tree = new EasyChain("delphTree");
	
	for(unsigned i=0;i<files.size();i++){
       		tree->AddSmartW(files[i],weights[i]);
		cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
	}

	int Nevents=tree->GetEntries();
	cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

	// CutFlow variables
	const int CutNumb = 12;
	const char * CutList[CutNumb] = {"Presel","lep. veto","nJets >= 3",
                                         "jet1 > 120","jet2 > 70","jet3 > 60","bjets >= 2",
					 "HT > 1000","dphi12 > 0.5","Meff > 0.2","MET > 250","MET > 500"};
//					 "HT > 1000","dphi12 > 2.4","Meff > 0.2","MET > 250","MET > 500"};
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
		EvWeight *= fw * 1000;

//		double HT   = tree->Get(HT,"HT"); 
		double HT   = tree->Get(HT,"HT40"); 
		hAllHT->Fill(HT,EvWeight);

		// 0. CF presel
		 CFCounter[0]+= EvWeight;
		iCFCounter[0]++;

std::vector<TLorentzVector> &Electrons = tree->Get(&Electrons,"Electrons");
std::vector<int> &ElCh = tree->Get(&ElCh,"ElectronCh");
std::vector<TLorentzVector> &Muons = tree->Get(&Muons,"Muons");
std::vector<int> &MuCh = tree->Get(&MuCh,"MuonCh");
std::vector<double> &ElIso = tree->Get(&ElIso,"ElectronIso");
std::vector<double> &MuIso = tree->Get(&MuIso,"MuonIso");

for(int i=0;i<ElIso.size();++i)hiso->Fill(ElCh[i]);
for(int i=0;i<MuIso.size();++i)hiso->Fill(MuIso[i]);

		// 1. Lepton veto
		// etamin in ntupler 2.5, ptmin 10
		int    Nel = tree->Get(   Nel,   "Nel");
		int    Nmu = 0;//tree->Get(   Nmu,   "Nmu"); 
		for(unsigned i=0;i<Muons.size();++i) if(fabs(Muons[i].Eta())<=2.4 )Nmu++;
		hNmu->Fill(Nmu,EvWeight);
		hNel->Fill(Nel,EvWeight);
		if(Nel+Nmu > 0) continue;
		 CFCounter[1]+= EvWeight;
		iCFCounter[1]++;

		// 2. nJets >= 3
		vector<TLorentzVector> &Jets = tree->Get(&Jets,"Jets");
		int Njet = tree->Get(Njet,"Njet");
		int tightNjet = 0;

		for(int jid = 0; jid < Njet; jid++)
		  if(Jets[jid].Pt() > 40) tightNjet++;

		hNtj->Fill(tightNjet,EvWeight);
		if(tightNjet < 3) continue;
		 CFCounter[2]+= EvWeight;
		iCFCounter[2]++;

		// 3. Lead jet pt cut

		hJ0Pt->Fill(Jets[0].Pt(),EvWeight);
		if(Jets[0].Pt() < 120) continue;		
		 CFCounter[3]+= EvWeight;
		iCFCounter[3]++;

		// 4. Second Lead jet pt cut
		hJ1Pt->Fill(Jets[1].Pt(),EvWeight);
		if(Jets[1].Pt() < 70) continue;		
		 CFCounter[4]+= EvWeight;
		iCFCounter[4]++;

		// 5. THird Lead jet pt cut
		hJ2Pt->Fill(Jets[2].Pt(),EvWeight);
		if(Jets[2].Pt() < 60) continue;		
		 CFCounter[5]+= EvWeight;
		iCFCounter[5]++;

		// 6. Btag cut
		int Nbjet = 0;//tree->Get(Nbjet,"Nbjet");  // ntuple bjets>30
		vector<int> &JetB = tree->Get(&JetB,"JetB");		
		for(unsigned i=0;i<Jets.size();i++) 
			if(JetB[i]>0&&Jets[i].Pt()>40) 
				Nbjet++;
		hNbjet->Fill(Nbjet,EvWeight);
		if(Nbjet < 2) continue;		
		 CFCounter[6]+= EvWeight;
		iCFCounter[6]++;

		// 8. HT cut
		hHT->Fill(HT,EvWeight);
		hHT2->Fill(HT,EvWeight);
		if(HT < 1000) continue;
		 CFCounter[7]+= EvWeight;
		iCFCounter[7]++;

		// 7. dPhi
		vector<double> &JetMETdPhi = tree->Get(&JetMETdPhi,"JetMETdPhi");		
		double dPhi = TMath::Min(JetMETdPhi[0],JetMETdPhi[1]);
		hDphi->Fill(dPhi,EvWeight);
		if(dPhi < 0.5) continue; // was >
//		if(dPhi < 2.4) continue; // was >
		 CFCounter[8]+= EvWeight;
		iCFCounter[8]++;

		// 9. MET/Meff
		double MET  = tree->Get(MET,"MET"); 
		double Meff = MET + HT;

		hMeff->Fill(Meff,EvWeight);
		hMETMeff->Fill(MET/Meff,EvWeight);
		if(MET/Meff < 0.2) continue;
		 CFCounter[9]+= EvWeight;
		iCFCounter[9]++;

		hMET->Fill(MET,EvWeight);
		// 10. MET
		if(MET < 500) continue;
		 CFCounter[10]+= EvWeight;
		iCFCounter[10]++;

		if(MET < 750) continue;
		 CFCounter[11]+= EvWeight;
		iCFCounter[11]++;


		
	}
	// ^loop end^



	ofstream tfile;
	tfile.open("AtlasH_"+outname+".txt");
	tfile << "########################################" << endl;
	tfile << "Cut efficiency numbers:" << endl;
	for(int ci = 0; ci < CutNumb; ci++)
	{
		tfile << "After cut " << CutList[ci] <<  "\t\t\t"
		      << CFCounter[ci]  << "\t events left\t"<< iCFCounter[ci] <<" cnt\t"<< endl;
		CutFlow->SetBinContent(1+ci,CFCounter[ci]);
	}

	TFile * outf = new TFile("AtlasH_"+outname+"_his.root","RECREATE");
	hDphi->Write();
	hHT->Write();
	hHT2->Write();
	hAllHT->Write();
	hMeff->Write();
	hMETMeff->Write();
	hMET->Write();
	hNel->Write();
	hNmu->Write();
	hNtj->Write();
	hJ0Pt->Write();
	hJ1Pt->Write();
	hJ2Pt->Write();
	hNbjet->Write();

hiso->Write();
}
