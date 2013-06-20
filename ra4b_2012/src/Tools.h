#ifndef Tools_h
#define Tools_h

#include <vector>
#include "NtupleTools2_h.h"
#include "AnalysisObject.h"
#include "Particle.h"
#include "Muon.h"
#include "Electron.h"
#include "Jet.h"
#include "Tau.h"

using namespace std;

namespace desy_tools {

  float Consistency( LorentzM refP4, vector<LorentzM>& compP4);
  float Consistency( LorentzM refP4, EasyChain* tree, const char* compLabelP4);
  float Consistency( LorentzM refP4, int refCh, vector<LorentzM>& tmpCompP4, vector<int>& tmpCompCh);
  float Consistency( LorentzM refP4, int refCh, EasyChain* tree, const char* compLabelP4, const char* compLabelCh);

  template <class Comp> bool CleaningByDR( LorentzM const & ref, vector<Comp*>& comp, float DR = 0.3);
  bool CleaningByDR( LorentzM const & ref, vector<LorentzM*>& comp, float DR = 0.3);

  template <class Obj1, class Obj2> bool compare_Object_Pt( Obj1* obj1, Obj2* obj2);

  double HT( vector<Jet*>& jets);
}

template <class Comp> 
  bool desy_tools::CleaningByDR( LorentzM const & ref, vector<Comp*>& comp, float DR){

  for(int icomp=0; icomp<(int)comp.size(); ++icomp){
    if(DeltaR( ref, comp.at(icomp)->P4()) < DR) return true;  
  }

  return false;
};

template <class Obj1, class Obj2> 
  bool desy_tools::compare_Object_Pt( Obj1* obj1, Obj2* obj2){

  //Ordering mechanism for Particles. Used for stl containers with sort.
  //If obj1 has a higher Pt than obj2, it should return true. This means that obj1
  //should be placed before obj2 in the list.
  
  if ( obj1->Pt() > obj2->Pt() ) {
    return true;
  }
  else return false;
};
#endif
