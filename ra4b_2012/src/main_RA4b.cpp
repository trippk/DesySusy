//========================================================================//
//                                                                        //
//                         RA4 b                                          //
//                                                                        //
//========================================================================//
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "Math/VectorUtil.h"
#include "NtupleTools2.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "eventselection.h"
#include "SampleInformation.h"
#include "makeElectrons.h"
#include "makeMuons.h"
#include "triggers_RA4b.h"
#include "triggers_RA4b_frequency.h"
#include "vertices_RA4b.h"
#include "metAndHT_RA4b.h"
#include "evtqual_RA4b.h"
#include "SetTriggers_RA4b.h"
#include "SetConditions_RA4b.h"
#include "cschalo_RA4b.h"
#include "trackingFailure_RA4b.h"
#include "THTools.h"
#include "TStopwatch.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "makeJets.h"
#include "HistoMaker.h"
#include "mt2w_interface.h"
#include "subTree.h"
#include "subTreeFactory.h"
#include "EventInfo.h"
#include "systematics_RA4b.h"
#include "makeSystematics.h"
#include "CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "Utilities.cc"
#include "JetCorrectorParameters.cc"
#include "SimpleJetCorrectionUncertainty.cc"
#include "JetCorrectionUncertainty.cc"
#include "typelookup.cc"
#include "Exception.cc"

using namespace std;
using namespace ROOT::Math::VectorUtil;
//===================================================================
//                   GLOBAL VARIABLES
//===================================================================

bool checkthisevent=false;

vector<string> triggernames;
vector<string> triggernames_short;
bool pcp;
double EventWeight;



//================================================================================
int main(int argc, char** argv){



  //MT2W CALCULATOR
  mt2w_bisect::mt2w_interface mt2w_calc;

  //
  const double PI = 4.0*atan(1.0);  


  //PCP=TRUE FOR DEBUGGING
  pcp=false;


  EventWeight=1.0;


  //CONFIG. The program will always take the file config.txt in the output directory
  //specified in the execution of runOnAll
  string MainDir="./";
  ConfigReader config(MainDir+"config.txt",argc,argv);
  config.Add(MainDir+"para_config.txt");
  config.Add(MainDir+"pu_config.txt");

  //NAME OF THE FILE
  TString filename = config.getTString("filename");
  
  EasyChain* tree = new EasyChain("/susyTree/tree");
  // add file(s) or folder(s)
  int f = tree->AddSmart(filename);


  TString outname = config.getTString("outname",tree->GetUniqeName());
  string outname_string=(string)outname;
  // output directory and output file name(derived from input name)
  cout<<"the output file name is "<<outname<<endl;;
  TFile *outfile = TFile::Open(outname,"RECREATE");
  //TFile *outprovafile = TFile::Open("outprovafile.root","RECREATE");
  // set output root file for cut flow (to be done!)
  //  CutSet::setTFile(outfile);
  

 
  string replacestring=".root";
  string treeFileName="";
  int pos = outname_string.find(replacestring);
  size_t thelength=replacestring.length();
  if (pos != string::npos){
    treeFileName= outname_string.replace(pos,thelength,"_tree.root");
  }
  //
  TFile* treeFile=TFile::Open((TString)treeFileName,"RECREATE");
  //  treeFile->cd();
  TString treeType = config.getTString("treeType","default"); 
  cout<<"going to call new tree with "<<treeFile<<endl;
  subTree* subTree= subTreeFactory::NewTree(treeType,treeFile,(string)"./");
  bool doSmallTree=true;
  outfile->cd();  




  //data or monte carlo?
  bool isData=config.getBool("isData");
  //=================================================================


  //ofstream events1;
  //  ofstream events2;
  //  ofstream onejet;
  //  ofstream twojet;
  //  ofstream threejet;
  
  //events1.open("afterjets_fast.txt");
  //events2.open("after_1ele_nomu.txt");
  //  onejet.open("after_1jet.txt");
  //  twojet.open("after_2jet.txt");
  //  threejet.open("after_3jet.txt");
  /*

  tree->SetBranchStatus("*",0);

  tree->SetBranchStatus("event",1);
  tree->SetBranchStatus("run",1);
  tree->SetBranchStatus("metP4PF",1);
  tree->SetBranchStatus("nTracksAll",1);
  tree->SetBranchStatus("nTracksHighPurity",1);
  tree->SetBranchStatus("ecalDeadCellTPFilterFlag",1);
  tree->SetBranchStatus("ak5JetPFTrkCountingHighEffBJetTagsPat",1);
  tree->SetBranchStatus("electronP4Pat",1);
  tree->SetBranchStatus("electronChargePat",1);
  tree->SetBranchStatus("electronDr03EcalRecHitSumEtPat",1);
  tree->SetBranchStatus("electronDr03HcalTowerSumEtPat",1);
  tree->SetBranchStatus("electronDr03TkSumPtPat",1);
  tree->SetBranchStatus("electronP4Pat",1);
  tree->SetBranchStatus("electronESuperClusterEtaPat",1);
  tree->SetBranchStatus("electronGsfTrackDxyPat",1);
  tree->SetBranchStatus("electronGsfTrackDzPat",1);
  tree->SetBranchStatus("electronFbremPat",1);
  tree->SetBranchStatus("electronESuperClusterOverPPat",1);
  tree->SetBranchStatus("electronIdTightPat",1);
  tree->SetBranchStatus("electronIdMediumPat",1);
  tree->SetBranchStatus("DESYelectronIdTriggerTightPat",1);
  tree->SetBranchStatus("electronESuperClusterEtaPat",1);
  tree->SetBranchStatus("ak5JetPFPFJetIDloosePat",1);
  tree->SetBranchStatus("ak5JetPFCorrectedP4Pat",1);
  tree->SetBranchStatus("ak5JetPFCorrFactorPat",1);
  tree->SetBranchStatus("ak5JetPFCombinedSecondaryVertexBJetTagsPat",1);
  tree->SetBranchStatus("ak5JetPFCombinedSecondaryVertexMVABJetTagsPat",1);
  tree->SetBranchStatus("muonP4Pat",1);
  tree->SetBranchStatus("muonChargePat",1);
  tree->SetBranchStatus("muonPfIsolationR04DeltaBCorrectedPat",1);
  tree->SetBranchStatus("muonIsPFMuonPat",1);
  tree->SetBranchStatus("muonIsGlobalMuonPat",1);
  tree->SetBranchStatus("muonIsTrackerMuonPat",1);
  tree->SetBranchStatus("muonP4Pat",1);
  tree->SetBranchStatus("muonIsGlobalMuonPat",1);
  tree->SetBranchStatus("muonIsTrackerMuonPat",1);
  tree->SetBranchStatus("muonTMOneStationTightPat",1);
  tree->SetBranchStatus("muonNumberOfTrackerLayersWithMeasurementPat",1);
  tree->SetBranchStatus("muonInnerTrackNormalizedChi2Pat",1);
  tree->SetBranchStatus("muonInnerTrackDxyPat",1);
  tree->SetBranchStatus("muonInnerTrackDzPat",1);
  tree->SetBranchStatus("muonNumberOfValidPixelHitsPat",1);
  tree->SetBranchStatus("muonP4Pat",1);
  tree->SetBranchStatus("muonChargePat",1);
  tree->SetBranchStatus("muonIsGlobalMuonPat",1);
  tree->SetBranchStatus("muonGlobalTrackDxyBSPat",1);
  tree->SetBranchStatus("muonGlobalTrackDzPat",1);
  tree->SetBranchStatus("muonEcalIsoPat",1);
  tree->SetBranchStatus("muonHcalIsoPat",1);
  tree->SetBranchStatus("muonTrackIsoPat",1);
  tree->SetBranchStatus("muonIDGlobalMuonPromptTightPat",1);
  tree->SetBranchStatus("muonGlobalTracknumberOfValidHitsPat",1);
  tree->SetBranchStatus("muonGlobalTracknormalizedChi2Pat",1);
  tree->SetBranchStatus("muonGlobalTracknumberOfValidMuonHitsPat",1);
  tree->SetBranchStatus("muonNumberOfMatchedStationsPat",1);
  tree->SetBranchStatus("muonInnerTrackDxyPat",1);
  tree->SetBranchStatus("muonInnerTrackDzPat",1);
  tree->SetBranchStatus("muonNumberOfValidPixelHitsPat",1);
  tree->SetBranchStatus("muonNumberOfTrackerLayersWithMeasurementPat",1);
  tree->SetBranchStatus("muonIsPFMuonPat",1);
  tree->SetBranchStatus("muonIsGlobalMuonPat",1);
  tree->SetBranchStatus("muonSelectedPF",1);
  tree->SetBranchStatus("metP4PF",1);
  tree->SetBranchStatus("metP4TC",1);
  tree->SetBranchStatus("trackingFailureFilterFlag",1);
  tree->SetBranchStatus("prescaled",1);
  tree->SetBranchStatus("triggered",1);
  tree->SetBranchStatus("DESYtriggerNameMap",1);
  tree->SetBranchStatus("vertexPosition",1);
  tree->SetBranchStatus("vertexIsFake",1);
  tree->SetBranchStatus("vertexNdof",1);
  tree->SetBranchStatus("vertexNtrks",1);
  if(!isData){
      tree->SetBranchStatus("pileupTrueNumInteractionsBX0",1);
  }

  */





  //===========================================================================
  // SET THE INFORMATION ABOUT THE SAMPLE
  //===========================================================================
  SampleInformation mySampleInfo;

  //it HAS to be read from config.txt, otherwise the default is
  //undefined
  string WhatSample=config.getString("SampleName","undefined");
  string WhatSubSample=config.getString("SubSampleName","undefined");
  string WhatEstimation=config.getString("Estimation","undefined");
  string WhatTail=config.getString("Tail","undefined");
  mySampleInfo.SetInformation(WhatSample,WhatSubSample,WhatEstimation,WhatTail);
  //print it
  mySampleInfo.DumpInformation();
  //===========================================================================



  //===========================================================================
  // SET THE CONDITIONS FOR THE SAMPLE AND ESTIMATION
  //===========================================================================
  SetOfCuts myCuts;
  SetConditions_RA4b(mySampleInfo);
  //===========================================================================


  if(pcp)cout<<"going to set the triggers "<<endl;
  //===========================================================================
  // SET THE TRIGGERS
  //===========================================================================
  vector<const char*> triggernames;
  SetTriggers_RA4b(mySampleInfo,triggernames);
  //===========================================================================
  if(pcp)cout<<"out of set the triggers "<<endl;
  if(pcp)cout<<"check point 1"<<endl;







  //======================================================
  //WEIGHTS
  //======================================================



  //PU

  bool oldpuw = false; //the obselete method and values
  //if(WhatSample=="TTJets")oldpuw=true;

  vector<double> PUmc;
  vector<double> PUdata;
  int nobinsmc=0;
  int nobinsdata=0;
  if(!isData && !oldpuw){
    nobinsmc = config.getDouble("PU_"+WhatSample+"_"+WhatSubSample+"_mc",PUmc," ");
    nobinsdata = config.getDouble("PU_data",PUdata," ");
  } 
  else if(!isData && oldpuw){
    nobinsmc = config.getDouble("oldPU_"+WhatSample+"_"+WhatSubSample+"_mc",PUmc," ");
   } 

  if(nobinsmc!=nobinsdata && !oldpuw && !isData){
    cout << "problem in pu inf in para_config - number of bins in MC PU dist is different than data!" << endl;
    return 0;
  }

  double InitialEventWeight=1.0;             //Event weight do to Lumi and xsec.





  //REWEIGHT DUE TO THE DATA LUMINOSITY
  //this should be done offline
  bool DoOnlineRW=false;
  if(DoOnlineRW){
    cout<<"GOING TO REWEIGHT THE MC SAMPLE TO THE DATA LUMI "<<endl;
    double xsecMC=1.0;                         //Monte Carlo Cross Section
    double Lumidata=5097.2;                    //Luminosity of the data
    double FilterEff=1.0;                      //Filter Efficiency
    long   Ntotal_MC=-1;                       //Total events in Monte Carlo
   
    if(!isData){
      FilterEff=config.getDouble("FE_"+WhatSample+"_"+WhatSubSample);
      xsecMC=config.getDouble("xs_"+WhatSample+"_"+WhatSubSample);
      Ntotal_MC=config.getLong("TNoE_"+WhatSample+"_"+WhatSubSample);
      cout<<"FILTER EFFICIENCY FOR THIS SUBSAMPLE  "<<FilterEff<<endl;
      cout<<"CROSS SECTION OF THIS SUBSAMPLE "<<xsecMC<<endl;
      cout<<"TOTAL NUMBER OF ENTRIES "<<Ntotal_MC<<endl;
      cout<<"LUMINOSITY OF THE DATA TO COMPARE THE MC "<<Lumidata<<endl;
      
      InitialEventWeight=Lumidata*xsecMC*FilterEff/Ntotal_MC;
      if(Ntotal_MC<=0){
	cout<<"The denominator is 0 or negative, this should NEVER happen. ERROR "<<endl;
	cout<<"Leaving the program "<<endl;
	return 0;
      }
      cout<< " THE EVENT WEIGHT DUE TO FilterEff*xSec(*Lumi/TNoE) = "<< InitialEventWeight<<endl;
    }
  }
 
  //CutSet::global_event_weight  = InitialEventWeight;
  //========================================================





  //keep track of the weights
  TH1::SetDefaultSumw2(true);
  //=========================




  //===============================================================
  //===============================================================
  //FILE READING (and all that)
  //===============================================================
  //===============================================================
  //files read;
  cout<<tree->GetNtrees()<<" Files read"<<endl;
  if(pcp)cout<<"check point 2"<<endl;
  //=================================================================
  int N = tree->GetEntries();
  //=================================================================
  cout<<"THERE ARE "<<N<<" EVENTS IN "<<filename<<endl;
  









  //================================
  //SET THE BJET WORKING POINTS
  //================================
  map<string, map<string, double> > bJetWP; 
  Jet::SetWP("8TeV", &bJetWP); 
  //  cout<<"the working point has been set to "<<Jet::GetbJetWP()["CSV"]["Medium"]<<endl;
  //  exit(1);

  //=================================
  //SET THE CONTROL PLOTS HISTO MAKER
  //================================ 
  bool DoControlPlots=config.getBool("DoControlPlots",true); 
  HistoMaker::setTFile(outfile); 
  HistoMaker ControlPlots("AnalyzeSUSY"," "); 
  outfile->cd();




  
  //================================================================
  TH1I* isdata = new TH1I("isdata","data =1 means Data",1,0,1);
  if(isData){isdata->SetBinContent(1,1);}
  else{isdata->SetBinContent(1,0);}
  //================================================================
 
  //================================================================   
  TH1I* num_entries = new TH1I("num_entries","number of entries",1,0,1);
  num_entries->SetBinContent(1,N);
  //================================================================

  //================================================================
  TH1I*num_entries_weighted = new TH1I("num_entries_weighted","number of entries with the event weight",1,0,1);
  num_entries_weighted->SetBinContent(1,N,EventWeight);
  //================================================================

  if(pcp)cout<<"check point before histos"<<endl;





  //===========================================
  //CUT FLOW MONITORING
  //===========================================
  //OK and OKold for the cutflow
  bool OK=false;
  bool OKold=true;
  CutSet globalFlow("global flow");
  CutSet::setTFile(outfile);
  CutSet* p2globalFlow= &globalFlow;
  CutSet systematicsFlow("systematics flow");
  //===========================================

    
  //===========================================
  bool turntriggersoff=true;
  if(turntriggersoff){
    cout<<"-----------TURNTRIGGERSOFF IS true!!-----------"<<endl;
    if(isData){
      cout<<"--------NO TRIGGERS, NO DATA--------------------"<<endl;
      exit(1);
    }
  }
  //===========================================



  TH1D* EW_AfterTrigger= new TH1D("EW_AfterTrigger","Event weight after the trigger",30,0.0,100.0);
  TH1D* EW_AfterPU=new TH1D("EW_AfterPU","Event Weight after PU RW",100,0.0,10.0);
  //TH1D* triggers_prescale= new TH1D("trigger_prescale","the prescale of the trigger",50,0.0,10.0);

  static double HTmin=config.getFloat("ABDCHTmin",375);
  static double HTmax=config.getFloat("ABDCHTmax",650);
  static double METSig_min=config.getFloat("ABDCMETSigmin",2.5);
  static double METSig_max=config.getFloat("ABDCMETSigmax",5.5);
 

  //AFTER THE CUT FLOW
  TH2D* HT_METSig= new TH2D("HT_METSig","HT vs METSig",50,HTmin,2000.,50,METSig_min,40.);  
  TH1D* RegionA = new TH1D("RegionA", "Number of entries in A",1,0.5,1.5);
  TH1D* RegionB = new TH1D("RegionB", "Number of entries in B",1,0.5,1.5);
  TH1D* RegionC = new TH1D("RegionC", "Number of entries in C",1,0.5,1.5);
  TH1D* RegionD = new TH1D("RegionD", "Number of entries in D",1,0.5,1.5);

  //ONE BTAG
  map <const char*,TH1D*> OneBtag_1DPlots;
  map <const char*,TH2D*> OneBtag_2DPlots;
  TDirectory* onebtagdir =outfile->mkdir("ONE_BTAG");
  onebtagdir->cd();
  OneBtag_1DPlots["RegionA"]= new TH1D("RegionA", "Number of entries in A with one btag",1,0.5,1.5);
  OneBtag_1DPlots["RegionB"]= new TH1D("RegionB", "Number of entries in B with one btag",1,0.5,1.5);
  OneBtag_1DPlots["RegionC"]= new TH1D("RegionC", "Number of entries in C with one btag",1,0.5,1.5);
  OneBtag_1DPlots["RegionD"]= new TH1D("RegionD", "Number of entries in D with one btag",1,0.5,1.5);
  OneBtag_2DPlots["HT_METSig"]= new TH2D("HT_METSig","HT vs METSig",50,HTmin,2000.,50,METSig_min,40.);  

  //TWO BTAGS
  map <const char*,TH1D*> TwoBtag_1DPlots;
  map <const char*,TH2D*> TwoBtag_2DPlots;
  TDirectory* twobtagdir =outfile->mkdir("TWO_BTAG");
  twobtagdir->cd();
  TwoBtag_1DPlots["RegionA"]= new TH1D("RegionA", "Number of entries in A with two btag",1,0.5,1.5);
  TwoBtag_1DPlots["RegionB"]= new TH1D("RegionB", "Number of entries in B with two btag",1,0.5,1.5);
  TwoBtag_1DPlots["RegionC"]= new TH1D("RegionC", "Number of entries in C with two btag",1,0.5,1.5);
  TwoBtag_1DPlots["RegionD"]= new TH1D("RegionD", "Number of entries in D with two btag",1,0.5,1.5);
  TwoBtag_2DPlots["HT_METSig"]= new TH2D("HT_METSig","HT vs METSig",50,HTmin,2000.,50,METSig_min,40.);  


  //TWOPLUS BTAGS
  map <const char*,TH1D*> TwoplusBtag_1DPlots;
  map <const char*,TH2D*> TwoplusBtag_2DPlots;
  TDirectory* twoplusbtagdir =outfile->mkdir("TWOPLUS_BTAG");
  twoplusbtagdir->cd();
  TwoplusBtag_1DPlots["RegionA"]= new TH1D("RegionA", "Number of entries in A with twoplus btag",1,0.5,1.5);
  TwoplusBtag_1DPlots["RegionB"]= new TH1D("RegionB", "Number of entries in B with twoplus btag",1,0.5,1.5);
  TwoplusBtag_1DPlots["RegionC"]= new TH1D("RegionC", "Number of entries in C with twoplus btag",1,0.5,1.5);
  TwoplusBtag_1DPlots["RegionD"]= new TH1D("RegionD", "Number of entries in D with twoplus btag",1,0.5,1.5);
  TwoplusBtag_2DPlots["HT_METSig"]= new TH2D("HT_METSig","HT vs METSig",50,HTmin,2000.,50,METSig_min,40.);  


  //
  //
  if(pcp)cout<<"check point before the event loop"<<endl;



  bool isquick=config.getBool("quick",true);
  bool quick=false;
  cout<<"isquick is"<<isquick<<endl;
 










  //===================================================================
  //SYSTEMATIC UNCERTAINTIES
  //===================================================================
  static bool doSystematics=config.getBool("doSystematics",true);
  bool OKsyst=false;
  Systematics systematics;
  if (!isquick && doSystematics){
    //it cant do systematics on the quick mode
    systematics.AddUncertainty((string)"jetup",treeFile);
    systematics.AddUncertainty((string)"jetdown",treeFile);
    systematics.AddUncertainty((string)"clustersup",treeFile);
    systematics.AddUncertainty((string)"clustersdown",treeFile);
  }


  //=============INITIALIZE THE JETUncertainty object
  // Instantiate uncertainty sources
  const int nsrc = 16;
  const char* srcnames[nsrc] =
    {"Absolute", "HighPtExtra", "SinglePion", "Flavor", "Time",
     "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF",
     "RelativeStatEC2", "RelativeStatHF", "RelativeFSR",
     "PileUpDataMC", "PileUpOOT", "PileUpPt", "PileUpBias", "PileUpJetRate"};

  for (int isrc = 0; isrc < nsrc; isrc++) {
    const char *name = srcnames[isrc];
    std::string test="src/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
    //std::string test="/tmp/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
    JetCorrectorParameters *p ;
    if (systematics.IsEnabled()){
      p= new JetCorrectorParameters(test, name);
      systematics.vsrc.push_back(new JetCorrectionUncertainty(*p));
    }
  }
  //==========Total uncertainty for reference
  //  std::string test  ="/tmp/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
    std::string test="src/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";

  if(systematics.IsEnabled()){
    systematics.total = new JetCorrectionUncertainty(*(new JetCorrectorParameters(test, "Total")));
  }
















  //=============================================================================
  //=============================================================================
  //LOOP OVER EVENTS
  //=============================================================================
  //=============================================================================

  //N=1000;
  for(int i=0;i<N;++i){

    //This is necessary, do not touch!
    if(i==0){
      quick=false;
    }else{
      quick=isquick;
    }
     
    //    pcp=true;
    //    if(i>10)continue;

    timer();


    
    
    if(pcp)cout<<"check point about to get entry "<< i<<endl;

    //====================================================
    tree->GetEntry(i);
    //====================================================

    if(pcp)cout<<"check point after getting the entry "<< i<<endl;    
    if(pcp)cout<<"check point lets get the Event "<< i<<endl;    
    unsigned int Event   = tree->Get(Event,"event");    
    unsigned int Run   = tree->Get(Run,"run");    
    if(pcp)cout<<"check point got the Event "<< i<< " "<<Event<<endl;    

    



    //    pcp=true;
    //
    //    if (i>10){
    //      pcp=false;
    //      continue;
    //    }
    //    cout<<Event<<endl;
    //CHECK dz_track and dy_track due to the differences with other groups
    if(pcp){
      vector<LorentzM>& Muons = tree->Get(&Muons, "muonP4Pat");
      vector<float>& Dxy_track = tree->Get( &Dxy_track,   "muonInnerTrackDxyPat");
      vector<float>& Dz_track = tree->Get( &Dz_track,   "muonInnerTrackDzPat");
      cout<<"dz and dxy of ALL MUONS"<<endl;
      for (int imu=0;imu<(int)Muons.size();++imu){
	cout<<"dz of "<<imu<< " = "<<Dz_track.at(imu)<<endl;
	cout<<"dxy of "<<imu<< " = "<<Dxy_track.at(imu)<<endl;
      }
    }



    //DEBUGGING:
    if(pcp){
      cout<<endl;
      cout<<endl;
      cout<<"**************************************"<<endl;
      cout<<"NEW EVENT : "<<Event<<endl;
      cout<<"**************************************"<<endl;
      cout<<endl;
      cout<<endl;
    }


    //================================================
    //EVENT WEIGHT
    //================================================
    EventWeight=InitialEventWeight;
    //================================================
    //  cout << "weight before PUrw -> " << EventWeight << endl;


    //==============================================
    // PILE UP RW
    //==============================================
    double PUWeight=0;
    if(!isData) {
      float PUnumInter    = tree->Get( PUnumInter, "pileupTrueNumInteractionsBX0");
      int relevantNumPU = PUnumInter;
      if( relevantNumPU >= nobinsmc ) {
	cout << "something wrong with the pile up info!!! - exceed max number of vertex:     " << nobinsmc <<endl;
	return 0; 
      }

      else if( oldpuw) {
	PUWeight= PUmc.at( relevantNumPU );
      }
      else {
	PUWeight= PUdata.at( relevantNumPU )/PUmc.at( relevantNumPU );
      }
      
      EventWeight *= PUWeight;
    }
    //cout << "weight before PUrw -> " << EventWeight << endl;
    EW_AfterPU->Fill(EventWeight);
    CutSet::global_event_weight  = EventWeight;

    //==============================================


    if(checkthisevent){
      cout<<"=================================================== "<<endl;
      cout<<"going to check the event "<<Event<<endl;
      cout<<"=================================================== "<<endl;
    }





    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //           C R E A T E    O B J E C T S                            //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   




    //============================================
    // Make Muons
    vector<Muon> Muons;
    vector<Muon*> LooseMuons;
    vector<Muon*> TightMuons;
    vector<Muon*> VetoMuons;
    Muons=makeAllMuons(tree);
    makeTightMuons(tree,Muons,TightMuons);
    makeLooseMuons(tree,Muons,LooseMuons);
    makeVetoMuons(tree,Muons,VetoMuons);

    //  makeSoftMuons(tree ,Muons,SoftMuons);
    //============================================


    //============================================
    // Make Electrons
    vector<Electron>  Electrons;
    vector<Electron*> LooseElectrons;
    vector<Electron*> TightElectrons;
    vector<Electron*> VetoElectrons;
    Electrons=makeAllElectrons(tree);
    //    makeLooseElectrons(tree,Electrons,LooseElectrons);
    makeTightElectrons(tree,Electrons,TightElectrons);
    makeVetoElectrons(tree, Electrons,VetoElectrons);
    //============================================


    //============================================    
    // Make Jets
    vector<Jet>  Jets;
    vector<Jet*> AllJets;
    vector<Jet*> GoodJets;
    vector<Jet*> CleanedJets;

    Jets=makeAllJets(tree);
    for (int ijet=0;ijet<(int)Jets.size();++ijet){
      AllJets.push_back(&Jets.at(ijet));
    }
    //    cout<<"size of the collection "<<Jets.size()<<endl;
    makeGoodJets(tree,AllJets,GoodJets);
    //    cout<<"size of all jets is "<<AllJets.size()<<endl;
    //    cout<<"size of the thing! "<<GoodJets.size()<<endl;
    makeCleanedJets( GoodJets, CleanedJets, Muons, Electrons);
    //============================================
    

    //NUMBER OF BTAGS==============================
    static string btagAlgorithm= config.getString("bTagAlgorithm","CSV");
    static string btagWorkingPoint = config.getString("bTagWorkingPoint","Medium");
    int NumberOfbTags=0;
    for (int ijet=0;ijet<CleanedJets.size();++ijet){
      if(CleanedJets.at(ijet)->IsBJet(btagAlgorithm,btagWorkingPoint)){
	NumberOfbTags++;
      }
    }



    //============================================
    //Make HT
    //============================================
    double HT=0;
    double px=0;
    double py=0;
    double MHT=0;
    for(int ijet=0;ijet<(int)CleanedJets.size();++ijet){
      HT=HT+CleanedJets.at(ijet)->Pt();
      px+=  CleanedJets.at(ijet)->P4().Px();
      py+=  CleanedJets.at(ijet)->P4().Py();
    }
    MHT=sqrt(px*px + py*py);
    //============================================

    //============================================
    //Make MET
    LorentzM& PFmet = tree->Get(&PFmet, "metP4TypeIPF");
    double MET=(double)PFmet.Et() ;
    double METSig = MET / sqrt(HT);
    //============================================


    if (pcp){
      cout<<endl;
      cout<<"MET = "<<MET<<endl;
      cout<<"HT = "<<HT<<endl;
      cout<<endl;
    }



    //===========================================
    //CALCULATE SYSTEMATICS VARIATIONS    
    //===========================================
    if (systematics.IsEnabled()){

      //clear all the vectors inside systematics
      //otherwise they accumulate
      systematics.Reset();
      if (systematics.DoJetUp() || systematics.DoJetDown()){
	//=========RESCALE THE JETS
	rescaleJets(tree,AllJets,systematics);
      
	//=========RESCALE THE MET
	rescaleMET(tree,systematics.GetsysJet("jetup"),systematics, "jetup");
	rescaleMET(tree,systematics.GetsysJet("jetdown"),systematics, "jetdown");
	//now systematics contain the varied jet collection
	//and also the MET

	//======FEED THE RESCALED JETS TO makeGoodJets
	makeGoodJets(tree,systematics.GetsysJet("jetup"),systematics.GetsysJet("jetup_good"));
	//
	makeGoodJets(tree,systematics.GetsysJet("jetdown"),systematics.GetsysJet("jetdown_good"));
	//
	//======CLEAN THEM
	makeCleanedJets(systematics.GetsysJet("jetup_good"),systematics.GetsysJet("jetup_good_clean"),Muons,Electrons);
	makeCleanedJets(systematics.GetsysJet("jetdown_good"),systematics.GetsysJet("jetdown_good_clean"),Muons,Electrons);

	//=====GET THE NEW HT
	rescaleHT(systematics.GetsysJet("jetup_good_clean"), systematics, "jetup");
	rescaleHT(systematics.GetsysJet("jetdown_good_clean"), systematics, "jetdown");
      }
      

      if(systematics.GetSysMap()["clustersup"]){

	rescaleClusters(AllJets,systematics);
	//RESCALE THE MET
	rescaleMET(tree,systematics.GetsysJet("clustersup"),systematics,"clustersup");
	rescaleMET(tree,systematics.GetsysJet("clustersdown"),systematics,"clustersdown");
      }
      //
      //
      //
      //test this
      //      cout<<"the size of input jets "<<systematics.GetsysJet("down").size()<<endl;
      //      cout<<"size of cleaned jets down"<<systematics.GetsysJet("down_good_clean").size()<<endl;
      //      cout<<"size of cleaned jets up"<<systematics.GetsysJet("up_good_clean").size()<<endl;
      //      for (int ijet=0;ijet<systematics.GetsysJet("up_good_clean").size(); ++ijet){
      //	cout<<"loop is "<<ijet<<endl;
      //	cout<<"pt up "<<systematics.GetsysJet("up_good_clean").at(ijet)->Pt()<<endl;
      //      }
      //for (int ijet=0;ijet<systematics.GetsysJet("down_good_clean").size(); ++ijet){
      //cout<<"pt down "<<systematics.GetsysJet("down_good_clean").at(ijet)->Pt()<<endl;
      //}
    }



    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //                     C  U  T      F L O W                           //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   



    ControlPlots.MakePlots("Before_CutFlow", TightMuons, TightElectrons, CleanedJets, PFmet); 




    //====================================================================
    // TRIGGERS
    //====================================================================
    if(!turntriggersoff ){
      OK = triggers_RA4b(tree, triggernames,EventWeight);
      //OK=true;
      //================================================
      if(pcp)cout<<"check point triggers called"<<endl;
      //
      if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
      if( !globalFlow.keepIf("triggers", OK )  && quick ) continue;    
      EW_AfterTrigger->Fill(EventWeight);
      //
      if(DoControlPlots && OK)ControlPlots.MakePlots("Triggers", TightMuons, TightElectrons, CleanedJets, PFmet); 
      //treeCuts["Triggers"]=OK;
    }
    //====================================================================



    //====================================================================
    bool  hcalLaserEventFilterFlag   = tree->Get( hcalLaserEventFilterFlag,    "hcalLaserEventFilterFlag"  );
    OK= hcalLaserEventFilterFlag;
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if( !globalFlow.keepIf( "hcalLaserFilter"        , OK     ) && quick ) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("hcalLaserFilter", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================


    //====================================================================
    bool eeBadSCPassed = tree->Get( eeBadSCPassed, "eeBadScFilterFlag"   );
    OK=eeBadSCPassed;
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if( !globalFlow.keepIf("eeBadSCFilter"     , OK) && quick ) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("eeBadSCFilter", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================



    //====================================================================
    // SCRAPING VETO
    //====================================================================
    unsigned int nTracksAll=tree->Get(nTracksAll,"nTracksAll");
    unsigned int nTracksHighPurity=tree->Get(nTracksHighPurity,"nTracksHighPurity");

    OK=true;
    if(pcp)cout<<"check point trackpurity"<<endl;
    if(nTracksAll>10){
      OK=false;
      if(double(nTracksHighPurity)/nTracksAll > 0.25)OK=true;
    }

    if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
    if(  !globalFlow.keepIf("Scraping_Veto", OK ) && quick ) continue;
    if(pcp)cout<<"pure tracks passed"<<endl;
    if(DoControlPlots && OK)ControlPlots.MakePlots("Scraping_Veto", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================    




    //====================================================================    
    // check vertices
    //====================================================================
    //At least one good vertex
    //good vertex=not fake,|d|<24.  
    vector<int> goodVert;
    if(pcp)cout<<"check point calling vertex"<<endl;     
    OK = vertices_RA4b(tree,goodVert);
    if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
    if(  !globalFlow.keepIf("PV", OK)    && quick ) continue;
    if(pcp)cout<<"check point  vertex called"<<endl;
    if(DoControlPlots && OK)ControlPlots.MakePlots("PV", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================




    //====================================================================    
    // check event quality
    //====================================================================    
    //it only checks hbheNoiseFilterResult
    OK = evtqual_RA4b(tree);
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(pcp)cout<<"check point calling event quality"<<endl;
    if( !globalFlow.keepIf("HBHE", OK)          && quick ) continue;
    if(pcp)cout<<"noise passed"<<endl;
    if(DoControlPlots && OK)ControlPlots.MakePlots("HBHE", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================





    //====================================================================    
    // check CSCHALO
    //====================================================================    
    //it only checks hbheNoiseFilterResult
    OK = cschalo_RA4b(tree);
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if( !globalFlow.keepIf("CSC_HALO", OK)          && quick ) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("CSC_HALO", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================




    //====================================================================    
    // check trackingFailure
    //====================================================================    
    //it only checks hbheNoiseFilterResult
    OK = trackingFailure_RA4b(tree);
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if( !globalFlow.keepIf("trackingFailure", OK)          && quick ) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("trackingFailure", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================




    //====================================================================    
    // check ECAL_TP
    //====================================================================    
    //as of CMSSW427_patch7, RA2TPfilter_cff.py and EcalDeadCellEventFilter_cfi do the 
    //same.
    bool ECAL_TP = tree->Get(ECAL_TP,"ecalDeadCellTPFilterFlag");
    OK=ECAL_TP;
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if( !globalFlow.keepIf("ECAL_TP", OK)          && quick ) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("ECAL_TP", TightMuons, TightElectrons, CleanedJets, PFmet); 
    //====================================================================
    //
    //    treeCuts["Cleaning"]=globalFlow.applyCuts("ECAL_TP trackingFailure CSC_HALO HBHE PV Scraping_Veto");

    //====================================================================    
    // check Mu BEfilter
    //====================================================================    
    //     bool ECAL_TP = tree->Get(ECAL_TP,"ecaldeadcellfilterflag");
    //     OK=ECAL_TP;
    //     if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    //     if( !globalFlow.keepIf("ECAL_TP", OK)          && quick ) continue;
    //====================================================================





    //LEPTON SELECTION//
    string SigMu = config.getString("SignalMuon_Selection","Tight");
    string WidMu = config.getString("WideMuon_Selection","Veto");
    string SigEl = config.getString("SignalElectron_Selection","Tight");
    string WidEl = config.getString("WideElectron_Selection","Veto");



    vector<Muon*> SignalMuons;
    vector<Muon*> WideMuons;
    for (int imu=0; (int)imu<Muons.size();++imu){
      if (Muons.at(imu).IsID(SigMu) ){  
	SignalMuons.push_back(&Muons.at(imu));
      }
      else if (!Muons.at(imu).IsID(SigMu) && Muons.at(imu).IsID(WidMu) ){
	WideMuons.push_back(&Muons.at(imu) ) ;
      }
    }
    vector<Electron*> SignalElectrons;
    vector<Electron*> WideElectrons;
    for (int imu=0; (int)imu<Electrons.size();++imu){
      if (Electrons.at(imu).IsID(SigEl) ){  
	SignalElectrons.push_back(&Electrons.at(imu));
      }
      else if (!Electrons.at(imu).IsID(SigEl) && Electrons.at(imu).IsID(WidEl) ){
	WideElectrons.push_back(&Electrons.at(imu) ) ;
      }
    }

   
    //===============================================
    //SINGLE LEPTON
    //===============================================
    if(pcp){
      cout<<"the sum of the sizes of leptons is "<<(int)SignalElectrons.size()+(int)SignalMuons.size()<<endl;
      cout<<SignalElectrons.size()<<" "<<SignalMuons.size()<<endl;
    }

    OK=SetOfCuts::Leptons.NUM.Examine((int)SignalElectrons.size()+(int)SignalMuons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("One_single_lepton",OK) && quick) continue;    
    
    
    

    //===================================
    //JETS
    //===================================
    //
    globalFlow.keepIf(">=1 jet",CleanedJets.size()>=1);
    globalFlow.keepIf(">=2 jet",CleanedJets.size()>=2);
    globalFlow.keepIf("3>jets",CleanedJets.size()>=3);
    globalFlow.keepIf("3=>jets and 1=>btags",NumberOfbTags>=1);
    globalFlow.keepIf(">=4 jet",CleanedJets.size()>=4);
    globalFlow.keepIf(">=4 and 1=> btagsjeCt",CleanedJets.size()>=4 && NumberOfbTags>=1);
    OK=SetOfCuts::Jets.NUM.Examine(CleanedJets.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Jet_Cuts",OK) && quick) continue;    
    //    events1<<Event<<endl;

    if(DoControlPlots && OK)ControlPlots.MakePlots("Jet_Cuts", SignalMuons, SignalElectrons, CleanedJets, PFmet); 
    //treeCuts["Jet_Cuts"]=OK;



    //==========JET SYSTEMATICS===============
    if (systematics.IsEnabled()){
      if(systematics.DoJetUp()){
	OKsyst=SetOfCuts::Jets.NUM.Examine((int)systematics.GetsysJet("jetup_good_clean").size());
	globalFlow.keepIf("Jet_Cuts_JetUp",OKsyst);
      }
      if(systematics.DoJetUp()){
	OKsyst=SetOfCuts::Jets.NUM.Examine((int)systematics.GetsysJet("jetdown_good_clean").size());
	globalFlow.keepIf("Jet_Cuts_JetDown",OKsyst);
      }
    }
    //===================================








    //=================================
    //MUONS
    //=================================
    //
    //
    OK=SetOfCuts::SignalMuons.NUM.Examine(SignalMuons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Signal_Muons",OK) && quick) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("Signal_Muons", SignalMuons, TightElectrons, CleanedJets, PFmet); 
    //treeCuts["Signal_Muons"]=OK;
    //
    //
    //
    //
    OK=SetOfCuts::SignalElectrons.NUM.Examine(SignalElectrons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Signal_Electrons",OK) && quick) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("Signal_Electrons", SignalMuons, SignalElectrons, CleanedJets, PFmet); 
    //treeCuts["Signal_Electrons"]=OK;
    //
    //
    //
    //    if( OK){
    //      events2<<Event<<endl;
    //    }
    //
    //
    //
    OK=SetOfCuts::WideMuons.NUM.Examine(WideMuons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Wide_Muons",OK) && quick) continue;    
    if(DoControlPlots && OK)ControlPlots.MakePlots("Wide_Muons", WideMuons, TightElectrons, CleanedJets, PFmet); 
    //    treeCuts["Wide_Muons"]=OK;
    //
    //
    //
    //
    OK=SetOfCuts::WideElectrons.NUM.Examine(WideElectrons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Wide_Electrons",OK) && quick) continue;    
    if(DoControlPlots && OK)ControlPlots.MakePlots("Wide_Electrons", SignalMuons, WideElectrons, CleanedJets, PFmet); 
    //treeCuts["Wide_Electrons"]=OK;
    //
    //===================================









    //====================================================================
    //MZ AND RESONANT LEPTONS
    //====================================================================
    //this never returns false, it merely updates the values of
    //isZ and isResonant for the selectedLeptons

    bool doMzandResonance=false;
    if (doMzandResonance){
      if(pcp)cout<<"check xpoint calling mzVeto"<<endl;
      //=======================================================
      //      OK=mzveto_RA4b(tree,Leptons);
      //=======================================================
      if(pcp)cout<<"check point after mzVeto"<<endl;


      if(pcp)cout<<"check point calling resonances"<<endl;
      //=======================================================
      //OK=resonance_RA4b(tree,Leptons);
      //=======================================================
      if(pcp)cout<<"check point after resonances"<<endl;
    }
    //======================================================





    //===================================================================
    //HT
    //===================================================================
    HT=0.0;
    for(int ijet=0;ijet<(int)CleanedJets.size();++ijet){
      HT=HT+CleanedJets.at(ijet)->Pt();	
    }
    
    OK=SetOfCuts::Event.HT.Examine(HT);
    if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("HT", OK ) && quick ) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("HT", SignalMuons, SignalElectrons, CleanedJets, PFmet); 


    if (systematics.IsEnabled()){
      if (systematics.GetSysMap()["jetup"]){
	//cout<<"testing HT = "<<systematics.GetsysHT("jetup")<<endl;
	OKsyst=SetOfCuts::Event.HT.Examine(systematics.GetsysHT("jetup"));
	//cout<<"pointer to the flow = "<<systematics.GetsysFlow("jetup")<<endl;
	globalFlow.keepIf("HT_JetUp",OKsyst);
      }
      if (systematics.GetSysMap()["jetdown"]){
	OKsyst=SetOfCuts::Event.HT.Examine(systematics.GetsysHT("jetdown"));
	globalFlow.keepIf("HT_JetDown",OKsyst);
      }
    }
    //===================================================================








    //====================================================================
    // CHECK MET 
    //====================================================================
    OK = metAndHT_RA4b(tree);
    if(pcp)cout<<"check point MET"<<endl;
    if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
    if(  !globalFlow.keepIf("MET", OK ) && quick ) continue;
    if(pcp)cout<<"check point out of MET_HT"<<endl;
    if(DoControlPlots && OK)ControlPlots.MakePlots("MET", SignalMuons, SignalElectrons, CleanedJets, PFmet); 

    
    if (systematics.IsEnabled()){
      if (systematics.GetSysMap()["jetup"]){
	//	cout<<"feeding up "<<systematics.GetsysMET("jetup")<<endl;
	OKsyst=SetOfCuts::Event.MET.Examine(systematics.GetsysMET("jetup"));
	globalFlow.keepIf("MET_JetUp", OKsyst);
      }
      if (systematics.GetSysMap()["jetdown"]){
	//	cout<<"feeding down "<<systematics.GetsysMET("jetdown")<<endl;
	OKsyst=SetOfCuts::Event.MET.Examine(systematics.GetsysMET("jetdown"));
	globalFlow.keepIf("MET_JetDown", OKsyst);
      }
      if(systematics.GetSysMap()["clustersup"]){
	OKsyst=SetOfCuts::Event.MET.Examine(systematics.GetsysMET("clustersup"));
	globalFlow.keepIf("MET_clustersup", OKsyst);
      }
      if(systematics.GetSysMap()["clustersdown"]){
	OKsyst=SetOfCuts::Event.MET.Examine(systematics.GetsysMET("clustersdown"));
	globalFlow.keepIf("MET_clustersdown", OKsyst);
      }
    }
    //treeCuts["MET"]=OK;
    //cout<<"the met here is "<<MET<<endl;
    //====================================================================








    //===================================================================
    //EXAMINE THE TRIGGERS THAT PASSED ALL THE SELECTION
    //===================================================================
    bool studytriggers=false;
    if(studytriggers){
      bool randombool=triggers_RA4b_frequency(tree,triggernames);
    }
    //===================================================================







    
    //====================COPY THE CUTFLOWS
    vector<TString> globalFlowNames;
    //globalFlowNames.push_back("triggers");
    TString rawString;
    bool OKall=false;
    if(!isquick){
      globalFlowNames.push_back("hcalLaserFilter");
      globalFlowNames.push_back("eeBadSCFilter");
      globalFlowNames.push_back("Scraping_Veto");
      globalFlowNames.push_back("PV");
      globalFlowNames.push_back("HBHE");
      globalFlowNames.push_back("CSC_HALO");
      globalFlowNames.push_back("trackingFailure");
      globalFlowNames.push_back("ECAL_TP");
      globalFlowNames.push_back("Signal_Muons");
      globalFlowNames.push_back("Signal_Electrons");
      globalFlowNames.push_back("Wide_Muons");
      globalFlowNames.push_back("Wide_Electrons");
      globalFlowNames.push_back("One_single_lepton");      
      globalFlowNames.push_back("Jet_Cuts");
      globalFlowNames.push_back("HT");
      globalFlowNames.push_back("MET");

      for(int iname=0;iname<(int)globalFlowNames.size();++iname){
	rawString+=globalFlowNames.at(iname)+" ";
      }
      
      OKall=globalFlow.applyCuts(rawString);
	//events1<<Event<<endl;
      if(pcp){
	cout<<"OKall is "<<OKall<<endl;
      }

    }



    if(systematics.IsEnabled()){

      typedef map<string,bool>::iterator map_it;
      for (map_it iter=systematics.GetSysMap().begin(); iter != systematics.GetSysMap().end(); iter++){
	if(iter->second){
	  if(iter->first=="jetup"){
	    
	    for (int iname=0;iname<(int)globalFlowNames.size();++iname){
	      //COPY THE CUTFLOW
	      if (globalFlowNames.at(iname) == "Jet_Cuts"){
		OK=globalFlow.DidItPass("Jet_Cuts_JetUp");
	      }
	      else if (globalFlowNames.at(iname) == "HT"){
		OK=globalFlow.DidItPass("HT_JetUp");
	      }
	      else if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_JetUp");
	      }
	      else{
		OK=globalFlow.DidItPass(globalFlowNames.at(iname));
	      }
	      //======copy the global flow
	      systematics.GetsysFlow("jetup")->keepIf(globalFlowNames.at(iname), OK);
	      
	    }//iname
	    //cout<<"the jet up cutflow "<<endl;
		
	  }
	  else if(iter->first=="jetdown"){
	    
	    for (int iname=0;iname<(int)globalFlowNames.size();++iname){
	      //COPY THE CUTFLOW
	      if (globalFlowNames.at(iname) == "Jet_Cuts"){
		OK=globalFlow.DidItPass("Jet_Cuts_JetDown");
	      }
	      else if (globalFlowNames.at(iname) == "HT"){
		OK=globalFlow.DidItPass("HT_JetDown");
	      }
	      else if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_JetDown");
	      }
	      else{
		OK=globalFlow.DidItPass(globalFlowNames.at(iname));
	      }
	      //======copy the global flow
	      systematics.GetsysFlow("jetdown")->keepIf(globalFlowNames.at(iname), OK);
	    }
	  }
	  else if(iter->first=="clustersup"){
	    for (int iname=0;iname<(int)globalFlowNames.size();++iname){
	      //COPY THE CUTFLOW
	      if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_clustersup");
	      }
	      else{
		OK=globalFlow.DidItPass(globalFlowNames.at(iname));		
	      }
	      systematics.GetsysFlow("clustersup")->keepIf(globalFlowNames.at(iname), OK);
	    }
	  }
	  else if(iter->first=="clustersdown"){
	    for (int iname=0;iname<(int)globalFlowNames.size();++iname){
	      //COPY THE CUTFLOW
	      if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_clustersdown");
	      }
	      else{
		OK=globalFlow.DidItPass(globalFlowNames.at(iname));		
	      }
	      //======copy the global flow
	      systematics.GetsysFlow("clustersdown")->keepIf(globalFlowNames.at(iname), OK);
	    }
	  }
	  //======APPLY THE CUTFLOW
	  //cout<<"applying the cutflow to"<<iter->first<<endl;
	  OK=systematics.GetsysFlow(iter->first)->applyCuts(EventWeight);
	  //record it
	  systematics.passCuts[iter->first]=OK;
	}
      }

    }//systematics.IsEnabled()


    

    
    //avoid having the first iteration entering here when running in 
    //quick mode
    if(isquick && i==0 && !OK)continue;
























    //LorentzM& PFmet = tree->Get(&PFmet, "metP4PF");
    METSig = MET / sqrt(HT); 


    EventInfo info;
    info.Event=Event;
    info.Run=Run;
    info.EventWeight=EventWeight;
    info.PUWeight=PUWeight;




    //=====================FILL THE TREES====================
    //
    //
    //
    
    if (isquick || (!isquick && OKall)){
      if(doSmallTree){
	subTree->Fill( &info, tree, SignalMuons, SignalElectrons, CleanedJets, PFmet);
      }
    }
	

    //====================SYSTEMATICS TREES
    if( systematics.IsEnabled()){
      typedef map<string,bool>::iterator map_it;
      for (map_it iter=systematics.GetSysMap().begin(); iter != systematics.GetSysMap().end(); iter++){
	if(iter->second){    
	  if(systematics.passCuts.at(iter->first)){
	    systematics.GetsysDefaultTree(iter->first)->Fill(&info, tree, SignalMuons,SignalElectrons,systematics.GetsysJet("jetup_good_clean"),*systematics.GetsysMEVector(iter->first));
	  }
	}
      }
    }

    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   


    //                    A  B  C  D   Categorization                     //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   





    if(HT< HTmin)continue;
    if(METSig <METSig_min)continue;


    HT_METSig->Fill(HT,METSig,EventWeight);
      
    const char* Region;
    if (  HT < HTmax ){
      Region="A";
      if ( METSig > METSig_max)Region="C";
    }else{
      Region="B";
      if( METSig > METSig_max)Region="D";
    }


    if( Region=="A"){
      RegionA->Fill(1);
    }else if(Region=="B"){
      RegionB->Fill(1);

    }else if(Region=="C"){
      RegionC->Fill(1);

    }else if(Region=="D"){
      RegionD->Fill(1);
    }




    //============================================================
    //
    //     ONE  B-TAG
    //   
    //============================================================
    if(NumberOfbTags==1){
      onebtagdir->cd();
      if( Region=="A"){
	OneBtag_1DPlots["RegionA"]->Fill(1);
      }else if(Region=="B"){
	OneBtag_1DPlots["RegionB"]->Fill(1);
	
      }else if(Region=="C"){
	OneBtag_1DPlots["RegionC"]->Fill(1);
	
      }else if(Region=="D"){
	OneBtag_1DPlots["RegionD"]->Fill(1);
      }
      OneBtag_2DPlots["HT_METSig"]->Fill(HT,METSig);
    }


    //============================================================
    //
    //      TWO B-TAG
    //   
    //============================================================
    else if(NumberOfbTags==2){
      twobtagdir->cd();
      if( Region=="A"){
	TwoBtag_1DPlots["RegionA"]->Fill(1);
      }else if(Region=="B"){
	TwoBtag_1DPlots["RegionB"]->Fill(1);
	
      }else if(Region=="C"){
	TwoBtag_1DPlots["RegionC"]->Fill(1);
	
      }else if(Region=="D"){
	TwoBtag_1DPlots["RegionD"]->Fill(1);
      }
      TwoBtag_2DPlots["HT_METSig"]->Fill(HT,METSig);
    }

    //============================================================
    //
    //     TWO OR MORE
    //   
    //============================================================
    else if(NumberOfbTags>2){
      twoplusbtagdir->cd();
      if( Region=="A"){
	TwoplusBtag_1DPlots["RegionA"]->Fill(1);
      }else if(Region=="B"){
	TwoplusBtag_1DPlots["RegionB"]->Fill(1);
	
      }else if(Region=="C"){
	TwoplusBtag_1DPlots["RegionC"]->Fill(1);
	
      }else if(Region=="D"){
	TwoplusBtag_1DPlots["RegionD"]->Fill(1);
      }
      TwoplusBtag_2DPlots["HT_METSig"]->Fill(HT,METSig);
    }


  

    

    
  }//End of the event loop
     
  if(pcp)cout<<"out of the event loop"<<endl;





  if(systematics.IsEnabled()){
    
    typedef map<string,bool>::iterator map_it;
    for (map_it iter=systematics.GetSysMap().begin(); iter != systematics.GetSysMap().end(); iter++){
      if(iter->second){
	//set a different directory for this
	systematics.GetsysFlow(iter->first)->setTDir((TString)iter->first);
	systematics.GetsysFlow(iter->first)->dumpToHist();
      }
    }
  }
  subTree->Write();
  //==========WRITE THE TREES
  if(systematics.IsEnabled()){
    typedef map<string,bool>::iterator map_it;
    for (map_it iter=systematics.GetSysMap().begin(); iter != systematics.GetSysMap().end(); iter++){
      if(iter->second){
	systematics.GetsysDefaultTree((string)iter->first)->Write();
      }
    }
  }
  treeFile->Close();



  //==================================================
  //REST OF THE HISTOGRAMS
  //==================================================
  CutSet::setTDir("CutFlow");
  outfile->cd();
  TListIter* list = new TListIter(gDirectory->GetList()); //-->iterates over list of objects in memory
  TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
  while(nobj){

    if(nobj->IsA()->InheritsFrom("TH1")){
      nobj->Write();
      //nobj->Write("",TObject::kOverwrite) -->Overwrites the histogram if alredy present
    }
    //delete(nobj);
    //TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
    nobj=list->Next();   //nobj point now to the next object
  }
  delete(list);
  //==================================================


  //mytree->Write();


  //write the control plots

  //other dir
  onebtagdir->Write();
  twobtagdir->Write();
  twoplusbtagdir->Write();


  config.printUsed();
  //  globalFlow.printAll();
  globalFlow.dumpToHist(); 
  



  
  //  ControlPlots.HistoMaker::~HistoMaker();
  //  globalFlow.CutSet::~CutSet();
  //outfile->Close();
  //  events1.close();
  //  events2.close();
  //  onejet.close();
  //  twojet.close();
  //  threejet.close();
  
  //  treeFile->cd();

  


  //PROGRAM END
  //EndOfProgram:
  
  
  return 0;
}
   
