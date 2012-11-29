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
#include "TagEff.h"
#include "makeOsLepPairs.h"
#include "Event.h"

//BOOST LIB FOR SHARED PTR
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace ROOT::Math::VectorUtil;
//===================================================================
//                   GLOBAL VARIABLES
//===================================================================

bool pcp = false; //Set to true for debugging.

//================================================================================
int main(int argc, char** argv){

  //CONFIG. The program will always take the file config.txt in the output directory
  //specified in the execution of runOnAll
  string MainDir="./";
  ConfigReader config(MainDir+"config_2.txt",argc,argv);
  config.Add(MainDir+"para_config.txt");
  config.Add(MainDir+"pu_config.txt");

  //NAME OF THE FILE TO READ IN
  TString filename = config.getTString("filename");

  // add file(s) or folder(s)
  EasyChain* tree = new EasyChain("/susyTree/tree");
  int f = tree->AddSmart(filename);

  ////////////////////////////////
  //Create output files
  /////////////////////////////
  TString outname = config.getTString("outname",tree->GetUniqeName());
  cout<<"the output file name is "<<outname<<endl;;
  TFile *outfile = TFile::Open(outname,"RECREATE");
 
  string outname_string=(string)outname;
  string replacestring=".root";
  string treeFileName="";
  size_t pos = outname_string.find(replacestring);
  size_t thelength=replacestring.length();
  if (pos != string::npos) treeFileName= outname_string.replace(pos,thelength,"_tree.root");
  TFile* treefile=TFile::Open((TString)treeFileName,"RECREATE");

  //keep track of the weights
  TH1::SetDefaultSumw2(true);
  //=========================

  //data or monte carlo?
  bool isData=config.getBool("isData");
  outfile->cd();  
  TH1I* h_isData = new TH1I("isdata","data =1 means Data",1,0,1);
  if(isData){h_isData->SetBinContent(1,1);}
  else{h_isData->SetBinContent(1,0);}

  //===========================================================================

  //Get the cut names
  std::vector<std::string> cutVec;
  stringstream cutNamesParser (config.getString("cutNames",""));
  while(cutNamesParser.good()) {
    std::string dummyString;
    cutNamesParser >> dummyString;
    cutVec.push_back(dummyString);
  }
  cout << "Found the following cuts: "<< endl;
  for (std::vector<std::string>::const_iterator cutIt = cutVec.begin() ; cutIt != cutVec.end() ; cutIt++) {
    cout << *cutIt << endl;
  }
  cout << endl;

  //Get the systematic names
  std::vector<std::string> folderNames;
  folderNames.push_back("CENTRAL");
  stringstream sysNamesParser (config.getString("sysNames",""));
  while(sysNamesParser.good()) {
    std::string dummyString;
    sysNamesParser >> dummyString;
    folderNames.push_back(dummyString);
  }
  cout << "Found the following systematics: "<< endl;
  for (std::vector<std::string>::const_iterator fIt = folderNames.begin() ; fIt != folderNames.end() ; fIt++) {
    cout << *fIt << endl;
  }
  cout << endl;

  //Get the systematic options
  sysOpt defaultSysOptions;
  defaultSysOptions.jerDo  = config.getBool("CENTRAL_JER_DO", false);
  defaultSysOptions.jerErr = config.getBool("CENTRAL_JER_ERROR", 0.);
  //For each systematic have a corresponding set of options.
  std::map<std::string, sysOpt> sysOptMap;

  //For each folder name, create a folder in the outfile and treefile
  std::map<std::string, TDirectory*>  outfileDirMap;
  //CutFlows Global
  std::map<std::string, boost::shared_ptr<CutSet> > globalflowMap;
  //CutFlows Trigger
  std::map<std::string, boost::shared_ptr<CutSet> > triggerflowMap;
  //CutFlows Electron
  std::map<std::string, boost::shared_ptr<CutSet> > electronTightMap;
  std::map<std::string, boost::shared_ptr<CutSet> > electronVetoMap;
  std::map<std::string, boost::shared_ptr<CutSet> > electronTightCleanedMap;
  std::map<std::string, boost::shared_ptr<CutSet> > electronVetoCleanedMap;
  //CutFlows Muon
  std::map<std::string, boost::shared_ptr<CutSet> > muonLooseMap;
  std::map<std::string, boost::shared_ptr<CutSet> > muonTightMap;
  std::map<std::string, boost::shared_ptr<CutSet> > muonVetoMap;
  //CutFlows Jets
  std::map<std::string, boost::shared_ptr<CutSet> > jetsGoodMap;
  std::map<std::string, boost::shared_ptr<CutSet> > jetsCleanedMap;

  std::map<std::string, cutFlowSet > cutFlowsMap;


  //Control plots
  std::map<std::string, boost::shared_ptr<HistoMaker> > controlPlotsMap;
  //Subtrees
  std::map<std::string, TDirectory*>  treefileDirMap;
  std::map<std::string, subTree*> subTreeMap;
  std::vector<std::string>::iterator folderNameIt;

  TString treeType = config.getTString("treeType","default"); 
  cout<<"Using subTree type: "<<treeType<<endl;

  for (folderNameIt = folderNames.begin() ; folderNameIt != folderNames.end() ; folderNameIt++ ) {
    TDirectory * newDir = outfile->mkdir(folderNameIt->c_str());
    if (newDir == 0) cout << "ERROR CREATING FOLDER IN OUTFILE!" << endl;
    else outfileDirMap.insert( make_pair(*folderNameIt, newDir) );
        
    //Create all of the cut flows.
    globalflowMap.insert( make_pair(*folderNameIt, new CutSet("Global_Flow") ) );
    triggerflowMap.insert( make_pair(*folderNameIt, new CutSet("Trigger_Flow") ) );
    electronTightMap.insert( make_pair(*folderNameIt, new CutSet("Tight_Electron_Selection") ) );
    electronVetoMap.insert( make_pair(*folderNameIt, new CutSet("Veto_Electron_Selection") ) );
    electronTightCleanedMap.insert( make_pair(*folderNameIt, new CutSet("Cleaned_Tight_Electron_Selection") ) );
    electronVetoCleanedMap.insert( make_pair(*folderNameIt, new CutSet("Cleaned_Veto_Electron_Selection") ) );
    muonLooseMap.insert( make_pair(*folderNameIt, new CutSet("Loose_Muon_Selection") ) );
    muonTightMap.insert( make_pair(*folderNameIt, new CutSet("Tight_Muon_Selection") ) );
    muonVetoMap.insert( make_pair(*folderNameIt, new CutSet("Veto_Muon_Selection") ) );
    jetsGoodMap.insert( make_pair(*folderNameIt, new CutSet("Good_Jet_Selection") ) );
    jetsCleanedMap.insert( make_pair(*folderNameIt, new CutSet("Cleaned_Jet_Selection") ) );

    //Create cutFlowSet for convenience with passing to the event class
    cutFlowSet cutFlows;
    cutFlows.globalflow = globalflowMap[*folderNameIt].get();
    cutFlows.triggerflow = triggerflowMap[*folderNameIt].get();
    cutFlows.electronTight = electronTightMap[*folderNameIt].get();
    cutFlows.electronVeto = electronVetoMap[*folderNameIt].get();
    cutFlows.electronTightCleaned = electronTightCleanedMap[*folderNameIt].get();
    cutFlows.electronVetoCleaned = electronVetoCleanedMap[*folderNameIt].get();
    cutFlows.muonLoose  = muonLooseMap[*folderNameIt].get();
    cutFlows.muonTight = muonTightMap[*folderNameIt].get();
    cutFlows.muonVeto  = muonVetoMap[*folderNameIt].get();
    cutFlows.jetsGood = jetsGoodMap[*folderNameIt].get();
    cutFlows.jetsCleaned = jetsCleanedMap[*folderNameIt].get();
    cutFlowsMap.insert( make_pair(*folderNameIt, cutFlows) );

    //Initialise the globalflow map
    globalflowMap[*folderNameIt]->initialiseCutNames(cutVec);

    //Get the systematic options
    sysOpt sysOptions;
    sysOptions.jerDo = config.getBool(*folderNameIt + "_JER_DO", defaultSysOptions.jerDo);
    sysOptions.jerErr = config.getBool(*folderNameIt + "_JER_ERR", defaultSysOptions.jerErr);
    sysOptMap.insert( make_pair(*folderNameIt, sysOptions) );
    
    //Create control plots handler
    controlPlotsMap.insert( make_pair(*folderNameIt, new HistoMaker("AnalyzeSUSY", " ") ) );
    controlPlotsMap[*folderNameIt]->setDir(outfileDirMap[*folderNameIt]);

    //Create the subTree analysers
    newDir = treefile->mkdir(folderNameIt->c_str());
    if (newDir == 0) cout << "ERROR CREATING FOLDER IN TREEFILE!"<< endl;
    else treefileDirMap.insert(make_pair(*folderNameIt, newDir));
    subTreeMap.insert( make_pair(*folderNameIt, subTreeFactory::NewTree(treeType,treefile,*folderNameIt) ) );
  }

  //=================================================================
  //Get number of files and number of entries
  cout<<tree->GetNtrees()<<" Files read"<<endl;
  int N = tree->GetEntries();
  cout<<"THERE ARE "<<N<<" EVENTS IN "<<filename<<endl;

  outfile->cd();
  TH1I* num_entries = new TH1I("num_entries","number of entries",1,0,1);
  num_entries->SetBinContent(1,N);
  //================================================================

  event ev(tree);
  if(ev.setSampleInformation()) {

    cout << "Entering loop!" << endl;
    for(int i=0;i<N;++i){
      timer();

      for (std::vector<std::string>::const_iterator folderName = folderNames.begin(); folderName != folderNames.end() ; folderName++ ) {

	//Set the systematic options
	ev.setSysOptions(sysOptMap[*folderName]);

	//Set the cut flows
	ev.setCutFlows(cutFlowsMap[*folderName]);

	//Set the control plots and trees
	ev.setControlPlots(controlPlotsMap[*folderName].get());
	ev.setSubTree(subTreeMap[*folderName]);

	//Read in the tree
	ev.getEntry(i);
	
	//Do Pile-up reweighting
	ev.doPU_RW();
	
	//Create all Objects
	ev.createObjects();
	
	//Apply all cuts, and if pass store in the tree.
	if (ev.applyCuts(cutVec)) {
	  ev.fillTree();
	}
      }
      
    }
  }
  else {
    cout << "ERROR : Could not setup sample information."<< endl;
  }
  
  //Dump control plots to file
  typedef std::map<std::string, boost::shared_ptr<HistoMaker> >::iterator controlPlotsIt_t;
  for (controlPlotsIt_t controlPlotsIt = controlPlotsMap.begin() ; controlPlotsIt != controlPlotsMap.end(); controlPlotsIt++ ) {
    controlPlotsIt->second->autodump = false;
    //controlPlotsIt->second->dumpToFile();
  }

  //Dump cut flows to file
  typedef std::map<std::string, cutFlowSet >::iterator cutSetIt_t;
  for (cutSetIt_t cutSetIt = cutFlowsMap.begin() ; cutSetIt != cutFlowsMap.end(); cutSetIt++ ) {
    cutSetIt->second.globalflow->setDir(outfileDirMap[cutSetIt->first]); //Set the dir for all CutSets.
    cutSetIt->second.globalflow->dumpToHist();
    cutSetIt->second.triggerflow->dumpToHist();
    cutSetIt->second.electronTight->dumpToHist();
    cutSetIt->second.electronVeto->dumpToHist();
    cutSetIt->second.electronTightCleaned->dumpToHist();
    cutSetIt->second.electronVetoCleaned->dumpToHist();
    cutSetIt->second.muonLoose->dumpToHist();
    cutSetIt->second.muonTight->dumpToHist();
    cutSetIt->second.muonVeto->dumpToHist();
    cutSetIt->second.jetsGood->dumpToHist();
    cutSetIt->second.jetsCleaned->dumpToHist();
  }

  //Write all files and close
  outfile->Write();
  outfile->Close();
  treefile->Write();
  treefile->Close();

  config.printUsed();
  return 0;
}
   
