#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "eventselection.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"

#include "Math/VectorUtil.h"
using namespace std;

extern bool pcp;



vector<Jet> makeAllJets(EasyChain* tree){

 if(pcp){
    cout<<endl;
    cout<<"INSIDE makeAllJets "<<endl;
    cout<<endl;
  }

  vector<Jet> Jets; 
  Jets.clear();

  vector<LorentzM>&  Jets_p4 = tree->Get(&Jets_p4, "ak5JetPFCorrectedP4Pat");
  vector<float>&     Jets_CorrFactor = tree->Get(&Jets_CorrFactor, "ak5JetPFCorrFactorPat");
  
  for(int ijet = 0; ijet<Jets_p4.size(); ijet++){  

    Jet dummyJet;
    dummyJet.Set(ijet, &Jets_p4.at(ijet), Jets_CorrFactor.at(ijet), "AK5");    
    Jets.push_back(dummyJet);
    
  }
  return Jets;
}



void makeGoodJets(EasyChain* tree, vector<Jet*>& AllJets, vector<Jet*>& goodJets){

  if(pcp){
    cout<<endl;
    cout<<"INSIDE makeJets "<<endl;
    cout<<endl;
  }

  goodJets.clear();

  static CutSet jetFlow("good_jet_selection");
  jetFlow.autodump=true;


  //  vector<LorentzM>&  Jets_p4 = tree->Get(&Jets_p4, "ak5JetPFCorrectedP4Pat");
  vector<int>&       Jets_ID = tree->Get(&Jets_ID,"ak5JetPFPFJetIDloosePat");
  vector<float>&     Jets_CSV = tree->Get(&Jets_CSV, "ak5JetPFCombinedSecondaryVertexBJetTagsPat");
  vector<float>&     Jets_CSVMVA = tree->Get(&Jets_CSVMVA, "ak5JetPFCombinedSecondaryVertexMVABJetTagsPat");
  unsigned int Event   = tree->Get(Event,"event");    
  
  extern bool pcp;

  if(pcp){
    cout<<endl;
    cout<<"=================================================== "<<endl;
    cout<<"      going to check the jets in event "<< Event <<endl;
    cout<<"=================================================== "<<endl;
  }

  if(pcp)cout<<"total number of jets "<<AllJets.size()<<endl;

  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE JETS
  //====================================================================
  ConfigReader config;
  static float  PTMIN  =  config.getFloat("Jets_PTMIN",  40. );
  static float  ETAMAX  = config.getFloat("Jets_ETAMAX", 2.4 );
  static string btagAlgorithm= config.getString("bTagAlgorithm","CSV");
  static string btagWorkingPoint = config.getString("bTagWorkingPoint","Medium");
  static bool   quick = config.getBool("quick_JETS",true);
  //====================================================================

  bool OK=false;

  for(int ijet = 0; (int)ijet<AllJets.size(); ijet++){

    //Jet dummyJet;

    OK=false;
    int indx = AllJets.at(ijet)->GetIndexInTree();

    if(pcp)cout<<"pt,eta, phi and id -->"<<AllJets.at(ijet)->pt()<<" "<<AllJets.at(ijet)->eta()<<" "<<AllJets.at(ijet)->Phi()<<" "<<Jets_ID.at(indx)<<endl;

    jetFlow.keepIf("before cuts in jets", true);

    //
    OK=AllJets.at(ijet)->pt() > PTMIN;
    if( !jetFlow.keepIf("Jets_PTMIN", OK) && quick) continue;

    // 
    OK=fabs(AllJets.at(ijet)->eta()) < ETAMAX;
    if( !jetFlow.keepIf("jets_ETAMAX",OK) && quick ) continue;
    //
    OK=Jets_ID.at(indx)!=0;
    if( !jetFlow.keepIf("ak5JetPFPFJetIDloosePat", OK ) && quick ) continue;

    //
    if(!quick){
      OK=jetFlow.applyCuts("Jets_PTMIN jets_ETAMAX ak5JetPFPFJetIDloosePat");
      if (!OK)continue;
    }
    
    AllJets.at(ijet)->SetBJetDisc("CSV", Jets_CSV.at(ijet));
    AllJets.at(ijet)->SetID("Loose",1);
    goodJets.push_back(AllJets.at(ijet));
    //
    //
  }//Loop over Jets
  
  //  return AllJets;
  //retun true;
};

void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon>& Muons, vector<Electron>& Electrons){
  
  Jets_Out.clear();
  
  //Distance between the jet and the iso leptons

  static CutSet CrossCleaning("Cleaned Jets");
  CrossCleaning.autodump=true;
  
  if(pcp){
    cout<<endl;
    cout<<"INSIDE makeCleanedJets "<<endl;
    cout<<endl;
  }

  for(int ijet = 0; ijet<Jets_In.size(); ijet++){

    bool dumpJet=false;
  
    for(int imu=0; imu<(int)Muons.size(); ++imu){
      if(pcp)cout<<"distance from jet "<<ijet<<" to muon "<<imu<< " = "<<
	//if(pcp)cout<<"the muon "<<imu<<"  "<<imu<< " = "<<
	
	DeltaR(Jets_In.at(ijet)->P4(), Muons.at(imu).P4())<<" lep= "<<Muons.at(imu).Flavor()<<endl;

      if ( !Muons.at(imu).IsID("Tight") &&  !Muons.at(imu).IsID("Veto"))continue;

      if(DeltaR(Jets_In.at(ijet)->P4(),Muons.at(imu).P4())<0.3) {
	if (pcp){
	  cout<<endl;
	  cout<<"CLEANING BEING DONE"<<endl;
	  cout<<"the jet "<< ijet<< "with pt = "<<Jets_In.at(ijet)->Pt()<<endl;
	  cout<<"is going to be cleaned because of the muon "<<imu<<" with "<<endl;
	  cout<<"a pt of "<<Muons.at(imu).Pt()<<endl;
	  cout<<"this muon has id Tight "<<Muons.at(imu).IsID("Tight")<<endl;
	  cout<<"this muon has id Veto "<<Muons.at(imu).IsID("Veto")<<endl;
	  cout<<endl;
	}
	dumpJet=true;
	break;
      }
    }

    if (!CrossCleaning.keepIf("CrossCleaning Muons",!dumpJet)) continue;


    for(int iel=0; iel<(int)Electrons.size();++iel){
      if(pcp)cout<<"distance from jet "<<ijet<<" to electron "<<iel<< " = "<<
	
	DeltaR(Jets_In.at(ijet)->P4(), Electrons.at(iel).P4())<<" lep= "<<Electrons.at(iel).Flavor()<<endl;
      //if (Electrons.at(iel).IsID("Veto")) cout << "warning" << endl;      

      if ( !Electrons.at(iel).IsID("Tight") &&  !Electrons.at(iel).IsID("Veto"))continue;

      if(DeltaR(Jets_In.at(ijet)->P4(),Electrons.at(iel).P4())<0.3) {
	if (pcp){
	  cout<<"the jet "<< ijet<< "with pt = "<<Jets_In.at(ijet)->Pt()<<endl;
	  cout<<"is going to be cleaned because of the electron "<<iel<<" with "<<endl;
	  cout<<"a pt of "<<Electrons.at(iel).Pt()<<endl;
	}
	dumpJet=true;
	break;
      }
    }
    if (!CrossCleaning.keepIf("CrossCleaning Electrons",!dumpJet)) continue;


    //WE want this jet, keep it.
    Jets_Out.push_back(Jets_In.at(ijet));

    if(pcp)cout<<"pt and eta of jet "<<ijet<<" = "<<Jets_In.at(ijet)->P4().pt()<<" "<<Jets_In.at(ijet)->P4().eta()<<endl;

  }




}
