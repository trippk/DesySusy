// used to be readerEWKinoNew.C
#include "NtupleTools3.h"
#include<fstream>
#include<iostream>
#include<vector>
#include<TLorentzVector.h>
#include<TCanvas.h>
#include<TH1F.h>
#include<TFile.h>
using namespace std;

void readerEWKino(TString list, TString outname,bool useW=true){

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

  TH1I* hNjet       = new TH1I("hNjet","n jets",20,0,20);
  TH1I* hNtightJet  = new TH1I("hNtightJet","n tight jets",20,0,20);
  TH1I* hNBjet      = new TH1I("hNbjet","n b jets",20,0,20);
  TH1I* hNtightBjet = new TH1I("hNtightBjet","n tight b jets",20,0,20);

  TH1F* hMET   = new TH1F("hMET","MET",100,0,2000);
  TH1F* hMT    = new TH1F("hMT","MT",100,0,1000);
  TH1F* hHT    = new TH1F("hHT","all HT",100,0,5000);
  TH1F* hHTb   = new TH1F("hHTb","after btag HT",100,0,5000);
  TH1F* hMETb   = new TH1F("hMETb","after btag MET",100,0,2000);
  TH1F* hMTb    = new TH1F("hMTb","after btag MT",100,0,1000);

  TH1F * hLep1Pt = new TH1F("Lep1Pt","Pt of leading lepton; p_{T} [GeV]",100,0,1000);
  TH1F * hLep1PtSS = new TH1F("Lep1PtSS","Pt of leading SS lepton; p_{T} [GeV]",100,0,1000);
  TH1F * hLepAllPt = new TH1F("LepAllPt","Pt of all used leptons; p_{T} [GeV]",100,0,2000);

  TH1F * hLep1Ptsr1 = new TH1F("Lep1Pt_SR1","Pt of leading lepton in SR1; p_{T} [GeV]",100,0,1000);
  TH1F * hLep1Ptsr3 = new TH1F("Lep1Pt_SR3","Pt of leading lepton in SR3; p_{T} [GeV]",100,0,1000);
  TH1F * hLep1Ptsr1t = new TH1F("Lep1Pt_SR1tight","Pt of leading lepton in SR1 tight; p_{T} [GeV]",100,0,1000);

  TH1F * hDiLMass = new TH1F("DiLMass","OS DiLepton mass",100,0,200);
  TH1F * hDiLMass2 = new TH1F("DiLMass2","OS DiLepton mass",100,0,400);
  TH1F * hDiLMassSS = new TH1F("DiLMassSS","SS DiLepton mass",100,0,200);
  TH1F * hDiLMassSS2 = new TH1F("DiLMassSS2","SS DiLepton mass",100,0,400);

  TH1I * hNel = new TH1I("hNel","Number of electrons",6,0,6);
  TH1I * hNmu = new TH1I("hNmu","Number of muons",6,0,10);
  TH1I * hNlep = new TH1I("hNlep","Number of leptons",10,0,10);

  TH1F* hJet1Pt = new TH1F("hJet1Pt","jet 1 pt",50,0,2000);
  TH1F* hJet2Pt = new TH1F("hJet2Pt","jet 2 pt",50,0,2000);
  TH1F* hJet3Pt = new TH1F("hJet3Pt","jet 3 pt",50,0,2000);

  TH1F* hJet1Ptb = new TH1F("hJet1Ptb","after btag jet 1 pt",50,0,2000);
  TH1F* hJet2Ptb = new TH1F("hJet2Ptb","after btag jet 2 pt",50,0,2000);
  TH1F* hJet3Ptb = new TH1F("hJet3Ptb","after btag jet 3 pt",50,0,2000);

  EasyChain* tree = new EasyChain("delphTree");
 
  for(unsigned i=0;i<files.size();i++){
  	tree->AddSmartW(files[i],weights[i]);
  	cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
  }
  int Nevents=tree->GetEntries();
  cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;


    const int CutNumb = 15;
    const char * CutList[CutNumb] = {"Presel","2lepton req","MET > 120","SS req","Z veto",
    "-> SR1","-> SR0(120)","-> SR3(200)","-> SR3b","SR3HT<200",
    "b(20)veto","jets(40)<=4","jets(40)<=3","jets(40)>0","jets(100)>0"};

    TH1D* CutFlow= new TH1D("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
    // label bins
    for(int cj = 0; cj < CutNumb; cj++)
      CutFlow->GetXaxis()->SetBinLabel(cj+1,CutList[cj]);

  // CutFlow variables
  double CutFlowCnt[CutNumb];
  int   iCutFlowCnt[CutNumb];

  for (int i=0;i < CutNumb; i++){
     CutFlowCnt[i] = 0;
    iCutFlowCnt[i] = 0;
  }
  //	double EvWeight = 1;

  for(int entry=0; entry < Nevents; entry++){
    //	for(int i=0;i<N;i+=1000){
    //		progress();

    progressT();
    double Xsec = tree->GetEntryW(entry); // LO-xsec/nevents

    double EvWeight = 1;
    if(useW) EvWeight = tree->Get(EvWeight,"EventWeight"); // includes  k-factor
    EvWeight *= Xsec * 1000; // rewe to lumi

    // Creating lepton collection
    //    std::vector<lepton_s> lepton;
    vector<TLorentzVector> Lepton;// = new vector<TLorentzVector>;
    vector<int> LepFlavour;

    int Nel = tree->Get(   Nel,   "Nel");
    int Nmu = tree->Get(   Nmu,   "Nmu");

    int Nlept = Nel + Nmu;
    //		cout << "Nleptons:\t" << Nlept << endl;
		
    std::vector<TLorentzVector> &Electrons = tree->Get(&Electrons,"Electrons");
    std::vector<int> &ElCh = tree->Get(&ElCh,"ElectronCh");
    std::vector<TLorentzVector> &Muons = tree->Get(&Muons,"Muons");
    std::vector<int> &MuCh = tree->Get(&MuCh,"MuonCh");

    bool  vetoE[100];for(int ie=0;ie<Nel;ie++)vetoE[ie]=0;
    bool  vetoM[100];for(int im=0;im<Nmu;im++)vetoM[im]=0;
    // EWKino leptons
    int nlept=0;
    for(int ie=0;ie<Nel;ie++) {
    	vetoE[ie]|=(Electrons[ie].Pt()<20)||(fabs(Electrons[ie].Eta())>2.4);
    	nlept++;
    }
    for(int im=0;im<Nmu;im++) {
    	vetoM[im]|=(Muons[im].Pt()<20)||(fabs(Muons[im].Eta())>2.4);
    	nlept++;
    }
    vector<bool> veto;
    for(int elid = 0; elid < Nel; elid++)
      { veto.push_back(vetoE[elid]);
	Lepton.push_back(Electrons[elid]);
	LepFlavour.push_back( 11*ElCh[elid]);
      }

    for(int muid = 0; muid < Nmu; muid++)
      { veto.push_back(vetoM[muid]);
	Lepton.push_back(Muons[muid]);
	LepFlavour.push_back(13*MuCh[muid]);
      }


    double LeadPt = 0;
    TLorentzVector lv_sum;
    int hasSS = 0; // has same sign leptons
    int hasZ = 0; // has Z like pair
		
		
    for(int lid = 0; lid < Nlept; lid++)
      { 
      if(veto[lid]) continue;
      hLepAllPt->Fill(Lepton[lid].Pt(),EvWeight);
	for(int sid = 0; (sid < Nlept) && (lid != sid); sid++)
	  { 
	  if(veto[sid]) continue;
	    int sumflvr = LepFlavour[lid] + LepFlavour[sid]; // summ of lepton flavour, possible comb: SS: 22,24,26 OS: 0,2
	    if(abs(sumflvr) > 20) {
	    	hasSS++;
	    	double m=lv_sum.M(); 
		if(m>0)hDiLMassSS->Fill(m,EvWeight);
		if(m>0)hDiLMassSS2->Fill(m,EvWeight);
	    }
		      
	    if(sumflvr == 0) // may be Z decay
	      {
		lv_sum = Lepton[lid] + Lepton[sid];//lv_lep1 + lv_lep2;
			  
		if(abs(lv_sum.M()-91.) < 15) hasZ++;
		hDiLMass->Fill(lv_sum.M(),EvWeight);
		hDiLMass2->Fill(lv_sum.M(),EvWeight);
	      }
	  }

	// get leading lepton Pt
	if(Lepton[lid].Pt() > LeadPt)
	  LeadPt = Lepton[lid].Pt();
      }
				
    if(Nlept > 0)
      if(LeadPt>0)hLep1Pt->Fill(LeadPt,EvWeight);

    double HT   = tree->Get(HT,"HT"); 
    hHT->Fill(HT,EvWeight);		
    double MET  = tree->Get(MET,"MET"); 
    hMET->Fill(MET,EvWeight);		
    double MT   = tree->Get(MT,"MT"); 
//    if(Nlept > 0) {
    if(MT > 0) {
    	hMT->Fill(MT,EvWeight);
    }

    //////////////////////
    // Start of CutFlow //
    //////////////////////

    // 0. CF presel
     CutFlowCnt[0]+= EvWeight;
    iCutFlowCnt[0]+= 1;

    // 1. 2 Lepton requirement 
    hNel->Fill(Nmu,EvWeight);
    hNmu->Fill(Nel,EvWeight);
    hNlep->Fill(nlept,EvWeight);
    if( nlept < 2)  continue;
     CutFlowCnt[1]+= EvWeight;
    iCutFlowCnt[1]+= 1;

    if(hasSS > 0)
      if(LeadPt>0)hLep1PtSS->Fill(LeadPt,EvWeight);

    // 2. MET
    if( MET < 120 ) continue;
     CutFlowCnt[2]+= EvWeight;
    iCutFlowCnt[2]+= 1;

    // 3. SS requirement
    if( !(hasSS > 0) ) continue;		
     CutFlowCnt[3] += EvWeight;
    iCutFlowCnt[3]+= 1;

    // 4. Z decay Veto
    if( hasZ > 0 ) continue;
     CutFlowCnt[4] += EvWeight;
    iCutFlowCnt[4]+= 1;

    //// End of PreSelection

    vector<TLorentzVector> &Jets = tree->Get(&Jets,"Jets");
    int Njet = tree->Get(Njet,"Njet");
    int tightNjet = 0;
    int hardNjet = 0;
    for(int jid = 0; jid < Njet; jid++){
	    if(Jets[jid].Pt() > 40) tightNjet++;
	    if(Jets[jid].Pt() > 100) hardNjet++;
    }
    hNtightJet->Fill(tightNjet,EvWeight);
    hNjet->Fill(Njet,EvWeight);

    if(Njet>0) hJet1Pt->Fill(Jets[0].Pt(),EvWeight);
    if(Njet>1) hJet2Pt->Fill(Jets[1].Pt(),EvWeight);
    if(Njet>2) hJet3Pt->Fill(Jets[2].Pt(),EvWeight);

    vector<int> &JetB = tree->Get(&JetB,"JetB");
    int Nbjet = tree->Get(Nbjet,"Nbjet");
    int tightNbjet = 0;
    for(int i = 0; i < Njet; i++)
	if(Jets[i].Pt() > 40 && JetB[i]>0) tightNbjet++;

    hNBjet->Fill(Nbjet,EvWeight);//pt is >20
    hNtightBjet->Fill(tightNbjet,EvWeight);

    // SR1
    if((MET > 120) && (MET < 200) && (tightNjet < 3) && (Nbjet == 0) )
      {
 	 CutFlowCnt[5] += EvWeight;
        iCutFlowCnt[5]+= 1;
	hLep1Ptsr1->Fill(LeadPt,EvWeight);
      }
    // SRr0
    if((MET > 120) && (tightNjet < 3) && (tightNbjet == 0) )
      {
 	 CutFlowCnt[6] += EvWeight;
        iCutFlowCnt[6]+= 1;
	hLep1Ptsr1t->Fill(LeadPt,EvWeight);
      }
    // SR3
    // MET > 200
    if((MET > 200))
      {
 	 CutFlowCnt[7] += EvWeight;
        iCutFlowCnt[7] += 1;
	hLep1Ptsr3->Fill(LeadPt,EvWeight);
      }
    // SR3 + b veto
    if((MET > 200)&& Nbjet==0)
      {
 	 CutFlowCnt[8] += EvWeight;
        iCutFlowCnt[8] += 1;
	hLep1Ptsr3->Fill(LeadPt,EvWeight);
      }
    if((MET > 200)&& HT<200)
      {
 	 CutFlowCnt[9] += EvWeight;
        iCutFlowCnt[9] += 1;
	hLep1Ptsr3->Fill(LeadPt,EvWeight);
      }

    // 5. btag 40 pt
//    if(tightNbjet>0) continue;
    if(Nbjet>0) continue;
     CutFlowCnt[10] += EvWeight;
    iCutFlowCnt[10] += 1;

    hHTb->Fill(HT,EvWeight);		
    hMETb->Fill(MET,EvWeight);		
    hMTb->Fill(MT,EvWeight);		
    if(Njet>0) hJet1Ptb->Fill(Jets[0].Pt(),EvWeight);
    if(Njet>1) hJet2Ptb->Fill(Jets[1].Pt(),EvWeight);
    if(Njet>2) hJet3Ptb->Fill(Jets[2].Pt(),EvWeight);

    // njets <=4 40 pt
    if(tightNjet>4) continue;
     CutFlowCnt[11] += EvWeight;
    iCutFlowCnt[11] += 1;
    // njets <=3 40 pt
    if(tightNjet>3) continue;
     CutFlowCnt[12] += EvWeight;
    iCutFlowCnt[12] += 1;
    if(tightNjet==0) continue;
     CutFlowCnt[13] += EvWeight;
    iCutFlowCnt[13] += 1;

    if(hardNjet==0) continue;
     CutFlowCnt[14] += EvWeight;
    iCutFlowCnt[14] += 1;


		
  }
  // ^loop end^

  ofstream tfile;
  tfile.open("EWKino_"+outname+".txt");
  tfile << "########################################" << endl;
  tfile << "Cut efficiency numbers:" << endl;
  for(int ci = 0; ci < CutNumb; ci++)
    {
      tfile << "After cut " << CutList[ci] <<  "\t\t\t" 
            << CutFlowCnt[ci]  << "\t events left\t"<< iCutFlowCnt[ci] <<" cnt\t"<< endl;
      CutFlow->SetBinContent(1+ci,CutFlowCnt[ci]);
    }

  //  for (int i=0;i < 20; i++)
  //  cout << i << "\t" << CutFlowCnt[i] << endl;;
	

  TFile * outf = new TFile("EWKino_"+outname+"_his.root","RECREATE");

  // write to file

  hMET->Write();
  hMT->Write();
  hHT->Write();

  hMETb->Write();
  hMTb->Write();
  hHTb->Write();

  hLep1Pt->Write();
  hLep1PtSS->Write();  
  hLepAllPt->Write();

  hDiLMass->Write();
  hDiLMass2->Write();
  hDiLMassSS->Write();
  hDiLMassSS2->Write();

  hNjet->Write();
  hNtightJet->Write();
  hNBjet->Write();
  hNtightBjet->Write();
  hNel->Write();
  hNmu->Write();
  hNlep->Write();

  hJet1Pt->Write();
  hJet2Pt->Write();
  hJet3Pt->Write();

  hJet1Ptb->Write();
  hJet2Ptb->Write();
  hJet3Ptb->Write();

  outf->mkdir("LeadLepton");
  outf->cd("LeadLepton");
  hLep1Ptsr1->Write();
  hLep1Ptsr3->Write();
  hLep1Ptsr1t->Write();


}


