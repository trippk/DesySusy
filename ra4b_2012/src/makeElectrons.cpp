#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "eventselection.h"
#include "Electron.h"

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

  //lets calculate the isolation now.

  Electron dummyElectron;
  for (int iel=0;iel<(int)Electrons.size();++iel){

    if (Electrons.at(iel).Pt() < 10.0)continue;
    if( fabs(Electrons.at(iel).Eta()) >2.5)continue;

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
  //


  //===========================================================================================
  //LOOP OVER ELECTRONS
  //===========================================================================================
  
  ElectronFlow.keepIf("number of calls to loose electronRA4b",true);
  bool OK=false;
  for(int iel=0;iel<(int)AllElectrons.size();++iel){

    int indx=AllElectrons.at(iel).GetIndexInTree();
    //
    OK=AllElectrons.at(iel).pt()>PTMIN;
    if(!ElectronFlow.keepIf("pt>el_pt_min_low", OK) && quick) continue;
    //
    OK=fabs(AllElectrons.at(iel).eta())<ETAMAX;
    if(!ElectronFlow.keepIf("abs(eta)<etamax",OK) && quick)continue;
    //
    if(!ElectronFlow.keepIf("notinetagap",fabs(El_SuperClusterPositionETA.at(indx))<1.4442 || fabs(El_SuperClusterPositionETA.at(indx))>1.566) && quick)continue;
    //
    AllElectrons.at(iel).SetID("Loose",true);
    LooseElectrons.push_back(&AllElectrons.at(iel));
  }//Loop over electrons
  return true;
};



bool makeTightElectrons(EasyChain* tree, vector<Electron>& AllElectrons, vector<Electron*>& TightElectrons){

 
  static CutSet ElectronFlow("good tight electron selection");
  extern   bool pcp;
  ElectronFlow.autodump=true;

  TightElectrons.clear();
  const char* idname="undefined";

  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE TIGHT LEPTONS
  //====================================================================
  ConfigReader config;
  static float  PTMIN   =config.getFloat("TightElectrons_PTMIN",  20. );
  static float  ETAMAX  =config.getFloat("TightElectrons_ETAMAX", 2.5 );
  static bool quick     =config.getBool("TightElectrons_quick",true);  
  static string selection =config.getString("Electron_Selection","Tight");
  static float trackdxyMAX = config.getFloat("TightElectron_trackdxyMAX",0.02);
  static float trackdzMAX = config.getFloat("TightElectron_trackdzMAX",0.1);
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


  bool TTver=false;
  if(selection=="Tight"){
    idname="electronIdTightPat";
  }else if(selection=="Medium"){
    idname="electronIdMediumPat";
  }else if(selection=="TriggerTight"){
    idname="DESYelectronIdTriggerTightPat";
    TTver=true;
  }

  else{
    cout<<"ID NOT SET. ERROR"<<endl;
    
  }
  vector<int>&     id = tree->Get( &id,idname);



 
  //===========================================================================================
  //LOOP OVER ELECTRONS
  //===========================================================================================
  
  ElectronFlow.keepIf("number of calls to tight electronRA4b",true);

  bool OK=false;
  for(int iel=0;iel<(int)AllElectrons.size();++iel){
    
    int indx=AllElectrons.at(iel).GetIndexInTree();
    AllElectrons.at(iel).SetID("Tight",false);

    //PT
    OK = AllElectrons.at(iel).Pt() > PTMIN;
    if(!ElectronFlow.keepIf("pt>el_pt_min_low", OK && quick) && quick) continue;

    //ETA
    OK=fabs(AllElectrons.at(iel).Eta()) < ETAMAX;
    if(!ElectronFlow.keepIf("eta max", OK))continue;
    //
    OK=fabs(El_SuperClusterPositionETA.at(indx))<1.4442 || fabs(El_SuperClusterPositionETA.at(indx))>1.566;
    if(!ElectronFlow.keepIf("notinetagap",OK && quick))continue;    

    //ID
    OK=id.at(indx);
    if(!ElectronFlow.keepIf("Electron_ID",OK && quick)) continue;

    //additional cuts 
    if (FbremPF.at(indx)<0.15 ){
      OK=fabs(AllElectrons.at(iel).Eta())< 1.0 && EoverPin.at(indx) > 0.95;
    }else {
      OK=true;
    }
    if (!ElectronFlow.keepIf("FbremPF_EoverPin", OK && quick) )continue;

  
    if(TTver){
      OK=fabs(El_GsfTrackDxy.at(indx)) < trackdxyMAX ;
      if( !ElectronFlow.keepIf("|dxy|<0.02", OK && quick )) continue;
      OK=fabs(El_GsfTrackDz.at(indx)) < trackdzMAX;
      if( !ElectronFlow.keepIf("|dz|<0.1"  , OK && quick )) continue;      
    }
        
    AllElectrons.at(iel).SetID("Tight",true);
    TightElectrons.push_back(&AllElectrons.at(iel));

  }//Loop over electrons

  return true;

};






bool makeVetoElectrons(EasyChain* tree, vector<Electron>& AllElectrons, vector<Electron*>& VetoElectrons){


  
  static CutSet ElectronFlow("Veto electron selection");
  extern   bool pcp;
  ElectronFlow.autodump=true;

  VetoElectrons.clear();

  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE VETO ELECTRONS
  //====================================================================
  ConfigReader config;
  static float  PTMIN   =config.getFloat("VetoElectrons_PTMIN",  15. );
  static float  ETAMAX  =config.getFloat("VetoElectrons_ETAMAX", 2.5 );
  static bool quick     =config.getBool("quick");  
  static string selection =config.getString("VetoElectron_Selection","Veto");
  //====================================================================

  ElectronFlow.keepIf("number of calls to VETO electronRA4b",true);


  //=====================================================
  // the electron collections
  //=====================================================
  if(pcp)cout<<"check point about to get the electrons"<<endl;

  vector<float>&    El_SuperClusterPositionETA   = tree->Get( &El_SuperClusterPositionETA, "electronESuperClusterEtaPat");

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
  vector<int>&     id = tree->Get( &id,idname);




  //===========================================================================================
  //LOOP OVER ELECTRONS
  //===========================================================================================
  
  ElectronFlow.keepIf("number of calls to VETO electronRA4b",true);
  int NOfVetoElectrons=0;
  bool OK=false;

  for(int iel=0;iel<(int)AllElectrons.size();++iel){
    int indx=AllElectrons.at(iel).GetIndexInTree();
    //I only take into account the ones that are not Signal
    
    //
    OK=AllElectrons.at(iel).Pt() > PTMIN;
    if(!ElectronFlow.keepIf("ptmin ",OK))continue;
    //
    OK=fabs(AllElectrons.at(iel).Eta()) < ETAMAX;
    if(!ElectronFlow.keepIf("eta max ",OK))continue;
    //
    OK=fabs(El_SuperClusterPositionETA.at(indx))<1.4442 || fabs(El_SuperClusterPositionETA.at(indx))>1.566;
    if(!ElectronFlow.keepIf("notinetagap",OK && quick))continue;    

    //
    if(AllElectrons.at(iel).IsID("Tight")  && id.at(indx)==0) {
      cout << "ERROR. The veto electron is tighter than the tight electron :)!" << endl; 
      exit(1);
    }
    //
    if(!ElectronFlow.keepIf("Veto_Electron_ID",id.at(indx)) ) continue;
    //
    NOfVetoElectrons++;
    AllElectrons.at(iel).SetID("Veto",true);
    
    VetoElectrons.push_back(&AllElectrons.at(iel));

  }//Loop over electrons


  return true;



};

