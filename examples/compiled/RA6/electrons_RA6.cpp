#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"

using namespace std;


bool electrons_RA6(EasyChain* tree, vector<unsigned>& selEl, CutSet& selCut) {

  //  cout<<"electrons"<<endl;

  ConfigReader config;
  // static since we read only once
  static float el_pt_min_low   = config.getFloat("el_pt_min_low",  20);
  //static float el_pt_min_high  = config.getFloat("el_pt_min_high", 20);
  static float el_eta_max      = config.getDouble("el_eta_max",   2.5);
  static bool  quick           = config.getBool("quick"      ,false);
  static bool  isOldNtuple     = config.getBool("isOldNtuple",false);

  typedef LorentzM LOR;
  typedef float df;
  //if isOldNtuple:
    //typedef LorentzV LOR;
    //typedef double df;
    

  vector<LOR>& Electrons = tree->Get(&Electrons, "electronP4Pat");

  for( unsigned el=0; el<Electrons.size(); ++el ) {
    TString etaCut ="|eta|<";
    etaCut+=el_eta_max;
    if( !selCut.keepIf(etaCut , fabs(Electrons.at(el).eta())  <  el_eta_max    ) && quick ) continue;
    TString ptCut ="pt>";
    ptCut+=el_pt_min_low;
    if( !selCut.keepIf(ptCut  , Electrons.at(el).pt()         >  el_pt_min_low ) && quick ) continue;
    vector<bool>&     El_isEB                = tree->Get( &El_isEB , "DESYelectronIsEBPat" );
    vector<bool>&     El_isEE                = tree->Get( &El_isEE , "DESYelectronIsEEPat" );
    vector<float>&    El_Dr03EcalRecHitSumEt = tree->Get( &El_Dr03EcalRecHitSumEt, "electronDr03EcalRecHitSumEtPat");
    vector<float>&    El_Dr03HcalTowerSumEt  = tree->Get( &El_Dr03HcalTowerSumEt , "electronDr03HcalTowerSumEtPat" );
    vector<float>&    El_Dr03TkSumPt         = tree->Get( &El_Dr03TkSumPt        , "electronDr03TkSumPtPat"        );

    //cout<<"++++++++++++++++++++++"<<El_isEB.size()<<"   "<<El_isEE.size()<<"   "<<Electrons.size()<<"+++++++++++"<<endl;

    double El_RelIso=100.;
    if( El_isEE.at(el) )
      El_RelIso = ( El_Dr03TkSumPt.at(el) + 
		    El_Dr03EcalRecHitSumEt.at(el) + 
		    El_Dr03HcalTowerSumEt.at(el)
		    ) / max((float) Electrons.at(el).Pt(), (float) 20.);
    else if( El_isEB.at(el) )
      El_RelIso = ( El_Dr03TkSumPt.at(el) + 
		    max(0.,(El_Dr03EcalRecHitSumEt.at(el)-1.)) + 
		    El_Dr03HcalTowerSumEt.at(el)
		    ) / max((float) Electrons.at(el).Pt(), (float) 20.);
    
    if( !selCut.keepIf("RelIso<.15"        , El_RelIso                      <    .15 ) && quick ) continue;

//     vector<XYZPointD>& El_SC3vec          = tree->Get( &El_SC3vec,   "DESYelectronSuperClusterPositionPat" );
//     vector<df>&        El_SCenergy        = tree->Get( &El_SCenergy, "DESYelectronSuperClusterEnergyPat" );

//     float SC_ET=El_SCenergy.at(el)*sin(El_SC3vec.at(el).theta());
//     if( !selCut.keepIf("ScEt>10"           , SC_ET                          >  10    ) && quick ) continue;

//     vector<bool>&     El_EcalDrivenSeed   = tree->Get( &El_EcalDrivenSeed,     "DESYelectronEcalDrivenSeedPat");
//     if( !selCut.keepIf("EcalDrivenSeed"    , El_EcalDrivenSeed.at(el)                ) && quick ) continue;

    vector<float>&    idVBTF90r        = tree->Get( &idVBTF90r, "DESYelectronSimpleEleId90relIsoPat" );
    if( !selCut.keepIf("VBTF90==7"       , idVBTF90r.at(el)           ==  7   ) && quick ) continue;
    //additional cuts to meet trigger requ:
    vector<float>&    HoverE           = tree->Get( &HoverE,    "electronHcalOverEcalPat"                  );
    vector<float>&    sIetaIeta        = tree->Get( &sIetaIeta, "electronSigmaIetaIetaPat"                 );
    vector<float>&    dphi             = tree->Get( &dphi,      "electronDeltaPhiSuperClusterTrackAtVtxPat");
    vector<float>&    deta             = tree->Get( &deta,      "electronDeltaEtaSuperClusterTrackAtVtxPat");
    if( El_isEB.at(el) ) {
      if( !selCut.keepIf("HoverE<0.1(0.075)"     , HoverE.at(el)    <  0.1   ) && quick ) continue;
      if( !selCut.keepIf("sIeatIeat<0.011(0.031)", sIetaIeta.at(el) <  0.011 ) && quick ) continue;
      if( !selCut.keepIf("dphi<0.01(0.01)"       , dphi.at(el)      <  0.01  ) && quick ) continue;
      if( !selCut.keepIf("deta<0.15(0.1)"        , deta.at(el)      <  0.15  ) && quick ) continue;
    }
    else if( El_isEE.at(el) ) {
      if( !selCut.keepIf("HoverE<0.1(0.075)"     , HoverE.at(el)    <  0.075 ) && quick ) continue;
      if( !selCut.keepIf("sIeatIeat<0.011(0.031)", sIetaIeta.at(el) <  0.031 ) && quick ) continue;
      if( !selCut.keepIf("dphi<0.01(0.01)"       , dphi.at(el)      <  0.01  ) && quick ) continue;
      if( !selCut.keepIf("deta<0.15(0.1)"        , deta.at(el)      <  0.1   ) && quick ) continue;
    }

    vector<df>&   El_GsfTrackDxy          = tree->Get( &El_GsfTrackDxy,    "electronGsfTrackDxyPat"        );
    if( !selCut.keepIf("|d0|<0.02"         , fabs(El_GsfTrackDxy.at(el))    <    .02 ) && quick ) continue;
    vector<df>&   El_GsfTrackDz           = tree->Get( &El_GsfTrackDz   , "electronGsfTrackDzPat"         );
    if( !selCut.keepIf("|dz|<1.00"         , fabs(El_GsfTrackDz.at(el))     <   1.   ) && quick ) continue;
    
    vector<int>& missingHits              = tree->Get( &missingHits,   "electronConversionMissingHitsPat");
    if( !selCut.keepIf("numMisHits<2"     , missingHits.at(el)              <   2    ) && quick ) continue;



    if( quick || selCut.applyCuts("RA6 electron selection", ptCut+" "+etaCut+" VBTF90==7 HoverE<0.1(0.075) sIeatIeat<0.011(0.031) dphi<0.01(0.01) deta<0.15(0.1) |d0|<0.02 |dz|<1.00 numMisHits<2 RelIso<.15" ))
      selEl.push_back(el);

  }// electron loop


}//main
