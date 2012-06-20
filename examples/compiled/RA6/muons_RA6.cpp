#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"
#include "Math/VectorUtil.h"

using namespace ROOT::Math::VectorUtil;
using namespace std;


bool muons_RA6(EasyChain* tree, vector<unsigned>& selMu, CutSet& selCut) {

  //cout<<"mu start"<<endl;

  ConfigReader config;
  // static since we read only once
  static float mu_pt_min_low   = config.getFloat("mu_pt_min_low",  20);
  //static float mu_pt_min_high  = config.getFloat("mu_pt_min_high", 20);
  static float mu_eta_max      = config.getDouble("mu_eta_max",   2.4);
  static bool  quick           = config.getBool("quick"      , false );
  static bool  isOldNtuple     = config.getBool("isOldNtuple", false );

  typedef LorentzM LOR;
  typedef float df;
  typedef int ui;
  //if isOldNtuple:
    //typedef LorentzV LOR;
    //typedef double df;
    //typedef unsigned ui;


  vector<LOR>& Muons = tree->Get(&Muons, "muonP4Pat");

  for( unsigned mu=0; mu<Muons.size(); ++mu ) {

    //Eta, Pt
    TString etaCut ="|eta|<";
    etaCut+=mu_eta_max;
    if( !selCut.keepIf(etaCut          , fabs(Muons.at(mu).eta())  <  mu_eta_max    ) && quick ) continue;
    TString ptCut ="pt>";
    ptCut+=mu_pt_min_low;
    if( !selCut.keepIf(ptCut           , Muons.at(mu).pt()         >  mu_pt_min_low ) && quick ) continue;

    //IsoCut
    vector<float>& pfIso      = tree->Get( &pfIso,      "muonPfIsolationR04DeltaBCorrectedPat"                                );
    double RelIso_max20 = pfIso.at(mu) / max((float)20.,(float)Muons.at(mu).Pt());
    if( !selCut.keepIf("RelIso<.12"       , RelIso_max20           <    .12 ) && quick ) continue;
    TString cutList = etaCut+" "+ptCut+" RelIso<.12";
   
    //Tight muon (defined by POG Twiki: SWGuidMuonId)
    vector<int>&   IsGlobal     = tree->Get( &IsGlobal,     "muonIsGlobalMuonPat"                           );
    if( !selCut.keepIf("isGlobal"         , IsGlobal.at(mu)                 ) && quick ) continue;
    cutList+=" "+"isGlobal";

    vector<int>&   IsPf         = tree->Get( &IsPf,     "muonIsPFMuonPat"                           );
    if( !selCut.keepIf("isPf"             , IsPf.at(mu)                 ) && quick ) continue;
    cutList+=" "+"isPf";

    //Tracking cuts
    vector<df>&    GT_nChi2     = tree->Get( &GT_nChi2,     "muonGlobalTracknormalizedChi2Pat"              );
    if( !selCut.keepIf("GTnormChi<10"     , GT_nChi2.at(mu)        <  10    ) && quick ) continue;
    cutList+=" "+"GTnormChi<10";

    vector<ui>&    GT_nValOHits = tree->Get( &GT_nValOHits, "muonGlobalTracknumberOfValidMuonHitsPat"   );
    if( !selCut.keepIf("GTouterhits>0"    , GT_nValOHits.at(mu)    >   0    ) && quick ) continue;
    cutList+=" "+"GTouterhits>0";

    vector<unsigned int>&    GT_nMatchedStat = tree->Get( &GT_nMatchedStat, "muonNumberOfMatchedStationsPat"   );
    if( !selCut.keepIf("GTnMatchedStat>1"    , GT_nMatchedStat.at(mu)    >   1    ) && quick ) continue;
    cutList+=" "+"GTnMatchedStat>1";

    //This is a tighter cut that given by the POG. Assume this is 0.2mm, not 2mm
    vector<df>&    GT_Dxy       = tree->Get( &GT_Dxy,       "muonGlobalTrackDxyPat"                         );
    if( !selCut.keepIf("GTdxy<.02"        , fabs(GT_Dxy.at(mu))    <    .02 ) && quick ) continue;
    cutList+=" "+"GTdxy<.02";

    //Using the 5mm recommended, but other groups seem to use looser cuts, e.g. 1cm
    vector<df>&    GT_Dz        = tree->Get( &GT_Dz,        "muonGlobalTrackDzPat"                          );
    if( !selCut.keepIf("GTdz<0.5"           , fabs(GT_Dz.at(mu))     <   0.5  ) && quick ) continue;
    cutList+=" "+"GTdz<0.5";

    vector<unsigned int>& GT_nPixelHits = tree->Get( &GT_nPixelHits, "muonNumberOfValidPixelHitsPat"   );
    if( !selCut.keepIf("GTnPixelHits>0"           , GT_nPixelHits.at(mu)     >   0  ) && quick ) continue;
    cutList+=" "+"GTnPixelHits>0";

    vector<unsigned int>& GT_nTrkLayers = tree->Get( &GT_nTrkLayers, "muonNumberOfTrackerLayersWithMeasurementPat"   );
    if( !selCut.keepIf("GTnTrkLayers>5"           , GT_nTrkLayers.at(mu)     >   5  ) && quick ) continue;
    cutList+=" "+"GTnTrkLayers>5";


    //cutList=ptCut+" "+etaCut+" isGlobal isTracker GTnormChi<10 GTtrackerhits>=11 GTdxy<.02 GTdz<1 EcalVetoDep<4 HcalVetoDep<6 RelIso<.15 GTouterhits>0 PtDiffToNextPFmuon<1"; //Old list
    if( quick || selCut.applyCuts("RA6 muon selection", cutList) ) selMu.push_back(mu);


  }// muon loop

  //cout<<"mu end"<<endl;

}//main

