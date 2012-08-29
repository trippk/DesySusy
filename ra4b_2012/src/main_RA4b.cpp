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

  mt2w_bisect::mt2w_interface mt2w_calc;

  const double PI = 4.0*atan(1.0);  
    pcp=false;
  //pcp=true;
    EventWeight=1.0;


  //CONFIG. The program will always take the file config.txt in the output directory
  //specified in the execution of runOnAll
  
  string MainDir="./";
  ConfigReader config(MainDir+"config.txt",argc,argv);
  config.Add(MainDir+"para_config.txt");
  config.Add(MainDir+"pu_config.txt");
  TString filename = config.getTString("filename");
  
  TString treeType = config.getTString("treeType","default"); 
  subTree* subTree= subTreeFactory::NewTree(treeType);

  EasyChain* tree = new EasyChain("/susyTree/tree");
  // add file(s) or folder(s)
  int f = tree->AddSmart(filename);

  TString outname = config.getTString("outname",tree->GetUniqeName());
  string outname_string=(string)outname;
  

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

  bool DoOnlineRW=false;

  //PU

  bool oldpuw = false; //the obselete method and values
  if(WhatSample=="TTJets")oldpuw=true;

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
  //
  //bool DoOnlineRW=false;

  double InitialEventWeight=1.0;             //Event weight do to Lumi and xsec.
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
  

  //================================================================
  //OUTPUT FILE
  //================================================================

  // output directory and output file name(derived from input name)
  cout<<"the output file name is "<<outname<<endl;;
  TFile *outfile = TFile::Open(outname,"RECREATE");
  //TFile *outprovafile = TFile::Open("outprovafile.root","RECREATE");

  // set output root file for cut flow (to be done!)
  CutSet::setTFile(outfile);


  
  string replacestring=".root";
  string treeFileName="";
  int pos = outname_string.find(replacestring);
  size_t thelength=replacestring.length();
  if (pos != string::npos){
    treeFileName= outname_string.replace(pos,thelength,"_tree.root");
  }

  TFile* treeFile=TFile::Open((TString)treeFileName,"RECREATE");
  treeFile->cd();

  //  outfile->cd();
  //================================================================



  //================================
  //SET THE BJET WORKING POINTS
  //================================
  map<const char*, map<const char*, double> > bJetWP; 
  Jet::SetWP("8TeV", &bJetWP); 

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





  bool OK=false;
  bool OKold=true;


  //===========================================
  //CUT FLOW MONITORING
  //===========================================
  CutSet globalFlow("global flow");
  CutSet* p2globalFlow= &globalFlow;
  //===========================================

    
  //===========================================
  bool turntriggersoff=false;
  if(turntriggersoff){
    cout<<"-----------TURNTRIGGERSOFF IS true!!-----------"<<endl;
    if(isData){
      cout<<"--------NO TRIGGERS NO DATA--------------------"<<endl;
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
    vector<Muon*> SoftMuons;
    Muons=makeAllMuons(tree);
    makeTightMuons(tree,Muons,TightMuons);
    makeLooseMuons(tree,Muons,LooseMuons);
    makeSoftMuons(tree ,Muons,SoftMuons);
    //============================================


    //============================================
    // Make Electrons
    vector<Electron>  Electrons;
    vector<Electron*> LooseElectrons;
    vector<Electron*> TightElectrons;
    vector<Electron*> VetoElectrons;
    Electrons=makeAllElectrons(tree);
    makeLooseElectrons(tree,Electrons,LooseElectrons);
    makeTightElectrons(tree,Electrons,TightElectrons);
    makeVetoElectrons(tree, Electrons,VetoElectrons);
    //============================================


    //============================================    
    // Make Jets
    vector<Jet>  Jets;
    vector<Jet*> GoodJets;
    vector<Jet*> CleanedJets;

    Jets=makeJets(tree);

    Jet dummyJet;
    for (int ijet=0; ijet<(int)Jets.size(); ++ijet){
      GoodJets.push_back(&Jets.at(ijet));
    }
    makeCleanedJets( GoodJets, CleanedJets, TightMuons, TightElectrons);
    //============================================

    //============================================
    //Make HT
    double HT=0;
    double px=0;
    double py=0;
    double MHT=0;
    for(int ijet=0;ijet<(int)CleanedJets.size();++ijet){
      HT=HT+CleanedJets.at(ijet)->Pt();
      px+=  CleanedJets.at(ijet)->p4.Px();
      py+=  CleanedJets.at(ijet)->p4.Py();
    }
    MHT=sqrt(px*px + py*py);
    //============================================

    //============================================
    //Make MET
    LorentzM& PFmet = tree->Get(&PFmet, "metP4PF");
    double METSig = PFmet.Et() / sqrt(HT);
    //============================================









    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    

    //                     C  U  T      F L O W                           //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   

    int NVertices=0;
    int NBtags=0;

    //if(DoControlPlots)FillControlPlots(CplotsMap["Before_CutFlow"],Muons,Electrons,Jets,PFmet,NVertices,NBtags);



    ControlPlots.MakePlots("Before_CutFlow", TightMuons, TightElectrons, CleanedJets, PFmet); 




    //
    //====================================================================
    // TRIGGERS
    //====================================================================
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
    NVertices=(int)goodVert.size();
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
    string WidMu = config.getString("WideMuon_Selection","Loose");
    string SigEl = config.getString("SignalElectron_Selection","Tight");
    string WidEl = config.getString("WideElectron_Selection","Veto");


    //=================================
    //MUONS
    //=================================
    //
    //
    //
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
    //
    //
    OK=SetOfCuts::SignalMuons.NUM.Examine(SignalMuons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Signal_Muons",OK) && quick) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("Signal_Muons", SignalMuons, TightElectrons, CleanedJets, PFmet); 
    //treeCuts["Signal_Muons"]=OK;
    //
    //
    OK=SetOfCuts::WideMuons.NUM.Examine(WideMuons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Wide_Muons",OK) && quick) continue;    
    if(DoControlPlots && OK)ControlPlots.MakePlots("Wide_Muons", WideMuons, TightElectrons, CleanedJets, PFmet); 
    //    treeCuts["Wide_Muons"]=OK;
    //
    //
    //=================================
    //ELECTRONS
    //=================================
    //
    //
    //
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
    //
    //
    OK=SetOfCuts::SignalElectrons.NUM.Examine(SignalElectrons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Signal_Electrons",OK) && quick) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("Signal_Electrons", SignalMuons, SignalElectrons, CleanedJets, PFmet); 
    //treeCuts["Signal_Electrons"]=OK;
    //
    //
    OK=SetOfCuts::WideElectrons.NUM.Examine(WideElectrons.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Wide_Electrons",OK) && quick) continue;    
    if(DoControlPlots && OK)ControlPlots.MakePlots("Wide_Electrons", SignalMuons, WideElectrons, CleanedJets, PFmet); 
    //treeCuts["Wide_Electrons"]=OK;
    //
    //===================================




    //===================================
    //JETS
    //===================================
    //
    OK=SetOfCuts::Jets.NUM.Examine(CleanedJets.size());
    if(i==0 && isquick){OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("Jet_Cuts",OK) && quick) continue;    
    if(DoControlPlots && OK)ControlPlots.MakePlots("Jet_Cuts", SignalMuons, SignalElectrons, CleanedJets, PFmet); 
    //treeCuts["Jet_Cuts"]=OK;
    //===================================





    if(pcp)cout<<"before btagging the jets"<<endl;
    //===================================================================
    //BTAGGING OF JETS
    //===================================================================

    //    for (int ijet=0;ijet<=CleanedJets.size(); ++ijet){
    //      CleanedJets.at(ijet)->SetWP("8TeV",);
    //    }
    
    /*
      double TCHE_bTag_cutVal=config.getFloat("Btag_CutValue",3.3);
      int NOfbtags=0;
      vector<float>& TCHE_bTag   = tree->Get(&TCHE_bTag  , "ak5JetPFTrkCountingHighEffBJetTagsPat"); 
      for(int ijet=0;ijet<(int)selectedJets.size();++ijet){
      int m=selectedJets.at(ijet)->GetIndexInTree();
      if(TCHE_bTag.at(m) > TCHE_bTag_cutVal ){
      selectedJets.at(ijet)->SetBtag(true);
      //if i==0, be careful
      if(i==0 && OK && OKold){NOfbtags++; }
      if(i!=1){NOfbtags++;}
      }
      }
    */
    //===================================================================
    
    
    //===================================================================
    //Requirements on the number of btags
    //===================================================================
    //OK=SetOfCuts::Event.NumberOfBtags.Examine(NOfbtags);
    //    if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
    //if(!globalFlow.keepIf(SetOfCuts::Event.NumberOfBtags.name,OK) && quick)continue;
    //===================================================================
    //    if(pcp)cout<<"number of btags imposed "<<endl;
    int NOfbtags=0;



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
    for(int ijet=0;ijet<(int)CleanedJets.size();++ijet){
      HT=HT+CleanedJets.at(ijet)->Pt();	
    }

    OK=SetOfCuts::Event.HT.Examine(HT);
    if(i==0 && isquick){ OK=OK&&OKold; OKold=OK;}
    if(!globalFlow.keepIf("HT", OK ) && quick ) continue;
    if(DoControlPlots && OK)ControlPlots.MakePlots("HT", SignalMuons, SignalElectrons, CleanedJets, PFmet); 
    //treeCuts["HT"]=OK;
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
    //treeCuts["MET"]=OK;
    //====================================================================








    //===================================================================
    //EXAMINE THE TRIGGERS THAT PASSED ALL THE SELECTION
    //===================================================================
    bool studytriggers=false;
    if(studytriggers){
      bool randombool=triggers_RA4b_frequency(tree,triggernames);
    }
    //===================================================================



    //avoid having the first iteration entering here:
    if(i==0 && !OK)continue;


















    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   


    //                    A  B  C  D   Categorization                     //


    //=====================================================================   
    //=====================================================================   
    //=====================================================================   
    //=====================================================================   





    
    //randvector.push_back(


    //LorentzM& PFmet = tree->Get(&PFmet, "metP4PF");
    METSig = PFmet.Et() / sqrt(HT); 


    EventInfo info;
    info.Event=Event;
    info.Run=Run;
    info.EventWeight=EventWeight;
    info.PUWeight=PUWeight;

    bool doSmallTree=true;
    if(doSmallTree){
      subTree->Fill( &info, tree, SignalMuons, SignalElectrons, CleanedJets, PFmet);
    }


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
    if(NOfbtags==1){
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
    else if(NOfbtags==2){
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
    else if(NOfbtags>2){
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



  // save histogramms



  //==================================================
  outfile->cd();
  TListIter* list = new TListIter(gDirectory->GetList()); //-->iterates over list of objects in memory
  TObject *nobj=(TObject*) list->Next(); //nobj now points to the first object in memory
  while(nobj){

    if(nobj->IsA()->InheritsFrom("TH1")){
      nobj->Write();
      //nobj->Write("",TObject::kOverwrite) -->Overwrites the histogram if alredy present
    }
    nobj=list->Next();   //nobj point now to the next object
  }

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



  
  

  treeFile->cd();
  subTree->Write();

  //PROGRAM END
  //EndOfProgram:
  

  return 0;
  
}
   
