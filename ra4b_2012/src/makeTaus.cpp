#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "makeTaus.h"
#include "Tools.h"

using namespace std;

extern bool pcp;

//======================================================
// make All Taus
//======================================================

vector<Tau> makeAllTaus(EasyChain* tree){
  
  if(pcp) cout<<endl<<"inside makeAllTaus "<<endl<<endl;
 
  //====================================================================
  // VARIABLE INITIALIZATION
  //====================================================================
  vector<Tau>       allTaus;
  Tau               dummyTau;
  vector<LorentzM>& taus                  = tree->Get( &taus, "DESYtauP4Pat");
  vector<int>&      charge                = tree->Get( &charge,"DESYtauChargePat");
  vector<float>&    idDecayModeFinding    = tree->Get( &idDecayModeFinding,"DESYtauTauIddecayModeFindingPat");
  vector<float>&    idMediumIsolationMVA2 = tree->Get( &idMediumIsolationMVA2,"DESYtauTauIdbyMediumIsolationMVA2Pat");
  //=end=variables=initialization=======================================

  //====================================================================
  // READ OR DEFINE THE CUTS FOR ALL ELECTRONS
  //====================================================================
  ConfigReader config;

  static float PTMIN  = config.getFloat( "AllTAUS_PTMIN",  10. );
  //static float ETAMAX = config.getFloat( "AllTAUS_ETAMAX",  2.4);
  //=end=define=cuts====================================================

  //=============================================
  // LOOP OVER THE TAUS
  //=============================================

  for (int itau=0;itau<(int)taus.size();++itau){
    
    if (charge.at(itau)==0) continue;

    if(pcp){
      cout<<"tau("<<itau<<") pt = "<<taus.at(itau).Pt() <<endl;
      cout<<"tau("<<itau<<") eta = "<<taus.at(itau).Eta() <<endl;
    }
    if (taus.at(itau).Pt() < PTMIN)continue;

    dummyTau.Set( itau, taus.at(itau), charge.at(itau), -1.);
    dummyTau.SetID( "DecayModeFinding", idDecayModeFinding.at(itau) > 0.5);
    dummyTau.SetID( "MediumIsolationMVA2", idMediumIsolationMVA2.at(itau) > 0.5);  

    allTaus.push_back(dummyTau);
  }
  //=end=loop=over=taus=================================================

  return allTaus;
}

//======================================================
// make Clean Taus
//======================================================

void makeCleanTaus(vector<Tau>& allTaus, vector<Tau*>& cleanTaus, vector<Muon*>& muons, vector<Electron*>& electrons){
  
  if(pcp) cout<<endl<<"inside makeCleanTaus"<<endl<<endl;
  cleanTaus.clear();

  //====================================================================
  //READ OR DEFINE THE CUTS
  //====================================================================
  ConfigReader config;

  static float  DRmu  =  config.getFloat("taus_CLEANDELTAR_MU",  0.4 );
  static float  DRel  =  config.getFloat("taus_CLEANDELTAR_EL",  0.4 );
  //=end=define=cuts====================================================

  //=============================================
  // LOOP OVER THE TAUS
  //=============================================
  
  bool OK = false;
  if(pcp)cout<<"about to enter the clean taus loop"<<endl;

  for(int itau=0; itau < (int)allTaus.size(); ++itau){
    
    if(pcp)cout<<"inside the tau loop at iteration "<<itau<<endl;

    allTaus.at(itau).SetID( "Clean", false);
    
    //OK=desy_tools::CleaningByDR( allJets.at(ijet)->P4(), muons, DRmu);
    //if(!flow->keepIf("Cross Clean with Muons",OK)) continue;
    if( desy_tools::CleaningByDR( allTaus.at(itau).P4(), muons, DRmu)) continue;
    //
    //OK=desy_tools::CleaningByDR( allJets.at(ijet)->P4(), electrons, DRel);
    //if(!flow->keepIf("Cross Clean with Electrons",OK)) continue;
    if( desy_tools::CleaningByDR( allTaus.at(itau).P4(), electrons, DRel)) continue;

    allTaus.at(itau).SetID( "Clean", true);
    cleanTaus.push_back( &allTaus.at(itau));
  }//=end=loop=over=taus============================================

  return;
}

//======================================================
// make Veto Taus
//======================================================

bool makeVetoTaus( EasyChain* tree, vector<Tau>& allTaus,vector<Tau*>& vetoTaus, Particle* selectedLepton){

  if(pcp)cout<<"inside makeVetoTaus"<<endl;

  //====================================================================
  // READ OR DEFINE THE CUTS FOR THE VETO TAUS
  //====================================================================
  ConfigReader config;

  static float PTMIN  = config.getFloat(  "VetoTaus_PTMIN", 20. );
  //=end=define=cuts====================================================

  //=============================================
  // LOOP OVER THE TAUS
  //=============================================

  bool OK=false;
  if(pcp)cout<<"about to enter the veto taus loop"<<endl;

  for(int itau=0;itau<allTaus.size();++itau){

    if(pcp)cout<<"inside the tau loop at iteration "<<itau<<endl;

    allTaus.at(itau).SetID( "Veto", false);
    int indx=allTaus.at(itau).GetIndexInTree();
    
    OK=allTaus.at(itau).IsID("Clean");
    if (!OK) continue;
    //
    OK=allTaus.at(itau).pt() >= PTMIN;
    if (!OK) continue;
    //
    OK=allTaus.at(itau).IsID("DecayModeFinding");
    if (!OK) continue;
    //
    OK=allTaus.at(itau).IsID("MediumIsolationMVA2");
    if (!OK) continue;
    //
    OK=allTaus.at(itau).Charge() != selectedLepton->Charge();
    if(!OK) continue;

    allTaus.at(itau).SetID( "Veto", true);
    vetoTaus.push_back(&allTaus.at(itau));
  }//=end=loop=over=taus============================================

  return true;
}
