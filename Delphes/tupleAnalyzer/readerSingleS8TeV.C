// root -q -b readerSingleS8Tev.C+

// el_pt>30 |el_eta|< 1.4442 change for 14TeV
// mu_pt>25 |mu_eta|<2.1
// lepton_veto_pt 5GeV
// dphi>0.8
// mt>120
// 
// stop->top+neut_0 MET>150,200,250,300
//      small delta_m: chi2<5
//      large delta_m: chi2<5, MT2W>200
//
// stop->b+char_1 MET>150,200,250,300
//      small delta_m: 
//      large delta_m: MT2W>200  leading b-jet pt>100GeV

// here MT2W>200 b-jet pt>100GeV MET>150,250

#include "NtupleTools3.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>


using namespace std;

void readerSingleS8TeV(TString list, TString outname,bool useW=true){

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

	//at 1lep 4jets 1b
	TH1F* aDphi  = new TH1F("aDphi","#Delta#phi",100,0,4);
	TH1F* aMET   = new TH1F("aMET","MET",100,0,3000);
	TH1F* aMT    = new TH1F("aMT","MT",100,0,500);
	TH1F* aMT2W2  = new TH1F("aMT2W2","MT2W 1,2b",100,0,500);
	TH1F* aMT2W  = new TH1F("aMT2W","MT2W 1,2b only",100,0,500);
	TH1F* aHT    = new TH1F("aHT","HT40",100,0,4000);
	TH1F* aLepPt = new TH1F("aLepPt","single lep Pt",100,0,1000);
	TH1F* a1JetPt = new TH1F("a1JetPt","highest jet Pt",100,0,2000);

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
	const int CutNumb = 12;
	const char * CutList[CutNumb] = {"Presel       ",
					 "nJets >= 3   ",
					 "1 or more l  ",
					 "nJets >= 4   ",
					 "bjets == 1/2 ",
					 "sngl. lep.   ",
					 "loose veto   ",
					 "MET>150      ",
					 "MT>120       ",
					 "MT2W>200     ",
                                         "dphi12 > 0.8 ",
                                         "tau veto     "};
//	const char * CutList[CutNumb] = {"Presel","sngl. lep.","loose veto","tau veto","nJets >= 3","nJets >= 4",
//                                         "bjets == 1/2","MET>500","MT>120","MT2W>250",
//                                         "dphi12 > 0.5","   HT>750","   HT>1000","   HT>1500",
//                                         "or Meff>0.2","   Meff>0.3","   Meff>0.4"};
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

		double HT   = tree->Get(HT,"HT"); // 30 GeV
//		double HT   = tree->Get(HT,"HT40"); 
		hAllHT->Fill(HT,EvWeight);

		// 0. CF presel
		 CFCounter[0]+= EvWeight;
		iCFCounter[0]++;

		std::vector<TLorentzVector> &Electrons = tree->Get(&Electrons,"Electrons");
		std::vector<TLorentzVector> &Muons = tree->Get(&Muons,"Muons");

		// 1. Lepton veto
		// etamin in ntupler 4.0, ptmin 5
		int    Nel_tight = 0;
		int    Nmu_tight = 0;
		int    Nel_loose = 0;
		int    Nmu_loose = 0;
		// 
		double lepPt=0;
		for(unsigned i=0;i<Muons.size();++i) 
			if(fabs(Muons[i].Eta())<=2.1&&Muons[i].Pt()>25 ){
				Nmu_tight++;
				lepPt=Muons[0].Pt();
			}
			else if(fabs(Muons[i].Eta())<=2.4&&Muons[i].Pt()>5 ){
				Nmu_loose++;
			}
		for(unsigned i=0;i<Electrons.size();++i) 
			if(fabs(Electrons[i].Eta())<=1.4442&&Electrons[i].Pt()>30 ){
				Nel_loose++;
				lepPt=Electrons[0].Pt();
			}
			else if(fabs(Electrons[i].Eta())<=2.4&&Electrons[i].Pt()>5 ){
				Nel_loose++; 			
			}

		hNmu->Fill(Nmu_tight,EvWeight);
		hNel->Fill(Nel_tight,EvWeight);
		hNlep->Fill(Nel_tight+Nmu_tight,EvWeight);
//-- el
		// at least 1 hard lepton
		if(Nel_tight+Nmu_tight < 1) continue;
		 CFCounter[1]+= EvWeight;
		iCFCounter[1]++;
		
		vector<TLorentzVector> &Jets = tree->Get(&Jets,"Jets");
		int Njet_loose = tree->Get(Njet_loose,"Njet");
		int Njet_tight = 0;
		int Ntaujet = 0;
		vector<int> &JetTau = tree->Get(&JetTau,"JetTau");		
		for(int i = 0;i  < Njet_loose; i++){
			if(Jets[i].Pt() > 30&&fabs(Jets[i].Eta())<2.4) Njet_tight++;
			if(Jets[i].Pt() > 20&&fabs(Jets[i].Eta())<2.4 && JetTau[i]>0) Ntaujet++;
		}


		hAllLepPt->Fill(lepPt,EvWeight);

		// 2. nJets >= 3
		hNtj->Fill(Njet_tight,EvWeight);
		if(Njet_tight < 3) continue;
		 CFCounter[2]+= EvWeight;
		iCFCounter[2]++;

		// 2. nJets >= 4
		if(Njet_tight < 4) continue;
		 CFCounter[3]+= EvWeight;
		iCFCounter[3]++;

		// 3. Btag cut
		int Nbjet = 0;
		vector<int> &JetB = tree->Get(&JetB,"JetB");		
		for(unsigned i=0;i<Jets.size();i++) 
			if(JetB[i]>0&&Jets[i].Pt() > 40) Nbjet++;
		hNbjet->Fill(Nbjet,EvWeight);
		if(Nbjet<1||Nbjet>2) continue;		
		 CFCounter[4]+= EvWeight;
		iCFCounter[4]++;

		// exactly 1 hard lepton, no other loose
		if(Nel_tight+Nmu_tight != 1) continue;
		 CFCounter[5]+= EvWeight;
		iCFCounter[5]++;
		// loose el-mu veto
		if(Nel_loose+Nmu_loose  > 0) continue;
		 CFCounter[6]+= EvWeight;
		iCFCounter[6]++;

		// distributions at 1lep4jet1b
		vector<double> &JetMETdPhi = tree->Get(&JetMETdPhi,"JetMETdPhi");		
		double dPhi = TMath::Min(JetMETdPhi[0],JetMETdPhi[1]);
		aDphi->Fill(dPhi,EvWeight);
		double MET  = tree->Get(MET,"MET");
		aMET->Fill(MET,EvWeight);
		double MT2W  = tree->Get(MT2W,"MT2W");
		aMT2W2->Fill(MT2W,EvWeight);
		if(MT2W>0&&MT2W<499)aMT2W->Fill(MT2W,EvWeight);
		double MT  = tree->Get(MT,"MT");
		aMT->Fill(MT,EvWeight);
		aHT->Fill(HT,EvWeight);
		aLepPt->Fill(lepPt,EvWeight);
		if(Jets[0].Pt()>40.) a1JetPt->Fill(Jets[0].Pt(),EvWeight);

		hLepPtJb->Fill(lepPt,EvWeight);

//		double MET  = tree->Get(MET,"MET");
//		double MT2W  = tree->Get(MT2W,"MT2W");
		if(MT2W>0)hMT2Wpre->Fill(MT2W,EvWeight);

		//  MET/Meff
		double Meff = MET + HT;
		if(MET>0)hMETMeff->Fill(MET/Meff,EvWeight);

		// 4. MET cut
		hMET->Fill(MET,EvWeight);
		if(MET<150) continue;
		 CFCounter[7]+= EvWeight;
		iCFCounter[7]++;

		hLepPtM->Fill(lepPt,EvWeight);

		// 5. MT cut
//		double MT  = tree->Get(MT,"MT");
		hMT->Fill(MT,EvWeight);
		if(MT<120) continue;
		 CFCounter[8]+= EvWeight;
		iCFCounter[8]++;

		// 5. MT2W cut
		if(MT2W>0)hMT2W->Fill(MT2W,EvWeight);
		if(MT2W<250) continue;
		 CFCounter[9]+= EvWeight;
		iCFCounter[9]++;

		hLepPtMM->Fill(lepPt,EvWeight);

		// 6. dPhi
//		vector<double> &JetMETdPhi = tree->Get(&JetMETdPhi,"JetMETdPhi");		
//		double dPhi = TMath::Min(JetMETdPhi[0],JetMETdPhi[1]);
		hDphi->Fill(dPhi,EvWeight);
		if(dPhi < 0.8) continue;
		 CFCounter[10]+= EvWeight;
		iCFCounter[10]++;

		// tau veto
		if(Ntaujet > 0) continue;
		 CFCounter[11]+= EvWeight;
		iCFCounter[11]++;
		
/*
		// 8. HT cut
		hHT->Fill(HT,EvWeight);
//		if(HT < 500) continue;
		if(HT > 750) {
		 CFCounter[11]+= EvWeight;
		iCFCounter[11]++;
		}
		if(HT > 1000) {
		 CFCounter[12]+= EvWeight;
		iCFCounter[12]++;
		}
		if(HT > 1500) {
		 CFCounter[13]+= EvWeight;
		iCFCounter[13]++;
		}
//		if(MET/Meff < 0.2) continue;
		if(MET/Meff > 0.2) {
		 CFCounter[14]+= EvWeight;
		iCFCounter[14]++;
		}
		if(MET/Meff > 0.3) {
		 CFCounter[15]+= EvWeight;
		iCFCounter[15]++;
		}
		if(MET/Meff > 0.4) {
		 CFCounter[16]+= EvWeight;
		iCFCounter[16]++;
		}
*/
	}
	// ^loop end^



	ofstream tfile;
	tfile.open("SingleS8TeV_"+outname+".txt");
	tfile << "########################################" << endl;
	tfile << "Cut efficiency numbers:" << endl;
	double last=0;
	double first=0;
	for(int ci = 0; ci < CutNumb; ci++)
	{
		double d=0;
		double e=0;
		if(iCFCounter[ci]>0){
			d=CFCounter[ci];
			e=d/sqrt(iCFCounter[ci]);
			if(last==0) {
				last=CFCounter[ci];
				first=CFCounter[ci];
			}
		}
		tfile << CutList[ci] <<  "\t"
		      << setw(12) << d  << " +- " << setw(12)<<e <<"\t to prev.\t "
		      << setw(12) << CFCounter[ci]/last<<"\t relative  " 
		      << setw(12) << CFCounter[ci]/first<<endl;
		CutFlow->SetBinContent(1+ci,CFCounter[ci]);
		last=CFCounter[ci];
	}

	TFile * outf = new TFile("SingleS8TeV_"+outname+"_his.root","RECREATE");

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

	aDphi->Write();
	aMET->Write();
	aMT->Write();
	aMT2W->Write();
	aMT2W2->Write();
	aHT->Write();
	aLepPt->Write();
	a1JetPt->Write();

}
