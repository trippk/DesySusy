#ifndef AnalysisObject_h
#define AnalysisObject_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>

using namespace std;
using namespace ROOT::Math::VectorUtil;

class AnalysisObject {
  
protected:

  LorentzM  p4;
  LorentzM* pp4; 
  int       maptotree;
  vector<TString> allIDs;
  map<TString, bool> id;
  
public:
  AnalysisObject(){
    pp4=0;
    allIDs.clear();
    id.clear();
  }

  ~AnalysisObject(){
    pp4=NULL;
  }

  AnalysisObject(const AnalysisObject& copy);

  LorentzM P4();
  LorentzM* pP4();

  double Pt();
  double pt();
  double Eta();
  double eta();
  double Phi();
  double phi();

  double E();
  double e();
  double Px();
  double px();
  double Py();
  double py();
  double Pz();
  double pz();

  int GetIndexInTree();

  bool IsID(string id);
  void SetID(string key, bool value);

  void Set(int maptotree_In, LorentzM * momuntum_In);
};

#endif
