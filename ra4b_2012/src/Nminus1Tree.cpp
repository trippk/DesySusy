#include "Nminus1Tree.h"
#include "TLeaf.h"
#include "TObjArray.h"
#include "ConfigReader.h"

//typedef LorentzVector<PtEtaPhiE4D<double> > ROOT::Math::PtEtaPhiEVector
#include "Math/Vector4D.h"

Nminus1Tree::Nminus1Tree(){

  nFill=0;
  mytree = new TTree("subTree","a subTree");
  
  event=0;
  run=0;

  xs = 0.;
  nEvents = 0;
  FE = 0.;
  globalWeight = 0.;
  PUWeight = 0.;
  eventWeight = 0.;
  
  el=new LorentzED(0.,0.,0.,0.);
  elPt=0.;
  mu=new LorentzED(0.,0.,0.,0.);
  muPt=0.;
  
  mt2wEl=0.;
  mt2wMu-0.;
  
  njets=0;
  for (int iwp=0; iwp<3; iwp++) {
    nbjets[iwp]=0;
  }
  
  for (int ijet=0; ijet<7; ijet++) {
    jets[ijet]=new LorentzED(0.,0.,0.,0.);
    jetsPt[ijet]=0.;
    bjetdisc[ijet]=0.;
    for (int iwp=0; iwp<3; iwp++) {
      isbjet[ijet][iwp]=false;
    }
  }
  
  vMET=new LorentzED(0.,0.,0.,0.);
  MET=0.;
  vMHT=new LorentzED(0.,0.,0.,0.);
  MHT=0.;
  HT=0.;
     
  elSig = 0.;
  muSig = 0.;
  METSig = 0.;
  MHTSig = 0.;

  mtEl=0.;
  mtMu=0.;

  leptonsFromTop=0;

  mytree->Branch("Event",&event,"event/I");
  mytree->Branch("Run",&run,"run/I");
  mytree->Branch("xs",&xs,"xs/D");
  mytree->Branch("nEvents",&nEvents,"nEvents/I");
  mytree->Branch("FE",&FE,"FE/D");
  mytree->Branch("globalWeight",&globalWeight,"globalWeight/D");
  mytree->Branch("PUWeight",&PUWeight,"PUWeight/D");
  mytree->Branch("eventWeight",&eventWeight,"eventWeight/D");
  mytree->Branch("el.","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >",&el);
  mytree->Branch("elPt",&elPt,"elPt/D");
  mytree->Branch("mu.","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >",&mu);
  mytree->Branch("muPt",&muPt,"muPt/D");
  mytree->Branch("mt2wEl",&mt2wEl,"mt2wEl/D");
  mytree->Branch("mt2wMu",&mt2wMu,"mt2wMu/D");
  mytree->Branch("njets",&njets,"njets/I");
  
  TString WP[3];
  WP[0]="L"; WP[1]="M"; WP[2]="T";

  for (int iwp=0; iwp<3; iwp++) {
    TString name="nbjets_CSV"; name+=WP[iwp];
    TString size=name; size+="/I";
    mytree->Branch(name,&nbjets[iwp],size);
  }

  for (int ijet=0; ijet<7; ijet++) {

    TString name="jet"; name+=ijet+1;
    mytree->Branch(name+".","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >",&jets[ijet]);

    name="jet"; name+=ijet+1; name+="Pt";
    TString size=name; size+="/D";
    mytree->Branch(name,&jetsPt[ijet],size);

    name="jet"; name+=ijet+1; name+="_CSV";
    size=name; size+="/D";
    mytree->Branch(name,&bjetdisc[ijet],size);

    name="isbjet"; name+=ijet+1; name+="_CSV";
    TString tmp;
    for (int iwp=0; iwp<3; iwp++) {
      tmp=name; tmp+=WP[iwp];
      size=tmp; size+="/O";
      mytree->Branch(tmp,&isbjet[ijet][iwp],size);

    }
  }

  mytree->Branch("vMET.","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >",&vMET);
  mytree->Branch("MET",&MET,"MET/D");
  mytree->Branch("vMHT.","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >",&vMHT);
  mytree->Branch("MHT",&MHT,"MHT/D");
  mytree->Branch("HT",&HT,"HT/D");

  mytree->Branch("elSig",&elSig,"elSig/D");
  mytree->Branch("muSig",&muSig,"muSig/D");
  mytree->Branch("METSig",&METSig,"METSig/D");
  mytree->Branch("MHTSig",&MHTSig,"MHTSig/D");

  mytree->Branch("mtEl",&mtEl,"mtEl/D");
  mytree->Branch("mtMu",&mtMu,"mtMu/D");
				
  mytree->Branch("leptonsFromTop",&leptonsFromTop,"leptonsFromTop/I");

}

void Nminus1Tree::Write(){
  mytree->Write();
};

void Nminus1Tree::Fill( EventInfo* info, EasyChain* tree, vector<Muon*>& muons_in, vector<Electron*>& electrons_in, vector<Jet*>& jets_in,  vector<GenJet*>& genjets_in, LorentzM& met_in) {
   
  nFill++;

  if (nFill == 1) {
    this->FirstFill(tree);
  }
  
  this->SetToZero();

  event = info->Event;
  run = info->Run;    

  xs = info->xs;
  nEvents = info->NEvents;
  FE = info->FE;
  globalWeight = info->GlobalWeight;
  PUWeight = info->PUWeight;
  eventWeight = info->EventWeight;
  
  if (electrons_in.size()>0) {
    SetVector(el, electrons_in.at(0));
    elPt=electrons_in.at(0)->Pt();
  }
  if (muons_in.size()>0) {
    SetVector(mu, muons_in.at(0));
    muPt=muons_in.at(0)->Pt();
  }

  mt2wEl=mt2w_calc.get_mt2w( electrons_in, jets_in, met_in);
  mt2wMu=mt2w_calc.get_mt2w( muons_in, jets_in, met_in);

  njets=jets_in.size();

  const char* WP[3];
  WP[0]="Loose"; WP[1]="Medium"; WP[2]="Tight";

  HT=0.;
  double HTx=0;
  double HTy=0;

  for (int ijet=0; ijet<jets_in.size(); ijet++) {
    HT+=jets_in.at(ijet)->Pt();
    HTx+=jets_in.at(ijet)->P4().Px();
    HTy+=jets_in.at(ijet)->P4().Py();
    if (ijet<7) {
      SetVector(jets[ijet], jets_in.at(ijet));
      jetsPt[ijet]=jets_in.at(ijet)->Pt();
      bjetdisc[ijet]=jets_in.at(ijet)->BJetDisc("CSV");
      for (int iwp=0; iwp<3; iwp++) {
	isbjet[ijet][iwp]=jets_in.at(ijet)->IsBJet("CSV",WP[iwp]);
	if (isbjet[ijet][iwp]) nbjets[iwp]++;
      }
    }
  }

  SetVector(vMET, &met_in);
  MET=met_in.Et();
  vMHT->SetPxPyPzE(HTx,HTy,0.,0.);
  MHT=vMHT->Pt();

  elSig = elPt/sqrt(HT);
  muSig = muPt/sqrt(HT);
  METSig = MET/sqrt(HT);
  MHTSig = MHT/sqrt(HT);
 
  if (electrons_in.size()>0) {
    mtEl=sqrt(2*elPt*MET*(1-cos(el->Phi()-vMET->Phi() ) ) );
  }

  if (muons_in.size()>0) {
    mtMu=sqrt(2*muPt*MET*(1-cos(mu->Phi()-vMET->Phi() ) ) );;
  }

  this->FillScan(tree);

  leptonsFromTop=0;

  ConfigReader config;

  static bool isData=config.getBool("isData");

  if (!isData) {
    std::vector<int>& PdgId  = tree->Get( &PdgId, "genPdgId");
    std::vector<int>& Status = tree->Get( &Status, "genStatus");
    std::vector<int>& MotherIndex = tree->Get( &MotherIndex, "genMotherIndex");
    std::vector<int>& MotherPdgId = tree->Get( &MotherPdgId, "genMotherPdgId");
    
    for (int igen=0; igen<Status.size(); igen++) {
      if (abs(PdgId.at(igen))==6) {
	//std::cout<<IdToString(PdgId.at(igen)) << std::endl;
	leptonsFromTop+=gentools::Decay(igen,&MotherIndex,&PdgId,"");
      }
    }
  }

  mytree->Fill();
};

void Nminus1Tree::FirstFill(EasyChain* tree){

  TObjArray* leaves= tree->GetListOfLeaves();
  int entries=leaves[0].GetEntries();
  
  for (int i=0; i<leaves[0].GetEntries(); i++){
    
    TLeaf* leaf=(TLeaf*) leaves[0][i];
    TString name=leaf->GetTitle();
    
    if (name.Contains("susyScan") > 0) {
      TString type=leaf->GetTypeName();
      type.ToLower();
      if (type.Contains("bool") > 0)
	scanBool[name]=0;
      else if (type.Contains("int") > 0)
	scanInt[name]=0;
      else if (type.Contains("float") > 0 )
	scanFloat[name]=0.; 
      else if (type.Contains("double") > 0)
	scanDouble[name]=0.; 
    }
  }

  map<TString, bool>::iterator itScanBool;
  for (itScanBool = scanBool.begin(); itScanBool != scanBool.end(); ++itScanBool) {
    TString nameType=itScanBool->first; nameType+="/B";
    mytree->Branch(itScanBool->first, &itScanBool->second, nameType);
  }

  map<TString, int>::iterator itScanInt;
  for (itScanInt = scanInt.begin(); itScanInt != scanInt.end(); ++itScanInt) {
    TString nameType=itScanInt->first; nameType+="/I";
    mytree->Branch(itScanInt->first, &itScanInt->second, nameType);
  }

  map<TString, float>::iterator itScanFloat;
  for (itScanFloat = scanFloat.begin(); itScanFloat != scanFloat.end(); ++itScanFloat) {
    TString nameType=itScanFloat->first; nameType+="/F";
    mytree->Branch(itScanFloat->first, &(itScanFloat->second), nameType);
  }

  map<TString, double>::iterator itScanDouble;
  for (itScanDouble = scanDouble.begin(); itScanDouble != scanDouble.end(); ++itScanDouble) {
    TString nameType=itScanDouble->first; nameType+="/D";
    mytree->Branch(itScanDouble->first, &(itScanDouble->second), nameType);
  }

}

void Nminus1Tree::FillScan(EasyChain* tree){

  map<TString, bool>::iterator itScanBool;
  map<TString, int>::iterator itScanInt;
  map<TString, float>::iterator itScanFloat;
  map<TString, double>::iterator itScanDouble;

  for (itScanBool = scanBool.begin(); itScanBool != scanBool.end(); ++itScanBool)
    itScanBool->second=tree->Get( itScanBool->second, itScanBool->first);

  for (itScanInt = scanInt.begin(); itScanInt != scanInt.end(); ++itScanInt)
    itScanInt->second=tree->Get( itScanInt->second, itScanInt->first);
  
  for (itScanFloat = scanFloat.begin(); itScanFloat != scanFloat.end(); ++itScanFloat)
    itScanFloat->second=tree->Get( itScanFloat->second, itScanFloat->first);

  for (itScanDouble = scanDouble.begin(); itScanDouble != scanDouble.end(); ++itScanDouble)
    itScanDouble->second=tree->Get( itScanDouble->second, itScanDouble->first);
  
}

void Nminus1Tree::SetToZero(){

  event=0;
  run=0;

  xs = 0.;
  nEvents = 0;
  FE = 0.;
  globalWeight = 0.;
  PUWeight = 0.;
  eventWeight = 0.;
  
  el->SetPxPyPzE(0.,0.,0.,0.);
  elPt=0.;
  mu->SetPxPyPzE(0.,0.,0.,0.);
  muPt=0.;
  
  mt2wEl=0.;
  mt2wMu-0.;
  
  njets=0;
  for (int iwp=0; iwp<3; iwp++) {
    nbjets[iwp]=0;
  }
  
  for (int ijet=0; ijet<7; ijet++) {
    jets[ijet]->SetPxPyPzE(0.,0.,0.,0.);
    jetsPt[ijet]=0.;
    bjetdisc[ijet]=0.;
    for (int iwp=0; iwp<3; iwp++) {
      isbjet[ijet][iwp]=false;
    }
  }
  
  vMET->SetPxPyPzE(0.,0.,0.,0.);
  MET=0.;
  vMHT->SetPxPyPzE(0.,0.,0.,0.);
  MHT=0.;
  HT=0.;

  elSig = 0.;
  muSig = 0.;
  METSig = 0.;
  MHTSig = 0.;

  mtEl=0.;
  mtMu=0.;

  leptonsFromTop=0;

}

std::vector<int> gentools::GetDaughters(int ParticleIndex, std::vector<int>* MotherIndex) {

  std::vector<int> daughters;
  daughters.clear();

  for( int igen=0; igen<MotherIndex->size(); igen++) {
    if (MotherIndex->at(igen) == ParticleIndex)
      daughters.push_back(igen);
  }
  return daughters;
}


int gentools::Decay(int ParticleIndex, std::vector<int>* MotherIndex, std::vector<int>* PdgId, TString spazio) {

  TString spazionew="\t"; spazionew+=spazio;

  std::vector<int> daughters;
  daughters.clear();
  daughters=GetDaughters( ParticleIndex, MotherIndex );

  int count=0;

  for( int igen=0; igen<daughters.size(); igen++) {
    int id = PdgId->at(daughters.at(igen));
    //std::cout<<spazio<< "'------>" << IdToString(id) << std::endl;
    if ( abs(id) == 6 ||  abs(id) == 24 || abs(id) == 15 )
      count+=Decay(daughters.at(igen), MotherIndex, PdgId, spazionew);
    if ( abs(id) == 11 ||  abs(id) == 13 )
      count++;
  }

  return count;
}

TString gentools::IdToString(int id){
  TString particle;
  particle="";
  if (id<0) particle+="anti-";

  id=abs(id);
  if (id==1){ particle+="d"; }
  else if (id==2) {particle+="u";}
  else if (id==3) {particle+="s";}
  else if (id==4) {particle+="c";}
  else if (id==5) {particle+="b";}
  else if (id==6) {particle+="t";}
  else if (id==11){particle+="e";}
  else if (id==12){particle+="nu_e";}
  else if (id==13){particle+="mu";}
  else if (id==14){particle+="nu_mu";}
  else if (id==15){particle+="tau";}
  else if (id==16){particle+="nu_tau";}
  else if (id==24){particle+="W+";}
  else {particle+=id;}

  return particle;

}
