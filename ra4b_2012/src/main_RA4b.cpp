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
#include "simpleElectron.h"
#include "simpleMuon.h"
#include "simpleJet.h"
#include "treeVariables.h"




using namespace std;
using namespace ROOT::Math::VectorUtil;
//===================================================================
//                   GLOBAL VARIABLES
//===================================================================

bool pcp = false; //Set to true for debugging.



//================================================================================
int main(int argc, char** argv){


  gSystem->Load("libRA4bDict.so");

  void DefineBranches(TTree* tree,TreeVariables& name);

  //PCP=TRUE FOR DEBUGGING
  pcp=true;


  double EventWeight=1.0;

  //CONFIG. The program will always take the file config.txt in the output directory
  //specified in the execution of runOnAll
  string MainDir="./";
  ConfigReader config(MainDir+"config.txt",argc,argv);
  config.Add(MainDir+"para_config.txt");
  config.Add(MainDir+"pu_config.txt");

  //NAME OF THE FILE
  TString filename = config.getTString("filename");
  TFile* infile=new TFile(filename,"READ");
  if (!infile->IsOpen()){
    std::cout<<"not open"<<std::endl;
  }else{
    std::cout<<"it has been opened"<<std::endl;
  }


  TTree* tree= (TTree*)infile->Get("subTree");
  cout<<"pointer found at "<<tree<<endl;
  //  EasyChain* tree = new EasyChain("subTree");
  //TTree* tree = 
    //int f = tree->AddSmart(filename);


  //TString outname = config.getTString("outname",tree->GetUniqeName());
  TString outname= "test.root";
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

  outfile->cd();  
  //=================================================================

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


  //keep track of the weights
  TH1::SetDefaultSumw2(true);
  //=========================




  //===============================================================
  //===============================================================
  //FILE READING (and all that)
  //===============================================================
  //===============================================================
  //files read;
  //cout<<tree->GetNtrees()<<" Files read"<<endl;
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
  //===========================================



  TreeVariables variables;
  //DefineBranches(tree,variables);


  bool quick=config.getBool("quick",true);


  //=============================================================================
  //=============================================================================
  //LOOP OVER EVENTS
  //=============================================================================
  //=============================================================================

  N=10;
  for(int i=0;i<N;++i){

    timer();
 
    
    if(pcp)cout<<"check point about to get entry "<< i<<endl;

    //====================================================
    tree->GetEntry(i);
    //====================================================

    
    unsigned int Event = variables.Event;
    unsigned int Run = variables.Run;

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
    //EventWeight=InitialEventWeight;
    EventWeight=1.0;
    //================================================
    //  cout << "weight before PUrw -> " << EventWeight << endl;

    vector<simpleJet> Jets=*(variables.Jets);
    vector<simpleMuon> Muons=*(variables.Muons);
    vector<simpleElectron> Electrons=*(variables.Electrons);

    //vector<simpleJet>& Jets=tree->Get(&Jets,"Jets");
    //vector<simpleMuon>& Muons=tree->Get(&Muons,"Muons");
    //vector<simpleElectron>& Electrons=tree->Get(&Electrons,"Electrons");
    
    
    //=========GET CLEAN JETS
    vector<simpleJet*> cleanJets;
    vector<string> cleanJets_attributes;
    cleanJets_attributes.push_back("clean");
    makeSimpleJetCollection(cleanJets_attributes,Jets,cleanJets);


    //========GET TIGHT MUONS
    vector<simpleMuon*> TightMuons;
    vector<string> tightmuons_attributes;
    tightmuons_attributes.push_back("Tight");
    makeSimpleMuonCollection(tightmuons_attributes,Muons,TightMuons);


    //========GET TIGHT ELECTRONS
    vector<simpleElectron*> TightElectrons;
    vector<string> tightelectrons_attributes;
    tightelectrons_attributes.push_back("Tight");
    makeSimpleElectronCollection(tightelectrons_attributes,Electrons,TightElectrons);




    
    //============================================
    //Make HT AND MHT
    //============================================
    double HT=0;
    double px=0;
    double py=0;
    double MHT=0;
    for(int ijet=0;ijet<(int)cleanJets.size();++ijet){
      HT=HT+cleanJets.at(ijet)->Pt();
      px+=  cleanJets.at(ijet)->P4().Px();
      py+=  cleanJets.at(ijet)->P4().Py();
    }
    MHT=sqrt(px*px + py*py);
    //============================================

    //============================================
    //Make MET
    LorentzM PFmet = *(variables.PFmet);
    double MET=(double)PFmet.Et() ;
    //============================================

    //RESOLUTION
    //LorentzM& TrueME = tree->Get(&TrueME,"genmetP4True");
    //double TrueMET=TrueME.Pt();
    //METRes->Fill((MET-TrueMET)/TrueMET,EventWeight);




    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //                     C  U  T      F L O W                           //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   



    //if(DoControlPlots)ControlPlots.MakePlots("Before_CutFlow", TightMuons, TightElectrons, cleanJets, PFmet); 



    //LEPTON SELECTION//
    string SigMu = config.getString("SignalMuon_Selection","Tight");
    string WidMu = config.getString("WideMuon_Selection","Veto");
    string SigEl = config.getString("SignalElectron_Selection","Tight");
    string WidEl = config.getString("WideElectron_Selection","Veto");





    vector<simpleMuon*> SignalMuons;
    vector<simpleMuon*> WideMuons;
    for (int imu=0; (int)imu<Muons.size();++imu){
      if (Muons.at(imu).IsID(SigMu) ){  
	SignalMuons.push_back(&Muons.at(imu));
      }
      else if (!Muons.at(imu).IsID(SigMu) && Muons.at(imu).IsID(WidMu) ){
	WideMuons.push_back(&Muons.at(imu) ) ;
      }
    }

    vector<simpleElectron*> SignalElectrons;
    vector<simpleElectron*> WideElectrons;
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
    //cout<<"the size of ALLJets is -1 "<<Jets.size()<<endl;       
    if(!globalFlow.keepIf("One_single_lepton",OK) && quick) continue;    






    //===================================
    //JETS
    //===================================
    //
    OK=SetOfCuts::Jets.NUM.Examine(cleanJets.size());
    if(!globalFlow.keepIf("Jet_Cuts",OK) && quick) continue;    
    //    events1<<Event<<endl;

    //if(DoControlPlots && OK)ControlPlots.MakePlots("Jet_Cuts", SignalMuons, SignalElectrons, cleanJets, PFmet); 






    //=================================
    //MUONS
    //=================================
    //
    //
    OK=SetOfCuts::SignalMuons.NUM.Examine(SignalMuons.size());
    if(!globalFlow.keepIf("Signal_Muons",OK) && quick) continue;
    //if(DoControlPlots && OK)ControlPlots.MakePlots("Signal_Muons", SignalMuons, TightElectrons, cleanJets, PFmet); 
    //
    //
    //
    //
    //
    OK=SetOfCuts::WideMuons.NUM.Examine(WideMuons.size());
    if(!globalFlow.keepIf("Wide_Muons",OK) && quick) continue;    
    //if(DoControlPlots && OK)ControlPlots.MakePlots("Wide_Muons", WideMuons, TightElectrons, cleanJets, PFmet); 
    //
    //
    //
    //



    //================================
    //ELECTRONS
    //================================
    //
    //
    OK=SetOfCuts::SignalElectrons.NUM.Examine(SignalElectrons.size());
    if(!globalFlow.keepIf("Signal_Electrons",OK) && quick) continue;
    //if(DoControlPlots && OK)ControlPlots.MakePlots("Signal_Electrons", SignalMuons, SignalElectrons, cleanJets, PFmet); 
    //
    //
    //
    //
    //
    OK=SetOfCuts::WideElectrons.NUM.Examine(WideElectrons.size());
    if(!globalFlow.keepIf("Wide_Electrons",OK) && quick) continue;    
    //if(DoControlPlots && OK)ControlPlots.MakePlots("Wide_Electrons", SignalMuons, WideElectrons, cleanJets, PFmet); 
    //
    //===================================






    //===================================================================
    //HT
    //===================================================================
    HT=0.0;
    for(int ijet=0;ijet<(int)cleanJets.size();++ijet){
      HT=HT+cleanJets.at(ijet)->Pt();	
    }
    
    OK=SetOfCuts::Event.HT.Examine(HT);
    if(!globalFlow.keepIf("HT", OK ) && quick ) continue;
    //if(DoControlPlots && OK)ControlPlots.MakePlots("HT", SignalMuons, SignalElectrons, cleanJets, PFmet); 
    //===================================================================





    //====================================================================
    // CHECK MET 
    //====================================================================
    OK = SetOfCuts::Event.MET.Examine(MET);
    if(pcp)cout<<"check point MET"<<endl;
    if(  !globalFlow.keepIf("MET", OK ) && quick ) continue;
    if(pcp)cout<<"check point out of MET_HT"<<endl;
    //if(DoControlPlots && OK)ControlPlots.MakePlots("MET", SignalMuons, SignalElectrons, cleanJets, PFmet); 
    //====================================================================

















    /*
    EventInfo info;
    info.Event=Event;
    info.Run=Run;
    info.EventWeight=EventWeight;
    info.PUWeight=PUWeight;
    info.PUWeight_up=PUWeight_up;
    info.PUWeight_down=PUWeight_down;
    info.NBtags=NumberOfbTags;
    //
    if(!isData) info.PUInter    = goodVert.size();
    else        info.PUInter    = goodVert.size();
    //
    //
    */
    
    
  }//End of the event loop
  
  //==================================================
  //REST OF THE HISTOGRAMS
  //==================================================
  CutSet::setTDir("CutFlow");
  outfile->cd();
  cout<<"writing the histos"<<endl;
  TListIter* list = new TListIter(gDirectory->GetList()); //-->iterates over list of objects in memory
  TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
  while(nobj){

    if(nobj->IsA()->InheritsFrom("TH1")){

      nobj->Write();
    }
    nobj=list->Next();   //nobj point now to the next object
  }
  delete(list);
  cout<<"wrote the histos"<<endl;
  //==================================================

  
  globalFlow.dumpToHist(); 
  cout<<"cutflow dumped"<<endl;


  //PROGRAM END
  //EndOfProgram:

  
  return 0;
}





void DefineBranches(TTree* tree,TreeVariables& name){
    
  //GET THE BRANCHES
  tree->SetBranchAddress("Run",&name.Run);
  tree->SetBranchAddress("Jets",&name.Jets);
  tree->SetBranchAddress("Electrons",&name.Electrons);
  tree->SetBranchAddress("Muons",&name.Muons);
  tree->SetBranchAddress("Jets",&name.Jets);
  tree->SetBranchAddress("PFmet",&name.PFmet);
}
