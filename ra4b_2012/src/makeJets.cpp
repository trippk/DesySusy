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


vector<Jet> makeJets(EasyChain* tree ){

  if(pcp){
    cout<<endl;
    cout<<"INSIDE makeJets "<<endl;
    cout<<endl;
  }

  vector<Jet> Jets;
  Jets.clear();

  static CutSet jetFlow("good jet selection");
  jetFlow.autodump=true;

  vector<int>&       Jets_ID = tree->Get(&Jets_ID,"ak5JetPFPFJetIDloosePat");
  vector<LorentzM>&  Jets_p4 = tree->Get(&Jets_p4, "ak5JetPFCorrectedP4Pat");
  vector<float>&     Jets_CorrFactor = tree->Get(&Jets_CorrFactor, "ak5JetPFCorrFactorPat");
  vector<float>&     Jets_CSV = tree->Get(&Jets_CSV, "ak5JetPFCombinedSecondaryVertexBJetTagsPat");
  vector<float>&     Jets_CSVMVA = tree->Get(&Jets_CSVMVA, "ak5JetPFCombinedSecondaryVertexMVABJetTagsPat");

  unsigned int Event   = tree->Get(Event,"event");    
  
  extern bool pcp;

  if(pcp){
    cout<<"=================================================== "<<endl;
    cout<<"      going to check the jets in event "<< Event <<endl;
    cout<<"=================================================== "<<endl;
  }

  if(pcp)cout<<"total number of jets "<<Jets.size()<<endl;

  //====================================================================
  //READ OR DEFINE THE CUTS FOR THE JETS
  //====================================================================
  ConfigReader config;
  static float  PTMIN  =  config.getFloat("Jets_PTMIN",  40. );
  static float  ETAMAX  = config.getFloat("Jets_ETAMAX", 2.4 );
  static string btagAlgorithm= config.getString("bTagAlgorithm","CSV");
  static string btagWorkingPoint = config.getString("bTagWorkingPoint","Medium");
  //====================================================================

  bool OK=false;

  for(int ijet = 0; ijet<Jets_p4.size(); ijet++){

    Jet dummyJet;

    OK=false;

    if(pcp)cout<<"pt,eta, phi and id -->"<<Jets_p4.at(ijet).pt()<<" "<<Jets_p4.at(ijet).eta()<<" "<<Jets_p4.at(ijet).Phi()<<" "<<Jets_ID[ijet]<<endl;

    jetFlow.keepIf("jets no cuts yet in jets", true);

    //
    OK=Jets_p4.at(ijet).pt() > PTMIN;
    if( !jetFlow.keepIf("Jets pt min", OK )) continue;
    // 
    OK=fabs(Jets_p4.at(ijet).eta()) < ETAMAX;
    if( !jetFlow.keepIf("jets eta max",OK) ) continue;
    //
    if( !jetFlow.keepIf("ak5JetPFPFJetIDloosePat", Jets_ID[ijet]!=0 )) continue;

    dummyJet.Set(ijet, &Jets_p4.at(ijet), Jets_CorrFactor.at(ijet), "AK5");
    dummyJet.SetBJetDisc("CSV", Jets_CSV.at(ijet));
    dummyJet.SetID("Loose",1);
    Jets.push_back(dummyJet);

  }//Loop over Jets

  return Jets;
  //retun true;
};

void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon> Muons, vector<Electron> Electrons){
  
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
