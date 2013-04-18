#ifndef treeVariables_h
#define treeVariables _h

#include <vector>
#include "NtupleTools2.h"
#include "simpleElectron.h"
#include "simpleMuon.h"
#include "simpleJet.h"


class TreeVariables{
  
public:
  
  TreeVariables(){
    Run=0;
    Event=0;
    Jets=0;
    Electrons=0;
    Muons=0;
    PFmet=0;
  }


  int Run;
  int Event;
  std::vector<simpleJet>* Jets;
  std::vector<simpleElectron>* Electrons;
  std::vector<simpleMuon>* Muons;
  ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >* PFmet;  

};

#endif
