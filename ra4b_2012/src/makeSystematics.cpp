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
#include "makeJets.h"
#include "TRandom3.h"

//typedef boost::shared_ptr<LorentzM> Ptr_LorentzM;
//typedef boost::shared_ptr<Jet> Ptr_Jet;

using namespace std;
using namespace ROOT::Math::VectorUtil;

extern bool pcp;


void rescaleHT(vector<Ptr_Jet>& Jets_in,Systematics& systematics, const string name){
  
  double newHT=0;
  for (int i=0;i<Jets_in.size();++i){ 
    newHT+=Jets_in.at(i)->Pt();
  }
  systematics.SetsysHT(name,newHT);
}


void rescaleJets(EasyChain* tree, vector<Ptr_Jet>& Jets_in, Systematics& systematics){

 
  void JECUncertainty(Ptr_Jet Jet_in, Systematics& systematics, EasyChain* tree);
  /*This function rescales the pt of the jets
    and recalculates HT and MET.
  */



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

    Ptr_LorentzM dummyLorentz_UP (new LorentzM(Jets_in.at(i)->P4())); 
    Ptr_LorentzM dummyLorentz_DOWN( new LorentzM(Jets_in.at(i)->P4()));
    //cout<<"just created the pointers "<<dummyLorentz_UP<<" and "<<dummyLorentz_DOWN<<endl;

    //==========RESCALE IT
    *dummyLorentz_UP *=(1+Jets_in.at(i)->GetCorrectionUncertainty("up"));
    *dummyLorentz_DOWN *=(1-Jets_in.at(i)->GetCorrectionUncertainty("down"));
    
    //=========CONSTRUCT THE NEW JET OBJECT
    //it now will co-own the dummyLorentz
    Ptr_Jet dummyJet_UP(new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_UP));
    Ptr_Jet dummyJet_DOWN(new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_DOWN));
    dummyJet_UP->copyJetStuff(Jets_in.at(i).get());
    dummyJet_DOWN->copyJetStuff(Jets_in.at(i).get());
    //cout<<"check "<<dummyJet_UP->BJetDisc("CSV")<<endl;
    //Ptr  dummyJet_UP(new Jet(Jets_in.at(i)); 
    //==========STORE IT IN systematics
    systematics.GetsysJet("jetup").push_back(dummyJet_UP);
    systematics.GetsysJet("jetdown").push_back(dummyJet_DOWN);

  }
  //  cout<<endl;
}

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

  LorentzM& PFmet = tree->Get(&PFmet, "metP4TypeIPF");
  

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









void JECUncertainty(Ptr_Jet Jet_in, Systematics& systematics, EasyChain* tree){


  //extern TH1D* JetUncDiff;
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

  //JetUncDiff->Fill((sqrt(sum2_up)-ak5JetPFJecUncPat.at(Jet_in->GetIndexInTree()))/sqrt(sum2_up));

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
  float getJetRes(double,double);

  ConfigReader config;
  string smearing = config.getString("JetRes_smearing","mixed");
  bool doRecoSmearing=config.getBool("doRecoSmearing",true);

  //cout<<"fucking smearing"<<endl;

  for (int ijet=0; ijet<Jets.size();++ijet){
    
    if(Jets.at(ijet)->Pt()<10.0)continue;


    double etajet=Jets.at(ijet)->Eta();
    double ptjet =Jets.at(ijet)->Pt();    
    //===========GENJET MATCHED TO IT
    double ptGen=-1.0;
    double oldE=Jets.at(ijet)->P4().E();
    double JerSF_CENTRAL=getJerSF(etajet,0.0);
    double JerSF_UP=getJerSF(etajet,1.0);
    double JerSF_DOWN=getJerSF(etajet,-1.0);
    
    Ptr_Jet newJet_CENTRAL(new Jet(Jets.at(ijet)));
    Ptr_Jet newJet_UP(new Jet(Jets.at(ijet)));
    Ptr_Jet newJet_DOWN(new Jet(Jets.at(ijet)));

    //===========OBTAIN THE SCALE FACTOR
    double ratio_CENTRAL=0.0;
    double ratio_UP=0.0;
    double ratio_DOWN=0.0;
    //
    //
    //MATCHED JETS
    if(Jets.at(ijet)->IsMatch()){
      ptGen=Jets.at(ijet)->GetPartner()->Pt();
      
    }
    else{
      ptGen=0;
    }

    
    //string doCut_ = config.getString("JetRes_smearing","mixed");

    bool doGenMethod=false;
    if(Jets.at(ijet)->IsMatch() && smearing=="mixed"){
      doGenMethod=true;

      //double newptreco=max(0.,ptGen+JerSF_UP*(ptjet-ptGen));
      if(fabs(ptjet-ptGen)/ptGen>0.5){
	doGenMethod=false;
      }    
    }
    
    //if(doGenMethod != (Jets.at(ijet)->IsMatch() && smearing=="mixed") ){
    //cout<<"whaaaaaat"<<endl;
    //cout<<doGenMethod<<" "<<Jets.at(ijet)->IsMatch() << smearing <<endl;
    //}
    //if(Jets.at(ijet)->IsMatch() && smearing=="mixed"){
    if(doGenMethod){

      //cout<<"here!!!"<<endl;
      //===========RESCALE IT
      double rescFactor_CENTRAL=max(0.,ptGen+JerSF_CENTRAL*(ptjet-ptGen));
      double rescFactor_UP=max(0.,ptGen+JerSF_UP*(ptjet-ptGen));
      double rescFactor_DOWN=max(0.,ptGen+JerSF_DOWN*(ptjet-ptGen));
      ratio_CENTRAL=rescFactor_CENTRAL/Jets.at(ijet)->Pt();
      ratio_UP    =rescFactor_UP/Jets.at(ijet)->Pt();
      ratio_DOWN  =rescFactor_DOWN/newJet_UP->Pt();
    }
    else{
      if(doRecoSmearing){
	//else if (smearing=="reco_smearing" || (smearing=="mixed" && !Jets.at(ijet)->IsMatch()) ){
	//if they are not matched, put the unsmeared ones 
	double newE=0;
	double newJetRes=getJetRes(ptjet, etajet);
	//
	double gSigma_CENTRAL=JerSF_CENTRAL*JerSF_CENTRAL-1.0;
	double gSigma_UP=JerSF_UP*JerSF_UP-1.0;
	double gSigma_DOWN=JerSF_DOWN*JerSF_DOWN-1.0;
	//
	newE=oldE;
	if(gSigma_CENTRAL>0.0){
	  gSigma_CENTRAL=newJetRes*sqrt(gSigma_CENTRAL);
	  TRandom3 rand1(0);
	  newE+=rand1.Gaus(0.0,gSigma_CENTRAL);
	}
	ratio_CENTRAL=newE/oldE;
	//
	newE=oldE;
	if(gSigma_UP>0.0){
	  gSigma_UP=newJetRes*sqrt(gSigma_UP);
	  TRandom3 rand2(0);
	  newE+=rand2.Gaus(0.0,gSigma_UP);
	}
	ratio_UP=newE/oldE;
	//
	newE=oldE;
	if(gSigma_DOWN>0.0){
	  gSigma_DOWN=newJetRes*sqrt(gSigma_DOWN);
	  TRandom3 rand3(0);
	  newE+=rand3.Gaus(0.0,gSigma_DOWN);
	}
	ratio_DOWN=newE/oldE;
      }

    }
      
    //cout<<"the correction factor UP is "<<JerSF_UP<<endl;
    //===========CONSTRUCT THE NEW JET OBJECTS
    //Jet* newJet_UP =new Jet(Jets.at(ijet));
    //Jet* newJet_DOWN =new Jet(Jets.at(ijet));

    //cout<<"the new pt up is "<<newJet_UP->Pt()<<endl;

    newJet_CENTRAL->SetP4(newJet_CENTRAL->P4()*ratio_CENTRAL);
    newJet_UP->SetP4(newJet_UP->P4()*ratio_UP);
    newJet_DOWN->SetP4(newJet_DOWN->P4()*ratio_DOWN);
    //=====================
    systematics.GetsysJet("jetRescentral").push_back(newJet_CENTRAL);
    systematics.GetsysJet("jetResup").push_back(newJet_UP);
    systematics.GetsysJet("jetResdown").push_back(newJet_DOWN);

    double ptsmear=newJet_UP->Pt();

    //extern vector<TH1D*> JetResPtBins;
    //extern vector<TH1D*> SMfactor_PtBins;
    //extern vector<TH1D*> JetResEtaBins;
    //extern vector<TH1D*> JetResPtBins_smear;
    //extern vector<TH1D*> JetResEtaBins_smear;
 
    int ptbin=-1;
    if(Jets.at(ijet)->Pt()<30){ptbin =0;}
    else if(Jets.at(ijet)->Pt()<50){ptbin =1;}
    else if(Jets.at(ijet)->Pt()<80){ptbin =2;}
    else if(Jets.at(ijet)->Pt()<150){ptbin =3;}
    else {ptbin =4;}
    
    //if(!(ptbin ==0 || ptbin==1 || ptbin ==2 || ptbin ==3 || ptbin==4)){
    //cout<<"aqui ptbin = "<<ptbin<<" "<<Jets.at(ijet)->Pt()<<endl;
    //}


    /*///////////////////////////////////////////////////
    if(Jets.at(ijet)->IsMatch()){ 
      JetResPtBins.at(ptbin)->Fill((ptsmear-ptGen)/ptGen);
      SMfactor_PtBins.at(ptbin)->Fill((ptsmear-ptjet)/ptjet);
    }else{
      //JetResPtBins_smear.at(ptbin)->Fill((ptsmear-ptjet)/ptjet);
    }
    
    int etabin=-1;
    if(fabs(Jets.at(ijet)->Eta())<0.5){etabin =0;}
    else if(fabs(Jets.at(ijet)->Eta())<1.1){etabin =1;}
    else if(fabs(Jets.at(ijet)->Eta())<1.7){etabin =2;}
    else if(fabs(Jets.at(ijet)->Eta())<2.3){etabin =3;}
    else {etabin =4;}
    
    //if(!(etabin ==0 || etabin==1 || etabin ==2 || etabin ==3 || etabin==4)){
    //cout<<"aqui etabin = "<<etabin<<" "<<abs(Jets.at(ijet)->Eta())<<endl;
    //}
    if(Jets.at(ijet)->IsMatch()){ 
      JetResEtaBins.at(etabin)->Fill((ptsmear-ptjet)/ptGen);
    }else{
      JetResEtaBins_smear.at(etabin)->Fill((ptsmear-ptjet)/ptjet);
    }


    *////////////////////////////////////////////////////////
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
  //cout<<"index in tree before the critical call"<<endl;
  //for (int ijet=0;ijet<systematics.GetsysJet("jetup").size();++ijet){
  //cout<<"the index is "<<systematics.GetsysJet("jetup").at(ijet)->GetIndexInTree()<<endl;
  //}
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
  //=======COLLECTION OF JETS TO BE STORED IN THE SYSTEMATICS TREE
  systematics.SetTreeJetCollection("jetup","jetup_good_clean");
  systematics.SetTreeJetCollection("jetdown","jetdown_good_clean");
  //
  //======NUMBER OF BTAGS
  systematics.CalculateNumberOfbTags("jetup","jetup_good_clean");
  systematics.CalculateNumberOfbTags("jetdown","jetdown_good_clean");

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

void JetResolutionSmearing(EasyChain* tree, Systematics& systematics, vector<Ptr_Jet>& AllJets, vector<Muon*>& pMuons, vector<Electron*>& pElectrons){
   if( systematics.GetSysMap()["jetResup"]){
     string sysName="";
     for (int isys=0;isys<3;++isys){
      if(isys==0){sysName="jetRescentral";}
      else if(isys==1){sysName="jetResup";}
      else if(isys==2){sysName="jetResdown";}
      else{cout<<"wrong jet resolution option"<<endl;}

      rescaleJetsJER(AllJets,systematics);
      //=========RESCALE THE MET
      rescaleMET(tree,AllJets,systematics, sysName);
      //======FEED THE RESCALED JETS TO makeGoodJets
      makeGoodJets(tree,systematics.GetsysJet(sysName),systematics.GetsysJet(sysName+"_good"));
      //======CLEAN THEM
      makeCleanedJets(systematics.GetsysJet(sysName+"_good"),systematics.GetsysJet(sysName+"_good_clean"),pMuons,pElectrons);
      //=====GET THE NEW HT
      rescaleHT(systematics.GetsysJet(sysName+"_good_clean"), systematics, sysName);
      //======NUMBER OF BTAGS
      systematics.CalculateNumberOfbTags(sysName,sysName+"_good_clean");
    }
  }
}


void JetUncertaintyInitialize(Systematics& systematics){

  ConfigReader config;

  //===================================================================
  //SYSTEMATIC UNCERTAINTIES
  //===================================================================
  
  //=============INITIALIZE THE JETUncertainty object
  typedef boost::shared_ptr<JetCorrectorParameters> JetCorrectorParam_Ptr;
  // Instantiate uncertainty sources
  const int nsrc = 16;
  const char* srcnames[nsrc] =
    {"Absolute", "HighPtExtra", "SinglePion", "Flavor", "Time",
     "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF",
     "RelativeStatEC2", "RelativeStatHF", "RelativeFSR",
     "PileUpDataMC", "PileUpOOT", "PileUpPt", "PileUpBias", "PileUpJetRate"};

  string jetuncfile;
  for (int isrc = 0; isrc < nsrc; isrc++) {
    const char *name = srcnames[isrc];
    bool isBatchJob=config.getBool("isBatchJob",false);
    std::string test1="src/Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
    std::string test2="../../Summer12_V2_DATA_AK5PF_UncertaintySources.txt";
    
    if(!isBatchJob){
      jetuncfile=test1;
    }else{
      jetuncfile=test2;
    }

    JetCorrectorParameters *p;
    if (systematics.IsEnabled()){
      p= new JetCorrectorParameters(jetuncfile, name);
      systematics.vsrc.push_back(new JetCorrectionUncertainty(*p));
    }
  }

  if(systematics.IsEnabled()){
    systematics.total = new JetCorrectionUncertainty(*(new JetCorrectorParameters(jetuncfile, "Total")));
  }



}












