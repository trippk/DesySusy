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
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
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
#include "TagEff.h"
#include "genJet.h"
#include <boost/shared_ptr.hpp>
#include "typedefs.h"
#include "trigStudyTree.h"


using namespace std;
using namespace ROOT::Math::VectorUtil;
//===================================================================
//                   GLOBAL VARIABLES
//===================================================================

bool pcp = false; //Set to true for debugging.
TH1D* JetUncDiff= new TH1D("JetUncDiff","diff in jet uncertainty",50,-0.02,0.02);


//Globals to remove
//bool checkthisevent=false;
//vector<string> triggernames;
//vector<string> triggernames_short;
//double EventWeight;



//================================================================================
int main(int argc, char** argv){

  //PCP=TRUE FOR DEBUGGING
  pcp=false;


  double EventWeight=1.0;

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
  cout<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<"The output file name is "<<outname<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<endl;
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

  //==============SUBTREE
  //
  TFile *treeFile=0;
  subTree* SubTree=0;
  bool doSubTree=config.getBool("doSubTree",true);
  if(doSubTree){
    treeFile=TFile::Open((TString)treeFileName,"RECREATE");
    TString treeType = config.getTString("treeType","default"); 
    SubTree= subTreeFactory::NewTree(treeType,treeFile,(string)"");
  }
  //==========================================


  //==============SUBTREE FOR THE TRIGGER STUDY
  bool doTrigStudy=config.getBool("doTrigStudy",true);
  trigStudyTree* TrigStudyTree=0;
  if(TrigStudyTree){
    if(!treeFile) treeFile=TFile::Open((TString)treeFileName,"RECREATE");
    TrigStudyTree = (trigStudyTree*)subTreeFactory::NewTree("trigStudyTree",treeFile,(string)"");
  }
  //===========================================

  outfile->cd();  




  //data or monte carlo?
  bool isData=config.getBool("isData");
  //=================================================================



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

  //Tag
  int bin=0;
  bin = config.getInt("lasttagbin",3); // set the lasttagbin i.e 3 to get tag weightings for 0,1,2,3+
  //TagEff tag(WhatSample,WhatSubSample);
  //tag.lastbin(bin);





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
  


  //=================================
  //SET THE CONTROL PLOTS HISTO MAKER
  //================================ 
  bool DoControlPlots=config.getBool("DoControlPlots",true); 
  //HistoMaker::setTFile(outfile); 
  HistoMaker ControlPlots("AnalyzeSUSY"," "); 
  ControlPlots.setTFile(outfile);
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
  bool turntriggersoff=config.getBool("TurnTriggersOff",false);
  if(turntriggersoff){
    cout<<"-----------TURNTRIGGERSOFF IS true!!-----------"<<endl;
    if(isData){
      cout<<endl;
      cout<<endl;
      cout<<"--------NO TRIGGERS, NO DATA------------------"<<endl;
      cout<<endl;
      cout<<endl;
      exit(1);
    }
  }
  //===========================================





  TH1D* EW_AfterTrigger= new TH1D("EW_AfterTrigger","Event weight after the trigger",30,0.0,100.0);
  TH1D* EW_AfterPU=new TH1D("EW_AfterPU","Event Weight after PU RW",100,0.0,10.0);
  TH1D* checkMET= new TH1D("checkMET","check of the MET",30,0.0,300.0);



  if(pcp)cout<<"check point before the event loop"<<endl;

  bool isquick=config.getBool("quick",true);
  bool quick=false;
  cout<<endl;
  cout<<endl;
  cout<<"isquick is"<<isquick<<endl;
  cout<<endl;
  cout<<endl;

 









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
    systematics.AddUncertainty((string)"jetResup",treeFile);
    systematics.AddUncertainty((string)"jetResdown",treeFile);
  }


  //=============INITIALIZE THE JETUncertainty object
  typedef boost::shared_ptr<JetCorrectorParameters> JetCorrectorParam_Ptr;
  // Instantiate uncertainty sources
  const int nsrc = 16;
  const char* srcnames[nsrc] =
    {"Absolute", "HighPtExtra", "SinglePion", "Flavor", "Time",
     "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF",
     "RelativeStatEC2", "RelativeStatHF", "RelativeFSR",
     "PileUpDataMC", "PileUpOOT", "PileUpPt", "PileUpBias", "PileUpJetRate"};

  string jetuncfile;
  for (int isrc = 0; isrc < nsrc; isrc++) {
    const char *name = srcnames[isrc];
    
    bool isBatchJob=config.getBool("isBatchJob",false);
    std::string test1="src/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
    std::string test2="../../src/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
    
    if(!isBatchJob){
      jetuncfile=test1;
    }else{
      jetuncfile=test2;
    }

    JetCorrectorParameters *p;
    if (systematics.IsEnabled()){
      p= new JetCorrectorParameters(jetuncfile, name);
      systematics.vsrc.push_back(new JetCorrectionUncertainty(*p));
    }
  }
  //==========Total uncertainty for reference
  //  std::string test  ="/tmp/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
  //std::string test="src/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";

  if(systematics.IsEnabled()){
    systematics.total = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jetuncfile, "Total")));
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
    //if(i>2000)continue;

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

    
    //clear all the vectors inside systematics
    //otherwise they accumulate
    if (systematics.IsEnabled())systematics.Reset();


    //    pcp=true;
    //
    //    if (i>10){
    //      pcp=false;
    //      continue;
    //    }
    //    cout<<Event<<endl;


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
    double PUWeight=1;
    int relevantNumPU=0;
    if(!isData) {
      float PUnumInter    = tree->Get( PUnumInter, "pileupTrueNumInteractionsBX0");
      relevantNumPU = (int) PUnumInter;
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
    HistoMaker::global_event_weight = EventWeight; //Set the weight to be used in the HistoMaker class
    //==============================================


    if(pcp){
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
    vector<Muon*> pMuons;
    vector<Muon*> LooseMuons;
    vector<Muon*> TightMuons;
    vector<Muon*> VetoMuons;
    Muons=makeAllMuons(tree);
    makeTightMuons(tree,Muons,TightMuons);
    makeLooseMuons(tree,Muons,LooseMuons);
    makeVetoMuons(tree,Muons,VetoMuons);
    for (int imu=0; imu<Muons.size();++imu){
      pMuons.push_back(&Muons.at(imu));
    }
    //  makeSoftMuons(tree ,Muons,SoftMuons);
    //============================================


    //============================================
    // Make Electrons
    vector<Electron>  Electrons;
    vector<Electron*> pElectrons;
    vector<Electron*> LooseElectrons;
    vector<Electron*> TightElectrons;
    vector<Electron*> VetoElectrons;
    Electrons=makeAllElectrons(tree);
    //    makeLooseElectrons(tree,Electrons,LooseElectrons);
    makeTightElectrons(tree,Electrons,TightElectrons);
    makeVetoElectrons(tree, Electrons,VetoElectrons);
    for (int iel=0; iel<Electrons.size();++iel){
      pElectrons.push_back(&Electrons.at(iel));
    }
    //============================================



    //============================================    
    // Make Jets
    //cout<<"vector of jets declared"<<endl;
    vector<Ptr_Jet> AllJets;
    vector<Ptr_Jet> GoodJets;
    vector<Ptr_Jet> CleanedJets;
    makeAllJets(tree,AllJets);
    makeGoodJets(tree,AllJets,GoodJets);
    makeCleanedJets( GoodJets, CleanedJets, pMuons, pElectrons);

    //=======MATCHING OF JETS
    vector<Ptr_GenJet> allGenJets;
    if(!isData){
      makeAllGenJets(tree,allGenJets);
      matchGenJets(tree,allGenJets,AllJets);
    }
    //============================================

    //Define TAG algorithm & working points==============================
    static string btagAlgorithm= config.getString("bTagAlgorithm","CSV");
    static string btagWorkingPoint = config.getString("bTagWorkingPoint","Medium");
    int NumberOfbTags=0;
   



    //============================================
    //Make HT AND MHT
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
    //============================================


    if (pcp){
      cout<<endl;
      cout<<"MET = "<<MET<<endl;
      cout<<"HT = "<<HT<<endl;
      cout<<endl;
    }




    //===========================================
    //CALCULATE SYSTEMATICS VARIATIONS: JETS
    //===========================================
    if (systematics.IsEnabled()){


      ShiftJetEnergyScale(tree,systematics,AllJets,pMuons,pElectrons);
      ShiftClustersEnergyScale(tree,systematics,AllJets,pMuons,pElectrons);

      
      if( systematics.GetSysMap()["jetResup"]){
	rescaleJetsJER(AllJets,systematics);
	//=======RESCALE MET
	//=========RESCALE THE MET
	rescaleMET(tree,AllJets,systematics, "jetResup");
	rescaleMET(tree,AllJets,systematics, "jetResdown");

	//rescaleMET(tree,systematics.GetsysJet("jetResup"),systematics,"jetResup");
	//rescaleMET(tree,systematics.GetsysJet("jetResdown"),systematics,"jetResdown");
	//cout<<"size of jetResup jets "<<systematics.GetsysJet("jetResup").size()<<endl;
	//======FEED THE RESCALED JETS TO makeGoodJets
	makeGoodJets(tree,systematics.GetsysJet("jetResup"),systematics.GetsysJet("jetResup_good"));
	makeGoodJets(tree,systematics.GetsysJet("jetResdown"),systematics.GetsysJet("jetResdown_good"));
	//======CLEAN THEM
	makeCleanedJets(systematics.GetsysJet("jetResup_good"),systematics.GetsysJet("jetResup_good_clean"),Muons,Electrons);
	makeCleanedJets(systematics.GetsysJet("jetResdown_good"),systematics.GetsysJet("jetResdown_good_clean"),Muons,Electrons);
	//	cout<<"size of jetResup good jets "<<systematics.GetsysJet("jetResup_good").size()<<endl;
	//=====GET THE NEW HT
	rescaleHT(systematics.GetsysJet("jetResup_good_clean"), systematics, "jetResup");
	rescaleHT(systematics.GetsysJet("jetResdown_good_clean"), systematics, "jetResdown");
	//=======COLLECTION TO BE STORED IN THE TREE
	systematics.SetTreeJetCollection("jetResup","jetResup_good_clean");
	systematics.SetTreeJetCollection("jetResdown","jetResdown_good_clean");
      }
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



    if(DoControlPlots)ControlPlots.MakePlots("Before_CutFlow", TightMuons, TightElectrons, CleanedJets, PFmet); 




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
    //cout<<"the size of ALLJets is -1 "<<AllJets.size()<<endl;       
    if(!globalFlow.keepIf("One_single_lepton",OK) && quick) continue;    


    //number of tags & tag w.
    //NumberOfbTags = tag.JetLoop(CleanedJets); //cout << "number of btags " << NumberOfbTags << endl; // loop over jets to get tag w., return number of btags

    //===================================
    //JETS
    //===================================
    //
    //globalFlow.keepIf(">=1 jet",CleanedJets.size()>=1);
    //globalFlow.keepIf(">=2 jet",CleanedJets.size()>=2);
    //globalFlow.keepIf("3>jets",CleanedJets.size()>=3);
    //globalFlow.keepIf("3=>jets and 1=>btags",NumberOfbTags>=1);
    //globalFlow.keepIf(">=4 jet",CleanedJets.size()>=4);
    //globalFlow.keepIf(">=4 and 1=> btagsjeCt",CleanedJets.size()>=4 && NumberOfbTags>=1);
    OK=SetOfCuts::Jets.NUM.Examine(CleanedJets.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Jet_Cuts",OK) && quick) continue;    
    //    events1<<Event<<endl;

    if(DoControlPlots && OK)ControlPlots.MakePlots("Jet_Cuts", SignalMuons, SignalElectrons, CleanedJets, PFmet); 
    //treeCuts["Jet_Cuts"]=OK;



    //==========JET SYSTEMATICS===============
    if (systematics.IsEnabled()){      
      if(systematics.GetSysMap()["jetup"]){
	OKsyst=SetOfCuts::Jets.NUM.Examine((int)systematics.GetsysJet("jetup_good_clean").size());
	globalFlow.keepIf("Jet_Cuts_jetup",OKsyst);
      }
      if(systematics.GetSysMap()["jetdown"]){
	OKsyst=SetOfCuts::Jets.NUM.Examine((int)systematics.GetsysJet("jetdown_good_clean").size());
	globalFlow.keepIf("Jet_Cuts_jetdown",OKsyst);
      }
      if(systematics.GetSysMap()["jetResup"]){
	OKsyst=SetOfCuts::Jets.NUM.Examine((int)systematics.GetsysJet("jetResup_good_clean").size());
	globalFlow.keepIf("Jet_Cuts_jetResup",OKsyst);
      }
      if(systematics.GetSysMap()["jetResdown"]){
	OKsyst=SetOfCuts::Jets.NUM.Examine((int)systematics.GetsysJet("jetResdown_good_clean").size());
	globalFlow.keepIf("Jet_Cuts_jetResdown",OKsyst);
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

    //cout<<"the size of ALLJets is "<<AllJets.size()<<endl;   
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





    //cout<<"the size of ALLJets is 1 "<<AllJets.size()<<endl;   



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


    //cout<<"the size of ALLJets is 2 "<<AllJets.size()<<endl;   

    if (systematics.IsEnabled()){
      if (systematics.GetSysMap()["jetup"]){
	//cout<<"testing HT = "<<systematics.GetsysHT("jetup")<<endl;
	OKsyst=SetOfCuts::Event.HT.Examine(systematics.GetsysHT("jetup"));
	//cout<<"pointer to the flow = "<<systematics.GetsysFlow("jetup")<<endl;
	globalFlow.keepIf("HT_jetup",OKsyst);
      }
      if (systematics.GetSysMap()["jetdown"]){
	OKsyst=SetOfCuts::Event.HT.Examine(systematics.GetsysHT("jetdown"));
	globalFlow.keepIf("HT_jetdown",OKsyst);
      }
      if (systematics.GetSysMap()["jetResup"]){
	OKsyst=SetOfCuts::Event.HT.Examine(systematics.GetsysHT("jetResup"));
	globalFlow.keepIf("HT_jetResup",OKsyst);
      }
      if (systematics.GetSysMap()["jetResdown"]){
	OKsyst=SetOfCuts::Event.HT.Examine(systematics.GetsysHT("jetResdown"));
	globalFlow.keepIf("HT_jetResdown",OKsyst);
      }
    }
    //===================================================================







    //cout<<"the size of ALLJets is 3 "<<AllJets.size()<<endl;   

    //====================================================================
    // CHECK MET 
    //====================================================================
    OK = metAndHT_RA4b(tree);
    if(pcp)cout<<"check point MET"<<endl;
    if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
    if(  !globalFlow.keepIf("MET", OK ) && quick ) continue;
    if(pcp)cout<<"check point out of MET_HT"<<endl;
    if(DoControlPlots && OK)ControlPlots.MakePlots("MET", SignalMuons, SignalElectrons, CleanedJets, PFmet); 

    //SYSTEMATICS- CUT ON MET    
    if (systematics.IsEnabled()){
      typedef map<string, bool>::iterator iter;
      for(iter it=systematics.GetSysMap().begin(); it!=systematics.GetSysMap().end();++it){
	OKsyst=false;
	if(it->second){ //--->this "if" is redundant in principle//
	  OKsyst=SetOfCuts::Event.MET.Examine(systematics.GetsysMET(it->first));
	  globalFlow.keepIf("MET_"+it->first, OKsyst);
	}
      }
    }
    //====================================================================





    //===================================================================
    //EXAMINE THE TRIGGERS THAT PASSED ALL THE SELECTION
    //===================================================================
    bool studytriggers=false;
    if(studytriggers){
      bool randombool=triggers_RA4b_frequency(tree,triggernames);
    }
    //===================================================================






















    
    //-- Selection and tree-output for the triggerStudy: Mu
    if(mySampleInfo.GetEstimation()=="TrigStudy-mu" && OK){
            if(TightElectrons.size() > 0) cout<<"TightElectrons.size() = " << TightElectrons.size() << endl;
      bool foundDiLepton=false;
      //cut: check if there is an OS muon pair with 60 < Minv < 120
      if(TightMuons.size()<2) foundDiLepton=false;
      else{
	for(Int_t i=0,N=TightMuons.size();i<N-1; ++i){
	  for(Int_t j=i+1; j<N; ++j){
	    if(TightMuons.at(i)->Charge() == TightMuons.at(j)->Charge()) continue;
	    double diLepMass = (TightMuons.at(i)->P4()+TightMuons.at(j)->P4()).M();
	    if(diLepMass > 60. && diLepMass < 120.) foundDiLepton=true;
	  }
	  if(foundDiLepton) break;
	}
      }
      OK=foundDiLepton;

      vector<string> ElMatchedTriggerFilter; //no electron matching info is needed for the TrigStudy-mu selection
      map<string,int> HLTprescaled          = tree->Get(&HLTprescaled, "prescaled");
      map<string,bool> HLTtrigger           = tree->Get(&HLTtrigger, "triggered");
      vector<string> MuMatchedTriggerFilter = tree->Get(&MuMatchedTriggerFilter, "DESYtriggerMuMatchedTriggerFilter");
      map<string,string> TriggerMap         = tree->Get(&TriggerMap, "DESYtriggerNameMap");
      //--------------- keep only the trigger Info for the Tight Leptons 
      //(therefore, the trigger info of other leptons is not saved into the output tree)
      vector<string> keepMatchingInfoForTightMuon;
      for(Int_t i=0,N=TightMuons.size(); i<N; ++i){
	Int_t indx = TightMuons.at(i)->GetIndexInTree();
	if(indx>= MuMatchedTriggerFilter.size()){
	  cout<<"WARNING: Asked for Trigger matching info that does not exist!"<<endl;
	  cout<<"MuMatchedTriggerFilter.size() = " << MuMatchedTriggerFilter.size() << endl;
	  cout<<"Asking for element " << indx << endl;	  
	}
	else keepMatchingInfoForTightMuon.push_back(MuMatchedTriggerFilter.at(indx));
      }
      MuMatchedTriggerFilter = keepMatchingInfoForTightMuon;
      
      //small consistency check
      if(keepMatchingInfoForTightMuon.size() != TightMuons.size()){
	cout<<"WARNING: Size of muon vector does not correspond to size of matching info vector!"<<endl;
	cout<<"TightMuons.size()                   = " << TightMuons.size() << endl;
	cout<<"keepMatchingInfoForTightMuon.size() = " << keepMatchingInfoForTightMuon.size() << endl;
      }
      if(TightElectrons.size()){
	cout<<"WARNING: Size of muon vector is not zero as it ought to be: "<< TightElectrons.size() << endl;
      }
      //cout<<"======================================"<<endl;
      //cout<<"Size: matchFilter | TightMuons | keepFilters = " 
      //<< MuMatchedTriggerFilter.size() << " | "
      //<< TightMuons.size() << " | "
      //<< keepMatchingInfoForTightMuon.size()
      //<<endl;

      //fill the leafs which will be saved in TrigStudyTree
      EventInfo myInfo;
      myInfo.HLTprescaled           = HLTprescaled;
      myInfo.HLTtrigger             = HLTtrigger;
      myInfo.ElMatchedTriggerFilter = ElMatchedTriggerFilter;
      myInfo.Event                  = Event;
      myInfo.EventWeight            = EventWeight;
      myInfo.MuMatchedTriggerFilter = MuMatchedTriggerFilter;
      myInfo.Run                    = Run;
      if(!isData) myInfo.PUInter    = relevantNumPU;
      else        myInfo.PUInter    = goodVert.size();
      myInfo.PUWeight               = PUWeight;
      myInfo.TriggerMap             = TriggerMap;
      
      if(!globalFlow.keepIf("OS muon pair found", OK ) && quick ) continue;
//       cout<<"my info blabal"<<myInfo.ElMatchedTriggerFilter.size()<<endl;
      TrigStudyTree->Fill(&myInfo, tree, TightMuons, TightElectrons, CleanedJets, PFmet);

      //cout<<"---------------------------------------------------------" <<endl;
      //       cout<<"ElMatchedTriggerFilter.size() = " << ElMatchedTriggerFilter.size() << endl;
      //       cout<<"MuMatchedTriggerFilter.size() = " << MuMatchedTriggerFilter.size() << endl;
      //       cout<<"HLTtrigger.size()             = " << HLTtrigger.size() << endl;
      //       cout<<"HLTprescaled.size()           = " << HLTprescaled.size() << endl;
      //       cout<<"TriggerMap.size()             = " << TriggerMap.size() << endl;
      //       for(map<string, bool>::iterator it=HLTtrigger.begin(); it != HLTtrigger.end(); ++it)    cout<<setw(80)<< it->first << " = " << it->second << endl;
      //       cout<<endl;
      //       for(map<string, int>::iterator it=HLTprescaled.begin(); it != HLTprescaled.end(); ++it) cout<<setw(80)<< it->first << " = " << it->second << endl;
      //       cout<<endl;
      //       for(map<string, string>::iterator it=TriggerMap.begin(); it != TriggerMap.end(); ++it)  cout<<setw(80)<< it->first << " = " << it->second << endl;
      //       cout<<endl;
    }
    
        //------------------------------------------------------------------- Selection and tree-output for the triggerStudy: El
    if(mySampleInfo.GetEstimation()=="TrigStudy-el" && OK){
      //cut: check if there is an OS electron pair with 60 < Minv < 120
      bool foundDiLepton=false;
      if(TightElectrons.size()<2) foundDiLepton=false;
      else{
	for(Int_t i=0,N=TightElectrons.size();i<N-1; ++i){
	  for(Int_t j=i+1; j<N; ++j){
	    if(TightElectrons.at(i)->Charge() == TightElectrons.at(j)->Charge()) continue;
	    double diLepMass = (TightElectrons.at(i)->P4()+TightElectrons.at(j)->P4()).M();
	    if(diLepMass > 60. && diLepMass < 120.) foundDiLepton=true;
	  }
	  if(foundDiLepton) break;
	}
      }
      OK=foundDiLepton;
      
      vector<string> ElMatchedTriggerFilter = tree->Get(&ElMatchedTriggerFilter, "DESYtriggerElMatchedTriggerFilter");
      map<string,int> HLTprescaled          = tree->Get(&HLTprescaled, "prescaled");
      map<string,bool> HLTtrigger           = tree->Get(&HLTtrigger, "triggered");
      vector<string> MuMatchedTriggerFilter; //no muon matching info is needed for the TrigStudy-el selection
      map<string,string> TriggerMap         = tree->Get(&TriggerMap, "DESYtriggerNameMap");
      
      //--------------------------- keep only the trigger Info for the Tight Leptons 
      //(therefore, the trigger info of other leptons is not saved into the output tree)
      vector<string> keepMatchingInfoForTightElectron;
      for(Int_t i=0,N=TightElectrons.size(); i<N; ++i){
	Int_t indx = TightElectrons.at(i)->GetIndexInTree();
	if(indx>= ElMatchedTriggerFilter.size()){
	  cout<<"WARNING: Asked for Trigger matching info that does not exist!"<<endl;
	  cout<<"ElMatchedTriggerFilter.size() = " << ElMatchedTriggerFilter.size() << endl;
	  cout<<"Asking for element " << indx << endl;	  
	}
	else keepMatchingInfoForTightElectron.push_back(ElMatchedTriggerFilter.at(indx));
      }
      ElMatchedTriggerFilter = keepMatchingInfoForTightElectron;

      //small consistency check
      if(keepMatchingInfoForTightElectron.size() != TightElectrons.size()){
	cout<<"WARNING: Size of electron vector does not correspond to size of matching info vector!"<<endl;
	cout<<"TightElectrons.size()                   = " << TightElectrons.size() << endl;
	cout<<"keepMatchingInfoForTightElectron.size() = " << keepMatchingInfoForTightElectron.size() << endl;
      }
      if(TightMuons.size()){
	cout<<"WARNING: Size of muon vector is not zero as it ought to be: "<< TightMuons.size() << endl;
      }
      //fill the leafs which will be saved in TrigStudyTree
      EventInfo myInfo;
      myInfo.HLTprescaled           = HLTprescaled;
      myInfo.HLTtrigger             = HLTtrigger;
      myInfo.ElMatchedTriggerFilter = ElMatchedTriggerFilter;
      myInfo.Event                  = Event;
      myInfo.EventWeight            = EventWeight;
      myInfo.MuMatchedTriggerFilter = MuMatchedTriggerFilter;
      if(!isData) myInfo.PUInter    = relevantNumPU;
      else        myInfo.PUInter    = goodVert.size();
      myInfo.Run                    = Run;
      myInfo.PUWeight               = PUWeight;
      myInfo.TriggerMap             = TriggerMap;
      
      if(!globalFlow.keepIf("OS electron pair found", OK ) && quick ) continue;
      TrigStudyTree->Fill(&myInfo, tree, TightMuons, TightElectrons, CleanedJets, PFmet);
      
      //cout<<"---------------------------------------------------------" <<endl;
      //       cout<<"ElMatchedTriggerFilter.size() = " << ElMatchedTriggerFilter.size() << endl;
      //       cout<<"MuMatchedTriggerFilter.size() = " << MuMatchedTriggerFilter.size() << endl;
      //       cout<<"HLTtrigger.size()             = " << HLTtrigger.size() << endl;
      //       cout<<"HLTprescaled.size()           = " << HLTprescaled.size() << endl;
      //       cout<<"TriggerMap.size()             = " << TriggerMap.size() << endl;
      //       for(map<string, bool>::iterator it=HLTtrigger.begin(); it != HLTtrigger.end(); ++it)    cout<<setw(80)<< it->first << " = " << it->second << endl;
      //       cout<<endl;
      //       for(map<string, int>::iterator it=HLTprescaled.begin(); it != HLTprescaled.end(); ++it) cout<<setw(80)<< it->first << " = " << it->second << endl;
      //       cout<<endl;
      //       for(map<string, string>::iterator it=TriggerMap.begin(); it != TriggerMap.end(); ++it)  cout<<setw(80)<< it->first << " = " << it->second << endl;
      //       cout<<endl;
    }












    
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
		OK=globalFlow.DidItPass("Jet_Cuts_jetup");
	      }
	      else if (globalFlowNames.at(iname) == "HT"){
		OK=globalFlow.DidItPass("HT_jetup");
	      }
	      else if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_jetup");
		//cout<<"did it pass "<<OK<<endl;
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
		OK=globalFlow.DidItPass("Jet_Cuts_jetdown");
	      }
	      else if (globalFlowNames.at(iname) == "HT"){
		OK=globalFlow.DidItPass("HT_jetdown");
	      }
	      else if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_jetdown");
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
	  else if(iter->first=="jetResup"){
	    for (int iname=0;iname<(int)globalFlowNames.size();++iname){
	      //COPY THE CUTFLOW
	      if (globalFlowNames.at(iname) == "Jet_Cuts"){
		OK=globalFlow.DidItPass("Jet_Cuts_jetResup");
	      }
	      else if (globalFlowNames.at(iname) == "HT"){
		OK=globalFlow.DidItPass("HT_jetResup");
	      }
	      else if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_jetResup");
	      }
	      else{
		OK=globalFlow.DidItPass(globalFlowNames.at(iname));
	      }
	      //======copy the global flow
	      systematics.GetsysFlow("jetResup")->keepIf(globalFlowNames.at(iname), OK);
	    }
	  }
	  else if(iter->first=="jetResdown"){
	    for (int iname=0;iname<(int)globalFlowNames.size();++iname){
	      //COPY THE CUTFLOW
	      if (globalFlowNames.at(iname) == "Jet_Cuts"){
		OK=globalFlow.DidItPass("Jet_Cuts_jetResdown");
	      }
	      else if (globalFlowNames.at(iname) == "HT"){
		OK=globalFlow.DidItPass("HT_jetResdown");
	      }
	      else if (globalFlowNames.at(iname) == "MET"){
		OK=globalFlow.DidItPass("MET_jetResdown");
	      }
	      else{
		OK=globalFlow.DidItPass(globalFlowNames.at(iname));
	      }
	      //======copy the global flow
	      systematics.GetsysFlow("jetResdown")->keepIf(globalFlowNames.at(iname), OK);
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



    EventInfo info;
    info.Event=Event;
    info.Run=Run;
    info.EventWeight=EventWeight;
    info.PUWeight=PUWeight;


    //cout<<"the size of ALLJets is 5 "<<AllJets.size()<<endl;   

    //=====================FILL THE TREES====================
    //
    //
    //
    
    if (isquick || (!isquick && OKall)){
      if(doSubTree){
	vector<Jet*> pCleanedJets;
	for (int ij=0;ij<CleanedJets.size();++ij){
	  pCleanedJets.push_back(CleanedJets.at(ij).get());
	}
	SubTree->Fill( &info, tree, SignalMuons, SignalElectrons, pCleanedJets, PFmet);
      }
    }


	
    //====================SYSTEMATICS TREES
    if( systematics.IsEnabled()){
      typedef map<string,bool>::iterator map_it;
      for (map_it iter=systematics.GetSysMap().begin(); iter != systematics.GetSysMap().end(); iter++){
	if(iter->second){    
	  if(systematics.passCuts.at(iter->first)){
	    systematics.GetsysDefaultTree(iter->first)->Fill(&info, tree, SignalMuons,SignalElectrons,systematics.GetsysJet("jetup_good_clean"),*systematics.GetsysMETVector(iter->first));
	  }
	}
      }
    }


    //cout<<"the size of ALLJets is 6 "<<AllJets.size()<<endl;   

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
  //subTree->Write();
  //==========WRITE THE TREES
  //   if(systematics.IsEnabled()){
  //     typedef map<string,bool>::iterator map_it;
  //     for (map_it iter=systematics.GetSysMap().begin(); iter != systematics.GetSysMap().end(); iter++){
  //       if(iter->second){
  // 	systematics.GetsysDefaultTree((string)iter->first)->Write();
  //       }
  //     }
  //   }
  treeFile->Write(); //Write all hists and tree associated with treeFile
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

  
  //distmatched->Write();
  //mytree->Write();
  JetUncDiff->Write();
  
  //write the control plots

  //other dir
  //tag.finalize(); //write the files
  //config.printUsed();
  //  globalFlow.printAll();
  globalFlow.dumpToHist(); 
  
  
  //systematics.~Systematics();
  
  
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

