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

  //For each folder name, create a folder in the outfile and treefile
  std::map<std::string, TDirectory*>  outfileDirMap;
  std::map<std::string, boost::shared_ptr<CutSet> > globalflowMap;
  std::map<std::string, boost::shared_ptr<HistoMaker> > controlPlotsMap;
  std::map<std::string, TDirectory*>  treefileDirMap;
  std::map<std::string, subTree*> subTreeMap;
  std::vector<std::string>::iterator folderNameIt;

  TString treeType = config.getTString("treeType","default"); 
  cout<<"Using subTree type: "<<treeType<<endl;

  for (folderNameIt = folderNames.begin() ; folderNameIt != folderNames.end() ; folderNameIt++ ) {
    TDirectory * newDir = outfile->mkdir(folderNameIt->c_str());
    if (newDir == 0) cout << "ERROR CREATING FOLDER IN OUTFILE!" << endl;
    else outfileDirMap.insert( make_pair(*folderNameIt, newDir) );
        
    globalflowMap.insert( make_pair(*folderNameIt, new CutSet("global flow") ) );
    controlPlotsMap.insert( make_pair(*folderNameIt, new HistoMaker("AnalyzeSUSY", " ") ) );
    
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

	ev.setCutFlow(globalflowMap[*folderName].get());
	ev.setControlPlots(controlPlotsMap[*folderName].get());
	ev.setSubTree(subTreeMap[*folderName]);

	ev.getEntry(i);
	
	ev.doPU_RW();
	
	ev.createObjects();
	
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
    controlPlotsIt->second->setDir(outfileDirMap[controlPlotsIt->first]);
    controlPlotsIt->second->dumpToFile();
  }

  //Dump cut flows to file
  typedef std::map<std::string, boost::shared_ptr<CutSet> >::iterator cutSetIt_t;
  for (cutSetIt_t cutSetIt = globalflowMap.begin() ; cutSetIt != globalflowMap.end(); cutSetIt++ ) {
    cutSetIt->second->setDir(outfileDirMap[cutSetIt->first]);
    cutSetIt->second->dumpToHist();
  }

  //Write all files and close
  outfile->Write();
  outfile->Close();
  treefile->Write();
  treefile->Close();

  config.printUsed();
  return 0;
}
   
