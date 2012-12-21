#ifndef JetMonitor_h
#define JetMonitor_h

#include <iostream>

#include "TFile.h"
#include "TDirectory.h"
#include "TH2I.h"
#include "TH3F.h"


class JetMonitor {

 public:
  JetMonitor(TDirectory * dirIn);
  ~JetMonitor();
  
  void dumpToFile();

  TH2I * hMatched;
  TH3F * hMatchedDeltaR;
  TH3F * hMatchedDeltaPt;
  TH3F * hMatchedScaleFactor;

  TH2I * hNotMatched;
  TH3F * hNotMatchedDeltaE;
  TH3F * hNotMatchedScaleFactor;

 private:

  TDirectory* cDir;

  void makePlots();

};

#endif
