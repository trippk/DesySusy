#include "JetMonitor.h"


using namespace std;

JetMonitor::JetMonitor(TDirectory * dirIn ):  cDir(0){

  TH1::SetDefaultSumw2(true);
  if (dirIn) cDir = dirIn->mkdir("JetMonitor");
  else cDir = 0;
  
  makePlots();

};

JetMonitor::~JetMonitor() {

}

void JetMonitor::dumpToFile(){

  if (cDir == 0) {
    cout << "JetMonitor::dumpToFile >> ERROR " << endl; 
  }
  else cDir->Write();

  return;
}
  
  
void JetMonitor::makePlots() {
  
  if (cDir == 0) {
    return;
  }

  cDir->cd();

  hMatched = new TH2I("Matched", "Matched;Pt;Eta", 14, 0., 350., 20, -5., 5.);
  hMatchedDeltaR = new TH3F("MatchedDeltaR", "MatchedDeltaR;Pt;Eta;DeltaR", 14, 0., 350., 20, -5., 5., 30, 0., 0.6);
  hMatchedDeltaPt = new TH3F("MatchedDeltaPt", "MatchedDeltaPt;Pt;Eta;DeltaPt", 14, 0., 350., 20, -5., 5., 40, -100., 100.);
  hMatchedScaleFactor = new TH3F("MatchedScaleFactor", "MatchedScaleFactor;Pt;Eta;SF", 14, 0., 350., 20, -5., 5., 40, 0., 2.);
  
  hNotMatched = new TH2I("NotMatched", "NotMatched;Pt;Eta", 14, 0., 350., 20, -5., 5.);
  hNotMatchedDeltaE = new TH3F("NotMatchedDeltaPt", "NotMatchedDeltaPt;Pt;Eta;DeltaPt", 14, 0., 350., 20, -5., 5., 40, -100., 100.);
  hNotMatchedScaleFactor = new TH3F("NotMatchedScaleFactor", "NotMatchedScaleFactor;Pt;Eta;SF", 14, 0., 350., 20, -5., 5., 40, 0., 2.);

  return;
}
