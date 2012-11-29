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

#include "makeJets.h"

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
  vector<int>&       Jets_genFlavor = tree->Get(&Jets_genFlavor, "ak5JetPFgenJetFlavourPat");  
  for(int ijet = 0; ijet<Jets_p4.size(); ijet++){  

    Jet dummyJet;
    dummyJet.Set(ijet, &Jets_p4.at(ijet), Jets_CorrFactor.at(ijet), "AK5");    
    dummyJet.SetGenFlavor(Jets_genFlavor.at(ijet));
    Jets.push_back(dummyJet);
    
  }
  return Jets;
}

void rescaleJER(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jerSF_err) {

  vector<LorentzM>&  genJets_p4 = tree->Get(&genJets_p4, "genak5GenJetsP4");
  vector<int>&  genJets_matched = tree->Get(&genJets_matched, "ak5JetPFGenJetMatchIndexPat");

  typedef vector<Jet*>::iterator jetIt;
  for (jetIt jet = AllJets.begin(); jet != AllJets.end(); jet++) {
    if (  (*jet)->Pt() > 10. ) {
      //Get core resolution factor.
      //Numbers taken from the Twiki CMS/JetResolution
      float jerSF = getJerSF((*jet)->Eta(), jerSF_err);
      if (pcp) cout << "jerSF: " << jerSF << endl;

      //Get the matched gen level jet.
      int indexOfMatchedGenJet = genJets_matched.at( (*jet)->GetIndexInTree() );
      if (pcp) cout << "Matched to gen jet: " << indexOfMatchedGenJet << endl;
      
      //Check if matched index is ok.
      if (indexOfMatchedGenJet < 0 || indexOfMatchedGenJet >= genJets_p4.size() ) continue;
      
      //Get the P4 of the matched jet.
      LorentzM genP4 = genJets_p4.at(indexOfMatchedGenJet);
      if (pcp) {
	cout << "Jet P4=(" << (*jet)->Px() << ","  << (*jet)->Py() << ","  << (*jet)->Pz() << ","  << (*jet)->E() << ")" << endl;
	cout << "Matched gen jet P4=(" << genP4.Px() << ","  << genP4.Py() << ","  << genP4.Pz() << ","  << genP4.E() << ")" << endl;
      }

      //Get the full factor by which to rescale the jet p4.
      float jetRescale = genP4.Pt() + jerSF * ( (*jet)->Pt() - genP4.Pt() );
      jetRescale /= (*jet)->Pt();
      if (jetRescale < 0.) jetRescale = 0.;
      if (pcp) cout << "Jet rescale factor: " << jetRescale << endl;
      
      //Rescale the Jet's P4.
      LorentzM oldP4 = (*jet)->P4();
      metCorr += oldP4 * (1. - jetRescale);
      (*jet)->SetP4(oldP4 * jetRescale);
    }
  }


  return;
}

float getJerSF(float eta, float err_factor) {

  float sf = 1.;

  if ( fabs(eta) < 0.5 ) {
    sf = 1.052 + err_factor * sqrt( pow(0.012,2) + pow(0.062,2) );
  }
  else if (fabs(eta) < 1.1) {
    sf = 1.057 + err_factor * sqrt( pow(0.012,2) + pow(0.056,2) );
  }
  else if (fabs(eta) < 1.7) {
    sf = 1.096 + err_factor * sqrt( pow(0.017,2) + pow(0.063,2) );
  }
  else if (fabs(eta) < 2.3 ){
    sf = 1.134 + err_factor * sqrt( pow(0.035,2) + pow(0.087,2) );
  }
  else {
    sf = 1.288 + err_factor * sqrt( pow(0.127,2) + pow(0.155,2) );
  }
  
  if (sf < 0.) {
    cout << "getJerSF >> ERROR sf is negative. Set to zero." << endl;
    sf = 0.;
  }

  return sf;
}


void makeGoodJets(EasyChain* tree, vector<Jet*>& AllJets, vector<Jet*>& goodJets, CutSet* flow_in){

  if(pcp){
    cout<<endl;
    cout<<"INSIDE makeJets "<<endl;
    cout<<endl;
  }

  goodJets.clear();

  static CutSet jetFlow("good_jet_selection");
  jetFlow.autodump=true;

  CutSet* flow = &jetFlow;
  if(flow_in){
    flow=flow_in;
    jetFlow.autodump=false;
  }


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

    flow->keepIf("before cuts in jets", true);

    //
    OK=AllJets.at(ijet)->pt() > PTMIN;
    if( !flow->keepIf("Jets_PTMIN", OK) && quick) continue;

    // 
    OK=fabs(AllJets.at(ijet)->eta()) < ETAMAX;
    if( !flow->keepIf("jets_ETAMAX",OK) && quick ) continue;
    //
    OK=Jets_ID.at(indx)!=0;
    if( !flow->keepIf("ak5JetPFPFJetIDloosePat", OK ) && quick ) continue;

    //
    if(!quick){
      OK=flow->applyCuts("Jets_PTMIN jets_ETAMAX ak5JetPFPFJetIDloosePat");
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

void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon>& Muons, vector<Electron>& Electrons, CutSet* flow_in){
  
  Jets_Out.clear();
  
  //Distance between the jet and the iso leptons
  ConfigReader config;
  static float  DELTAR_CUT  =  config.getFloat("Jets_CLEANDELTAR",  0.3 );

  static CutSet CrossCleaning("Cleaned Jets");
  CrossCleaning.autodump=true;

  CutSet* flow = &CrossCleaning;
  if(flow_in){
    flow=flow_in;
    CrossCleaning.autodump=false;
  }


  
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

      if(DeltaR(Jets_In.at(ijet)->P4(),Muons.at(imu).P4())<DELTAR_CUT) {
	if (pcp) {
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

    if (!flow->keepIf("CrossCleaning Muons",!dumpJet)) continue;

    for(int iel=0; iel<(int)Electrons.size();++iel){
      if(pcp)cout<<"distance from jet "<<ijet<<" to electron "<<iel<< " = "<<
	
	DeltaR(Jets_In.at(ijet)->P4(), Electrons.at(iel).P4())<<" lep= "<<Electrons.at(iel).Flavor()<<endl;
      //if (Electrons.at(iel).IsID("Veto")) cout << "warning" << endl;      

      if ( !Electrons.at(iel).IsID("Tight") &&  !Electrons.at(iel).IsID("Veto"))continue;

      if(DeltaR(Jets_In.at(ijet)->P4(),Electrons.at(iel).P4())< DELTAR_CUT) {
	if (pcp){
	  cout<<"the jet "<< ijet<< "with pt = "<<Jets_In.at(ijet)->Pt()<<endl;
	  cout<<"is going to be cleaned because of the electron "<<iel<<" with "<<endl;
	  cout<<"a pt of "<<Electrons.at(iel).Pt()<<endl;
	}
	dumpJet=true;
	break;
      }
    }
    if (!flow->keepIf("CrossCleaning Electrons",!dumpJet)) continue;


    //WE want this jet, keep it.
    Jets_Out.push_back(Jets_In.at(ijet));

    if(pcp)cout<<"pt and eta of jet "<<ijet<<" = "<<Jets_In.at(ijet)->P4().pt()<<" "<<Jets_In.at(ijet)->P4().eta()<<endl;

  }




}


void makeCleanedJets(vector<Jet*>& Jets_In, vector<Jet*>& Jets_Out, vector<Muon*>& Muons, vector<Electron*>& Electrons, CutSet* flow_in){
  
  Jets_Out.clear();
  
  //Distance between the jet and the iso leptons
  ConfigReader config;
  static float  DELTAR_CUT  =  config.getFloat("Jets_CLEANDELTAR",  0.3 );

  static CutSet CrossCleaning("Cleaned Jets");
  CrossCleaning.autodump=true;

  CutSet* flow = &CrossCleaning;
  if(flow_in){
    flow=flow_in;
    CrossCleaning.autodump=false;
  }
  
  if(pcp){
    cout<<endl;
    cout<<"INSIDE makeCleanedJets "<<endl;
    cout<<endl;
  }

  for(int ijet = 0; ijet<Jets_In.size(); ijet++){

    bool dumpJet=false;
  
    for(int iel=0; iel<(int)Electrons.size();++iel){
      if(DeltaR(Jets_In.at(ijet)->P4(),Electrons.at(iel)->P4())< DELTAR_CUT) {
	dumpJet=true;
	break;
      }
    }
    if (!flow->keepIf("CrossCleaning Electrons",!dumpJet)) continue;

    for(int imu=0; imu<(int)Muons.size(); ++imu){
      if(DeltaR(Jets_In.at(ijet)->P4(),Muons.at(imu)->P4())<DELTAR_CUT) {
	dumpJet=true;
	break;
      }
    }
    if (!flow->keepIf("CrossCleaning Muons",!dumpJet)) continue;


    //WE want this jet, keep it.
    Jets_Out.push_back(Jets_In.at(ijet));

    if(pcp)cout<<"pt and eta of jet "<<ijet<<" = "<<Jets_In.at(ijet)->P4().pt()<<" "<<Jets_In.at(ijet)->P4().eta()<<endl;

  }
}
