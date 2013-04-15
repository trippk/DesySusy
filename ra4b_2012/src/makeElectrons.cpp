#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "eventselection.h"
#include "Electron.h"
#include "simpleElectron.h"
#include "makeMuons.h"


using namespace std;

extern bool pcp;





//======================================================
vector<Electron> makeAllElectrons(EasyChain* tree){

  vector<Electron> AllElectrons;
  vector<LorentzM>& Electrons = tree->Get(&Electrons, "electronP4Pat");
  vector<int>&      charge   = tree->Get( &charge,"electronChargePat");
  vector<float>&      El_Dr03EcalRecHitSumEt    = tree->Get( &El_Dr03EcalRecHitSumEt,  "electronDr03EcalRecHitSumEtPat");
  vector<float>&      El_Dr03HcalTowerSumEt     = tree->Get( &El_Dr03HcalTowerSumEt,   "electronDr03HcalTowerSumEtPat"); 
  vector<float>&      El_Dr03TkSumPt            = tree->Get( &El_Dr03TkSumPt,          "electronDr03TkSumPtPat" );
  vector<float>&    El_SuperClusterPositionETA   = tree->Get( &El_SuperClusterPositionETA, "electronESuperClusterEtaPat");
  //lets calculate the isolation now.

  Electron dummyElectron;

  for (int iel=0;iel<(int)Electrons.size();++iel){

    if(pcp){
      cout<<"Electron "<<iel<<"; . Pt = "<<Electrons.at(iel).Pt()<<endl;
      cout<<"Electron "<<iel<<"; . Eta = "<<Electrons.at(iel).Eta()<<endl;
      cout<<"Electron "<<iel<<"; . Eta supercluster= "<<El_SuperClusterPositionETA.at(iel)<<endl;
      cout<<"Electron "<<iel<<"; . Phi = "<<Electrons.at(iel).Phi()<<endl;
    }
    //    if (Electrons.at(iel).Pt() < 10.0)continue;
    // if( fabs(Electrons.at(iel).Eta()) >2.5)continue;

    double EcalIso=El_Dr03EcalRecHitSumEt.at(iel);
    if(fabs(Electrons.at(iel).eta())<1.479)EcalIso=max(0.,(EcalIso-1.));
    double ElRelIso= (El_Dr03TkSumPt.at(iel)+EcalIso+El_Dr03HcalTowerSumEt.at(iel))/(double)Electrons.at(iel).Pt();

    dummyElectron.Set(iel,&Electrons.at(iel),charge.at(iel),ElRelIso);
    AllElectrons.push_back(dummyElectron);
  }
  return AllElectrons;
}





//
//======================================================
//


bool makeLooseElectrons(EasyChain* tree, vector<Electron>& AllElectrons, vector<Electron*>& LooseElectrons){

  Electron dummyElectron;

  static CutSet ElectronFlow("Loose_Electron_Selection");
  extern   bool pcp;
  ElectronFlow.autodump=true;

  LooseElectrons.clear();
  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE ELECTRONS
  //====================================================================
  ConfigReader config;
  static float  PTMIN   = config.getFloat("LooseElectrons_PTMIN",  15. );
  static float  ETAMAX  = config.getFloat("LooseElectrons_ETAMAX", 2.5 );
  static double ISOMAX  = config.getFloat("LooseElectrons_ISOMAX", 0.15 );
  static  bool quick=     config.getBool("LooseElectrons_quick",true);
  //====================================================================

  //=====================================================
  // the electron collection
  //=====================================================
  if(pcp)cout<<"check point about to get the electrons"<<endl;
  vector<LorentzM>& Electrons = tree->Get(&Electrons, "electronP4Pat");   
  vector<float>&    El_SuperClusterPositionETA   = tree->Get( &El_SuperClusterPositionETA, "electronESuperClusterEtaPat");
  vector<bool>& electronIsEBPat =  tree->Get(&electronIsEBPat,"DESYelectronIsEBPat");
  vector<bool>& electronIsEEPat =  tree->Get(&electronIsEEPat,"DESYelectronIsEEPat");
  //


  //===========================================================================================
  //LOOP OVER ELECTRONS
  //===========================================================================================
  
  ElectronFlow.keepIf("number of calls to loose electronRA4b",true);
  bool OK=false;
  for(int iel=0;iel<(int)AllElectrons.size();++iel){

    int indx=AllElectrons.at(iel).GetIndexInTree();
    //
    //OK=(AllElectrons.at(iel).pt()>=PTMIN || fabs(El_SuperClusterPositionETA.at(indx))>1.566);
    //    if (pcp){
    //      cout<<"eta supercluster of electron "<<indx<< " = "<<El_SuperClusterPositionETA.at(indx);
    //    }
    if(!ElectronFlow.keepIf("pt>el_pt_min_low", OK) && quick) continue;
    //
    //OK=fabs(AllElectrons.at(iel).eta())=<ETAMAX;
    //if(!ElectronFlow.keepIf("abs(eta)<etamax",OK) && quick)continue;
    OK=fabs(El_SuperClusterPositionETA.at(indx))<=ETAMAX;    
    if(!ElectronFlow.keepIf("abs(eta)<etamax",OK) && quick)continue;
    //
    OK=electronIsEEPat.at(indx) || electronIsEBPat.at(indx);
      if(!ElectronFlow.keepIf("notinetagap",OK ) && quick)continue;
    //
    AllElectrons.at(iel).SetID("Loose",true);
    LooseElectrons.push_back(&AllElectrons.at(iel));
  }//Loop over electrons
  return true;
};



bool makeTightElectrons(EasyChain* tree, vector<Electron>& AllElectrons, vector<Electron*>& TightElectrons, CutSet * flow_in){

  extern   bool pcp;
 
  static CutSet ElectronFlow("good tight electron selection");
  ElectronFlow.autodump=true;

  CutSet* flow = &ElectronFlow;
  if (flow_in) {
    ElectronFlow.autodump = false;
    flow = flow_in;
  }


  TightElectrons.clear();
  const char* idname="undefined";

  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE TIGHT LEPTONS
  //====================================================================
  ConfigReader config;
  static float  PTMIN   =config.getFloat("TightElectrons_PTMIN",  20. );
  static float  ETAMAX  =config.getFloat("TightElectrons_ETAMAX", 2.5 );
  static bool quick     =config.getBool("TightElectrons_quick",true);  
  static string selection =config.getString("Electron_Selection","Medium");
  static float trackdxyMAX = config.getFloat("TightElectron_trackdxyMAX",0.02);
  static float trackdzMAX = config.getFloat("TightElectron_trackdzMAX",0.1);
  static float PFRECO_MAXDIFF = config.getFloat("TightElectrons_PFRECO_MAXDIFF", 10.0);
  static float REQ_NOTINGAP_EXPLICIT = config.getBool("TightElectrons_REQ_NOTINGAP_EXPLICIT", false);
  //====================================================================





  //=====================================================
  // the electron collection
  //=====================================================
  if(pcp)cout<<"check point about to get the electrons"<<endl;
  vector<float>&    El_SuperClusterPositionETA   = tree->Get( &El_SuperClusterPositionETA, "electronESuperClusterEtaPat");
  vector<float>&    El_GsfTrackDxy         = tree->Get( &El_GsfTrackDxy, "electronGsfTrackDxyPat"      );
  vector<float>&    El_GsfTrackDz         = tree->Get( &El_GsfTrackDz, "electronGsfTrackDzPat"      );
  vector<float>&    FbremPF = tree->Get(&FbremPF,"electronFbremPat");
  vector<float>&    EoverPin = tree->Get(&EoverPin,"electronESuperClusterOverPPat");
  vector<LorentzM>& Ele_p4 = tree->Get(&Ele_p4, "electronP4Pat");
  vector<bool>& electronIsEBPat =  tree->Get(&electronIsEBPat,"DESYelectronIsEBPat");
  vector<bool>& electronIsEEPat =  tree->Get(&electronIsEEPat,"DESYelectronIsEEPat");


  bool TTver=false;
  if(selection=="Tight"){
    idname="electronIdTightPat";
  }else if(selection=="Medium"){
    idname="electronIdMediumPat";
  }else if(selection=="TriggerTight"){
    idname="DESYelectronIdTriggerTightPat";
    TTver=true;
  }else if(selection=="Loose"){
    idname="electronIdLoosePat";
  }


  else{
    cout<<"ID NOT SET. ERROR"<<endl;
    
  }
  vector<int>&     id = tree->Get( &id,idname);



 
  //===========================================================================================
  //LOOP OVER ELECTRONS
  //===========================================================================================
  
  flow->keepIf("number of calls to tight electronRA4b",true);

  bool OK=false;
  for(int iel=0;iel<(int)AllElectrons.size();++iel){
    
    int indx=AllElectrons.at(iel).GetIndexInTree();
    AllElectrons.at(iel).SetID("Tight",false);

    //PT
    OK = (AllElectrons.at(iel).Pt() >= PTMIN);// || fabs(El_SuperClusterPositionETA.at(indx))>1.566);
    if(!flow->keepIf("pt>el_pt_min_low", OK) && quick) continue;

    //ETA
    //OK=fabs(AllElectrons.at(iel).Eta()) <= ETAMAX;
    OK=fabs(El_SuperClusterPositionETA.at(indx))<=ETAMAX;
    if(!flow->keepIf("eta max", OK) && quick)continue;

    //ID
    OK=id.at(indx);
    if(!flow->keepIf("Electron_ID",OK) && quick) continue;

    //ADDED FOR HANNES SYNC
    OK=fabs(El_GsfTrackDxy.at(indx)) < trackdxyMAX ;
    if( !flow->keepIf("|dxy|<0.02", OK) && quick ) continue;
    OK=fabs(El_GsfTrackDz.at(indx)) < trackdzMAX;
    if( !flow->keepIf("|dz|<0.1"  , OK) && quick ) continue;      

    if(TTver){
      OK=fabs(El_GsfTrackDxy.at(indx)) < trackdxyMAX ;
      if( !flow->keepIf("|dxy|<0.02", OK) && quick ) continue;
      OK=fabs(El_GsfTrackDz.at(indx)) < trackdzMAX;
      if( !flow->keepIf("|dz|<0.1"  , OK) && quick ) continue;      
    }



    //additional cuts 
    /*    if (FbremPF.at(indx)<0.15 ){
      OK=fabs(AllElectrons.at(iel).Eta())< 1.0 && EoverPin.at(indx) > 0.95;
    }else {
      OK=true;
    }
    if (!ElectronFlow.keepIf("FbremPF_EoverPin", OK && quick) )continue;
    */

    //
    //OK=fabs(El_SuperClusterPositionETA.at(indx))<1.4442 || fabs(El_SuperClusterPositionETA.at(indx))>1.566;
    //OK=fabs(AllElectrons.at(iel).Eta() <1.4442 || fabs(AllElectrons.at(iel).Eta()) >1.566;
    bool NOTINGAP_EXPLICIT = fabs(El_SuperClusterPositionETA.at(indx))<1.4442 || fabs(El_SuperClusterPositionETA.at(indx))>1.566; //Required for Sync with Hannes
    OK=(electronIsEEPat.at(indx) || electronIsEBPat.at(indx)) && (!REQ_NOTINGAP_EXPLICIT || (NOTINGAP_EXPLICIT));
    if(!flow->keepIf("notinetagap",OK) && quick)continue;    


    OK=Consistency(Ele_p4.at(indx),tree,"electronP4PF")<PFRECO_MAXDIFF;
    if(!flow->keepIf("RecoPt-PFPt",OK) && quick) continue;
  
       
    AllElectrons.at(iel).SetID("Tight",true);
    TightElectrons.push_back(&AllElectrons.at(iel));

  }//Loop over electrons

  return true;

};






bool makeVetoElectrons(EasyChain* tree, vector<Electron>& AllElectrons, vector<Electron*>& VetoElectrons, CutSet* flow_in){


  
  static CutSet ElectronFlow("Veto electron selection");
  extern   bool pcp;
  ElectronFlow.autodump=true;

  CutSet* flow = &ElectronFlow;
  if (flow_in) {
    ElectronFlow.autodump = false;
    flow = flow_in;
  }

  VetoElectrons.clear();
  
  if(pcp){
    cout<<endl;
    cout<<"INSIDE VETO ELECTRONS"<<endl;
  }

  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE VETO ELECTRONS
  //====================================================================
  ConfigReader config;
  static float  PTMIN   =config.getFloat("VetoElectrons_PTMIN",  15. );
  static float  ETAMAX  =config.getFloat("VetoElectrons_ETAMAX", 2.5 );
  static bool quick     =config.getBool("VetoElectrons_quick",true);  
  static string selection =config.getString("VetoElectron_Selection","Veto");
  //====================================================================

  flow->keepIf("number of calls to VETO electronRA4b",true);


  //=====================================================
  // the electron collections
  //=====================================================
  if(pcp)cout<<"check point about to get the electrons"<<endl;

  //  vector<float>&    El_SuperClusterPositionETA   = tree->Get( &El_SuperClusterPositionETA, "electronESuperClusterEtaPat");
  vector<bool>& electronIsEBPat =  tree->Get(&electronIsEBPat,"DESYelectronIsEBPat");
  vector<bool>& electronIsEEPat =  tree->Get(&electronIsEEPat,"DESYelectronIsEEPat");
  bool TTver=false;
  const char* idname="undefined";
  if(selection=="Tight"){
    idname="electronIdTightPat";
  }else if(selection=="Medium"){
    idname="electronIdMediumPat";
  }else if(selection=="Veto"){
    idname="electronIdVetoPat";
  }else if(selection=="Loose"){
    idname="electronIdLoosePat";
  }
  else{
    cout<<"ID NOT SET. ERROR"<<endl;
    
  }
  if(pcp){
    cout<<"the idname used is "<<idname<<endl;
  }

  vector<int>&     id = tree->Get( &id,idname);




  //===========================================================================================
  //LOOP OVER ELECTRONS
  //===========================================================================================
  
  flow->keepIf("number of calls to VETO electronRA4b",true);
  int NOfVetoElectrons=0;
  bool OK=false;

  for(int iel=0;iel<(int)AllElectrons.size();++iel){
    int indx=AllElectrons.at(iel).GetIndexInTree();
    //I only take into account the ones that are not Signal
   
    if(pcp){
      cout<<"examining the electron "<<iel<<endl;
      cout<<"with a pt "<<AllElectrons.at(iel).Pt()<<endl;
      cout<<"an eta of "<<AllElectrons.at(iel).Eta()<<endl;
      cout<<"isTight "<<AllElectrons.at(iel).IsID("Tight")<<endl;
    }
    //
    OK=(AllElectrons.at(iel).Pt() > PTMIN);// || fabs(El_SuperClusterPositionETA.at(indx))>1.566);
     if(!flow->keepIf("ptmin ",OK) && quick)continue;
    //

    OK=electronIsEEPat.at(indx) || electronIsEBPat.at(indx);
     if(pcp){
       cout<<"passes notinthegap "<<OK<<endl;
       cout<<"isEE "<<electronIsEEPat.at(indx)<<endl;
       cout<<"isEB "<<electronIsEBPat.at(indx)<<endl;
     }        
    if(!flow->keepIf("notinetagap",OK) && quick)continue;    
    
    //
    //
    if(AllElectrons.at(iel).IsID("Tight")  && id.at(indx)==0) {
      cout << "ERROR. The veto electron is tighter than the tight electron :)!" << endl; 
      exit(1);
    }
    //
    if(pcp)cout<<"satisfies the id? "<<id.at(indx)<<endl;
    OK=id.at(indx);
    if(!flow->keepIf("Veto_Electron_ID",OK ) && quick ) continue;
    //
    NOfVetoElectrons++;
    AllElectrons.at(iel).SetID("Veto",true);
    
    if(pcp)cout<<"the electron "<<iel<<" is a VETO electron"<<endl;
       
    VetoElectrons.push_back(&AllElectrons.at(iel));

  }//Loop over electrons


  return true;



};

void makeCleanedElectrons(vector<Electron*>& Electrons_In, vector<Electron*>& Electrons_Out, vector<Muon>& Muons, CutSet* flow_in) {

  Electrons_Out.clear();
  
  //Distance between the jet and the iso leptons
  ConfigReader config;
  static float  DELTAR_CUT  =  config.getFloat("Electrons_CLEANDELTAR",  -1. );

  static CutSet CrossCleaning("Cleaned Electrons");
  CrossCleaning.autodump=true;

  CutSet* flow = &CrossCleaning;
  if (flow_in) {
    CrossCleaning.autodump = false;
    flow = flow_in;
  }

  
  if(pcp){
    cout<<endl;
    cout<<"INSIDE makeCleanedElectrons "<<endl;
    cout<<endl;
  }

  for(int iel = 0; iel<Electrons_In.size(); iel++){

    bool dumpElectron=false;
  
    for(int imu=0; imu<(int)Muons.size(); ++imu){
      if(DeltaR(Electrons_In.at(iel)->P4(),Muons.at(imu).P4())<DELTAR_CUT) {
	dumpElectron=true;
	break;
      }
    }

    if (!flow->keepIf("CrossCleaning Muons",!dumpElectron)) continue;

    Electrons_Out.push_back(Electrons_In.at(iel));

    if(pcp)cout<<"pt and eta of jet "<<iel<<" = "<<Electrons_In.at(iel)->P4().pt()<<" "<<Electrons_In.at(iel)->P4().eta()<<endl;

  }

  return;
}












void makeSimpleElectronCollection(vector<std::string>& attributes,vector<simpleElectron*>& electrons_in, vector<simpleElectron*>& outCollection ){
  //returns a vector of pointers based on the id of the electrons
  for (int ijet=0;ijet<electrons_in.size();++ijet){
    
    bool attributesOK=true;
    for (int iatt=0;iatt<attributes.size();++iatt){
      if ( !electrons_in.at(ijet)->IsID(attributes.at(iatt))){
	attributesOK=false;
	break;
      }
    }
    if(attributesOK){
      outCollection.push_back(electrons_in.at(ijet));
    }
  }  
}


void makeSimpleElectronCollection(vector<std::string>& attributes,vector<simpleElectron>& electrons_in, vector<simpleElectron*>& outCollection ){

  void makeSimpleElectronCollection(vector<std::string>&,vector<simpleElectron*>&,vector<simpleElectron*>&);  
  vector<simpleElectron*> dummyvector;
  for (int ijet=0;ijet<electrons_in.size();++ijet){
    dummyvector.push_back(&(electrons_in.at(ijet)));
  }
  makeSimpleElectronCollection(attributes,dummyvector,outCollection);
}





