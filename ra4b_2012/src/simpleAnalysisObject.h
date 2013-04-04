#ifndef simpleAnalysisObject_h
#define simpleAnalysisObject_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"

class simpleAnalysisObject{

protected:

  LorentzM p4;
  LorentzM* pp4;
  int maptotree;
  std::map<std::string, bool> id;  
public:

  simpleAnalysisObject();
  simpleAnalysisObject(const simpleAnalysisObject& copy);
  ~simpleAnalysisObject();  

  int GetIndexInTree() const;

  LorentzM P4() const;
  LorentzM* GetPointerP4() const;

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
  //
  bool IsID(const string & id) const;
  void SetID(const string & key, bool value);
  void Set(const int maptotree_In, LorentzM momentum_In);
  void SetP4(LorentzM momentum_In);

  ClassDef(simpleAnalysisObject,1);
};

#endif
