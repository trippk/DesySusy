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


using namespace std;
using namespace ROOT::Math::VectorUtil;

extern bool pcp;


void rescaleHT(vector<Jet*>& Jets_in,Systematics& systematics, const string name){
  
  double newHT=0;
  for (int i=0;i<Jets_in.size();++i){ 
    newHT+=Jets_in.at(i)->Pt();
  }
  systematics.SetsysHT(name,newHT);
}

void rescaleJets(EasyChain* tree, vector<Jet*>& Jets_in, Systematics& systematics){

  void JECUncertainty(Jet* Jet_in, Systematics& systematics);
  //void JECUncertainty(Jet* );

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
    JECUncertainty(Jets_in.at(i), systematics );
  }
  //the up and down variations are now inside the jets//

  //===========PUT THEM INSIDE THE systematics OBJECT

  vector<Jet*> dummyvector_UP;
  vector<Jet*> dummyvector_DOWN;
  for (int i=0;i<Jets_in.size();++i){


    //ONLY RESCALE JETS WITH PT > 10.0
    if (Jets_in.at(i)->Pt()<10.0)continue;
    //===========CONSTRUCT THE NEW LORENTZ VECTOR

    LorentzM* dummyLorentz_UP= new LorentzM(Jets_in.at(i)->P4());
    LorentzM* dummyLorentz_DOWN= new LorentzM(Jets_in.at(i)->P4());
    
    //==========RESCALE IT
    *dummyLorentz_UP *=(1+Jets_in.at(i)->GetCorrectionUncertainty("up"));
    *dummyLorentz_DOWN *=(1-Jets_in.at(i)->GetCorrectionUncertainty("down"));
    
    //=========CONSTRUCT THE NEW JET OBJECT
    //    cout<<"feeding dummyjet with "<<dummyLorentz_UP<<endl;
    Jet* dummyJet_UP = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_UP);
    Jet* dummyJet_DOWN = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_DOWN);

    //==========STORE IT IN systematics
    
    systematics.GetsysJet("jetup").push_back(dummyJet_UP);
    systematics.GetsysJet("jetdown").push_back(dummyJet_DOWN);

  }
  //  cout<<endl;
}


void rescaleClusters(vector<Jet*>& Jets_in,Systematics& systematics){

  for (int i=0;i<Jets_in.size();++i){
    
    if(Jets_in.at(i)->Pt()>10.0)continue;
    if(abs(Jets_in.at(i)->Eta())>5.0)continue;
    
    //===========CONSTRUCT THE NEW LORENTZ VECTOR
    LorentzM* dummyLorentz_UP= new LorentzM(Jets_in.at(i)->P4());
    LorentzM* dummyLorentz_DOWN= new LorentzM(Jets_in.at(i)->P4());
    
    //==========RESCALE IT
    *dummyLorentz_UP *=(1+0.1);
    *dummyLorentz_DOWN *=(1-0.1);
    
    //=========CONSTRUCT THE NEW JET OBJECT
    Jet* dummyJet_UP = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_UP);
    Jet* dummyJet_DOWN = new Jet(Jets_in.at(i)->GetIndexInTree(),dummyLorentz_DOWN);

    //==========STORE IT IN systematics
    systematics.GetsysJet("clustersup").push_back(dummyJet_UP);
    systematics.GetsysJet("clusterdown").push_back(dummyJet_DOWN);

  }
}
    




void rescaleMET(EasyChain* tree, vector<Jet*>& Jets_in, Systematics& systematics, string name){


  LorentzM& PFmet = tree->Get(&PFmet, "metP4TypeIPF");
  
  //  cout<<endl;
  //  cout<<endl;
  double sum_px=0.0;
  double sum_py=0.0;
  double sum_pz=0.0;

  for (int i=0;i<Jets_in.size();++i){ 
    //if (Jets_in.at(i)->Pt()<10.0)continue;
    //cout<<"rescaled px up is "<<systematics.GetsysJet(name).at(i)->Px()<<endl;
    //    cout<<"rescaled py up is "<<systematics.GetsysJet(name).at(i)->Py()<<endl;

    //    cout<<"change up "<<systematics.GetsysJet(name).at(i)->Px() - Jets_in.at(i)->Px()<<endl;
    sum_px+=systematics.GetsysJet(name).at(i)->Px() - Jets_in.at(i)->Px();
    sum_py+=systematics.GetsysJet(name).at(i)->Py() - Jets_in.at(i)->Py();
  }
  //  cout<<"total change up px"<<sum_px;
  //  cout<<"total change up py"<<sum_py;
  double newpx=PFmet.Px()+sum_px;
  double newpy=PFmet.Py()+sum_py;
  //  cout<<"the central one SHOULD BE "<<sqrt(PFmet.Px()
  systematics.SetsysMET(name, sqrt(newpx*newpx + newpy*newpy));
  systematics.SetsysMEVector(name,newpx,newpy);
  //  cout<<"new MET up "<<sqrt(newpx*newpx + newpy*newpy)<<endl;
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










void JECUncertainty(Jet* Jet_in, Systematics& systematics){


  // Calculate uncertainty per source and as a total
  double jetpt=Jet_in->Pt();
  double jeteta=Jet_in->Eta();
  double sum2_up(0), sum2_dw(0);

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


  Jet_in->SetCorrectionUncertainty("up",sum2_up);
  Jet_in->SetCorrectionUncertainty("down",sum2_dw);

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
