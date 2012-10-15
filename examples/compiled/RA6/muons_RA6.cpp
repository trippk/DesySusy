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

  static float mu_pt_min_low   = config.getFloat("mu_pt_min_low",  10.  );
  static float mu_eta_max      = config.getFloat("mu_eta_max"   ,   2.4 );
  static bool  quick           = config.getBool ("quick"        , false );

  bool isOneProbl=false;
  int      run   = tree->Get(run  , "run"        );
  unsigned event = tree->Get(event, "event"      );

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

  vector<LorentzM>& Muons = tree->Get(&Muons, "muonP4Pat");

  if(isOneProbl) cout<< run <<":"<<event<<":  nMu="<<Muons.size()<<endl;

  for( unsigned mu=0; mu<Muons.size(); ++mu ) {

    vector<float>& combRelPFIso = tree->Get( &combRelPFIso , "DESYmuonPfIsolationR03DeltaBCorrectedPat");
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  relIso="<<combRelPFIso.at(mu)<<endl;
    if( !selCut.keepIf("combRelPFIso<=.15" , combRelPFIso.at(mu)    <=   .15          ) && quick ) continue;

    TString ptCut ="pt>=";
    ptCut+=mu_pt_min_low;
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  Muons.at(mu).pt()="<<Muons.at(mu).pt()<<endl;
    if( !selCut.keepIf(ptCut           , Muons.at(mu).pt()         >=  mu_pt_min_low ) && quick ) continue;

    TString etaCut ="|eta|<=";
    etaCut+=mu_eta_max;
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  Muons.at(mu).eta()="<<Muons.at(mu).eta()<<endl;
    if( !selCut.keepIf(etaCut          , fabs(Muons.at(mu).eta())  <=  mu_eta_max    ) && quick ) continue;

    vector<int>&   IsGlobal     = tree->Get( &IsGlobal,     "muonIsGlobalMuonPat"                           );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  IsGlobal.at(mu)="<<IsGlobal.at(mu)<<endl;
    if( !selCut.keepIf("isGlobal"         , IsGlobal.at(mu)                 ) && quick ) continue;
    vector<int>&   IsPF         = tree->Get( &IsPF,         "muonIsPFMuonPat"                               );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  IsPF.at(mu)="<<IsPF.at(mu)<<endl;
    if( !selCut.keepIf("isPF"             , IsPF.at(mu)                     ) && quick ) continue;
    vector<int>&   IsTracker    = tree->Get( &IsTracker,    "muonIsTrackerMuonPat"                          );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  IsTracker.at(mu)="<<IsTracker.at(mu)<<endl;
    if( !selCut.keepIf("isTracker"        , IsTracker.at(mu)                ) && quick ) continue;

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
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  GT_nChi2.at(mu)="<<GT_nChi2.at(mu)<<endl;
     if( !selCut.keepIf("GTnormChi<10"          , GT_nChi2.at(mu)     <  10.   ) && quick ) continue;

     vector<unsigned>& GT_nValMHits = tree->Get( &GT_nValMHits, "muonGlobalTracknumberOfValidMuonHitsPat"    );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  GT_nValMHits.at(mu)="<<GT_nValMHits.at(mu)<<endl;
     if( !selCut.keepIf("GTvalMuonHits>0"       , GT_nValMHits.at(mu) >   0    ) && quick ) continue;

     vector<unsigned>& nMatchedSt   = tree->Get( &nMatchedSt,   "muonNumberOfMatchedStationsPat"             );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  nMatchedSt.at(mu)="<<nMatchedSt.at(mu)<<endl;
     if( !selCut.keepIf("nMatchedStations>1"  , nMatchedSt.at(mu)     >   1    ) && quick ) continue;

    vector<float>&   IT_dxy       = tree->Get( &IT_dxy,       "muonInnerTrackDxyPat"                       );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  IT_dxy.at(mu)="<<IT_dxy.at(mu)<<endl;
    if( !selCut.keepIf("ITdxy<=.02"             , fabs(IT_dxy.at(mu)) <=    .02 ) && quick ) continue;

    vector<float>&   IT_dz        = tree->Get( &IT_dz ,       "muonInnerTrackDzPat"                        );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  IT_dz.at(mu)="<<IT_dz.at(mu)<<endl;
    if( !selCut.keepIf("ITdz<=.1"               , fabs(IT_dz.at(mu))  <=    .1  ) && quick ) continue;
//      vector<float>&   GT_dxy       = tree->Get( &GT_dxy,       "muonGlobalTrackDxyPat"                       );
//      if( !selCut.keepIf("GTdxy<=.02"             , fabs(GT_dxy.at(mu)) <=    .02 ) && quick ) continue;
     
//      vector<float>&   GT_dz        = tree->Get( &GT_dz ,       "muonGlobalTrackDzPat"                        );
//      if( !selCut.keepIf("GTdz<=.1"               , fabs(GT_dz.at(mu))  <=    .1  ) && quick ) continue;

     vector<unsigned>& IT_nPixHits  = tree->Get( &IT_nPixHits,  "muonNumberOfValidPixelHitsPat"              );
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  IT_nPixHits.at(mu)="<<IT_nPixHits.at(mu)<<endl;
     if( !selCut.keepIf("ITnPixHits>0"          , IT_nPixHits.at(mu)  >   0    ) && quick ) continue;
     // is taken from inner track.

     vector<unsigned>& IT_trLayMeas = tree->Get( &IT_trLayMeas, "muonNumberOfTrackerLayersWithMeasurementPat");
    if(isOneProbl) cout<< run <<":"<<event<<":  mu "<<mu<<":  IT_trLayMeas.at(mu)="<<IT_trLayMeas.at(mu)<<endl;
     if( !selCut.keepIf("ITtrLayWithMeas>5"     , IT_trLayMeas.at(mu) >   5    ) && quick ) continue;


    if( quick || selCut.applyCuts("RA6 muon selection", ptCut+" "+etaCut+" combRelPFIso<=.15 isGlobal isPF isTracker GTnormChi<10 GTvalMuonHits>0 nMatchedStations>1 ITdxy<=.02 ITdz<=.1 ITnPixHits>0 ITtrLayWithMeas>5") )
      selMu.push_back(mu);


  }// muon loop

  //cout<<"mu end"<<endl;

}//main

