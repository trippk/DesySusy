#include <iostream>
#include <vector>
#include <string>
#include "TROOT.h"
#include "TSystem.h"
#include "TH1.h"
#include "TClonesArray.h"

#include "TLorentzVector.h"
#include "TStopwatch.h"

#include "classes/DelphesClasses.h"

#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootTreeWriter.h"
#include "ExRootAnalysis/ExRootTreeBranch.h"
#include "ExRootAnalysis/ExRootResult.h"
#include "ExRootAnalysis/ExRootUtilities.h"

#include "nTuplerCode.h"
#include "mt2w_bisect.h"

using namespace std;

//        mt2w_event.set_momenta(pl,pb1,pb2,pmiss);
//        MT2W.push_back(mt2w_event.get_mt2w());

double GetMt2w(const TLorentzVector& Lep,
               const vector<TLorentzVector>& selJet,
               const vector<int>& selJetB,  
               int Nbjet, double mx, double my){
        if(selJetB.size()!=selJet.size()) {
        	cout<<"GetMt2w unequal"<<endl;
        	return -1;
        }
	static mt2w_bisect::mt2w mt2w_event;
	if(Nbjet==2){
		int ib1=-1;
		int ib2=-1;
		for(int i=0;i<selJetB.size();i++){
			if(selJetB[i]>0&&ib1<0) ib1=i;
			else if(selJetB[i]>0&&ib1>0&&ib2<0) ib2=i;
			else if(selJetB[i]>0&&ib1>0&&ib2>0) return -1;
		}
		mt2w_event.set_momenta(Lep,selJet[ib1],selJet[ib2],mx,my);
		double mt2w = mt2w_event.get_mt2w();
		mt2w_event.set_momenta(Lep,selJet[ib2],selJet[ib1],mx,my);
		return TMath::Min( mt2w , mt2w_event.get_mt2w() );
	}
	if(Nbjet==1){
		int ib=-1;
		for(int i=0;i<selJetB.size();i++) if(selJetB[i]>0) ib=i;
		double mt2wmin=0;
		int N=selJet.size();
		if(N>10)N=10;
//		if(N>6)N=6;
		for(int i=0;i<N;i++){
			if(i==ib) continue;
			mt2w_event.set_momenta(Lep,selJet[ib],selJet[i],mx,my);
			double mt2w = mt2w_event.get_mt2w();
			mt2w_event.set_momenta(Lep,selJet[i],selJet[ib],mx,my);
			mt2w = TMath::Min( mt2w , mt2w_event.get_mt2w() );
			mt2wmin = TMath::Min( mt2wmin,mt2w);
		}

		return mt2wmin;
	}
	return 0;
};

double getIso(GenParticle *Track, TClonesArray * Particles)
// Get isolation of TRack amongst Particles
	      //double Eta, double Phi, GenParticle)
{
    double minR = 10;

    int Npart = Particles->GetEntries();

    double Eta = Track->Eta;
    double Phi = Track->Phi;

    for (unsigned int i=0; i < Npart; i++) {
      //   if(particle[i].PT < 1.) continue;

	GenParticle * particle = (GenParticle *) Particles->At(i);

	double tmpR = sqrt(pow((particle->Eta - Eta),2) + pow(TMath::Pi()-acos(cos(particle->Phi - Phi)),2));
      if((tmpR < minR) && (tmpR != 0) && (particle->Status == 3)) minR = tmpR;
    }

    return minR;
}

double dPhi(double phi1, double phi2)
{

//  return acos(cos(phi1-phi2));

  double dp = phi1 - phi2;

  while(dp >  TMath::Pi()) dp -=  2*TMath::Pi();
  while(dp < -TMath::Pi()) dp +=  2*TMath::Pi();

  return fabs(dp);

}


//------------------------------------------------------------------------------
//double loosElPtMin = 10.; // GeV]
double loosElPtMin = 5.; // GeV]
double goodElPtMin = 20.; // GeV]
//double goodElEtaMax = 2.4; // GeV]
double goodElEtaMax = 4.0; // GeV]

//double loosMuPtMin = 10.; // GeV]
double loosMuPtMin = 5.; // GeV]
double goodMuPtMin = 20.; // GeV]
//double goodMuEtaMax = 2.4; // GeV]
double goodMuEtaMax = 4.0; // GeV]

double loosJetPtMin = 20.; // GeV]
double goodJetPtMin = 30.; // GeV]
double goodJetPtMin2 = 40.; // GeV]
//double goodJetEtaMax = 2.5; // GeV]
double goodJetEtaMax = 4.0; // GeV]
double goodJetIso = 0.4;

/*
double LepVetoPt = 10;
double MInvMin = 76; 
double MInvMax = 106; 
*/
/*
double leadJet1PtMin = 120.; // GeV]
double leadJet2PtMin = 70.; // GeV]
double leadJet3PtMin = 60.; // GeV]

double METmin = 120.; // GeV]
double HTmin = 0.; // GeV]
double HTmax = 0.; // GeV]
double MCTmin =  0;
*/

using namespace std;
void nTupler(const char *inputFile, string outname)
{

  TFile::SetOpenTimeout(300000); // 5 min
  TStopwatch t;
  t.Start();
  TFile* file = TFile::Open(inputFile);
  cout<<t.RealTime()<<" real open time for "<<inputFile<<endl;
  int cnt=1;
  while(!file->IsOpen()&&cnt<10) {
  	cnt++;
  	cout<<"nTuplerCode: could not open "<<inputFile<<" - try "<<cnt<<endl;
  	file = TFile::Open(inputFile);
  }
  TTree* intree=0;
  if(file->IsOpen()){
	  intree = (TTree*) file->Get("Delphes");
  } else {
  	cout<<"Could not open "<<inputFile<<endl;
  	exit(0);
  }
  t.Start();
  intree->GetEntries();
  cout<<t.RealTime()<<" real GetEntries() time for "<<inputFile<<endl;

//  TChain chain("Delphes");

//  chain.Add(inputFile);
//  chain.BranchRef();  

  // Create object of class ExRootTreeReader
//  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  ExRootTreeReader *treeReader = new ExRootTreeReader(intree);
  Long64_t numberOfEntries = treeReader->GetEntries();
  
  // Get pointers to branches used in this analysis
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchEvent = treeReader->UseBranch("Event");
  TClonesArray *branchMET = treeReader->UseBranch("MissingET");
  TClonesArray *branchGP = treeReader->UseBranch("Particle");

  // Output file
  TFile * tfile = TFile::Open(outname.c_str(),"RECREATE");

  // Tree definition
  int EvNumber = 0;
  double EventWeight = 0;

  int Nel = 0;
  int Nmu = 0;
  int looNel = 0;
  int looNmu = 0;
  int rawNel = 0;
  int rawNmu = 0;
  int Njet = 0;
  int rawNjet = 0;
  int Nbjet = 0;
  int Ntaujet = 0;

  double HT = 0;
  double HT40 = 0;
  double MET = 0;
  double MET_Phi = 0;
  double DelphMET = 0; //delphes MET
  double DelphMET_Phi = 0; //delphes MET Phi
  double MT = 0;
  double MT0 = 0;
  double MT2W = 0;
  double MT2W0 = 0;

  LHEFEvent * Event;
  /*
  std::vector<Electron *> goodEl;
  std::vector<Muon *> goodMu;
  std::vector<Jet *> goodJet;
  */

  std::vector<TLorentzVector> selEl;
  std::vector<int> selElCharge;
  std::vector<double> selElIso;
  std::vector<TLorentzVector> selMu;
  std::vector<int> selMuCharge;
  std::vector<double> selMuIso;
  std::vector<TLorentzVector> selJet;
  std::vector<int> selJetB;
  std::vector<int> selJetTau;
  std::vector<double> selJetMETdPhi;

  //  std::vector<double> JetPt;
  //std::vector<double> JetPhi;

  TTree * tree = new TTree("delphTree","Delphes subTree");
  tree->Branch("EventWeight",&EventWeight,"EventWeight/D");

  // collections
  //  tree->Branch("Event",&Event);

  //  tree->Branch("Electrons",&goodEl,"Electron");
  tree->Branch("Electrons",&selEl);
  tree->Branch("Nel",&Nel,"Nel/I");
  tree->Branch("rawNel",&rawNel,"rawNel/I");
  tree->Branch("looNel",&looNel,"looNel/I");
  tree->Branch("ElectronCh",&selElCharge);
  tree->Branch("ElectronIso",&selElIso);

  //  tree->Branch("Muons",&goodMu,"Muon");
  tree->Branch("Muons",&selMu);
  tree->Branch("rawNmu",&rawNmu,"rawNmu/I");
  tree->Branch("Nmu",&Nmu,"Nmu/I");
  tree->Branch("looNmu",&looNmu,"looNmu/I");
  tree->Branch("MuonCh",&selMuCharge);
  tree->Branch("MuonIso",&selMuIso);

  tree->Branch("Jets",&selJet);
  tree->Branch("Njet",&Njet,"Njet/I");
  tree->Branch("rawNjet",&rawNjet,"rawNjet/I");
  tree->Branch("JetB",&selJetB);
  tree->Branch("JetTau",&selJetTau);
  tree->Branch("Nbjet",&Nbjet,"Nbjet/I");
  tree->Branch("Ntaujet",&Ntaujet,"Ntaujet/I");
  tree->Branch("JetMETdPhi",&selJetMETdPhi);


  //  std::vector<LorentzM> selJet;
  //  tree->Branch("SelJet",&selJet);

  //  tree->Branch("Jets",&goodJet,"Jet");
  //  tree->Branch("JetPt",&JetPt);
  //  tree->Branch("JetPhi",&JetPhi);

  tree->Branch("MT",&MT,"MT/D");
  tree->Branch("MT2W",&MT2W,"MT2W/D");
  tree->Branch("MT0",&MT0,"MT0/D");
  tree->Branch("MT2W0",&MT2W0,"MT2W0");

  // variables
  tree->Branch("HT",&HT,"HT/D");
  tree->Branch("HT40",&HT40,"HT40/D");
  tree->Branch("MET",&MET,"MET/D");
  tree->Branch("DelphMET",&DelphMET,"DelphMET/D");
  tree->Branch("MET_Phi",&MET_Phi,"MET_Phi/D");
  tree->Branch("DelphMET_Phi",&DelphMET_Phi,"DelphMET_Phi/D");

  TLorentzVector lv_jet;
  TLorentzVector lv_el;
  TLorentzVector lv_mu;
  TLorentzVector lv_RecoMET;


  // Loop over all events
  cout << "Start loop" << endl;
  for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {

    selJet.clear();
    selJetB.clear();
    selJetTau.clear();
    selJetMETdPhi.clear();

    selEl.clear();
    selElCharge.clear();
    selElIso.clear();

    selMu.clear();
    selMuCharge.clear();
    selMuIso.clear();

    MT=MT0=MT2W=MT2W0=0;

    if(entry%1000 == 0)
      cout << "Entry\t" << entry << endl;

    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
    
    Event = (LHEFEvent *) branchEvent->At(0);
    if(Event->Weight > 0)
      EventWeight = Event->Weight;
    else       EventWeight = 1;

    // Reset variables
    lv_RecoMET.SetPtEtaPhiE(0.,0.,0.,0.);

    // Electrons ///////////////////////////////
    Electron *electron;
    //    int ngp = branchGP->GetEntries();
    Nel = branchElectron->GetEntries();
    looNel=0;

    for(int iel = 0; iel < Nel; iel++)
      {
	electron = (Electron *) branchElectron->At(iel);
	
	if(!(electron->PT > loosElPtMin)) continue;
	if(!(fabs(electron->Eta) < goodElEtaMax)) continue;
	
	lv_el.SetPtEtaPhiM(electron->PT, electron->Eta, electron->Phi, 0.);
	lv_RecoMET += lv_el;
  	looNel++;
	if(!(electron->PT > goodElPtMin)) continue;
	looNel--;

	selEl.push_back(lv_el);
	selElCharge.push_back(electron->Charge);
	selElIso.push_back(electron->IsolationVar);

	/*
	GenParticle* part = (GenParticle*)electron->Particle.GetObject();
	//	if(part!=0) cout<<part->PID<<" "<<part->Status<<" "<<part->PT<<endl;
	if(part!=0)
	  selElIso.push_back(getIso(part,branchGP));
	*/
      }

    rawNel=Nel;
    Nel = selEl.size();

    // Muons ///////////////////////////////
    Muon *muon;
    Nmu = branchMuon->GetEntries();
    looNmu=0;

    for(int imu = 0; imu < Nmu; imu++)
      {
	muon = (Muon *) branchMuon->At(imu);

	if(!(muon->PT > loosMuPtMin)) continue;
	if(!(fabs(muon->Eta) < goodMuEtaMax)) continue;

	lv_mu.SetPtEtaPhiM(muon->PT, muon->Eta, muon->Phi, 0.);	
	lv_RecoMET += lv_mu;

	looNmu++;
	if(!(muon->PT > goodMuPtMin)) continue;
	looNmu--;
	
	selMu.push_back(lv_mu);
	selMuCharge.push_back(muon->Charge);
	selMuIso.push_back(muon->IsolationVar);

	/*
	GenParticle* part = (GenParticle*)muon->Particle.GetObject();
	if(part!=0)
	  selMuIso.push_back(getIso(part,branchGP));
	*/
      }

    rawNmu=Nmu;
    Nmu = selMu.size();

    // JETS //////////////////////////////////
    HT = 0;
    HT40 = 0;
    //    JetPt.clear();
    //JetPhi.clear();

    Jet *jet;
    std::vector<Jet *> goodJettmp;
    Nbjet=0;
    Ntaujet=0;
    Njet = branchJet->GetEntries();
    for(int ijet = 0; ijet < Njet; ijet++)
      {    	
	jet = (Jet *) branchJet->At(ijet);

	if(!(jet->PT > loosJetPtMin)) continue;
	if(!(fabs(jet->Eta) < goodJetEtaMax)) continue;

	lv_jet.SetPtEtaPhiM(jet->PT, jet->Eta, jet->Phi, jet->Mass);
	lv_RecoMET += lv_jet;	
	
	if (!(jet->PT > goodJetPtMin)) continue;

	selJet.push_back(lv_jet);
	selJetB.push_back(jet->BTag);
	selJetTau.push_back(jet->TauTag);

	if(jet->BTag>0) Nbjet++;
	if(jet->TauTag>0) Ntaujet++;

	HT += jet->PT;
	if(jet->PT > goodJetPtMin2)HT40+= jet->PT;

      }

    rawNjet=Njet;
    Njet = selJet.size();

    ///////////////////////////////////////////
    lv_RecoMET.SetPhi((lv_RecoMET.Phi() + TMath::Pi()));
    MET = lv_RecoMET.Pt(); // new MET
    MET_Phi = lv_RecoMET.Phi();

    for(int ijet = 0; ijet < Njet; ijet++)
      selJetMETdPhi.push_back(dPhi(selJet[ijet].Phi(),MET_Phi));
    
    DelphMET = ((MissingET*)branchMET->At(0))->MET; // Delphes MET
    DelphMET_Phi = ((MissingET*)branchMET->At(0))->Phi;

    // transverse mass and MT2W for exactly 1 lepton
    TLorentzVector* Lep=0;
    if(selEl.size()==1&&looNel==0&&(looNmu+selMu.size())==0) Lep = &selEl[0];
    if(selMu.size()==1&&looNmu==0&&(looNel+selEl.size())==0) Lep = &selMu[0];
    if(Lep!=0){
        MT  = sqrt(2*MET* Lep->Pt()*(1-cos(MET_Phi -Lep->Phi()))) ;
        MT0 = sqrt(2*DelphMET*Lep->Pt()*(1-cos(DelphMET_Phi-Lep->Phi())));
	if(Nbjet>0) MT2W = GetMt2w(*Lep,selJet,selJetB,Nbjet,lv_RecoMET.Px(),lv_RecoMET.Py()); 
    }
    // Fill Tree
    tree->Fill();
  }

  tfile->cd();
  tree->Write();
  tree->Print();
}
