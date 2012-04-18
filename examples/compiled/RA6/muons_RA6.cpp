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
    TString etaCut ="|eta|<";
    etaCut+=mu_eta_max;
    if( !selCut.keepIf(etaCut          , fabs(Muons.at(mu).eta())  <  mu_eta_max    ) && quick ) continue;
    TString ptCut ="pt>";
    ptCut+=mu_pt_min_low;
    if( !selCut.keepIf(ptCut           , Muons.at(mu).pt()         >  mu_pt_min_low ) && quick ) continue;

    vector<float>& ecalIso      = tree->Get( &ecalIso,      "muonEcalIsoPat"                                );
    vector<float>& hcalIso      = tree->Get( &hcalIso,      "muonHcalIsoPat"                                );
    vector<float>& trackIso     = tree->Get( &trackIso,     "muonTrackIsoPat"                               );
    double RelIso_max20 = (ecalIso.at(mu) + hcalIso.at(mu) + trackIso.at(mu))/max((float)20.,(float)Muons.at(mu).Pt());
    if( !selCut.keepIf("RelIso<.15"       , RelIso_max20           <    .15 ) && quick ) continue;

    vector<int>&   IsGlobal     = tree->Get( &IsGlobal,     "muonIsGlobalMuonPat"                           );
    if( !selCut.keepIf("isGlobal"         , IsGlobal.at(mu)                 ) && quick ) continue;
    vector<int>&   IsTracker    = tree->Get( &IsTracker,    "muonIsTrackerMuonPat"                          );
    if( !selCut.keepIf("isTracker"        , IsTracker.at(mu)                ) && quick ) continue;

    vector<df>&    GT_nChi2     = tree->Get( &GT_nChi2,     "muonGlobalTracknormalizedChi2Pat"              );
    if( !selCut.keepIf("GTnormChi<10"     , GT_nChi2.at(mu)        <  10    ) && quick ) continue;
    vector<ui>&    GT_nValTHits = tree->Get( &GT_nValTHits, "DESYmuonGlobalTrackNumberOfValidTrackerHitsPat");
    if( !selCut.keepIf("GTtrackerhits>=11", GT_nValTHits.at(mu)    >= 11    ) && quick ) continue;
    vector<ui>&    GT_nValOHits = tree->Get( &GT_nValOHits, "DESYmuonGlobalTrackNumberOfValidMuonHitsPat"   );
    if( !selCut.keepIf("GTouterhits>0"    , GT_nValOHits.at(mu)    >   0    ) && quick ) continue;

    vector<df>&    GT_Dxy       = tree->Get( &GT_Dxy,       "muonGlobalTrackDxyPat"                         );
    if( !selCut.keepIf("GTdxy<.02"        , fabs(GT_Dxy.at(mu))    <    .02 ) && quick ) continue;
    vector<df>&    GT_Dz        = tree->Get( &GT_Dz,        "muonGlobalTrackDzPat"                          );
    if( !selCut.keepIf("GTdz<1"           , fabs(GT_Dz.at(mu))     <   1.0  ) && quick ) continue;

    vector<df>&    EcalVetoDep  = tree->Get( &EcalVetoDep,  "muonEcalVetoDepPat"                            );
    if( !selCut.keepIf("EcalVetoDep<4"    , EcalVetoDep.at(mu)     <   4    ) && quick ) continue;
    vector<df>&    HcalVetoDep  = tree->Get( &HcalVetoDep,  "muonHcalVetoDepPat"                            );
    if( !selCut.keepIf("HcalVetoDep<6"    , HcalVetoDep.at(mu)     <   6    ) && quick ) continue;

    vector<LOR>&  PFMuons = tree->Get(&PFMuons, "muonP4PF");
    int closestPFmuon=-1;
    double deltaRtoClosestPFmuon=1000.;
    for( unsigned pfmu=0; pfmu<PFMuons.size(); ++pfmu ) {
      double locDR = DeltaR(Muons.at(mu),PFMuons.at(pfmu));
      if( locDR < deltaRtoClosestPFmuon ) {
	closestPFmuon         = pfmu;
	deltaRtoClosestPFmuon = locDR;
      }
    }

    if( !selCut.keepIf("PtDiffToNextPFmuon<1", closestPFmuon>=0 && fabs(Muons.at(mu).pt()-PFMuons.at(closestPFmuon).pt())<1 ) && quick ) continue;


    if( quick || selCut.applyCuts("RA6 muon selection", ptCut+" "+etaCut+" isGlobal isTracker GTnormChi<10 GTtrackerhits>=11 GTdxy<.02 GTdz<1 EcalVetoDep<4 HcalVetoDep<6 RelIso<.15 GTouterhits>0 PtDiffToNextPFmuon<1") )
      selMu.push_back(mu);

    /*missing cuts
from AN2011-464_v3:
- Relative transverse momentum error of silicon track used for muon fit is delta(pT )/pT < 0.1
- The muon is required to be a PF muon whose pT is no more than 1 GeV different than the reco muon (to ensure consistency with PF MET)

    */

  }// muon loop

  //cout<<"mu end"<<endl;

}//main

