#include <map>
#include <string>
#include <iostream>
#include "TSystem.h"
#include "TH1F.h"
#include "TFile.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "plotSet.h"

#include "muons_RA6.h"
#include "electrons_RA6.h"
#include "jetsPF_RA6.h"
#include "crossCleaning_RA6.h"
#include "vertices_RA6.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

int main(int argc, char** argv){


  ConfigReader config("config.txt",argc,argv);

  // input file name
  TString filename = config.getTString("filename","");

  // the main tree
  EasyChain* tree = new EasyChain("/susyTree/tree");
  tree->AddSmart(filename);

  int N = tree->GetEntries();
  cout<<endl<<N<<" events in "<<filename<<endl<<endl;

  // output directory and output file name(derived from input name)
  TString outdir   = config.getTString("outdir","./");
  TString outname  = config.getTString("outname",tree->GetUniqeName());

  TFile *outfile = TFile::Open(outname,"RECREATE");
  outfile->cd();

  CutSet::setTFile(outfile);
  static bool isData      = config.getBool( "isData",      false );
  static bool quick       = config.getBool( "quick",       false );
  static bool isOldNtuple = config.getBool( "isOldNtuple", false );

  static bool allowPrescale = config.getBool( "allowPrescale", false );

  static float     HT_min = config.getFloat(      "HT_min",  0. );
  static float    MET_min = config.getFloat(     "MET_min",  0. );

  typedef LorentzM LOR;

  CutSet globalFlow("global flow");

  CutSet RA6_Mu_selectionCuts("RA6: Muon Selection");
  CutSet RA6_El_selectionCuts("RA6: Electron Selection");
  CutSet RA6_JetPF_selectionCuts("RA6: Jet Selection");
  CutSet RA6_Vx_selectionCuts("RA6: Vertex Selection");

  plotSet ps("control_plots",true);
  plotSet plotsId("RA6",true);
  ps.setTFile(outfile);
  plotsId.setTFile(outfile);

  //Pile-up handling
  vector<float> PUweights;
  static TString PU_weightList = config.getTString( "PU_weightList", " " );

  int PU_weightListSIZE=0;
  if( PU_weightList != " " ) {
    PU_weightListSIZE = config.ListSize("PU_weightList");
    for(int i=0; i<PU_weightListSIZE; ++i)
      PUweights.push_back( atof( config.getString( "PU_weightList" , i).c_str() ) );
  }

  static bool TotKinFilt = config.getBool("TotKinFilt", true);

  vector<int> runsWithTriggerProb, runsWithAltTriggerProb, runsWithAltAltTriggerProb, runsWithAltAltAltTriggerProb;

  //N=10000;

  bool OK=false;
  for(int i=0;i<N;++i){

    //cout<<"looper started"<<endl;

    timer();

    bool isMuMu=false;
    bool isMuEl=false;
    bool isElEl=false;
    bool isRA6highPtMuMu = false;
    bool isRA6highPtMuEl = false;
    bool isRA6highPtElEl = false;
    vector<unsigned> RA6_selectedEl;
    vector<unsigned> RA6_selectedMu;
    vector<unsigned> RA6_selectedJetPF;
    vector<unsigned> RA6_selectedVx;

    tree->GetEntry(i);


    /////////
    // PU
    /////////

    if(!isData) {

      vector<int>& PUnumInter      = tree->Get( &PUnumInter      , "DESYPUinfoNumInteractions");
      vector<int>& PUbunchCrossing = tree->Get( &PUbunchCrossing , "DESYPUinfoBunchCrossing"  );

      int relevantNumPU=-1;
      int sumNumInter=0;
      for( size_t bc=0; bc<PUnumInter.size(); ++bc ) {
	TString plna = "pu";
	plna        += PUbunchCrossing.at(bc);
	ps.addPlot_withUnweighted( plna ,36,-0.5,35.5, PUnumInter.at(bc));
	if( PUbunchCrossing.at(bc) == 0 )
	  relevantNumPU = PUnumInter.at(bc);
      }

      ps.addPlot_withUnweighted( "relevantNumPU", 36,-0.5,35.5, relevantNumPU );

      if( PU_weightList == " " ) continue;
      if( relevantNumPU >= PU_weightListSIZE ) {
	plotSet::global_event_weight = 0;
	CutSet::global_event_weight  = 0;
      } else {
	plotSet::global_event_weight = PUweights.at( relevantNumPU );
	CutSet::global_event_weight  = PUweights.at( relevantNumPU );
      }
      //ps.addPlot_withUnweighted( "PU__after_weighting" ,26,-0.5,25.5, relevantNumPU );
      ps.addPlot_withUnweighted( "PU__after_weighting" ,36,-0.5,35.5, relevantNumPU );
    }

    //cout<<"PU stuff survived."<<endl;


    //event cleaning------------------------------------------------------------------------------
    if(!isData && TotKinFilt) {
      bool tkfPassed = tree->Get( tkfPassed, "DESYtotalKinematicsFilterPassed" );
      if( !globalFlow.keepIf("totalKinFilter", tkfPassed ) && quick ) continue;
    } else globalFlow.keepIf("totalKinFilter", true );
    
    bool     HBHEnoiseFilterResult = tree->Get( HBHEnoiseFilterResult, "hbheNoiseFilterResult" );
    if( !globalFlow.keepIf( "HBHEnoiseFilter"        , HBHEnoiseFilterResult        ) && quick ) continue;
    //cout<<"hbhe survived."<<endl;
    vertices_RA6 ( tree, RA6_selectedVx   , RA6_Vx_selectionCuts    );
    if( !globalFlow.keepIf( "at_least_1_vertex"      , RA6_selectedVx.size()    >=1 ) && quick ) continue;
    //cout<<"vertex survived."<<endl;
    int Ntracks_lt0p9    = tree->Get( Ntracks_lt0p9   , "tracksNEtaLT0p9AllTracks"    );
    int Ntracks_0p9to1p5 = tree->Get( Ntracks_0p9to1p5, "tracksNEta0p9to1p5AllTracks" );
    int Ntracks_gt1p5    = tree->Get( Ntracks_gt1p5   , "tracksNEtaGT1p5AllTracks"    );
    
    int NHPtracks_lt0p9    = tree->Get( NHPtracks_lt0p9   , "tracksNEtaLT0p9HighPurityTracks"    );
    int NHPtracks_0p9to1p5 = tree->Get( NHPtracks_0p9to1p5, "tracksNEta0p9to1p5HighPurityTracks" );
    int NHPtracks_gt1p5    = tree->Get( NHPtracks_gt1p5   , "tracksNEtaGT1p5HighPurityTracks"    );

    int Ntracks   = Ntracks_lt0p9 + Ntracks_0p9to1p5 + Ntracks_gt1p5;
    int NHPtracks = NHPtracks_lt0p9 + NHPtracks_0p9to1p5 + NHPtracks_gt1p5;

    if( !globalFlow.keepIf( "FracOfHPtracks", Ntracks <=10 ? true : (float) NHPtracks/Ntracks >=0.25 ) && quick ) continue;

    //cout<<"fracTrack survived"<<endl;

    //csc beam halo
    //ecal beam halo

    bool tffPassed  = tree->Get( tffPassed , "trackingfailurefilterflag" );
    if( !globalFlow.keepIf("trackFailFilter"   ,  tffPassed ) && quick ) continue;
    bool edcfPassed = tree->Get( edcfPassed, "ecaldeadcellfilterflag"    );
    if( !globalFlow.keepIf("ecalDeadCellFilter", edcfPassed ) && quick ) continue;
    bool bhctFailed = tree->Get( bhctFailed, "beamHaloCSCTightHaloId"    );
    if( !globalFlow.keepIf("beamHaloCSCFilter" ,!bhctFailed ) && quick ) continue;//   stimmt das?? Dass die Flag das Failed angibt?

    //cout<<"filter survived"<<endl;

    //trigger-------------------------------------------------------------------------------------
    int run = tree->Get(run, "run");
    map<string,bool  >& triggered      = tree->Get( &triggered     , "triggered"          );
    map<string,int   >& prescaled      = tree->Get( &prescaled     , "prescaled"          );
    map<string,string>& triggerNameMap = tree->Get( &triggerNameMap, "DESYtriggerNameMap" );

    TString          TrigName = "Trigger";
    TString       AltTrigName = "Trigger";
    TString    AltAltTrigName = "Trigger";
    TString AltAltAltTrigName = "Trigger";   
    bool oneAltTriggerGood  = false;

    static TString trigList = config.getTString( "trigList", " " );
    static int     trigSize = config.ListSize(   "trigList"      );

    if( trigList != " " ) {
      bool oneTriggerFound = false;
      for(int i=0; i<trigSize && !oneTriggerFound; ++i) {
	string trigName = config.getString( "trigList" , i);
	string trig = triggerNameMap[trigName];

	if( triggered.find( trig ) != triggered.end() ) {
	  if( isData && !allowPrescale && prescaled[trig] > 1 ) continue;
	  oneTriggerFound = true;
	  TrigName+=trig;
	  if( globalFlow.keepIf(        TrigName, triggered[ trig ] ) ) {
	    oneAltTriggerGood = true;
	    if( isData ) {
	      plotSet::global_event_weight = prescaled[trig];
	      CutSet::global_event_weight  = prescaled[trig];
	    }
	  }
	} 
      }
      if( !oneTriggerFound ) {
	if(find( runsWithTriggerProb.begin(), runsWithTriggerProb.end(), run ) == runsWithTriggerProb.end()) {
	  cout<<"In run "<<run<<" none of these triggers found: "<<trigList<<endl;
	  runsWithTriggerProb.push_back(run);
	}
	ps.addPlot("0TriggerNotFound",20000, 160000., 180000., run);
	continue;
      }
    }

    static TString trigAltList = config.getTString( "trigAltList", " " );
    static int     trigAltSize = config.ListSize(   "trigAltList"      );

    string trigAlt, trigAltAlt;


    if( trigAltList != " " /*&& !trigFound */) {
      bool oneTriggerFound = false;
      for(int i=0; i<trigAltSize && !oneTriggerFound; ++i) {
	string trigAltName = config.getString( "trigAltList" , i);
	trigAlt = triggerNameMap[trigAltName];
	if( triggered.find( trigAlt ) != triggered.end() ) {
	  if( isData && !allowPrescale && prescaled[trigAlt] > 1 ) continue;
	  oneTriggerFound = true;

	} 
      }
      if( !oneTriggerFound ) {
	if(find( runsWithAltTriggerProb.begin(), runsWithAltTriggerProb.end(), run ) == runsWithAltTriggerProb.end()) {
	  cout<<"In run "<<run<<" none of these triggers found: "<<trigAltList<<endl;
	  runsWithAltTriggerProb.push_back(run);
	}
	ps.addPlot("0TriggerNotFound",20000, 160000., 180000., run);
	continue;
      }
    }


    static TString trigAltAltList = config.getTString( "trigAltAltList", " " );
    static int     trigAltAltSize = config.ListSize(   "trigAltAltList"      );
    if( trigAltAltList != " "/* && !trigFound */) {
      bool oneTriggerFound = false;
      for(int i=0; i<trigAltAltSize && !oneTriggerFound; ++i) {
	string trigAltAltName = config.getString( "trigAltAltList" , i);
	trigAltAlt = triggerNameMap[trigAltAltName];
	if( triggered.find( trigAltAlt ) != triggered.end() ) {
	  if( isData && !allowPrescale && prescaled[trigAltAlt] > 1 ) continue;
	  oneTriggerFound = true;
	} 
      }
      if( !oneTriggerFound ) {
	if(find( runsWithAltAltTriggerProb.begin(), runsWithAltAltTriggerProb.end(), run ) == runsWithAltAltTriggerProb.end()) {
	  cout<<"In run "<<run<<" none of these triggers found: "<<trigAltAltList<<endl;
	  runsWithAltAltTriggerProb.push_back(run);
	}
	ps.addPlot("0TriggerNotFound",20000, 160000., 180000., run);
	continue;
      }
    }

    //MuEl and ElMu trigger combination:
    AltTrigName += trigAlt;
    AltTrigName += "_or_";
    AltTrigName += trigAltAlt;
    if( globalFlow.keepIf(     AltTrigName, triggered[ trigAlt ] || triggered[ trigAltAlt ]) ) {
      oneAltTriggerGood = true;
      if( isData && prescaled[ trigAlt ] != 1 && prescaled[ trigAltAlt ] != 1) {
	cout<<"!!! WARNING !!! Do not use OR combinations of prescaled triggers!";
	//plotSet::global_event_weight = prescaled[ trigAlt ];
	//CutSet::global_event_weight  = prescaled[ trigAlt ];
      }
    }


    static TString trigAltAltAltList = config.getTString( "trigAltAltAltList", " " );
    static int     trigAltAltAltSize = config.ListSize(   "trigAltAltAltList"      );
    if( trigAltAltAltList != " "/* && !trigFound */) {
      bool oneTriggerFound = false;
      for(int i=0; i<trigAltAltAltSize && !oneTriggerFound; ++i) {
	string trigAltAltAltName = config.getString( "trigAltAltAltList" , i);
	string trigAltAltAlt = triggerNameMap[trigAltAltAltName];
	if( triggered.find( trigAltAltAlt ) != triggered.end() ) {
	  if( isData && !allowPrescale && prescaled[trigAltAltAlt] > 1 ) continue;
	  oneTriggerFound = true;
	  AltAltAltTrigName+=trigAltAltAlt;
	  if( globalFlow.keepIf(  AltAltAltTrigName, triggered[ trigAltAltAlt ] ) ) {
	    oneAltTriggerGood = true;
	    if( isData ) {
	      plotSet::global_event_weight = prescaled[trigAltAltAlt];
	      CutSet::global_event_weight  = prescaled[trigAltAltAlt];
	    }
	  }
	} 
      }
      if( !oneTriggerFound ) {
	if(find( runsWithAltAltAltTriggerProb.begin(), runsWithAltAltAltTriggerProb.end(), run ) == runsWithAltAltAltTriggerProb.end()) {
	  cout<<"In run "<<run<<" none of these triggers found: "<<trigAltAltAltList<<endl;
	  runsWithAltAltAltTriggerProb.push_back(run);
	}
	ps.addPlot("0TriggerNotFound",20000, 160000., 180000., run);
	continue;
      }
    }

    if( quick && !oneAltTriggerGood ) continue;

    //cout<<"Trigger stuff survived."<<endl;

    /////////
    // object definition and selection:
    /////////

    muons_RA6    ( tree, RA6_selectedMu, RA6_Mu_selectionCuts );
    electrons_RA6( tree, RA6_selectedEl, RA6_El_selectionCuts );

    if(  globalFlow.keepIf( "at_least_2_muons"    , RA6_selectedMu.size()    >=2 ) ) isMuMu=true;
    if(  globalFlow.keepIf( "at_least_1_muon_and_1_electron",  
			                	    RA6_selectedMu.size()    >=1 &&
				                    RA6_selectedEl.size()    >=1 ) ) isMuEl=true;
    if(  globalFlow.keepIf( "at_least_2_electrons", RA6_selectedEl.size()    >=2 ) ) isElEl=true;

    if( !isMuMu && !isMuEl && !isElEl && quick ) continue;

    jetsPF_RA6   ( tree, RA6_selectedJetPF, RA6_JetPF_selectionCuts );

    ps.addPlot("JetMult", "JetMult(before cut)", 15, 0., 15., RA6_selectedJetPF.size(), "multiplicity of selected jets", "events");
    if(      !globalFlow.keepIf( "at_least_2_jets"     , RA6_selectedJetPF.size() >=2 ) && quick ) continue;

    vector<LOR>& JetsPF    = tree->Get( &JetsPF   , "ak5JetPF2PATCorrectedP4Pat");
    vector<LOR>& Electrons = tree->Get( &Electrons, "electronP4Pat"             );
    vector<LOR>& Muons     = tree->Get( &Muons    , "muonP4Pat"                 );

    for(int j=0; j<RA6_selectedJetPF.size(); ++j) {
      for(int m=0; m<RA6_selectedMu.size(); ++m) {
	ps.addPlot("dR_Jet_Mu", 50, 0., 5., 
		   DeltaR( JetsPF.at(RA6_selectedJetPF.at(j)), Muons.at(RA6_selectedMu.at(m)) ) );
      }
      for(int e=0; e<RA6_selectedEl.size(); ++e) {
	ps.addPlot("dR_Jet_El", 50, 0., 5., 
		   DeltaR( JetsPF.at(RA6_selectedJetPF.at(j)), Electrons.at(RA6_selectedEl.at(e)) ) );
      }
    }

    crossCleaning_RA6( tree,
		       RA6_selectedMu   , RA6_Mu_selectionCuts,
		       RA6_selectedEl   , RA6_El_selectionCuts,
		       RA6_selectedJetPF, RA6_JetPF_selectionCuts );
    //cout<<"cleaning survived."<<endl;

    for(int j=0; j<RA6_selectedJetPF.size(); ++j) {
      for(int m=0; m<RA6_selectedMu.size(); ++m) {
	ps.addPlot("dR_Jet_Mu_afterCC", 50, 0., 5., 
		   DeltaR( JetsPF.at(RA6_selectedJetPF.at(j)), Muons.at(RA6_selectedMu.at(m)) ) );
      }
      for(int e=0; e<RA6_selectedEl.size(); ++e) {
	ps.addPlot("dR_Jet_El_afterCC", 50, 0., 5., 
		   DeltaR( JetsPF.at(RA6_selectedJetPF.at(j)), Electrons.at(RA6_selectedEl.at(e)) ) );
      }
    }

    //redo object multiplicity cuts after cross cleaning
    isMuMu=false;
    isMuEl=false;
    isElEl=false;
    if(  globalFlow.keepIf( "at_least_2_muons_CC"    , RA6_selectedMu.size()    >=2 ) ) isMuMu=true;
    if(  globalFlow.keepIf( "at_least_1_muon_and_1_electron_CC",  
		                    	               RA6_selectedMu.size()    >=1 &&
			                               RA6_selectedEl.size()    >=1 ) ) isMuEl=true;
    if(  globalFlow.keepIf( "at_least_2_electrons_CC", RA6_selectedEl.size()    >=2 ) ) isElEl=true;

    if( !isMuMu && !isMuEl && !isElEl && quick ) continue;
    ps.addPlot("JetMult_afterCC", "JetMult(before cut, after CC)", 15, 0., 15., RA6_selectedJetPF.size(), "multiplicity of selected jets", "events");

    if(      !globalFlow.keepIf( "at_least_2_jets_CC"     , RA6_selectedJetPF.size() >=2 ) && quick ) continue;

    //cout<<"mu&elsP2 survived."<<endl;



    /////////////
    // final cuts
    /////////////
    LOR* metP4PF   = tree->Get( &metP4PF , "metP4TypeIPF"  );
    double HT_selectedJetPF=0;

    double minDPhi_MET_Jet = 100;
    for(size_t j=0; j<RA6_selectedJetPF.size(); ++j) {
      HT_selectedJetPF += JetsPF.at(RA6_selectedJetPF.at(j)).pt();
      if( fabs(DeltaPhi(JetsPF.at( RA6_selectedJetPF[j] ), *metP4PF )) < minDPhi_MET_Jet )
	minDPhi_MET_Jet = fabs(DeltaPhi(JetsPF.at( RA6_selectedJetPF[j] ), *metP4PF));
    }

    TString HT_cutString = "HT>=";
    HT_cutString        += HT_min;
    ps.addPlot(HT_cutString, "HT(before cut)", 80, 0., 800., HT_selectedJetPF, "HT(selected jets) [GeV]", "events");
    if( !globalFlow.keepIf( HT_cutString , HT_selectedJetPF > HT_min ) && quick ) continue;

    TString MET_cutString = "MET>=";
    MET_cutString        += MET_min;
    ps.addPlot(MET_cutString, "MET(before cut)", 50, 0., 500., metP4PF->Et(), "MET [GeV]", "events");
    if( !globalFlow.keepIf( MET_cutString, metP4PF->Et()    > MET_min ) && quick ) continue;


    static float lept_pt_min_high = config.getFloat("lept_pt_min_high", 20);
    static float lept_invMass_min = config.getFloat("lept_invMass_min", 10);
    static int   lept_relCharge   = config.getInt  ("lept_relCharge",   -1);


    vector<int>& El_charge = tree->Get( &El_charge, "electronChargePat" );
    double minDPhi_MET_El = 100;
    vector<pair<unsigned, unsigned> > highPtOSElElPairs;
    if( globalFlow.keepIf("at_least_1_ElEl_pair", isElEl) ) {
      for( size_t el1=0; el1<RA6_selectedEl.size()-1; ++el1 ) {
	if( Electrons.at( RA6_selectedEl[el1] ).pt() <= lept_pt_min_high ) continue;

	if( fabs(DeltaPhi(Electrons.at( RA6_selectedEl[el1] ), *metP4PF )) < minDPhi_MET_El )
 	  minDPhi_MET_El = fabs(DeltaPhi(Electrons.at( RA6_selectedEl[el1] ), *metP4PF));
	
	for( size_t el2=el1+1; el2<RA6_selectedEl.size(); ++el2 ) {
	  pair<unsigned, unsigned> tmpPair;
	  tmpPair.first  = RA6_selectedEl[el1];
	  tmpPair.second = RA6_selectedEl[el2];

	  if( El_charge.at(tmpPair.first)*El_charge.at(tmpPair.second) != lept_relCharge ) continue;
	  if( (Electrons.at( RA6_selectedEl[el1] )+Electrons.at( RA6_selectedEl[el2] )).mass() <= lept_invMass_min ) continue;
	  highPtOSElElPairs.push_back(tmpPair);
	}
      }
    }
    if( RA6_selectedEl.size() && fabs(DeltaPhi(Electrons.at( RA6_selectedEl[RA6_selectedEl.size()-1] ), *metP4PF )) < minDPhi_MET_El )
      minDPhi_MET_El = fabs(DeltaPhi(Electrons.at( RA6_selectedEl[RA6_selectedEl.size()-1] ), *metP4PF));

    vector<int>& Mu_charge = tree->Get( &Mu_charge, "muonChargePat" );
    double minDPhi_MET_Mu = 100;
    vector<pair<unsigned, unsigned> > highPtOSMuMuPairs;
    if( globalFlow.keepIf("at_least_1_MuMu_pair", isMuMu) ) {
      for( size_t mu1=0; mu1<RA6_selectedMu.size()-1; ++mu1 ) {
	if( Muons.at( RA6_selectedMu[mu1] ).pt() <= lept_pt_min_high ) continue;

 	if( fabs(DeltaPhi(Muons.at( RA6_selectedMu[mu1] ), *metP4PF )) < minDPhi_MET_Mu )
 	  minDPhi_MET_Mu = fabs(DeltaPhi(Muons.at( RA6_selectedMu[mu1] ), *metP4PF));

	for( size_t mu2=mu1+1; mu2<RA6_selectedMu.size(); ++mu2 ) {
	  pair<unsigned, unsigned> tmpPair;
	  tmpPair.first  = RA6_selectedMu[mu1];
	  tmpPair.second = RA6_selectedMu[mu2];

 	  if( Mu_charge.at(tmpPair.first) * Mu_charge.at( tmpPair.second ) != lept_relCharge ) continue;
 	  if( (Muons.at( RA6_selectedMu[mu1] )+Muons.at( RA6_selectedMu[mu2] )).mass() <= lept_invMass_min ) continue;
	  highPtOSMuMuPairs.push_back(tmpPair);
	}
      }
    }
    if( RA6_selectedMu.size() && fabs(DeltaPhi(Muons.at( RA6_selectedMu[RA6_selectedMu.size()-1] ), *metP4PF )) < minDPhi_MET_Mu )
      minDPhi_MET_Mu = fabs(DeltaPhi(Muons.at( RA6_selectedMu[RA6_selectedMu.size()-1] ), *metP4PF));

    vector<pair<unsigned, unsigned> > highPtOSMuElPairs;
    if( globalFlow.keepIf("at_least_1_MuEl_pair", isMuEl) ) {
      for( size_t mu=0; mu<RA6_selectedMu.size(); ++mu ) {
	for( size_t el=0; el<RA6_selectedEl.size(); ++el ) {
	  if( Muons.    at( RA6_selectedMu[mu] ).pt() <= lept_pt_min_high  &&
	      Electrons.at( RA6_selectedEl[el] ).pt() <= lept_pt_min_high  ) continue;
	  pair<unsigned, unsigned> tmpPair;
	  tmpPair.first  = RA6_selectedMu[mu];
	  tmpPair.second = RA6_selectedEl[el];

	  if( Mu_charge.at(tmpPair.first) * El_charge.at( tmpPair.second ) != lept_relCharge ) continue;
	  highPtOSMuElPairs.push_back(tmpPair);
	}
      }
    }

    if( !globalFlow.keepIf( "highPtOSleptPair", highPtOSElElPairs.size()+highPtOSMuMuPairs.size()+highPtOSMuElPairs.size()>0 ) && quick ) continue;

    globalFlow.keepIf( "highPtOSMuMuPair", highPtOSMuMuPairs.size() > 0 );
    globalFlow.keepIf( "highPtOSMuElPair", highPtOSMuElPairs.size() > 0 );
    globalFlow.keepIf( "highPtOSElElPair", highPtOSElElPairs.size() > 0 );

    //cout<<"directly before final cuts"<<endl;

    if(quick || globalFlow.applyCuts("RA6 common cuts", 
		"totalKinFilter trackFailFilter ecalDeadCellFilter beamHaloCSCFilter HBHEnoiseFilter at_least_1_vertex FracOfHPtracks at_least_2_jets_CC "+HT_cutString+" "+MET_cutString ) ) {
      
      //MuMu:
     if(globalFlow.applyCuts("RA6 MuMu",
			      TrigName+" at_least_1_MuMu_pair highPtOSMuMuPair")) isRA6highPtMuMu=true;

      //MuEl:
      if(globalFlow.applyCuts("RA6 MuEl",
			      AltTrigName+" at_least_1_MuEl_pair highPtOSMuElPair")) isRA6highPtMuEl=true;

      //ElEl:
      if(globalFlow.applyCuts("RA6 ElEl",
			      AltAltAltTrigName+" at_least_1_ElEl_pair highPtOSElElPair")) isRA6highPtElEl=true;
    
}
    TString top="";
    LOR firstLept, secondLept;

    if( isRA6highPtMuMu ) {
      top = "RA6MuMu";
      firstLept  = Muons.at( highPtOSMuMuPairs[0].first  );
      secondLept = Muons.at( highPtOSMuMuPairs[0].second );
      ps.addPlot(HT_cutString+"MuMu", "HT(after MuMu cuts)", 80, 0., 800., HT_selectedJetPF, "HT(selected jets) [GeV]", "events");
    } else if ( isRA6highPtElEl ) {
      top = "RA6ElEl";
      firstLept  = Electrons.at( highPtOSElElPairs[0].first  );
      secondLept = Electrons.at( highPtOSElElPairs[0].second );
      ps.addPlot(HT_cutString+"ElEl", "HT(after ElEl cuts)", 80, 0., 800., HT_selectedJetPF, "HT(selected jets) [GeV]", "events");
    }
    else if ( isRA6highPtMuEl ) {
      top = "RA6MuEl";
      firstLept  = Muons    .at( highPtOSMuElPairs[0].first  );
      secondLept = Electrons.at( highPtOSMuElPairs[0].second );
      ps.addPlot(HT_cutString+"MuEl", "HT(after MuEl cuts)", 80, 0., 800., HT_selectedJetPF, "HT(selected jets) [GeV]", "events");
    }


    double absmultp1p2 = sqrt( firstLept.Vect().mag2() * secondLept.Vect().mag2() );
    double skamultp1p2 = firstLept.px()*secondLept.px() +
      firstLept.py()*secondLept.py() +
      firstLept.pz()*secondLept.pz();
    double thetaAngle = Angle(firstLept.Vect(),secondLept.Vect());
    double cosThetaAngle = cos(thetaAngle);

    vector<float>& TCHE_bTag   = tree->Get(&TCHE_bTag  , "ak5JetPF2PATTrkCountingHighEffBJetTagsPat");
    static float TCHEbTag_cutVal = config.getFloat( "TCHEbTag_cutVal",   3.3 );

    int numBTags=0;

    for(size_t j=0; j<RA6_selectedJetPF.size(); ++j)
      if(TCHE_bTag.at(RA6_selectedJetPF.at(j)) > TCHEbTag_cutVal) ++numBTags;

    unsigned event = tree->Get(event,"event");
    unsigned lumi  = tree->Get(lumi ,"lumiSection");

    plotsId.addLeaf( top, "cos",      cosThetaAngle                   );
    plotsId.addLeaf( top, "p1p2",     absmultp1p2                     );
    plotsId.addLeaf( top, "p1",       sqrt( firstLept.Vect().mag2() ) );
    plotsId.addLeaf( top, "p2",       sqrt( secondLept.Vect().mag2()) );
    plotsId.addLeaf( top, "invM",     (firstLept + secondLept).mass() );
    plotsId.addLeaf( top, "weight",   plotSet::global_event_weight    );
    plotsId.addLeaf( top, "event",    (int) event                     );
    plotsId.addLeaf( top, "run",      (int) run                       );
    plotsId.addLeaf( top, "lumi",     (int) lumi                      );
    plotsId.addLeaf( top, "MET",      metP4PF->Et()                   );
    plotsId.addLeaf( top, "numJets",  (int) RA6_selectedJetPF.size()  );
    plotsId.addLeaf( top, "numBtags", (int) numBTags                  );
    plotsId.addLeaf( top, "HT",       HT_selectedJetPF                );


    //cout<<"before new plots"<<endl;

    if(HT_selectedJetPF>100 && metP4PF->Et()>120) {
      TString Zwin="";
      if((firstLept + secondLept).mass() > 70 && (firstLept + secondLept).mass() < 110 ) Zwin = "Zwin";

      ps.addPlot_withUnweighted(top+"ps_MET" +Zwin,"ps_MET" ,30,0.,300., metP4PF->Et()    );
      ps.addPlot_withUnweighted(top+"ps_HT"  +Zwin,"ps_HT"  ,50,0.,500., HT_selectedJetPF );
      ps.addPlot_withUnweighted(top+"ps_nMu" +Zwin,"ps_nMu" ,10,0., 10., RA6_selectedMu.size()    );
      ps.addPlot_withUnweighted(top+"ps_nEl" +Zwin,"ps_nEl" ,10,0., 10., RA6_selectedEl.size()    );
      ps.addPlot_withUnweighted(top+"ps_nJet"+Zwin,"ps_nJet",10,0., 10., RA6_selectedJetPF.size() );

      ps.addPlot_withUnweighted(top+"ps_minDPhiMu" +Zwin,"ps_minDPhiMu" ,25,0., 5., minDPhi_MET_Mu  );
      ps.addPlot_withUnweighted(top+"ps_minDPhiEl" +Zwin,"ps_minDPhiEl" ,25,0., 5., minDPhi_MET_El  );
      ps.addPlot_withUnweighted(top+"ps_minDPhiJet"+Zwin,"ps_minDPhiJet",25,0., 5., minDPhi_MET_Jet );

    }
    //cout<<"end of loop"<<endl;
  } // event loop


  RA6_Mu_selectionCuts.printAll();
  RA6_El_selectionCuts.printAll();
  RA6_JetPF_selectionCuts.printAll();
  RA6_Vx_selectionCuts.printAll();

  globalFlow.printAll();

  globalFlow.dumpToHist();
  RA6_Mu_selectionCuts.dumpToHist();
  RA6_El_selectionCuts.dumpToHist();
  RA6_JetPF_selectionCuts.dumpToHist();
  RA6_Vx_selectionCuts.dumpToHist();
}
