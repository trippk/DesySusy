#ifndef AnalysisObject_h
#define AnalysisObject_h

#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <map>
#include <TString.h>
#include <boost/shared_ptr.hpp>
#include "typedefs.h"

using namespace std;
using namespace ROOT::Math::VectorUtil;

//typedef boost::shared_ptr<LorentzM> Ptr_LorentzM;

class AnalysisObject {
  
protected:

  LorentzM  p4;
  LorentzM* pp4; 
  Ptr_LorentzM shared_pp4;
  std::map<std::string, bool> id;
  int       maptotree;
  //does the instance own the LorentzM?
  bool ownsP4;
  bool hasP4;
public:
  
  AnalysisObject();
  AnalysisObject(const AnalysisObject& copy);
  ~AnalysisObject();


  void CopyLorentzM(const AnalysisObject* target);
  int GetIndexInTree() const;
  bool HasP4();
  bool IsID(const string & id) const;
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
  void SetID(const string & key, bool value);
  void Set(const int maptotree_In,  LorentzM * const momentum_In);
  void Set(const int maptotree_In, Ptr_LorentzM momentum_In);
  void Set(const int maptotree_In, LorentzM momentum_In);
  void SetOwnedPointer(const int maptotree_In, LorentzM* const momentum_In);
  void SetExternalPointer(const int maptotree_In, LorentzM* const momentum_In);
  //void SetP4(const LorentzM momentum_In){p4= };
  void SetP4(LorentzM momentum_In);
};
#endif
