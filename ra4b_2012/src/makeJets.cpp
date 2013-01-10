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


/*
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
*/






void makeAllJets(EasyChain* tree, vector<Ptr_Jet>& AllJets){

 if(pcp){
    cout<<endl;
    cout<<"INSIDE makeAllJets "<<endl;
    cout<<endl;
 }

 // vector<Jet> Jets; 
 // Jets.clear();
 
 vector<LorentzM>&  Jets_p4 = tree->Get(&Jets_p4, "ak5JetPFCorrectedP4Pat");
 vector<float>&     Jets_CorrFactor = tree->Get(&Jets_CorrFactor, "ak5JetPFCorrFactorPat");
 vector<int>&       Jets_genFlavor = tree->Get(&Jets_genFlavor, "ak5JetPFgenJetFlavourPat");  


 for(int ijet = 0; ijet<Jets_p4.size(); ijet++){  
   //THE LORENTZ VECTOR IS OWNED BY THE TREE
   //SO NO shared_ptr to LorentzM CAN BE USED HERE

   //new normal shared_pointer-to-jet using a normal pointer-to-LorentzM (so that the jet does not
   //own the LorentzM)

   //========create the shared_ptr
   Ptr_Jet dummysharedJet(new Jet(ijet,&(Jets_p4.at(ijet)), Jets_CorrFactor.at(ijet), "AK5")); 

   //======Set the flavour
   dummysharedJet->SetGenFlavor(Jets_genFlavor.at(ijet)); 
   AllJets.push_back(dummysharedJet);


   //Clarification:
   //When the last Ptr_Jet is deleted, the Jet object is also deleted.
   //However, the pointer to the lorentzM is not a shared pointer
   //and the LorentzM remains alive, because the lorentzM has to be destroyed when 
   //the TTree goes to the next event.
 }

 //return Jets;
}




//makegoodJets with shared pointers
void makeGoodJets(EasyChain* tree, vector<Ptr_Jet>& AllJets, vector<Ptr_Jet>& goodJets){

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
    
    //cout<<"jets_csv at "<<ijet<<" is "<<Jets_CSV.at(ijet)<<endl;
    
    AllJets.at(ijet)->SetBJetDisc("CSV", Jets_CSV.at(ijet));

    //    cout<<"out of SetBJetDisc"<<endl;
    AllJets.at(ijet)->SetID("Loose",1);
    goodJets.push_back(AllJets.at(ijet));
    //
    //
  }//Loop over Jets
  
  //  return AllJets;
  //retun true;
};




//makeCleanedJets with shared pointers
void makeCleanedJets(vector<Ptr_Jet>& Jets_In, vector<Ptr_Jet>& Jets_Out, vector<Muon*>& Muons, vector<Electron*>& Electrons){
  
  Jets_Out.clear();
  
  //Distance between the jet and the iso leptons
  ConfigReader config;
  static float  DELTAR_CUT  =  config.getFloat("Jets_CLEANDELTAR",  0.3 );

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
	
	DeltaR(Jets_In.at(ijet)->P4(), Muons.at(imu)->P4())<<" lep= "<<Muons.at(imu)->Flavor()<<endl;

      if ( !Muons.at(imu)->IsID("Tight") &&  !Muons.at(imu)->IsID("Veto"))continue;

      if(DeltaR(Jets_In.at(ijet)->P4(),Muons.at(imu)->P4())<DELTAR_CUT) {
	if (pcp) {
	  cout<<endl;
	  cout<<"CLEANING BEING DONE"<<endl;
	  cout<<"the jet "<< ijet<< "with pt = "<<Jets_In.at(ijet)->Pt()<<endl;
	  cout<<"is going to be cleaned because of the muon "<<imu<<" with "<<endl;
	  cout<<"a pt of "<<Muons.at(imu)->Pt()<<endl;
	  cout<<"this muon has id Tight "<<Muons.at(imu)->IsID("Tight")<<endl;
	  cout<<"this muon has id Veto "<<Muons.at(imu)->IsID("Veto")<<endl;
	  cout<<endl;
	}
	dumpJet=true;
	break;
      }
    }

    if (!CrossCleaning.keepIf("CrossCleaning Muons",!dumpJet)) continue;

    for(int iel=0; iel<(int)Electrons.size();++iel){
      if(pcp)cout<<"distance from jet "<<ijet<<" to electron "<<iel<< " = "<<
	
	DeltaR(Jets_In.at(ijet)->P4(), Electrons.at(iel)->P4())<<" lep= "<<Electrons.at(iel)->Flavor()<<endl;
      //if (Electrons.at(iel).IsID("Veto")) cout << "warning" << endl;      

      if ( !Electrons.at(iel)->IsID("Tight") &&  !Electrons.at(iel)->IsID("Veto"))continue;

      if(DeltaR(Jets_In.at(ijet)->P4(),Electrons.at(iel)->P4())< DELTAR_CUT) {
	if (pcp){
	  cout<<"the jet "<< ijet<< "with pt = "<<Jets_In.at(ijet)->Pt()<<endl;
	  cout<<"is going to be cleaned because of the electron "<<iel<<" with "<<endl;
	  cout<<"a pt of "<<Electrons.at(iel)->Pt()<<endl;
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



void makeCleanedJets(vector<Ptr_Jet>& Jets_In, vector<Ptr_Jet>& Jets_Out, vector<Muon>& Muons, vector<Electron>& Electrons){

  cout<<"this function is deprecated, it should not be called!"<<endl;

  void makeCleanedJets(vector<Ptr_Jet>&, vector<Ptr_Jet>&, vector<Muon*>&, vector<Electron*>&); 
  Jets_Out.clear();

  vector<Muon*>newMuons;
  vector<Electron*>newElectrons;

  for (int imu=0;imu<Muons.size();++imu){
    newMuons.push_back(&Muons.at(imu));
  }
  for (int iel=0;iel<Electrons.size();++iel){
    newElectrons.push_back(&Electrons.at(iel));
  }

  makeCleanedJets(Jets_In,Jets_Out,newMuons,newElectrons);

}



void makeAllGenJets(EasyChain* tree, vector<Ptr_GenJet>& genjets){
  vector<LorentzM>&   GenP4 = tree->Get(&GenP4,"genak5GenJetsP4");
  vector<int>& jetflavor        = tree->Get(&jetflavor,"ak5JetPFgenJetFlavourPat");
  vector<LorentzM>&  Jets_p4 = tree->Get(&Jets_p4, "ak5JetPFCorrectedP4Pat");
  /*    if (GenP4.size() != jetflavor.size()){
      cout<<"warning, the sizes are different in makeAllGenJets"<<endl;
      cout<<GenP4.size()<<" vs "<<jetflavor.size()<<endl;
      cout<<"and"<<Jets_p4.size()<<endl;
      cout<<endl;
    }
  */
  //  }

  for(int ijet = 0; ijet<GenP4.size(); ijet++){
    Ptr_GenJet dummypointer(new GenJet(ijet,&GenP4.at(ijet),"AK5"));
    genjets.push_back(dummypointer);
  }
}



//template<typename Ptr_Jet1>
void matchGenJets(EasyChain* tree, vector<Ptr_GenJet>& genJets, vector<Ptr_Jet>& Jets){

  //extern TH1D* distmatched;
  
  vector<int>&   MatchedGenJets = tree->Get(&MatchedGenJets,"ak5JetPFGenJetMatchIndexPat");


  for (int ijet=0;ijet<Jets.size();++ijet){
    int indx=Jets.at(ijet)->GetIndexInTree();
    int mindex=MatchedGenJets.at(indx);
    if(mindex==-1 and Jets.at(ijet)->Pt()>40.0)continue;
    for (int mjet=0;mjet<genJets.size();++mjet){
      int mndx=genJets.at(mjet)->GetIndexInTree();
      bool match_found=false;
      if (mndx == mindex ){
	Jets.at(ijet)->SetIsMatch(true);
	genJets.at(mjet)->SetIsMatch(true);
	Jets.at(ijet)->SetPartner(genJets.at(mjet));
	genJets.at(mjet)->SetPartner(Jets.at(ijet));
	//	distmatched->Fill(ROOT::Math::VectorUtil::DeltaR(Jets.at(ijet)->P4(),genJets.at(mjet)->P4()));
      }
    }
  }
}





















//====================================
// PREVIOUS FUNCTIONS
//===================================

void rescaleJER(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jerSF_err, JetMonitor * pJetM) {

  vector<LorentzM>&  genJets_p4 = tree->Get(&genJets_p4, "genak5GenJetsP4");
  //vector<int>&  genJets_matched = tree->Get(&genJets_matched, "ak5JetPFGenJetMatchIndexPat");

  vector<int> recoToGen(AllJets.size(), -1);
  vector<int> genToReco(genJets_p4.size(), -1);
  matchJets(AllJets, genJets_p4, recoToGen, genToReco);

  typedef vector<Jet*>::iterator jetIt;
  for (jetIt jet = AllJets.begin(); jet != AllJets.end(); jet++) {
    if (  (*jet)->Pt() > 10. && fabs((*jet)->Eta()) < 5.) {
      //Get core resolution factor.
      //Numbers taken from the Twiki CMS/JetResolution
      float jerSF = getJerSF((*jet)->Eta(), jerSF_err);
      if (pcp) cout << "jerSF: " << jerSF << endl;

      //Get the matched gen level jet.
      //int indexOfMatchedGenJet = genJets_matched.at( (*jet)->GetIndexInTree() );
      int indexOfMatchedGenJet = recoToGen[jet-AllJets.begin()];
      if (pcp) cout << "Matched to gen jet: " << indexOfMatchedGenJet << endl;
      
      //Check if matched index is ok.
      if (indexOfMatchedGenJet >= 0 && indexOfMatchedGenJet < genJets_p4.size() ) {
      
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

	//Store the information in the jet monitor
	if (pJetM) {
	  pJetM->hMatched->Fill(oldP4.Pt(), oldP4.Eta());
	  float deltaR = ROOT::Math::VectorUtil::DeltaR(oldP4, genP4);;
	  pJetM->hMatchedDeltaR->Fill(oldP4.Pt(), oldP4.Eta(), deltaR);
	  float deltaPt = oldP4.Pt() - genP4.Pt();
	  pJetM->hMatchedDeltaPt->Fill(oldP4.Pt(), oldP4.Eta(), deltaPt);
	  pJetM->hMatchedScaleFactor->Fill(oldP4.Pt(), oldP4.Eta(), jetRescale);
	}
	
      }
      else {

	LorentzM oldP4 = (*jet)->P4();
	float oldE = oldP4.E();
	
	float jetRes = getJetRes(oldP4.Pt(), oldP4.Eta());
	
	float newE = oldE;
	float gSigma = jerSF*jerSF - 1.;
	if (gSigma > 0.) {
	  gSigma = jetRes * sqrt(gSigma);
	  TRandom3 rand(0); //Random num gen with seed set by sys time
	  newE += rand.Gaus(0., gSigma);
	}
	float jetRescale = newE / oldE;
	if (jetRescale < 0.) jetRescale = 0.;
	if (pcp) cout << "Jet rescale factor: " << jetRescale << endl;

	metCorr += oldP4 * (1. - jetRescale);
	(*jet)->SetP4(oldP4 * jetRescale);

	//Store the jet info in the jet monitor
	if (pJetM) {
	  pJetM->hNotMatched->Fill(oldP4.Pt(), oldP4.Eta());
	  pJetM->hNotMatchedDeltaE->Fill(oldP4.Pt(), oldP4.Eta(), newE - oldE);
	  pJetM->hNotMatchedScaleFactor->Fill(oldP4.Pt(), oldP4.Eta(), jetRescale);
	}

      }
    }
  }


  return;
}

float getJerSF(float eta, float err_factor) {

  float sf = 1.;

  //Values taken from AN-2011-330
  if ( fabs(eta) < 0.5 ) {
    sf = 1.052 + err_factor * 0.065;
  }
  else if (fabs(eta) < 1.1) {
    sf = 1.057 + err_factor * 0.059;
  }
  else if (fabs(eta) < 1.7) {
    sf = 1.096 + err_factor * 0.070;
  }
  else if (fabs(eta) < 2.3 ){
    sf = 1.134 + err_factor * 0.102;
  }
  else {
    sf = 1.288 + err_factor * 0.222;
  }
  
  if (sf < 0.) {
    cout << "getJerSF >> ERROR sf is negative. Set to zero." << endl;
    sf = 0.;
  }

  return sf;
}

void rescaleJES(EasyChain* tree, vector<Jet*>& AllJets, LorentzM & metCorr, float jesSF_err) {

  vector<float>&  jetJecUnc = tree->Get(&jetJecUnc, "ak5JetPFJecUncPat");

  for (int iJet = 0 ; iJet < AllJets.size(); iJet++) {
    Jet* cJet = AllJets.at(iJet);

    if (  (cJet)->Pt() > 10. && fabs((cJet)->Eta()) < 5.) {
      //Get the full factor by which to rescale the jet p4.
      float jetRescale = 1 + jesSF_err * jetJecUnc.at(iJet);
      if (jetRescale < 0.) jetRescale = 0.;
      if (pcp) cout << "Jet rescale factor: " << jetRescale << endl;
      
      //Rescale the Jet's P4.
      LorentzM oldP4 = cJet->P4();
      metCorr += oldP4 * (1. - jetRescale);
      cJet->SetP4(oldP4 * jetRescale);
    }
  }

  return;
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

float getJetRes(double pT, double eta) {

  double N=0., S=0., C=0., m=0.;

  if     ( fabs(eta) < 0.5) {N = -0.34921; S = 0.29783; C = 0.; m = 0.47112;}
  else if (fabs(eta) < 1.0) {N = -0.49974; S = 0.33639; C = 0.; m = 0.43069;}
  else if (fabs(eta) < 1.5) {N = -0.56165; S = 0.42029; C = 0.; m = 0.39240;}
  else if (fabs(eta) < 2.0) {N = -1.12329; S = 0.65789; C = 0.; m = 0.13960;}
  else if (fabs(eta) < 2.5) {N =  1.04792; S = 0.46676; C = 0.; m = 0.19314;}
  else if (fabs(eta) < 3.0) {N =  1.89961; S = 0.33432; C = 0.; m = 0.36541;}
  else if (fabs(eta) < 3.5) {N =  1.66267; S = 0.33780; C = 0.; m = 0.43943;}
  else if (fabs(eta) < 4.0) {N =  1.50961; S = 0.22757; C = 0.; m = 0.60094;}
  else if (fabs(eta) < 4.5) {N =  0.99052; S = 0.27026; C = 0.; m = 0.46273;}
  else                      {N =  1.37916; S = 0.28933; C = 0.; m = 0.61234;}

  double sgnN = N > 0. ? 1. : -1.;
  double sigma = sgnN * pow( N/pT , 2. ) + pow(S,2.)*pow(pT, m-1.) + pow(C,2.);
  if (sigma < 0.) sigma = 0.;

  sigma = pT * sqrt(sigma);

  return sigma;    
}


//Function for matching jets to genJets.
//Matching done by minimising deltaR. Ambiguities are resolved.
void matchJets( const vector<Jet*> & recoJets, const vector<LorentzM> & genJets, vector<int> & recoToGen, vector<int> & genToReco) {

  bool iterate = false;

  if (recoJets.size() != recoToGen.size() ) {
    cout << "matchJets >> ERROR. Vectors of different sizes!" << endl;
    return;
  }
  if (genJets.size() != genToReco.size() ) {
    cout << "matchJets >> ERROR. Vectors of different sizes!" << endl;
    return;
  }

  vector<vector<int> > genToReco_tmp;
  for (int iGen = 0; iGen < genToReco.size() ; iGen++ ) {
    vector<int> temp;
    if (genToReco[iGen] != -1) temp.push_back(genToReco[iGen]);
    genToReco_tmp.push_back( temp );
  }

  //Match any unmatched reco jets to any unmatched gen jet. 
  //Matching based on minimising deltaR.
  for (int iJet = 0; iJet < recoJets.size(); iJet++ ) {
    if (recoToGen[iJet] != -1) continue;

    int matchedGenJet = -1;
    double dR_min = 0.5; //Force matching to be within 0.5. Better choice?

    LorentzM recoP4 = recoJets[iJet]->P4();

    for (int iGen = 0; iGen < genToReco.size() ; iGen++) {
      if (genToReco[iGen] != -1) continue;
      LorentzM genP4 = genJets[iGen];
      double dR = DeltaR(genP4, recoP4);
      if (dR < dR_min) {
	dR_min = dR;
	matchedGenJet = iGen;
      }
    }

    if (matchedGenJet != -1) {
      recoToGen[iJet] = matchedGenJet;
      genToReco_tmp[matchedGenJet].push_back(iJet);
    }

  }

  //Now search for ambiguities.
  //Resolve by minimising deltaR.
  for (int iGen = 0 ; iGen < genToReco_tmp.size() ; iGen++ ) {
    
    vector<int> matchedRecoJets = genToReco_tmp[iGen];
    if (matchedRecoJets.size() == 1) {
      genToReco[iGen] = matchedRecoJets[0];
    }
    else if (matchedRecoJets.size() > 1) {
      //There is an ambiguity
      iterate = true;

      double dR_min = 1.0;
      int resolvedRecoMatch = -1;
      
      LorentzM genP4 = genJets[iGen];

      for (int iMatch = 0; iMatch < matchedRecoJets.size() ; iMatch++) {
	LorentzM recoP4 = recoJets[matchedRecoJets[iMatch]]->P4();
	double dR = DeltaR(recoP4, genP4);
	if (dR < dR_min) {
	  dR_min = dR;
	  resolvedRecoMatch = iMatch;
	}
      }
      
      if (resolvedRecoMatch != -1) {
	genToReco[iGen] = matchedRecoJets[resolvedRecoMatch];

	//Remove the link from the remaining reco jets
	matchedRecoJets.erase(matchedRecoJets.begin() + resolvedRecoMatch);
	for (int iMatch = 0; iMatch < matchedRecoJets.size() ; iMatch++ ) {
	  recoToGen[matchedRecoJets[iMatch]] = -1;
	}
      }
      else {
	cout << "THERE WAS AN ERROR IN THE MATCHING" << endl;
      }

    }
  }


  if (iterate) {
    if (pcp) cout << "matchJets >> ITERATING!" << endl;
    matchJets(recoJets, genJets, recoToGen, genToReco);
  }
  else return;
}
