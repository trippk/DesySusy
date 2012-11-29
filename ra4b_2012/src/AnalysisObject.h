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
  LorentzM* pp4_original; 
  int       maptotree;
  std::map<std::string, bool> id;
  
public:
  
  AnalysisObject();
  AnalysisObject(const AnalysisObject& copy);
  ~AnalysisObject();

  LorentzM P4() const;
  LorentzM* pOriginalP4() const;

  double Pt() const;
  double pt() const;
  double Eta() const;
  double eta() const;
  double Phi() const;
  double phi() const;

  double E() const;
  double e() const;
  double Px() const;
  double px() const;
  double Py() const;
  double py() const;
  double Pz() const;
  double pz() const;

  int GetIndexInTree() const;

  bool IsID(const string & id) const;
  void SetID(const string & key, bool value);

  void Set(int maptotree_In, LorentzM * momuntum_In);
  void SetP4(const LorentzM & p4_in) {p4 = p4_in;}
};

#endif
