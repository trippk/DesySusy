#ifndef TagEff_h
#define TagEff_h
#include "TH1F.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TFile.h"
#include <csignal>
#include <string>
#include <fstream>
#include "Math/VectorUtil.h"
#include "NtupleTools2_h.h"
#include <TString.h>
#include "AnalysisObject.h"
#include "Jet.h" 
using namespace std;
using namespace ROOT::Math::VectorUtil;

class TagEff{
private:
  TH2D *btag;
  TH2D *bmatched;
  TH2D *cmatched;
  TH2D *lmatched;
  TH2D *btag3;
  TH2D *ctag;
  TH2D *ctag3;
  TH2D *ltag;
  TH2D *ltag3;
  void comb(vector<double> &);
  bool status;
  TFile *ftageff;
  void fillmatch (double Pt, double Eta);
  void filltagmatch (double Pt, double Eta);
  void getratio();
  void ApplySF(vector<Jet*>&);
  int getptbin(double);
  string matchtype;
  void printvec (vector<double> & );
  void tagflavor (int);
  double multIns(vector<double> &);
  vector<double> weight;
  TH2D * combtag;
  double getEff (double Pt, double Eta);
  int lasttagbin;
public:
  TagEff(string sample, string subsample){
    ftageff=TFile::Open("tageffs.root"); //read the tag efficiencies from the current directory 
    double tagptbin[]={40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670, 1340}; 
    double tagetabin[]={0.,0.8,1.6,2.4};
    btag=0;  
    string btname =(string)"btag_"+sample+(string)"_"+subsample;
    string ctname = (string)"ctag_"+sample+(string)"_"+subsample;
    string ltname = (string)"ltag_"+sample+(string)"_"+subsample;
 
    string bmname = (string)"bmatched_"+sample+(string)"_"+subsample;
    string cmname = (string)"cmatched_"+sample+(string)"_"+subsample;
    string lmname = (string)"lmatched_"+sample+"_"+subsample;
    if(!ftageff) 
      {

	cout<< " please change the output file name (the file that contains histograms) to \"tageffs.root\", a general file will be provided soon" << endl;
	status =0; 
	btag = new TH2D((TString) btname, "tagged & b matched", 3, tagetabin , 14, tagptbin);
	ctag = new TH2D((TString) ctname, "tagged & c matched", 3, tagetabin , 14, tagptbin);
	ltag = new TH2D((TString) ltname, "tagged & light matched", 3, tagetabin , 14, tagptbin);
	bmatched = new TH2D((TString) bmname, "b matched", 3, tagetabin , 14, tagptbin); 
	cmatched = new TH2D((TString) cmname, "c matched", 3, tagetabin , 14, tagptbin); 
	lmatched = new TH2D((TString) lmname, "light quark matched", 3, tagetabin , 14, tagptbin); 

      }
    else
      {
	btag3 = new TH2D("btag3", "tagged & matched/matched ", 3, tagetabin , 14, tagptbin); 
	ctag3 = new TH2D("ctag3", "tagged & matched/matched ", 3, tagetabin , 14, tagptbin);
	ltag3 = new TH2D("ltag3", "tagged & matched/matched ", 3, tagetabin , 14, tagptbin); 
	combtag  = new TH2D("combtag", "temp histo", 3, tagetabin , 14, tagptbin);
	status = 1;

	ctag=(TH2D*)ftageff->Get((TString) ctname);
	ltag=(TH2D*)ftageff->Get((TString) ltname);
	bmatched=(TH2D*)ftageff->Get((TString) bmname);
	cmatched=(TH2D*)ftageff->Get((TString) cmname);
	lmatched=(TH2D*)ftageff->Get((TString) lmname);
	btag=(TH2D*)ftageff->Get((TString) btname);	
      }
  }
  TagEff(){ cout<<"Sample name did not set for tageff "<< endl;  raise (SIGABRT);}
  ~TagEff(){}
  vector<double>& getWeight();  
  void lastbin (int last=3) {lasttagbin=last; weight.assign(lasttagbin+1,0.);}
  void finalize ();
  int JetLoop(vector<Jet*>& pureJets);
};


#endif
