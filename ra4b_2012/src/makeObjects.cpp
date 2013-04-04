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
#include "makePileUp.h"
#include "simpleElectron.h"
#include "simpleMuon.h"
#include "simpleJet.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;
//===================================================================
//                   GLOBAL VARIABLES
//===================================================================

bool pcp = false; //Set to true for debugging.




//================================================================================
int main(int argc, char** argv){

  //PCP=TRUE FOR DEBUGGING
  pcp=false;


  double EventWeight=1.0;
  string MainDir="./";


  //========CONFIG READER
  ConfigReader config(MainDir+"config.txt",argc,argv);
  config.Add(MainDir+"para_config.txt");
  config.Add(MainDir+"pu_config.txt");
  

  //========NAME OF THE FILE
  TString filename = config.getTString("filename");
  EasyChain* tree = new EasyChain("/susyTree/tree");
  int f = tree->AddSmart(filename);

  //========NAME OF THE OUTPUT FILE
  TString outname = config.getTString("outname",tree->GetUniqeName());
  string outname_string=(string)outname;
  TFile *outfile = TFile::Open(outname,"RECREATE");
  outfile->cd();    
  cout<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<"The output file name is "<<outname<<endl;
  cout<<"-----------------------------------------------"<<endl;
  cout<<endl;


  //data or monte carlo?
  bool isData=config.getBool("isData");
  //=================================================================



  //===========================================================================
  SampleInformation mySampleInfo;

  //it HAS to be read from config.txt, otherwise the default is
  //undefined
  string WhatSample=config.getString("SampleName","undefined");
  string WhatSubSample=config.getString("SubSampleName","undefined");
  string WhatEstimation=config.getString("Estimation","undefined");
  string WhatTail=config.getString("Tail","undefined");
  mySampleInfo.SetInformation(WhatSample,WhatSubSample,WhatEstimation,WhatTail);
  mySampleInfo.DumpInformation();
  //===========================================================================



  if(pcp)cout<<"going to set the triggers "<<endl;
  //===========================================================================
  // SET THE TRIGGERS
  //===========================================================================
  vector<const char*> triggernames;
  //SetTriggers_RA4b()
  SetTriggers_RA4b(mySampleInfo,triggernames);
  
  //===========================================================================
  if(pcp)cout<<"out of set the triggers "<<endl;
  if(pcp)cout<<"check point 1"<<endl;







  //======================================================
  //pileUp Initialization
  //======================================================
  pileUpInfo pileUp;
  if (!isData){ pileUp.Initialize(mySampleInfo);}
  double InitialEventWeight=1.0;


  //what the hell is this? ozgur?
  //Tag
  int bin=0;
  bin = config.getInt("lasttagbin",3); // set the lasttagbin i.e 3 to get tag weightings for 0,1,2,3+
  //TagEff tag(WhatSample,WhatSubSample);
  //tag.lastbin(bin);


  bool quick=config.getBool("quick",false);


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
  if(pcp)cout<<"check point before tree->GetEntries"<<endl;
  //=================================================================
  int N = tree->GetEntries();
  //=================================================================
  cout<<"THERE ARE "<<N<<" EVENTS IN "<<filename<<endl;
  


  //================================================================
  TH1I* isdata = new TH1I("isdata","data =1 means Data",1,0,1);
  if(isData){isdata->SetBinContent(1,1);}
  else{isdata->SetBinContent(1,0);}
  //================================================================


  //================================================================   
  TH1I* num_entries = new TH1I("num_entries","number of entries",1,0,1);
  num_entries->SetBinContent(1,N);
  //================================================================

  //===========================================
  //CUT FLOW MONITORING
  //===========================================
  CutSet globalFlow("global flow");
  CutSet::setTFile(outfile);
  bool OK=false;

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














  //=============SYSTEMATIC UNCERTAINTIES INITIALISATION
  static bool doSystematics=config.getBool("doSystematics",true);

  Systematics systematics;
  if (!quick && doSystematics){
    systematics.AddUncertainty((string)"jetup");
    systematics.AddUncertainty((string)"jetdown");
    systematics.AddUncertainty((string)"clustersup");
    systematics.AddUncertainty((string)"clustersdown");
    systematics.AddUncertainty((string)"jetRescentral");
    systematics.AddUncertainty((string)"jetResup");
    systematics.AddUncertainty((string)"jetResdown");
  }
  //===================================================================
  

  //=======JET ENERGY SCALE UNCERTAINTY INITIALISATION
  JetUncertaintyInitialize(systematics);








  //=============================================================================
  //=============================================================================
  //LOOP OVER EVENTS
  //=============================================================================
  //=============================================================================

  //N=1000;
  cout<<"N? "<<N<<endl;
  for(int i=0;i<N;++i){
    quick=false;
    
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


    //==========RESET SYSTEMATICS    
    if (systematics.IsEnabled())systematics.Reset();


    //==========DEBUGGING:
    if(pcp){
      cout<<endl;
      cout<<endl;
      cout<<"**************************************"<<endl;
      cout<<"NEW EVENT : "<<Event<<endl;
      cout<<"**************************************"<<endl;
      cout<<endl;
      cout<<endl;
    }


    //========== PILE UP RW
    if(!isData){
      pileUp.RescaleWeight(tree,EventWeight,"central");
      CutSet::global_event_weight  = EventWeight;
    }



    if(pcp){
      cout<<"=================================================== "<<endl;
      cout<<"going to check the event "<<Event<<endl;
      cout<<"=================================================== "<<endl;
    }










    //====================================================================
    // APPLYING TRIGGERS
    //====================================================================
    if(!turntriggersoff ){
      OK = triggers_RA4b(tree, triggernames,EventWeight);
      if(pcp)cout<<"check point triggers called"<<endl;
      if( !globalFlow.keepIf("triggers", OK )  && quick ) continue;    
    }
    //====================================================================




    //====================================================================    
    // check vertices
    //====================================================================
    vector<int> goodVert;
    if(pcp)cout<<"check point calling vertex"<<endl;     
    OK = vertices_RA4b(tree,goodVert);
    if(  !globalFlow.keepIf("PV", OK)    && quick ) continue;
    if(pcp)cout<<"check point  vertex called"<<endl;
    //====================================================================








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
    for(int ijet = 0; (int)ijet<AllJets.size(); ijet++){
    }
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
    
    for (int ijet=0;ijet<CleanedJets.size();++ijet){
      if(CleanedJets.at(ijet)->IsBJet("CSV","Medium")){
	NumberOfbTags++;
      }
    }


    //======HT
    double HT=makeHT(CleanedJets);

    
    //============================================
    //Make MET
    LorentzM& PFmet = tree->Get(&PFmet, "metP4TypeIPF");
    double MET=(double)PFmet.Et() ;





    //===========================================
    //CALCULATE SYSTEMATICS VARIATIONS: JETS
    //===========================================
    if (systematics.IsEnabled()){
      ShiftJetEnergyScale(tree,systematics,AllJets,pMuons,pElectrons);
      ShiftClustersEnergyScale(tree,systematics,AllJets,pMuons,pElectrons);
      JetResolutionSmearing(tree,systematics,AllJets,pMuons,pElectrons);
    }

    //======EVENT INFO
    EventInfo info;
    info.Event=Event;
    info.Run=Run;
    info.EventWeight=EventWeight;

    info.PUWeight=pileUp.GetWeight("central");
    info.PUWeight_up=pileUp.GetWeight("up");
    info.PUWeight_down=pileUp.GetWeight("down");
    info.NBtags=NumberOfbTags;
    //
    if(!isData) info.PUInter    = goodVert.size();
    else        info.PUInter    = goodVert.size();
    //
    //




    //==========SIMPLIFY JETS,MUONS AND ELECTRONS========//
    //
    vector<simpleJet> mySimpleJets;
    for (int ijet=0;ijet<AllJets.size();++ijet){
      mySimpleJets.push_back(AllJets.at(ijet)->makeSimpleJet());
    }
    vector<simpleMuon> mySimpleMuons;
    for (int imu=0;imu<Muons.size();++imu){
      mySimpleMuons.push_back(Muons.at(imu).makeSimpleMuon());
    }
    vector<simpleElectron> mySimpleElectrons;
    for (int iel=0;iel<Electrons.size();++iel){
      mySimpleElectrons.push_back(Electrons.at(iel).makeSimpleElectron());
    }
    
    



    //WRITE THE OBJECTS HERE
 
    //return 0;
  }

}
