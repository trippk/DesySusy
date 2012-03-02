#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"
#include "Math/VectorUtil.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

bool crossCleaning_RA6(EasyChain* tree, 
		       vector<unsigned>& selMu , CutSet& muCut ,
		       vector<unsigned>& selEl , CutSet& elCut ,
		       vector<unsigned>& selJet, CutSet& jetCut ) {

  ConfigReader config;
  static bool  quick           = config.getBool( "quick"      , false );
  static bool  isOldNtuple     = config.getBool( "isOldNtuple", false );

  typedef LorentzM LOR;
  //if(isOldNtuple) 
  //typedef LorentzV LOR;
  
  vector<LOR>& Muons        = tree->Get(&Muons       ,"muonP4Pat"           );
  vector<int>& Mu_IsGlobal  = tree->Get(&Mu_IsGlobal ,"muonIsGlobalMuonPat" );
  vector<int>& Mu_IsTracker = tree->Get(&Mu_IsTracker,"muonIsTrackerMuonPat");

  vector<LOR>& Electrons    = tree->Get(&Electrons   ,"electronP4Pat"       );

  vector<unsigned> new_selEl;
  for(size_t e=0; e < selEl.size(); ++e ) {
    unsigned el = selEl.at(e);
    double DeltaRtoNextMuon = 100.;
    for( size_t mu=0; mu < Muons.size(); ++mu ) {
      if( Mu_IsTracker.at(mu) || Mu_IsGlobal.at(mu) ) {
	double dr= DeltaR( Electrons.at(el), Muons.at(mu) );
	if(dr<DeltaRtoNextMuon) DeltaRtoNextMuon = dr;
      }
    }
    if(elCut.keepIf("DeltaRtoNextMu>0.1", DeltaRtoNextMuon > .1 ) )
      new_selEl.push_back(el);
  }
  selEl = new_selEl;
  
  vector<LOR>& JetsPF = tree->Get(&JetsPF, "ak5JetPF2PATCorrectedP4Pat");

  vector<unsigned> new_selJet;
  for( unsigned j=0; j<selJet.size(); ++j) {
    unsigned jet = selJet.at(j);
    double DeltaRtoNextEl = 100;
    for( size_t e=0; e < selEl.size(); ++e ) {
      unsigned el = selEl.at(e);
      double dr= DeltaR( JetsPF.at(jet), Electrons.at(el) );
      if(dr<DeltaRtoNextEl) DeltaRtoNextEl = dr;
    }
    double DeltaRtoNextMu = 100;
    for( size_t m=0; m < selMu.size(); ++m ) {
      unsigned mu = selMu.at(m);
      double dr= DeltaR( JetsPF.at(jet), Muons.at(mu) );
      if(dr<DeltaRtoNextMu) DeltaRtoNextMu = dr;
    }

    if( !jetCut.keepIf("DeltaRtoNextEl>0.4", DeltaRtoNextEl >=   .4 ) && quick)
      continue;
    if( !jetCut.keepIf("DeltaRtoNextMu>0.4", DeltaRtoNextMu >=   .4 ) && quick)
      continue;
    if( quick || jetCut.applyCuts("jet cross cleaning against leptons", "DeltaRtoNextEl>0.4 DeltaRtoNextMu>0.4" ) )
      new_selJet.push_back(jet);
  }

  selJet = new_selJet;
}
