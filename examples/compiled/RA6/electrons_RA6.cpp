#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"

using namespace std;


bool electrons_RA6(EasyChain* tree, vector<unsigned>& selEl, CutSet& selCut) {

  //cout<<"electrons"<<endl;

  ConfigReader config;

  static float el_pt_min_low   = config.getFloat("el_pt_min_low",  10.);
  static float el_eta_max      = config.getFloat("el_eta_max",   2.4);
  static bool  quick           = config.getBool("quick"      ,false);

  bool isOneProbl=false;
  int      run   = tree->Get(run  , "run"        );
  unsigned event = tree->Get(event, "event"      );
//   //in Aachen but not me:
// if(run==191062 && event==173724117) isOneProbl=true;
// if(run==191830 && event==500801146) isOneProbl=true;
// if(run==193575 && event==378525614) isOneProbl=true;
// if(run==194076 && event==1780704) isOneProbl=true;
// if(run==194151 && event==548436154) isOneProbl=true;
// if(run==194315 && event==49895517) isOneProbl=true;
// if(run==194424 && event==618059385) isOneProbl=true;
// if(run==194789 && event==50048910) isOneProbl=true;
// if(run==194790 && event==32359325) isOneProbl=true;
// if(run==194912 && event==801636396) isOneProbl=true;
// if(run==195655 && event==484658815) isOneProbl=true;
// if(run==195749 && event==25008105) isOneProbl=true;
// if(run==195915 && event==715603444) isOneProbl=true;
// if(run==195948 && event==295979540) isOneProbl=true;
// if(run==194076 && event==214790825) isOneProbl=true;
// if(run==194912 && event==448763504) isOneProbl=true;
// if(run==195552 && event==607255040) isOneProbl=true;
// if(run==195950 && event==44923344) isOneProbl=true;
// if(run==195950 && event==991151454) isOneProbl=true;
// if(run==196047 && event==30959864) isOneProbl=true;
// if(run==196197 && event==738255119) isOneProbl=true;
// if(run==196438 && event==61554735) isOneProbl=true;

// //in mine but not Aachen:
// if(run==193336 && event==607270004) isOneProbl=true;
// if(run==196438 && event==141696094) isOneProbl=true;
// if(run==194150 && event==136483931) isOneProbl=true;
// if(run==194315 && event==199208976) isOneProbl=true;
// if(run==194429 && event==6474747) isOneProbl=true;
// if(run==195013 && event==261167716) isOneProbl=true;
// if(run==195378 && event==586513936) isOneProbl=true;
// if(run==195390 && event==209739431) isOneProbl=true;
// if(run==195774 && event==169278152) isOneProbl=true;

//SYNCH in 533
//mumu
//Aachen not me
if(run==190703 && event==188364467) isOneProbl=true;
if(run==190733 && event==247838198) isOneProbl=true;
if(run==191271 && event==258493671) isOneProbl=true;

//me not Aachen
if(run==194115 && event==26127789) isOneProbl=true;
if(run==194050 && event==544007612) isOneProbl=true;
if(run==194050 && event==840612795) isOneProbl=true;


//elel
//Aachen not me
if(run==193193 && event==261448119) isOneProbl=true;
if(run==190702 && event==128666827) isOneProbl=true;
if(run==190733 && event==76514246) isOneProbl=true;
if(run==190733 && event==303138892) isOneProbl=true;

//me not Aachen
if(run==195398 && event==242173458) isOneProbl=true;

  vector<LorentzM>& Electrons = tree->Get(&Electrons, "electronP4Pat");

  if(isOneProbl) cout<<Electrons.size()<<" electrons at all."<<endl;

  for( unsigned el=0; el<Electrons.size(); ++el ) {

    TString etaCut ="|eta|<=";
    etaCut+=el_eta_max;
    vector<float>& ESuperClusterEtaPat = tree->Get( &ESuperClusterEtaPat, "electronESuperClusterEtaPat");

    if(isOneProbl) cout<<"SC eta: "<<ESuperClusterEtaPat.at(el)<<",  LV eta: "<<Electrons.at(el).eta()<<endl;

    if( !selCut.keepIf(etaCut   , fabs(ESuperClusterEtaPat.at(el)) <= el_eta_max    ) && quick ) continue;

    if(isOneProbl) cout<<"pt : "<<Electrons.at(el).pt()<<endl;

    TString ptCut ="pt>=";
    ptCut+=el_pt_min_low;
    if( !selCut.keepIf(ptCut    , Electrons.at(el).pt()        >= el_pt_min_low ) && quick ) continue;

    vector<int>&  idLoose = tree->Get( &idLoose, "electronIdLoosePat"   );
    if(isOneProbl) cout<<"id: "<<idLoose.at(el)<<endl;


    vector<int>&  electronGsfTrackTrackerExpectedHitsInnerPat= tree->Get(&electronGsfTrackTrackerExpectedHitsInnerPat, "electronGsfTrackTrackerExpectedHitsInnerPat");
    if(isOneProbl) cout<<"id: exp hits: "<<electronGsfTrackTrackerExpectedHitsInnerPat.at(el)<<endl;
    vector<float>& GsfTrackDxyPat = tree->Get( &GsfTrackDxyPat, "electronGsfTrackDxyPat" ); 
    if(isOneProbl) cout<<"id: dxy: "<<GsfTrackDxyPat.at(el)<<endl;

    vector<float>& electronDeltaEtaSuperClusterTrackAtVtxPat = tree->Get( &electronDeltaEtaSuperClusterTrackAtVtxPat, "electronDeltaEtaSuperClusterTrackAtVtxPat");
    vector<float>& electronDeltaPhiSuperClusterTrackAtVtxPat = tree->Get( &electronDeltaPhiSuperClusterTrackAtVtxPat, "electronDeltaPhiSuperClusterTrackAtVtxPat");
    vector<float>& electronSigmaIetaIetaPat = tree->Get( &electronSigmaIetaIetaPat, "electronSigmaIetaIetaPat");
    if(isOneProbl) cout<<"id: dEta"<<electronDeltaEtaSuperClusterTrackAtVtxPat.at(el)<<endl;
    if(isOneProbl) cout<<"id: dPhi"<<electronDeltaPhiSuperClusterTrackAtVtxPat.at(el)<<endl;
    if(isOneProbl) cout<<"id: sigmaIetaIeta"<<electronSigmaIetaIetaPat.at(el)<<endl;
    vector<bool>& DESYelectronIsEBPat = tree->Get( &DESYelectronIsEBPat, "DESYelectronIsEBPat");
    if(isOneProbl) cout<<"id: isEB? "<<DESYelectronIsEBPat.at(el)<<endl;

    vector<float>& electronEcalEnergyPat = tree->Get( &electronEcalEnergyPat, "electronEcalEnergyPat");
    vector<float>& electronESuperClusterOverPPat = tree->Get( &electronESuperClusterOverPPat, "electronESuperClusterOverPPat");
    if(isOneProbl) cout<<"id: ooeee"<<1./electronEcalEnergyPat.at(el) - electronESuperClusterOverPPat.at(el)/electronEcalEnergyPat.at(el)<<endl;


    Float_t rho = tree->Get( rho,"rho");
    float   aeff=0.110;
    double rhoprime=std::max(double(rho),0.);

    vector<float> & electronNeutralHadronIsoPat = tree->Get( &electronNeutralHadronIsoPat, "electronNeutralHadronIsoPat");
    vector<float> & electronPhotonIsoPat = tree->Get( &electronPhotonIsoPat, "electronPhotonIsoPat");
    vector<float> & electronChargedHadronIsoPat = tree->Get( &electronChargedHadronIsoPat, "electronChargedHadronIsoPat");

    double iso_n = std::max(electronNeutralHadronIsoPat.at(el) + electronPhotonIsoPat.at(el) - rhoprime * aeff, 0.0);
    double iso = (iso_n + electronChargedHadronIsoPat.at(el)) / Electrons.at(el).pt();

    if(isOneProbl) cout<<"id: NHiso "<<electronNeutralHadronIsoPat.at(el)<<endl;
    if(isOneProbl) cout<<"id: Phiso "<<electronPhotonIsoPat.at(el)<<endl;
    if(isOneProbl) cout<<"id: CHiso "<<electronChargedHadronIsoPat.at(el)<<endl;

    if(isOneProbl) cout<<"id: iso "<<iso<<endl;
    // = tree->Get( &, "")
    // if(isOneProbl) cout<<"id: "<<.at(el)<<endl;
    vector<float>& GsfTrackDzPat = tree->Get( &GsfTrackDzPat, "electronGsfTrackDzPat" ); 
    if(isOneProbl) cout<<"gfstrack DZ: "<<GsfTrackDzPat.at(el)<<endl;

    if( !selCut.keepIf("IDloose", idLoose.at(el)                               ) && quick ) continue;



    if( !selCut.keepIf("dz<=0.1" , fabs(GsfTrackDzPat.at(el)) <= 0.1         ) && quick ) continue;

    if( !selCut.keepIf("gapVeto", fabs(ESuperClusterEtaPat.at(el))<=1.4442 || fabs(ESuperClusterEtaPat.at(el))>=1.566 ) && quick ) continue;

    if( quick || selCut.applyCuts("RA6 electron selection", ptCut+" "+etaCut+" IDloose dz<=0.1 gapVeto" ))
      selEl.push_back(el);

  }// electron loop

  //cout<<"el end"<<endl;
}//main
