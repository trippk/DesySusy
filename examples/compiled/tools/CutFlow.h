// to do outdump/print default true

// changes to hannes:
// SplitToWords replaced
// delimiter can be ','
//  cut name may contain spaces e.g.:      pt <
//  then cut list are comma sepearated e.g.:   "pt <, eta >, theta_good"
// but default presently keept as ' ' see below #define delimiter " "
//
// code by Hannes Schettler
// dk 25.4.2011
/*
  A cutflow with documentation of surviving events
  Can also handle weighted samples:
  - before the loop starts you create one or more instances of CutSet.
  For example:

  CutSet electronSelection("selecting good electrons");

  - within the loop you define each cut by its own, which consists of a unique name and a bool:

  electronSelection.keepIf("Pt",  el->pt() > elePtThreshold);
  electronSelection.keepIf("Eta", fabs(el->eta()) <e leEtaThreshold);

  - and you can combine them to cut flows by listing their names space separated:

  electronSelection.applyCuts("Pt Eta");

  - both, the "keepIf" and the "applyCuts" methods return a bool
  (true for survived and false for rejected) to do things like:

  I)             if( !electronSelection.keepIf("Pt", el->pt() > elePtThreshold) ) continue;
  or
  II)            if( electronSelection.applyCuts("Pt Eta") ) goodElectrons.push_back(el);

  Use case  I : keepIf (or rejectIf) is tested immediately and the event loop continues if the condition is not fullfilled
  Use case II : You collect several cuts and do a applyCuts e.g. for n-1 plots

  - at the end you can get a table which lists all defined cuts and cut flows.
  electronSelection.printAll();
  Keep in mind, the meaning of this number depend on your code!
  - applyCuts must appear only once per CutSet!
  DK histo store
*/

#ifndef CutFlow_h
#define CutFlow_h

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include "TString.h"
#include "TDirectory.h"
#include "TH1.h"

using namespace std;

#define delimiter " "

class CutSet {
public:

  CutSet(const TString&,const char* delim=delimiter);
  CutSet(): delim( delimiter ), autoprint(false), autodump(false) {CutSet("");}
  ~CutSet(){

    if(autoprint) printAll();

    if(autodump)  dumpToHist();
  }
  static double global_event_weight;

  bool rejectIf(const TString&, bool, double);
  bool keepIf(const TString& n, bool cond, double w) {return !rejectIf(n, !cond, w);}

  bool rejectIf(const TString& n, bool cond){
    return rejectIf(n, cond, CutSet::global_event_weight );
  }
  bool keepIf(const TString& n, bool cond){
    return keepIf(n, cond, CutSet::global_event_weight );
  }

  bool applyCuts(double w);
  bool applyCuts(const TString&, double);
  bool applyCuts(const TString& n, const TString& cuts, double w) {
    allFlowNames[cuts]=n;
    return applyCuts(cuts, w);
  }

  bool applyCuts(const TString& n){
    return applyCuts(n, CutSet::global_event_weight );
  }
  bool applyCuts(const TString& n, const TString& cuts) {
    return applyCuts(n, cuts, CutSet::global_event_weight );
  }

  void printAll();

  void printFlow(const TString&);
  static void setTFile(TFile *);
  static void setTDir(const TString& );
  void dumpToHist();
  bool autoprint;
  bool autodump;
  bool DidItPass(const TString&);
  bool DidAllPass();
private:
  TString Name;
  vector<TString>    allCuts;
  map<TString, int>  survivors;
  map<TString, int>  rejected;
  map<TString, bool> isRejected;
  vector<TString>                 allFlows;
  map<TString, TString>           allFlowNames;
  map<TString, map<TString,int> > flowSurvivors;
  map<TString, map<TString,int> > flowRejected;

  map<TString, double>                survivors_weighted;
  map<TString, double>                rejected_weighted;
  map<TString, map<TString, double> > flowSurvivors_weighted;
  map<TString, map<TString, double> > flowRejected_weighted;

  TString delim;// default delimiter
  // ROOT related
  static TFile* tfile;
  static TDirectory* tdir;
  TH1I* hi;
  TH1F* hf;
  


};
inline bool CutSet::DidItPass(const TString& n){
  if( find(allCuts.begin(),allCuts.end(),n) == allCuts.end() ){
    cout<<n<<" has not been set yet"<<endl;
    for (int icut=0;icut<allCuts.size(); ++icut){
      cout<<allCuts.at(icut)<<" has been set"<<endl;
    }
    return false;
  }else{
    return !isRejected[n];
  }
}
inline bool CutSet::rejectIf(const TString& n, bool cond, double w) {

  if( find(allCuts.begin(),allCuts.end(),n) == allCuts.end() ) allCuts.push_back(n);
  if(cond) {
    ++rejected[n];
    rejected_weighted[n] += w;
  } else {
    ++survivors[n];
    survivors_weighted[n] += w;
  }

  isRejected[n] = cond;

  return cond;
}


inline bool CutSet::applyCuts(double w){

  //  cout<<endl;
  //  cout<<endl;
  TString rawCutString="";
  for (int icut=0;icut<(int)allCuts.size();++icut){
    rawCutString+=allCuts.at(icut)+" ";
    //    cout<<allCuts.at(icut)<<endl;
  }
  rawCutString=rawCutString(0,rawCutString.Length()-1);
  //  cout<<endl;
  //  cout<<endl;
  //  cout<<"the cutstring is now"<<rawCutString<<endl;
  return this->applyCuts(rawCutString,w);
}

inline bool CutSet::DidAllPass(){

  bool OK=true;
  
  for (int icut=0;icut<(int)allCuts.size();++icut){
    if (isRejected[allCuts.at(icut)]){
      return false;
    }
  }
  return true;
}

inline bool CutSet::applyCuts(const TString& rawCutString, double w) {

  if( find(allFlows.begin(),allFlows.end(),rawCutString) == allFlows.end() )
    allFlows.push_back(rawCutString);

  Ssiz_t i=0;
  TString cutToApply;
  while(rawCutString.Tokenize(cutToApply,i,delim)) {
    if( isRejected.find(cutToApply) != isRejected.end() ) {
      if(!isRejected[cutToApply]) {
	++flowSurvivors[rawCutString][cutToApply];
	flowSurvivors_weighted[rawCutString][cutToApply] += w;
      } else {
	//	cout
	++flowRejected[rawCutString][cutToApply];
	flowRejected_weighted[rawCutString][cutToApply] += w;
	return false;
      }
    } else cout<<" ++ applied cut "<<cutToApply<<" not found!"<<endl;
  }

  return true;

}
#endif
