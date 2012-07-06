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

  static float mu_pt_min_low   = config.getFloat("mu_pt_min_low",  10);
  //static float mu_pt_min_high  = config.getFloat("mu_pt_min_high", 20);
  static float mu_eta_max      = config.getDouble("mu_eta_max",   2.4);
  static bool  quick           = config.getBool("quick"      , false );

  vector<LorentzM>& Muons = tree->Get(&Muons, "muonP4Pat");

  for( unsigned mu=0; mu<Muons.size(); ++mu ) {

    vector<float>& combRelPFIso = tree->Get( &combRelPFIso , "muonPfIsolationR04DeltaBCorrectedPat");
    if( !selCut.keepIf("combRelPFIso<.12" , combRelPFIso.at(mu)    <   .12          ) && quick ) continue;

    TString ptCut ="pt>";
    ptCut+=mu_pt_min_low;
    if( !selCut.keepIf(ptCut           , Muons.at(mu).pt()         >  mu_pt_min_low ) && quick ) continue;

    TString etaCut ="|eta|<";
    etaCut+=mu_eta_max;
    if( !selCut.keepIf(etaCut          , fabs(Muons.at(mu).eta())  <  mu_eta_max    ) && quick ) continue;

    vector<int>&   IsGlobal     = tree->Get( &IsGlobal,     "muonIsGlobalMuonPat"                           );
    if( !selCut.keepIf("isGlobal"         , IsGlobal.at(mu)                 ) && quick ) continue;
    vector<int>&   IsPF         = tree->Get( &IsPF,         "muonIsPFMuonPat"                               );
    if( !selCut.keepIf("isPF"             , IsPF.at(mu)                     ) && quick ) continue;

//old version of this cut:
//     vector<LOR>&  PFMuons = tree->Get(&PFMuons, "muonP4PF");
//     int closestPFmuon=-1;
//     double deltaRtoClosestPFmuon=1000.;
//     for( unsigned pfmu=0; pfmu<PFMuons.size(); ++pfmu ) {
//       double locDR = DeltaR(Muons.at(mu),PFMuons.at(pfmu));
//       if( locDR < deltaRtoClosestPFmuon ) {
// 	closestPFmuon         = pfmu;
// 	deltaRtoClosestPFmuon = locDR;
//       }
//     }

//     if( !selCut.keepIf("PtDiffToNextPFmuon<1", closestPFmuon>=0 && fabs(Muons.at(mu).pt()-PFMuons.at(closestPFmuon).pt())<1 ) && quick ) continue;


    vector<float>&   GT_nChi2     = tree->Get( &GT_nChi2,     "muonGlobalTracknormalizedChi2Pat"           );
    if( !selCut.keepIf("GTnormChi<10"          , GT_nChi2.at(mu)     <  10.   ) && quick ) continue;

    vector<unsigned>& GT_nValMHits = tree->Get( &GT_nValMHits, "muonGlobalTracknumberOfValidMuonHitsPat"    );
    if( !selCut.keepIf("GTvalMuonHits>0"       , GT_nValMHits.at(mu) >   0    ) && quick ) continue;

    vector<unsigned>& nMatchedSt   = tree->Get( &nMatchedSt,   "muonNumberOfMatchedStationsPat"             );
    if( !selCut.keepIf("nMatchedStations>1"  , nMatchedSt.at(mu)     >   1    ) && quick ) continue;

    vector<float>&   IT_dxy       = tree->Get( &IT_dxy,       "muonInnerTrackDxyPat"                       );
    if( !selCut.keepIf("ITdxy<.2"              , fabs(IT_dxy.at(mu)) <    .2  ) && quick ) continue;

    vector<float>&   IT_dz        = tree->Get( &IT_dz ,       "muonInnerTrackDzPat"                        );
    if( !selCut.keepIf("ITdz<.5"               , fabs(IT_dz.at(mu))  <    .5  ) && quick ) continue;

    vector<unsigned>& IT_nPixHits  = tree->Get( &IT_nPixHits,  "muonNumberOfValidPixelHitsPat"              );
    if( !selCut.keepIf("ITnPixHits>0"          , IT_nPixHits.at(mu)  >   0    ) && quick ) continue;

    vector<unsigned>& IT_trLayMeas = tree->Get( &IT_trLayMeas, "muonNumberOfTrackerLayersWithMeasurementPat");
    if( !selCut.keepIf("ITtrLayWithMeas>5"     , IT_trLayMeas.at(mu) >   5    ) && quick ) continue;


    if( quick || selCut.applyCuts("RA6 muon selection", ptCut+" "+etaCut+" combRelPFIso<.12 isGlobal isPF GTnormChi<10 GTvalMuonHits>0 nMatchedStations>1 ITdxy<.2 ITdz<.5 ITnPixHits>0 ITtrLayWithMeas>5") )
      selMu.push_back(mu);


  }// muon loop

  //cout<<"mu end"<<endl;

}//main

