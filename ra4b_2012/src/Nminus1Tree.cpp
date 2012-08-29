#include "Nminus1Tree.h"


Nminus1Tree::Nminus1Tree(){
     
  mytree = new TTree("subTree","a subTree");
  
  event=0;
  run=0;
  weight=0.0;
  PUWeight=0.0;
  
  el=new LorentzM(0.,0.,0.,0.);
  elPt=0.;
  mu=new LorentzM(0.,0.,0.,0.);
  muPt=0.;
  
  mt2wEl=0.;
  mt2wMu-0.;
  
  njets=0;
  for (int iwp=0; iwp<3; iwp++) {
    nbjets[iwp]=0;
  }
  
  for (int ijet=0; ijet<7; ijet++) {
    jets[ijet]=new LorentzM(0.,0.,0.,0.);
    jetsPt[ijet]=0.;
    bjetdisc[ijet]=0.;
    for (int iwp=0; iwp<3; iwp++) {
      isbjet[ijet][iwp]=false;
    }
  }
  
  vMET=new LorentzM(0.,0.,0.,0.);
  MET=0.;
  vMHT=new LorentzM(0.,0.,0.,0.);
  MHT=0.;
  HT=0.;
     
  elSig = 0.;
  muSig = 0.;
  METSig = 0.;
  MHTSig = 0.;

  mtEl=0.;
  mtMu=0.;

  mytree->Branch("Event",&event,"event/I");
  mytree->Branch("Run",&run,"run/I");
  mytree->Branch("Weight",&weight,"weight/D");
  mytree->Branch("PUWeight",&PUWeight,"PUWeight/D");
  mytree->Branch("el","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >",&el);
  mytree->Branch("elPt",&elPt,"elPt/D");
  mytree->Branch("mu","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >",&mu);
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
    mytree->Branch(name,"ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >",&jets[ijet]);

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

  mytree->Branch("vMET","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >",&vMET);
  mytree->Branch("MET",&MET,"MET/D");
  mytree->Branch("vMHT","ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >",&vMHT);
  mytree->Branch("MHT",&MHT,"MHT/D");
  mytree->Branch("HT",&HT,"HT/D");

  mytree->Branch("elSig",&elSig,"elSig/D");
  mytree->Branch("muSig",&muSig,"muSig/D");
  mytree->Branch("METSig",&METSig,"METSig/D");
  mytree->Branch("MHTSig",&MHTSig,"MHTSig/D");

  mytree->Branch("mtEl",&mtEl,"mtEl/D");
  mytree->Branch("mtMu",&mtMu,"mtMu/D");				

}

void Nminus1Tree::Write(){
  mytree->Write();
};

void Nminus1Tree::Fill( EventInfo* info, EasyChain* tree, vector<Muon*> muons_in, vector<Electron*> electrons_in, vector<Jet*> jets_in, LorentzM& met_in) {
    
  this->SetToZero();

  event = info->Event;
  run = info->Run;    
  weight=info->EventWeight;
  PUWeight=info->PUWeight;

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

  for (int ijet=0; ijet<jets_in.size() && ijet<7; ijet++) {
    HT+=jets_in.at(ijet)->Pt();
    HTx+=jets_in.at(ijet)->p4.Px();
    HTy+=jets_in.at(ijet)->p4.Py();
    SetVector(jets[ijet], jets_in.at(ijet));
    jetsPt[ijet]=jets_in.at(ijet)->Pt();
    bjetdisc[ijet]=jets_in.at(ijet)->BJetDisc("CSV");
    for (int iwp=0; iwp<3; iwp++) {
      isbjet[ijet][iwp]=jets_in.at(ijet)->IsBJet("CSV",WP[iwp]);
      if (isbjet[ijet][iwp]) nbjets[iwp]++;
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

  mytree->Fill();
};

void Nminus1Tree::SetToZero(){

  event=0;
  run=0;
  weight=0.0;
  PUWeight=0.0;
  
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

}
