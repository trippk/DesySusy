#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "TDirectory.h"
#include "THTools.h"
#include "eventselection.h"
#include "Electron.h"
#include "Muon.h"
#include "Jet.h"
#include "CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CMSSW_5_3_3_patch2/src/CondFormats/JetMETObjects/interface/SimpleJetCorrectionUncertainty.h"
#include "systematics_RA4b.h"
#include "Math/VectorUtil.h"
#include <boost/shared_ptr.hpp>
#include "typedefs.h"

//typedef boost::shared_ptr<LorentzM> Ptr_LorentzM;
//typedef boost::shared_ptr<Jet> Ptr_Jet;

using namespace std;
using namespace ROOT::Math::VectorUtil;

extern bool pcp;

//template<typename Ptr_Jet>
void rescaleHT(vector<Ptr_Jet>& Jets_in,Systematics& systematics, const string name){
  
  double newHT=0;
  for (int i=0;i<Jets_in.size();++i){ 
    newHT+=Jets_in.at(i)->Pt();
  }
  systematics.SetsysHT(name,newHT);
}

//template<typename Ptr_Jet>
void rescaleJets(EasyChain* tree, vector<Ptr_Jet>& Jets_in, Systematics& systematics){

  //  template<typename Ptr_Jet>  
  void JECUncertainty(Ptr_Jet Jet_in, Systematics& systematics, EasyChain* tree);
  //void JECUncertainty(Ptr_Jet );

  /*This function rescales the pt of the jets
    and recalculates HT and MET.
  */

  //cout<<"inside rescaleJetsHTMET"<<endl;

  ConfigReader config;

  static float  PTMIN  =  config.getFloat("Jets_PTMIN");
  //static float Jet_SCALE= config.getFloat(scale,1.0);
  //

  //===========FEED THE JETS TO THE JECUNC CALCULATOR
  for (int i=0;i<Jets_in.size();++i){
    JECUncertainty(Jets_in.at(i), systematics ,tree);
  }
  //the up and down variations are now inside the jets//

  //===========PUT THEM INSIDE THE systematics OBJECT

  vector<Ptr_Jet> dummyvector_UP;
  vector<Ptr_Jet> dummyvector_DOWN;
  for (int i=0;i<Jets_in.size();++i){


    //ONLY RESCALE JETS WITH PT > 10.0
    if (Jets_in.at(i)->Pt()<10.0)continue;
    //===========CONSTRUCT THE NEW LORENTZ VECTOR
    //LorentzM* dummyLorentz_UP= new LorentzM(Jets_in.at(i)->P4());
    //LorentzM* dummyLorentz_DOWN= new LorentzM(Jets_in.at(i)->P4());

    Ptr_LorentzM dummyLorentz_UP (new LorentzM(Jets_in.at(i)->P4())); 
    Ptr_LorentzM dummyLorentz_DOWN( new LorentzM(Jets_in.at(i)->P4()));
    //cout<<"just created the pointers "<<dummyLorentz_UP<<" and "<<dummyLorentz_DOWN<<endl;

    //==========RESCALE IT
    *dummyLorentz_UP *=(1+Jets_in.at(i)->GetCorrectionUncertainty("up"));
    *dummyLorentz_DOWN *=(1-Jets_in.at(i)->GetCorrectionUncertainty("down"));
    
    //=========CONSTRUCT THE NEW JET OBJECT
    //    cout<<"feeding dummyjet with "<<dummyLorentz_UP<<endl;
    //Jet* dummyJet_UP = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_UP);
    //    Jet* dummyJet_DOWN = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_DOWN);

    
    Ptr_Jet dummyJet_UP(new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_UP));
    Ptr_Jet dummyJet_DOWN(new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_DOWN));

    //==========STORE IT IN systematics
    systematics.GetsysJet("jetup").push_back(dummyJet_UP);
    systematics.GetsysJet("jetdown").push_back(dummyJet_DOWN);

  }
  //  cout<<endl;
}

//template<typename Ptr_Jet>
void rescaleClusters(vector<Ptr_Jet>& Jets_in,Systematics& systematics){

  for (int i=0;i<Jets_in.size();++i){
    
    if(Jets_in.at(i)->Pt()>10.0)continue;
    if(abs(Jets_in.at(i)->Eta())>5.0)continue;
    
    //===========CONSTRUCT THE NEW LORENTZ VECTOR
    //LorentzM* dummyLorentz_UP= new LorentzM(Jets_in.at(i)->P4());
    //LorentzM* dummyLorentz_DOWN= new LorentzM(Jets_in.at(i)->P4());
    Ptr_LorentzM dummyLorentz_UP( new LorentzM(Jets_in.at(i)->P4()));
    Ptr_LorentzM dummyLorentz_DOWN( new LorentzM(Jets_in.at(i)->P4()));
    
    //==========RESCALE IT
    *dummyLorentz_UP *=(1+0.1);
    *dummyLorentz_DOWN *=(1-0.1);
    
    //=========CONSTRUCT THE NEW JET OBJECT
    //Jet* dummyJet_UP = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_UP);
    //Jet* dummyJet_DOWN = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_DOWN);
    Ptr_Jet dummyJet_UP(new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_UP));
    Ptr_Jet dummyJet_DOWN(new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_DOWN));

    //==========STORE IT IN systematics
    systematics.GetsysJet("clustersup").push_back(dummyJet_UP);
    systematics.GetsysJet("clusterdown").push_back(dummyJet_DOWN);

  }
}
    



//template<typename Ptr_Jet>
void rescaleMET(EasyChain* tree, vector<Ptr_Jet>& Jets_in, Systematics& systematics, string name){

  //cout<<"doing rescale MET for "<<name<<endl;
  LorentzM& PFmet = tree->Get(&PFmet, "metP4TypeIPF");
  
  //  cout<<endl;
  //  cout<<endl;
  double sum_px=0.0;
  double sum_py=0.0;
  double sum_pz=0.0;

  //  cout<<"the size of the sysJet collecion "<<systematics.GetsysJet(name).size()<<endl;
  

  for (int i=0;i<systematics.GetsysJet(name).size();++i){
    int mndx=-1;
    int indx=systematics.GetsysJet(name).at(i)->GetIndexInTree();
    for (int k=0;k<Jets_in.size();++k){
      if (Jets_in.at(k)->GetIndexInTree()==indx){
	mndx=k;
      }
    }
    if(mndx==-1)continue;

    sum_px+=systematics.GetsysJet(name).at(i)->Px() - Jets_in.at(mndx)->Px();
    sum_py+=systematics.GetsysJet(name).at(i)->Py() - Jets_in.at(mndx)->Py();
    
  }
  //  for (int i=0;i<Jets_in.size();++i){ 
    //if (Jets_in.at(i)->Pt()<10.0)continue;
    //cout<<"rescaled px up is "<<systematics.GetsysJet(name).at(i)->Px()<<endl;
    //    cout<<"rescaled py up is "<<systematics.GetsysJet(name).at(i)->Py()<<endl;

    //    cout<<"change up "<<systematics.GetsysJet(name).at(i)->Px() - Jets_in.at(i)->Px()<<endl;
    //    sum_px+=systematics.GetsysJet(name).at(i)->Px() - Jets_in.at(i)->Px();
    //    sum_py+=systematics.GetsysJet(name).at(i)->Py() - Jets_in.at(i)->Py();
    //  }

  //cout<<"total change up px"<<sum_px<<endl;
  //cout<<"total change up py"<<sum_py<<endl;
  double newpx=PFmet.Px()-sum_px;
  double newpy=PFmet.Py()-sum_py;
  //  cout<<"the central one SHOULD BE "<<sqrt(PFmet.Px()
  systematics.SetsysMET(name, sqrt(newpx*newpx + newpy*newpy));
  systematics.SetsysMETVector(name,newpx,newpy);
  //  cout<<"====new MET up "<<sqrt(newpx*newpx + newpy*newpy)<<endl;
  //  cout<<"====old met was "<<PFmet.Et()<<endl;
  //  cout<<"in systematics, I stored"<<systematics.GetsysMETVector(name)->Et()<<endl;
  //  cout<<"and the value "<<systematics.GetsysMET(name)<<endl;

  //
  //

}


/*
void rescaleMET(EasyChain* tree, vector<Jet*>& Jets_in, Systematics& systematics){
  
  
  sum_px=0.0;
  sum_py=0.0;
  for (int i=0;i<Jets_in.size();++i){ 
    //if (Jets_in.at(i)->Pt()<10.0)continue;    
    //cout<<"rescaled px down is "<<systematics.GetsysJet("jetdown").at(i)->Px()<<endl;
    //cout<<"rescaled py down is "<<systematics.GetsysJet("jetdown").at(i)->Py()<<endl;
    //cout<<"change down "<<systematics.GetsysJet("jetdown").at(i)->Px() - Jets_in.at(i)->Px()<<endl;
    sum_px+=systematics.GetsysJet("jetdown").at(i)->Px() - Jets_in.at(i)->Px();
    sum_py+=systematics.GetsysJet("jetdown").at(i)->Py() - Jets_in.at(i)->Py();
  }

  //
  //cout<<"total change down px"<<sum_px;
  //cout<<"total change down py"<<sum_py;
  newpx=PFmet.Px()+sum_px;
  newpy=PFmet.Py()+sum_py;
  systematics.SetsysMET("jetdown", sqrt(newpx*newpx + newpy*newpy));
  systematics.SetsysMEVector("jetdown",newpx,newpy);
  //  cout<<"new MET down "<<sqrt(newpx*newpx + newpy*newpy)<<endl;
  //  cout<<"the central one is "<<PFmet.Pt()<<endl;
  //
  //
  //
  /*  vector<Jet> dummyvector; 
  for (int i=0;i<Jets_in.size();++i){
    Jet dummyJet=*(Jets_in.at(i));    
    if (&dummyJet == Jets_in.at(i)){ 
      cout<<"the adresses are the same. ERROR"<<endl; 
    } 
    dummyvector.push_back(dummyJet);
    }*/









//template<typename Ptr_Jet>
void JECUncertainty(Ptr_Jet Jet_in, Systematics& systematics, EasyChain* tree){


  extern TH1D* JetUncDiff;
  // Calculate uncertainty per source and as a total
  double jetpt=Jet_in->Pt();
  double jeteta=Jet_in->Eta();
  double sum2_up(0), sum2_dw(0);
  
  vector<float>& ak5JetPFJecUncPat= tree->Get(&ak5JetPFJecUncPat,"ak5JetPFJecUncPat");

  for (int isrc = 0; isrc < systematics.vsrc.size(); isrc++) {
    //
    //    cout<<"I am calling the setJetPt"<<endl;
    JetCorrectionUncertainty *unc = systematics.vsrc.at(isrc);
    unc->setJetPt(jetpt);
    unc->setJetEta(jeteta);
    //    cout<<"calling getUncertainty from here 1 at "<<isrc<<endl;
    
    double sup = unc->getUncertainty(true); // up variation
    //cout<<"sup = "<<sup<<endl;
    unc->setJetPt(jetpt);
    unc->setJetEta(jeteta);
    ///    cout<<"calling getUncertainty from here 2 at"<<isrc<<endl;
    double sdw = unc->getUncertainty(false); // down variation


    sum2_up += pow(max(sup,sdw),2);
    sum2_dw += pow(min(sup,sdw),2);
  } // for isrc


  Jet_in->SetCorrectionUncertainty("up",sqrt(sum2_up));
  Jet_in->SetCorrectionUncertainty("down",sqrt(sum2_dw));

  JetUncDiff->Fill((sqrt(sum2_up)-ak5JetPFJecUncPat.at(Jet_in->GetIndexInTree()))/sqrt(sum2_up));

  systematics.total->setJetPt(jetpt);
  systematics.total->setJetEta(jeteta);
  //  cout<<"calling getUncertainty from here 3"<<endl;
  double uncert_up = systematics.total->getUncertainty(true);
  //  xocout<<"calling getUncertainty from here 4"<<endl;
  //  double uncert_down = total->getUncertainty(false);

  // Check that quadratic sum of sources equals total uncertainty
  assert(fabs(uncert_up -   sqrt(sum2_up)) < 1e-3);
  //assert(fabs(uncert_down - sqrt(sum2_dw)) < 1e-3);



}




//template<typename Ptr_Jet>

void rescaleJetsJER(vector<Ptr_Jet>& Jets, Systematics& systematics){
  double getJerSF(double, double);
  for (int ijet=0; ijet<Jets.size();++ijet){
    
    if(Jets.at(ijet)->Pt()<10.0)continue;
    
    //===========GENJET MATCHED TO IT
    double ptGen=-1.0;
    if(Jets.at(ijet)->IsMatch()){
      ptGen=Jets.at(ijet)->GetPartner()->Pt();
    }
    else{
      continue;
    }
    //===========OBTAIN THE SCALE FACTOR
    double etajet=Jets.at(ijet)->Eta();
    double ptjet =Jets.at(ijet)->Pt();
    double JerSF_UP=getJerSF(etajet,1.0);
    double JerSF_DOWN=getJerSF(etajet,-1.0);
    //cout<<"the correction factor UP is "<<JerSF_UP<<endl;
    //===========CONSTRUCT THE NEW JET OBJECTS
    //Jet* newJet_UP =new Jet(Jets.at(ijet));
    //Jet* newJet_DOWN =new Jet(Jets.at(ijet));
    Ptr_Jet newJet_UP(new Jet(Jets.at(ijet)));
    Ptr_Jet newJet_DOWN(new Jet(Jets.at(ijet)));
    //cout<<"the new pt up is "<<newJet_UP->Pt()<<endl;
    //===========RESCALE IT
    double rescFactor_UP=max(0.,ptGen+JerSF_UP*(ptjet-ptGen));
    double rescFactor_DOWN=max(0.,ptGen+JerSF_DOWN*(ptjet-ptGen));
    double ratio_UP    =rescFactor_UP/newJet_UP->Pt();
    double ratio_DOWN  =rescFactor_DOWN/newJet_DOWN->Pt();
    newJet_UP->SetP4(newJet_UP->P4()*ratio_UP);
    newJet_DOWN->SetP4(newJet_DOWN->P4()*ratio_DOWN);
    //=====================
    systematics.GetsysJet("jetResup").push_back(newJet_UP);
    systematics.GetsysJet("jetResdown").push_back(newJet_DOWN);
    //    cout<<"new jet up "<<newJet_UP->Pt()<<endl;
    //    cout<<"new jet down "<<newJet_DOWN->Pt()<<endl;
    //    cout<<"central value"<<ptjet<<endl;
    //cout<<endl;
  }
}


//void JERUncertainty(Jet* Jet_in, Systematics& systematics){

  //double genJetPt=;
  //double getJetEta=;

  /*
  bool doGaussianResolution = config.getBool("doGaussianResolution",false);
  JetResolution ptResol(resFileName,doGaussianResolution);

  TF1* fPtResol=ptResol.resolutionEtaPt(genJetEta,genJetPt);
  float rndm = fPtResol->GetRandom();
  float jetpt = rndm*genpt;

  */
  

//}


double getJerSF(double eta, double err_factor) {

  double sf = 1.;

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





void ShiftJetEnergyScale(EasyChain* tree, Systematics& systematics, vector<Ptr_Jet>& jets, vector<Muon*>& Muons, vector<Electron*>& Electrons){
  
  void rescaleJets(EasyChain*, vector<Ptr_Jet>&,Systematics& );
  void rescaleMET(EasyChain*, vector<Ptr_Jet>&,Systematics&,string );
  void rescaleHT(vector<Ptr_Jet>&,Systematics&, const string );
  void makeGoodJets(EasyChain* tree, vector<Ptr_Jet>& AllJets, vector<Ptr_Jet>& goodJets );
  void makeCleanedJets(vector<Ptr_Jet>& Jets_In, vector<Ptr_Jet>& Jets_Out, vector<Muon*>& Muons, vector<Electron*>& Electrons);

  if(!systematics.GetSysMap()["jetup"])return;
  //=========RESCALE THE JETS
  rescaleJets(tree,jets,systematics);
  //
  //=========RESCALE THE MET
  rescaleMET(tree,systematics.GetsysJet("jetup"),systematics, "jetup");
  rescaleMET(tree,systematics.GetsysJet("jetdown"),systematics, "jetdown");
  //======FEED THE RESCALED JETS TO makeGoodJets
  makeGoodJets(tree,systematics.GetsysJet("jetup"),systematics.GetsysJet("jetup_good"));
  makeGoodJets(tree,systematics.GetsysJet("jetdown"),systematics.GetsysJet("jetdown_good"));
  //
  //======CLEAN THEM
  makeCleanedJets(systematics.GetsysJet("jetup_good"),systematics.GetsysJet("jetup_good_clean"),Muons,Electrons);
  makeCleanedJets(systematics.GetsysJet("jetdown_good"),systematics.GetsysJet("jetdown_good_clean"),Muons,Electrons);
  //
  //=====GET THE NEW HT
  rescaleHT(systematics.GetsysJet("jetup_good_clean"), systematics, "jetup");
  rescaleHT(systematics.GetsysJet("jetdown_good_clean"), systematics, "jetdown");
}

void ShiftClustersEnergyScale(EasyChain* tree, Systematics& systematics, vector<Ptr_Jet>& jets, vector<Muon*>& Muons, vector<Electron*>& Electrons){

  void rescaleClusters(vector<Ptr_Jet>&,Systematics&); 
  void rescaleMET(EasyChain*, vector<Ptr_Jet>&,Systematics&,string );
  if(!systematics.GetSysMap()["clustersup"])return;
  //
  rescaleClusters(jets,systematics);
  rescaleMET(tree,systematics.GetsysJet("clustersup"),systematics,"clustersup");
  rescaleMET(tree,systematics.GetsysJet("clustersdown"),systematics,"clustersdown");
  //
}
