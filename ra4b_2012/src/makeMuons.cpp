#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "eventselection.h"
#include "Muon.h"
#include "makeMuons.h"

using namespace std;

extern bool pcp;

//======================================================

//======================================================





//======================================================
//void makeAllMuons(EasyChain* tree, vector<Ptr_Muon>& Muons){



vector<Muon> makeAllMuons(EasyChain* tree){
  
  if(pcp){
    cout<<endl;
    cout<<"inside makeAllMuons "<<endl;
    cout<<endl;
  }
  vector<Muon> AllMuons;
  vector<LorentzM>& Muons = tree->Get(&Muons, "muonP4Pat");
  vector<int>&      charge   = tree->Get( &charge,"muonChargePat");
  vector<float>&    PFIso        = tree->Get(&PFIso,"DESYmuonPfIsolationR03DeltaBCorrectedPat");
  //
  Muon dummyMuon;
  for (int imu=0;imu<(int)Muons.size();++imu){
    
    if(pcp){
      cout<<"muon("<<imu<<") pt = "<<Muons.at(imu).Pt() <<endl;
      cout<<"muon("<<imu<<") eta = "<<Muons.at(imu).Eta() <<endl;
    }
    if (Muons.at(imu).Pt() < 10.0)continue;
    if (fabs(Muons.at(imu).Eta()) >= 2.5)continue;    

    dummyMuon.Set(imu,&Muons.at(imu),charge.at(imu),PFIso.at(imu));
    AllMuons.push_back(dummyMuon);
  }
  
  return AllMuons;
}

vector<Muon> makeTrkOrGlobalMuons(EasyChain* tree){
  
  if(pcp){
    cout<<endl;
    cout<<"inside makeTrkOrGlobalMuons "<<endl;
    cout<<endl;
  }
  vector<Muon> TrkOrGlobalMuons;
  vector<LorentzM>& Muons = tree->Get(&Muons, "muonP4Pat");
  vector<int>&      charge   = tree->Get( &charge,"muonChargePat");
  vector<float>&    PFIso        = tree->Get(&PFIso,"DESYmuonPfIsolationR03DeltaBCorrectedPat");

  vector<int>&      Mu_IsGlobal = tree->Get( &Mu_IsGlobal, "muonIsGlobalMuonPat" );
  vector<int>&      Mu_IsTracker = tree->Get( &Mu_IsGlobal, "muonIsTrackerMuonPat" );

  Muon dummyMuon;
  for (int imu=0;imu<(int)Muons.size();++imu){

    if(pcp){
      cout<<"muon("<<imu<<") pt = "<<Muons.at(imu).Pt() <<endl;
      cout<<"muon("<<imu<<") eta = "<<Muons.at(imu).Eta() <<endl;
    }

    if ( !Mu_IsGlobal.at(imu) && !Mu_IsTracker.at(imu) ) continue;

    dummyMuon.Set(imu,&Muons.at(imu),charge.at(imu),PFIso.at(imu));
    TrkOrGlobalMuons.push_back(dummyMuon);
  }
  
  return TrkOrGlobalMuons;
}


//
//======================================================
//



bool makeLooseMuons(EasyChain* tree, vector<Muon>& AllMuons, vector<Muon*>& LooseMuons, CutSet* flow_in){


  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE LOOSE LEPTONS
  //====================================================================
  ConfigReader config;

  static float  PTMIN  =  config.getFloat("LooseMuons_PTMIN",  15. );
  static float  ETAMAX  = config.getFloat("LooseMuons_ETAMAX", 2.5 );

  if(pcp)cout<<"inside looseMuons_RA4b"<<endl;
  
  static CutSet LooseMuonFlow("Loose_Muon_Selection");
  //  LooseMuonFlow.autoprint=true;
  LooseMuonFlow.autodump=true;

  CutSet* flow = &LooseMuonFlow;
  if (flow_in) {
    LooseMuonFlow.autodump = false;
    flow = flow_in;
  }


  LooseMuons.clear();


  //=============================================
  // THE MUON COLLECTIONS
  vector<int>&      Mu_isPF = tree->Get(&Mu_isPF,"muonIsPFMuonPat");
  vector<int>&      Mu_IsGlobal = tree->Get( &Mu_IsGlobal, "muonIsGlobalMuonPat" );
  vector<int>&      Mu_IsTracker = tree->Get( &Mu_IsGlobal, "muonIsTrackerMuonPat" );

  bool OK=false;


  if(pcp)cout<<"about to enter the muon loop"<<endl;
  //=============================================
  // LOOP OVER MUONS
  //=============================================
  int NLooseMuons=0;
  for(int imu=0;imu<AllMuons.size();++imu){
    if(pcp)cout<<"inside the muon loop at iteration "<<imu<<endl;	  

    int indx=AllMuons.at(imu).GetIndexInTree();

    OK=AllMuons.at(imu).pt()>=PTMIN;
    if(!flow->keepIf("pt>mu_pt_min_low",OK)) continue;
    //
    OK=fabs(AllMuons.at(imu).eta())<=ETAMAX;
    if(!flow->keepIf("abs(eta)<etamax",OK))continue;
    //
    OK=Mu_isPF.at(indx);
    if(!flow->keepIf("is PF",OK)) continue;
    //
    OK=Mu_IsGlobal.at(indx) || Mu_IsTracker.at(indx);
    if(!flow->keepIf("is tracker or reco",OK)) continue;    
    //
    if(pcp)cout<<"through with the loose muon cut"<<endl;    

    //if we are here, the muons are loose
    AllMuons.at(imu).SetID("Loose",true);
    LooseMuons.push_back(&AllMuons.at(imu));
    //
    NLooseMuons++;

  }//loop over muons
  if(pcp)cout<<"out of the muon loop"<<endl;

  return true;
  //  return LooseMuonFlow.keepIf("num of loose muons", SetOfCuts::LooseMuons.NUM.Examine(NLooseMuons) );

}




bool makeSoftMuons(EasyChain* tree, vector<Muon>& AllMuons, vector<Muon*>& SoftMuons){
  
  //vector<Muon*> Muons;

  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE LOOSE LEPTONS
  //====================================================================
  ConfigReader config;
  /*static float PTMIN  = config.getFloat("LooseMuons_PTMIN",  15. );
  static float ETAMAX = config.getFloat("LooseMuons_ETAMAX", 2.4 );
  static float ISOMAX = config.getFloat("LooseMuons_ISOMAX", 0.15 );
  static float dBSMAX = config.getFloat("LooseMuons_dBSMAX", 0.1 );
  static float dZMAX  = config.getFloat("SoftMuons_dZMAX", 1. );*/

  static int NTrackerLayersMIN =config.getInt("SoftMuons_NTrackerLayersMIN",6); // 
  static float InnerChi2MAX    = config.getFloat("SoftMuons_InnerChi2MAX",1.8); //
  static float dxyVertexMAX    = config.getFloat("SoftMuons_dxyVertexMAX",3.0); //
  static float dzVertexMAX     = config.getFloat("SoftMuons_dzVertexMAX",30.0); //
  static float  PTMIN          =  config.getFloat("SoftMuons_PTMIN",  15. ); //
  static float  ETAMAX         = config.getFloat("SoftMuons_ETAMAX", 2.4 );  //
  static int NValidPixelHitsMIN=config.getInt("SoftMuons_NValidPixelHitsMIN",1); //
  static float PFIsoCut          =config.getFloat("SoftPFIsoCutMIN",0.2); //
  //====================================================================

  if(pcp)cout<<"inside softMuons_RA4b"<<endl;
  
  static CutSet SoftMuonFlow("Soft_Muon_Selection");
  SoftMuonFlow.autodump=true;

  //=============================================
  // THE MUON COLLECTION
  //=============================================
  vector<int>&      IsGlobal     = tree->Get( &IsGlobal,      "muonIsGlobalMuonPat" );
  vector<int>&      IsTracker    = tree->Get( &IsGlobal,      "muonIsTrackerMuonPat" );
  vector<int>&      OneStationTight = tree->Get(&OneStationTight,"muonTMOneStationTightPat");
  vector<unsigned int>& NTrackerLayers =tree->Get(&NTrackerLayers,"muonNumberOfTrackerLayersWithMeasurementPat");
  vector<float>&    normalizedchi2 = tree->Get(&normalizedchi2,"muonInnerTrackNormalizedChi2Pat");
  vector<float>&    D0_track     = tree->Get( &D0_track,      "muonInnerTrackDxyPat");
  vector<float>&    DZ_track     = tree->Get( &DZ_track,      "muonInnerTrackDzPat");
  vector<unsigned int>& NValidPixelHits = tree->Get(&NValidPixelHits,"muonNumberOfValidPixelHitsPat");

  /* 
  vector<int>&      Mu_charge   = tree->Get( &Mu_charge,    "muonChargePat");
  vector<int>&      Mu_IsGlobal = tree->Get( &Mu_IsGlobal, "muonIsGlobalMuonPat" );
  vector<float>&    Mu_DxyBS = tree->Get( &Mu_DxyBS,   "muonGlobalTrackDxyBSPat");
  vector<float>&    Mu_Dz    = tree->Get( &Mu_Dz,      "muonGlobalTrackDzPat"   ); 
  vector<float>&    Mu_ecalIso  = tree->Get( &Mu_ecalIso,  "muonEcalIsoPat" );
  vector<float>&    Mu_hcalIso  = tree->Get( &Mu_hcalIso,  "muonHcalIsoPat" );
  vector<float>&    Mu_trackIso = tree->Get( &Mu_trackIso, "muonTrackIsoPat");	
  vector<int>&   Mu_IDGlobalPromptTight = tree->Get( &Mu_IDGlobalPromptTight,"muonIDGlobalMuonPromptTightPat" );*/
  //=============================================

  bool OK=false;
  //Muon dummyMuon;

  double highestpt=0;
  if(pcp)cout<<"about to enter the muon loop"<<endl;

  int NSoftMuons=0;
  //=============================================
  // LOOP OVER MUONS
  //=============================================
  for(int imu=0;imu<AllMuons.size();++imu){
    if(pcp)cout<<"inside the muon loop at iteration "<<imu<<endl;

    int indx=AllMuons.at(imu).GetIndexInTree();
    //==================================================================
    OK=AllMuons.at(imu).pt()>=PTMIN;
    if(!SoftMuonFlow.keepIf("pt>mu_pt_min_low",OK)) continue;
    //==================================================================

    //==================================================================
    OK=fabs(AllMuons.at(imu).eta())<=ETAMAX;
    if(!SoftMuonFlow.keepIf("abs(eta)<etamax",OK))continue;
    //==================================================================

    //==================================================================
    OK=OneStationTight.at(indx);
    if(!SoftMuonFlow.keepIf("one station tight",OK)) continue;    
    //==================================================================

    //==================================================================
    OK=NValidPixelHits.at(indx) >= NValidPixelHitsMIN;
    if(!SoftMuonFlow.keepIf("PixelHits min",OK)) continue;
    //==================================================================

    //==================================================================
    OK=fabs(D0_track.at(indx)) < dxyVertexMAX && fabs(DZ_track.at(indx) < dzVertexMAX);
    if(!SoftMuonFlow.keepIf("dxy and dz to vertex position",OK)) continue;  
    //==================================================================

    //==================================================================
    OK=NTrackerLayers.at(indx)>=NTrackerLayersMIN;
    if(!SoftMuonFlow.keepIf("NTrackerLayers min",OK)) continue;
    //==================================================================

    //==================================================================
    OK=normalizedchi2.at(indx) < InnerChi2MAX;
    if(!SoftMuonFlow.keepIf("normalised chi2",OK)) continue;
    //==================================================================

    //==================================================================
    OK=AllMuons.at(imu).RelIso() < PFIsoCut;
    if(!SoftMuonFlow.keepIf("PFIso",OK)) continue;
    //==================================================================

    
    //this muon is a soft muon
    AllMuons.at(imu).SetID("Soft",true);
    SoftMuons.push_back(&AllMuons.at(imu));
    NSoftMuons++;


  }//loop over muons
  if(pcp)cout<<"out of the soft muon loop"<<endl;

  return true;
  //return SoftMuonFlow.keepIf("num_of_sof_muons", SetOfCuts::SoftMuons.NUM.Examine(NSoftMuons) );  


};




bool makeTightMuons(EasyChain* tree, vector<Muon>& AllMuons,vector<Muon*>& TightMuons, CutSet* flow_in){


 
  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE TIGHT LEPTONS
  //====================================================================
  ConfigReader config;

  static float PTMIN  = config.getFloat("TightMuons_PTMIN",  20. ); //
  static float ETAMAX = config.getFloat("TightMuons_ETAMAX", 2.4 ); //
  static float ISOMAX = config.getFloat("TightMuons_ISOMAX", 0.12 ); //
  static float Chi2MAX = config.getFloat("TightMuons_Chi2MAX",10.0); //
  static int NValidGlobalTrackerHitsMIN = config.getInt("TightMuons_NValidGlobalTrackerHitsMIN", 0); //
  //static int NValidMuonHitsMIN = config.getInt("TightMuons_NValidMuonHitsMIN", 1); // ??
  static int NMatchedStationsMIN=config.getInt("TightMuons_NMatchedStationsMIN",1); //
  static float dxyVertexMAX = config.getFloat("TightMuons_dxyVertexMAX", 0.02); //
  //
  static float dzVertexMAX = config.getFloat("TightMuons_dzVertexMAX", 0.5); //
  static int NValidPixelHitsMIN=config.getInt("TightMuons_NValidPixelHitsMIN",0); //
  //
  static int NTrackerLayersMIN=config.getInt("TightMuons_NTrackerLayersMIN",5); //
  static float PFIsoCut          =config.getFloat("TightMuons_PFIsoCutMIN",0.12); //
  static float PFRECO_MAXDIFF = config.getFloat("TightMuons_PFRECO_MAXDIFF", 5.0);
  static bool REQ_ISTRACKER = config.getBool("TightMuons_REQ_ISTRACKER", false);

  //====================================================================

  if(pcp)cout<<"inside tightMuons_RA4b"<<endl;
  
  static CutSet TightMuonFlow("Tight_Muon_Selection");
  TightMuonFlow.autodump=true;

  CutSet* flow = &TightMuonFlow;
  if (flow_in) {
    TightMuonFlow.autodump = false;
    flow = flow_in;
  }


  bool OK=false;
  double highestpt=-1.;

  vector<LorentzM>& Mu_p4 = tree->Get(&Mu_p4, "muonP4Pat");
  vector<int>&   GT_nValTHits = tree->Get( &GT_nValTHits, "muonGlobalTracknumberOfValidHitsPat");
  vector<float>& normalisedchi2 = tree->Get(&normalisedchi2,"muonGlobalTracknormalizedChi2Pat");
  vector<unsigned int>& NValidGlobalTrackHits = tree->Get(&NValidGlobalTrackHits,"muonGlobalTracknumberOfValidMuonHitsPat");
  vector<unsigned int>& NMatchedStations = tree->Get(&NMatchedStations,"muonNumberOfMatchedStationsPat");
  vector<float>& Dxy_track = tree->Get( &Dxy_track,   "muonInnerTrackDxyPat");
  vector<float>& Dz_track = tree->Get( &Dz_track,   "muonInnerTrackDzPat");
  vector<unsigned int>& NValidPixelHits = tree->Get(&NValidPixelHits,"muonNumberOfValidPixelHitsPat");
  vector<unsigned int>& NTrackerLayers=tree->Get(&NTrackerLayers,"muonNumberOfTrackerLayersWithMeasurementPat");
  vector<int>&      Mu_isPF = tree->Get(&Mu_isPF,"muonIsPFMuonPat");
  vector<int>&      Mu_IsGlobal = tree->Get( &Mu_IsGlobal, "muonIsGlobalMuonPat" );
  vector<int>&      Mu_IsTracker = tree->Get( &Mu_IsTracker, "muonIsTrackerMuonPat" );


  
  if(pcp)cout<<"about to enter the tight muon loop"<<endl;
  //=============================================
  // LOOP OVER THE LOOSE MUONS                 //
  //=============================================
  int treeIndex=-1;
  
  int NTightMuons=0;

  for(int imu=0;imu<AllMuons.size();++imu){

    if(pcp)cout<<"inside the muon loop at iteration "<<imu<<endl;


    //    Muons_In.at(k)->SetID("Tight",0);
    int indx=AllMuons.at(imu).GetIndexInTree();
    
    OK=AllMuons.at(imu).pt() >= PTMIN;
    if(!flow->keepIf("pt>mu_pt_min_low TIGHT",OK)) continue;
    //
    OK=fabs(AllMuons.at(imu).Eta())<=ETAMAX;
    if(!flow->keepIf("abs(eta)<etamax TIGHT",OK))continue;
    //
    OK=AllMuons.at(imu).RelIso() < PFIsoCut;
    // OK=true;
    if(!flow->keepIf("PFIso",OK)) continue;

    OK=Mu_IsGlobal.at(indx);
    if(!flow->keepIf("is global",OK)) continue;    
    //
    OK=Mu_isPF.at(indx);
    if(!flow->keepIf("is PF",OK)) continue;
    //
    OK= ((!REQ_ISTRACKER) || Mu_IsTracker.at(indx));
    if(!flow->keepIf("is Tracker",OK)) continue;
    //
    OK=normalisedchi2.at(indx) <= Chi2MAX;
    if(!flow->keepIf("normalised chi2 for TIGHT muons",OK)) continue;
    //
    OK=NValidGlobalTrackHits.at(indx) > NValidGlobalTrackerHitsMIN;
    if(!flow->keepIf("global tracker hits for TIGHT muons",OK)) continue;
    //
    OK=NMatchedStations.at(indx)>NMatchedStationsMIN;
    if(!flow->keepIf("N Matched Statios for TIGHT muons",OK)) continue;
    //
    OK=fabs(Dxy_track.at(indx)) < dxyVertexMAX;
    if(!flow->keepIf("dxy to vertex position",OK)) continue;
    //
    OK=fabs(Dz_track.at(indx)) < dzVertexMAX;
    if(!flow->keepIf("dz to vertex position",OK)) continue;
    //
    OK=NValidPixelHits.at(indx) > NValidPixelHitsMIN;
    if(!flow->keepIf("PixelHits min",OK)) continue;
    //
    OK=NTrackerLayers.at(indx)>NTrackerLayersMIN;
    if(!flow->keepIf("NTrackerLayers min",OK)) continue;
 
    //if it got here, it must be a tight lepton
    //     cout <<    Consistency(Mu_p4.at(indx),tree,"muonP4PF") << endl;
    OK= Consistency(Mu_p4.at(indx),tree,"muonP4PF") < PFRECO_MAXDIFF;
    if(!flow->keepIf("RecoPt-PFPt",OK)) continue;

    AllMuons.at(imu).SetID("Tight",true);
    TightMuons.push_back(&AllMuons.at(imu));
    NTightMuons++;

  }//loop over muons
  if(pcp)cout<<"out of the muon loop"<<endl;

  return true;
};



bool makeVetoMuons(EasyChain* tree, vector<Muon>& AllMuons, vector<Muon*>& VetoMuons, CutSet* flow_in){

  

  extern   bool pcp;
  if(pcp){
    cout<<"INSIDE VETOMUONS"<<endl;
  }
  vector<float>& Dxy_track = tree->Get( &Dxy_track,   "muonInnerTrackDxyPat");
  vector<float>& Dz_track = tree->Get( &Dz_track,   "muonInnerTrackDzPat");

  static CutSet VetoMuonFlow("Veto_Muon_Selection");
  VetoMuonFlow.autodump=true;

  CutSet* flow = &VetoMuonFlow;
  if (flow_in) {
    VetoMuonFlow.autodump = false;
    flow = flow_in;
  }



  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE VETO MUONS
  //====================================================================
  ConfigReader config;
  static float  PTMIN   =config.getFloat("VetoMuons_PTMIN",  15. );
  static float  ETAMAX  =config.getFloat("VetoMuons_ETAMAX",  2.5);
  static double ISOMAX  =config.getFloat("VetoMuons_ISOMAX", 0.2);
  static float dxyVertexMAX = config.getFloat("VetoMuons_dxyVertexMAX", 0.2); //
  static float dzVertexMAX = config.getFloat("VetoMuons_dzVertexMAX", 0.5); //
  static bool quick     =config.getBool("quick");
  //====================================================================

  //===========================================================================================
  //LOOP OVER MUONS
  //===========================================================================================
  
  flow->keepIf("number of calls to VETO muonRA4b",true);
  float highestpt=-1.0;

  bool OK=false;
  int NOfVetoMuons=0;

  for(int iel=0;iel<(int)AllMuons.size();++iel){


    int indx=AllMuons.at(iel).GetIndexInTree();

    if (pcp){
      cout<<"dz at "<<indx<<" is "<<Dz_track.at(indx)<<endl;
      cout<<"dxy at "<<indx<<" is "<<Dxy_track.at(indx)<<endl;
    }

    //I only take into account the ones that are not Signal
    if (AllMuons.at(iel).IsID("Tight") )continue;
    
    //at the moment, VETO are those which are Loose and 
    if (!AllMuons.at(iel).IsID("Loose") )continue;

    OK=AllMuons.at(iel).RelIso() < ISOMAX;
    // OK=true;
    if(!flow->keepIf("PFIso",OK)) continue;
   
    if(pcp){
      cout<<"dxy at "<<indx <<" is "<<Dxy_track.at(indx)<<endl;
      cout<<"dxy at "<<0 <<" is "<<Dxy_track.at(0)<<endl;
    }
    OK=fabs(Dxy_track.at(indx)) < dxyVertexMAX;
    if(!flow->keepIf("dxy to vertex position",OK)) continue;
    //
    if(pcp){
      cout<<"dz is "<<Dz_track.at(indx)<<endl;
      cout<<"dz at "<<0 <<" is "<<Dz_track.at(0)<<endl;
      //cout<<"the whole vector Dz = "<<Dz_track<<endl;
    }
    OK=fabs(Dz_track.at(indx)) < dzVertexMAX;
    if(!flow->keepIf("dz to vertex position",OK)) continue;
   
    AllMuons.at(iel).SetID("Veto",true);
    //this muon is NOT tight, but it is SOFT, so its VETO
    VetoMuons.push_back(&AllMuons.at(iel));
    NOfVetoMuons++;

    if(pcp){
      cout<<endl;
      cout<<"the muon "<<iel<<" is a good veto muon"<<endl;
      cout<<endl;
    }

  }//Loop over muons

  return true;
  //return SetOfCuts::VetoMuons.NUM.Examine(NOfVetoMuons);


};


float Consistency( LorentzM vRef, EasyChain* tree, const char* name) {
  
  vector<LorentzM>& vConf = tree->Get(&vConf, name);

  LorentzM closest;
  float deltaRclosest=20.;
  float deltaR=0.;
  int NPF = 0;

  for (int i=0; i<vConf.size(); i++) {
    if (vConf.at(i).Pt() > 10.) {
      deltaR=ROOT::Math::VectorUtil::DeltaR(vRef, vConf.at(i));
      if (deltaR < deltaRclosest) {
	if (vConf.at(i).Pt()>0) {
	  closest = vConf.at(i);
	  deltaRclosest = deltaR;
	}
      }
      NPF++;
    }
  }

  if (NPF==0){
    //cout<<"no pf muons!!"<<endl;
    return 999.;
  }

  float result = fabs(vRef.Pt() - closest.Pt());

  return result;
}


void rescaleMUR(EasyChain* tree, vector<Muon*>&MuonsToRescale, LorentzM& metCorr, float murSF_err) {

  vector<LorentzM>& genP4 = tree->Get(&genP4, "genP4");
  vector<int>& genPdgId = tree->Get(&genPdgId, "genPdgId");

  //Loop over Muons
  for (int iMu = 0; iMu < MuonsToRescale.size(); iMu++) {

    LorentzM oldP4 = MuonsToRescale.at(iMu)->P4();

    //Match to Gen particle by minimising delta_r
    int MatchedGenParticle = -1;
    double minDeltaR = 99999999.;
    for (int iGen = 0; iGen < genP4.size() ; iGen++) {
      if (abs(genPdgId.at(iGen)) != 13 ) continue;
      float deltaR = ROOT::Math::VectorUtil::DeltaR(oldP4, genP4.at(iGen) );
      if (deltaR < minDeltaR) {
	minDeltaR = deltaR;
	MatchedGenParticle = iGen;
      }
    }

    if (minDeltaR > 0.5) {
      if (pcp) cout << "rescaleMUR >> No matched gen muon found. minDeltaR = " << minDeltaR << endl;
      continue;
    }
    else {
      if (pcp) cout << "rescaleMUR >> Matched gen muon found with minDeltaR = " << minDeltaR << endl;
    }

    if (pcp) {
      cout << "Muon P4=(" << MuonsToRescale.at(iMu)->Px() << ","  << MuonsToRescale.at(iMu)->Py() << ","  << MuonsToRescale.at(iMu)->Pz() << ","  << MuonsToRescale.at(iMu)->E() << ")" << endl;
      cout << "Matched gen muon P4=(" << genP4.at(MatchedGenParticle).Px() << ","  << genP4.at(MatchedGenParticle).Py() << ","  << genP4.at(MatchedGenParticle).Pz() << ","  << genP4.at(MatchedGenParticle).E() << ")" << endl;
    }

    //Create shift, as for jets
    float murSF = getMurSF(murSF_err);
    //float muRescale = genP4.at(MatchedGenParticle).Pt() + (murSF) * ( oldP4.Pt() - genP4.at(MatchedGenParticle).Pt() );
    //muRescale /= oldP4.Pt();
    //if (muRescale < 0.) muRescale = 0.;

    //metCorr += oldP4 * (1. - muRescale);
    //MuonsToRescale.at(iMu)->SetP4(oldP4 * muRescale);

    //Try smearing the whole P4 vector, not just the Pt.
    LorentzM newP4 = oldP4 + (murSF_err)*(oldP4 - genP4.at(MatchedGenParticle) );
    newP4.SetPxPyPzE(newP4.Px(), newP4.Py(), newP4.Pz(), newP4.P() ); //Maintain a zero mass.
    metCorr += (oldP4 - newP4);
    MuonsToRescale.at(iMu)->SetP4(newP4);

  }

  return;
}

float getMurSF(float err) {
  return (1.00 + err);
}

void rescaleMUR_simple(EasyChain* tree, vector<Muon*>&MuonsToRescale, LorentzM& metCorr, float murSF_err) {

  //Loop over Muons
  for (int iMu = 0; iMu < MuonsToRescale.size(); iMu++) {

    LorentzM oldP4 = MuonsToRescale.at(iMu)->P4();

    //Smear the energy of the muon
    float oldE = oldP4.E();
    TRandom3 rand(0);
    float newE = oldE + rand.Gaus(0., murSF_err*oldE);
    if (newE < 0) newE = 0.;

    float muRescale = newE / oldE;

    metCorr += oldP4 * (1. - muRescale);
    MuonsToRescale.at(iMu)->SetP4(oldP4 * muRescale);
  }

  return;
}
