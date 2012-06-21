#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"
#include "ElectronEffectiveArea.h"

using namespace std;


bool electrons_RA6(EasyChain* tree, vector<unsigned>& selEl, CutSet& selCut) {

  //cout<<"electrons"<<endl;

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

  //Get branches
  vector<LOR>& Electrons = tree->Get(&Electrons, "electronP4Pat");

  for( unsigned el=0; el<Electrons.size(); ++el ) {
    
    /////////////////////
    //PT and ETA cuts
    /////////////////////

    TString etaCut ="|eta|<";
    etaCut+=el_eta_max;
    const float eta = Electrons.at(el).eta();
    if( !selCut.keepIf(etaCut , fabs(eta)  <  el_eta_max    ) && quick ) continue;
    TString ptCut ="pt>";
    ptCut+=el_pt_min_low;
    if( !selCut.keepIf(ptCut  , Electrons.at(el).pt()         >  el_pt_min_low ) && quick ) continue;

    vector<bool>&     El_isEB                = tree->Get( &El_isEB , "DESYelectronIsEBPat" );
    vector<bool>&     El_isEE                = tree->Get( &El_isEE , "DESYelectronIsEEPat" );

    ////////////////////
    //PF Iso calculation
    ////////////////////
    //Iso deposits
    vector<float>&    El_chargedHadronIso = tree->Get( &El_chargedHadronIso, "electronChargedHadronIsoPat");
    vector<float>&    El_neutralHadronIso = tree->Get( &El_neutralHadronIso, "electronNeutralHadronIsoPat");
    vector<float>&    El_photonIso        = tree->Get( &El_photonIso, "electronPhotonIsoPat");

    //Rho and effective area for correction
    float& rho25 = tree->Get( &rho25, "rho25");
    float rhoPrime = std::max(rho25, 0.0);
    float AEff = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, eta, ElectronEffectiveArea::kEleEAData2011);

    float iso_n = std::max(El_photonIso.at(el) + El_neutralHadronIso.at(el) - rhoPrime * AEff, 0.0);
    float iso_ch = El_chargedHadronIso.at(el);
    float relPfIso = (iso_ch + iso_n) / Electrons.at(el).Pt(); 
    if( !selCut.keepIf("RelIso<.15"        , relPfIso                      <    .15 ) && quick ) continue;
    TString cutList = etaCut+" "+ptCut+" RelIso<.15";

    /////////////////////////////////
    //Cut based electron ID, medium WP
    ////////////////////////////////

    //Barrel and Endcap dependent cuts
    vector<float>&    dEtaIn             = tree->Get( &dEtaIn,      "electronDeltaEtaSuperClusterTrackAtVtxPat");
    vector<float>&    dPhiIn             = tree->Get( &dPhiIn,      "electronDeltaPhiSuperClusterTrackAtVtxPat");
    vector<float>&    sigmaIetaIeta      = tree->Get( &sigmaIetaIeta, "electronSigmaIetaIetaPat"                 );
    vector<float>&    HoverE             = tree->Get( &HoverE,    "electronHcalOverEcalPat"                  );

    if( El_isEB.at(el) ) {
      if( !selCut.keepIf("dEtaIn<0.004(0.007)"        , dEtaIn.at(el)      <  0.004  ) && quick ) continue;
      if( !selCut.keepIf("dPhiIn<0.06(0.03)"          , dPhiIn.at(el)      <  0.06   ) && quick ) continue;
      if( !selCut.keepIf("sigmaIetaIeta<0.01(0.03)", sigmaIetaIeta.at(el)  <  0.01 ) && quick ) continue;
      if( !selCut.keepIf("HoverE<0.12(0.10)"       , HoverE.at(el)         <  0.12   ) && quick ) continue;
    }
    else if( El_isEE.at(el) ) {
      if( !selCut.keepIf("dEtaIn<0.004(0.007)"        , dEtaIn.at(el)      <  0.007   ) && quick ) continue;
      if( !selCut.keepIf("dPhiIn<0.06(0.03)"          , dPhiIn.at(el)      <  0.03   ) && quick ) continue;
      if( !selCut.keepIf("sigmaIetaIeta<0.01(0.03)", sigmaIetaIeta.at(el)  <  0.03 ) && quick ) continue;
      if( !selCut.keepIf("HoverE<0.12(0.10)"       , HoverE.at(el)         <  0.10 ) && quick ) continue;
    }
    cutList +=" dEtaIn<0.004(0.007) dPhiIn<0.06(0.03) sigmaIetaIeta<0.01(0.03) HoverE<0.12(0.10)";
    //Barrel and Endcap independent cuts
    vector<float>&    El_GsfTrackDxy     = tree->Get( &El_GsfTrackDxy,    "electronGsfTrackDxyPat"        );
    vector<float>&    El_GsfTrackDz      = tree->Get( &El_GsfTrackDz   , "electronGsfTrackDzPat"         );
    if( !selCut.keepIf("|d0|<0.02"         , fabs(El_GsfTrackDxy.at(el))    <   0.02 ) && quick ) continue;
    if( !selCut.keepIf("|dz|<0.10"         , fabs(El_GsfTrackDz.at(el))     <   0.10 ) && quick ) continue;
    cutList +=" |d0|<0.02 |dz|<0.10";

    //For fabs(1/E-1/p)
    vector<float>&    ecalEnergy         = tree->Get( &ecalEnergy, "electronEcalEnergyPat"                 );
    vector<float>&    eSuperClusterOverP = tree->Get( &eSuperClusterOverP, "electronESuperClusterOverPPat" );
    float iE_minus_iP = fabs(1.0/ecalEnergy.at(el) - eSuperClusterOverP.at(el) / ecalEnergy.at(el) );
    if( !selCut.keepIf("|1/E-1/p|<0.05"     , iE_minus_iP              <   0.05    ) && quick ) continue;
    cutList +=" |1/E-1/p|<0.05"

    //Conversion Rejection
    vector<bool>& matchedConversions      = tree->Get( &matchedConversions,   "DESYelectronHasMatchedConversionsPat");
    if( !selCut.keepIf("NoMatchedConversions"     , !matchedConversions.at(el)    ) && quick ) continue;
    cutList +=" NoMatchedConversions"; 
    vector<int>& missingHits              = tree->Get( &missingHits,   "electronConversionMissingHitsPat");
    if( !selCut.keepIf("numMisHits<2"     , missingHits.at(el)              <   2    ) && quick ) continue;
    cutList +=" numMisHits<2";

    //Tight trigger requirement
    vector<int>& idTightTrig = tree->Get( &idTightTrig,   "DESYelectronIdTriggerTightPat");
    if( !selCut.keepIf("idTightTrig"     , idTightTrig.at(el) ) && quick ) continue;
    cutList +=" idTightTrig";

    if( quick || selCut.applyCuts("RA6 electron selection", cutList))
      selEl.push_back(el);

  }// electron loop

  //cout<<"el end"<<endl;
}//main
