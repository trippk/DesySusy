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
  TH2D *btag; ///////////////////////////////////////////////////////////
  TH2D *ctag; //this hist will carry information of tagged & matched jets
  TH2D *ltag; ///////////////////////////////////////////////////////////
  TH2D *bmatched; ///////////////////////////////////////////////////////////
  TH2D *cmatched; //this hist will carry information of matched jets
  TH2D *lmatched; ///////////////////////////////////////////////////////////
  TH2D *beff;
  TH2D *ceff;
  TH2D *leff;
  TH1D *h1tag;
  TH1D *h0tag;
  TH1D *h2tag;
  TH1D *h3mtag;
  TH1D *bpttag;
  TH1D *cpttag;
  TH1D *lpttag;
  TH1D *betatag;
  TH1D *cetatag;
  TH1D *letatag;
  TH1D *bptmatched;
  TH1D *cptmatched;
  TH1D *lptmatched;
  TH1D *betamatched;
  TH1D *cetamatched;
  TH1D *letamatched;
  TH1D *bptspec;
  TH1D *cptspec;
  TH1D *lptspec;
  TH1D *tagW;
  void vectoTH1(vector<double> &, TH1D *); 
  void comb(vector<double> &);
  bool status;
  TFile *ftageff;
  void fillmatch (double Pt, double Eta);
  void filltagmatch (double Pt, double Eta);
  void getratio();
  void ApplySF(vector<Jet*>&, int *);
  int getptbin(double);
  string matchtype;
  string btname;
  void printvec (vector<double> & );
  void tagflavor (int);
  double multIns(vector<double> &);
  vector<double> weight;
  TH2D * combtag;
  double getEff (double Pt, double Eta);
  int lasttagbin;
  double SF(double, double, string);
public:
  TagEff(string sample, string subsample){
    ftageff=TFile::Open("tageffs.root"); //read the tag efficiencies from the current directory 
    double tagptbin[]={40, 50, 60, 70, 80,100, 120, 160, 210, 260, 320, 400, 500, 670, 2000}; //bin->14
    //double tagptbin[]={40.,45., 50.,55., 60.,65., 70.,75., 80.,90.,100.,110., 120.,140., 160.,180., 210., 260., 320., 400., 500., 670., 2000.}; //bin->22
    double tagetabin[]={0.,0.8,1.6,2.4};
    btag=0;  
    btname =(string)"btag_"+sample+(string)"_"+subsample;
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
	bpttag = new TH1D((TString) btname+(TString)"pt", "", 14, tagptbin);
	cpttag = new TH1D((TString) ctname+(TString)"pt", "", 14, tagptbin);
	lpttag = new TH1D((TString) ltname+(TString)"pt", "", 14, tagptbin);
	bptmatched = new TH1D((TString) bmname+(TString)"pt", "", 14, tagptbin);
	cptmatched = new TH1D((TString) cmname+(TString)"pt", "", 14, tagptbin);
	lptmatched = new TH1D((TString) lmname+(TString)"pt", "", 14, tagptbin);
	betatag = new TH1D((TString) btname+(TString)"eta", "", 3, tagetabin);
	cetatag = new TH1D((TString) ctname+(TString)"eta", "", 3, tagetabin);
	letatag = new TH1D((TString) ltname+(TString)"eta", "", 3, tagetabin);
	betamatched = new TH1D((TString) bmname+(TString)"eta", "", 3, tagetabin);
	cetamatched = new TH1D((TString) cmname+(TString)"eta", "", 3, tagetabin);
	letamatched = new TH1D((TString) lmname+(TString)"eta", "", 3, tagetabin);
	bptspec = new TH1D((TString) bmname+(TString)"ptspec","",500,40.,2040.);
	cptspec = new TH1D((TString) cmname+(TString)"ptspec","",500,40.,2040.);
	lptspec = new TH1D((TString) lmname+(TString)"ptspec","",500,40.,2040.);
      }
    else
      {
	beff = new TH2D("beff", "tagged & matched/matched ", 3, tagetabin , 14, tagptbin); 
	ceff = new TH2D("ceff", "tagged & matched/matched ", 3, tagetabin , 14, tagptbin);
	leff = new TH2D("leff", "tagged & matched/matched ", 3, tagetabin , 14, tagptbin); 
	combtag  = new TH2D("combtag", "temp histo", 3, tagetabin , 14, tagptbin);
	status = 1;
	h0tag = new TH1D ("h0tag", "0 tag dist.", 100, 0. ,1.);
	h1tag = new TH1D ("h1tag", "1 tag dist.", 100, 0. ,1.);
	h2tag = new TH1D ("h2tag", "2 tag dist.", 100, 0. ,1.);
	h3mtag = new TH1D ("h3mtag", "3 & more tag dist.", 100, 0. ,1.);
	btag=(TH2D*)ftageff->Get((TString) btname);		
	ctag=(TH2D*)ftageff->Get((TString) ctname);
	ltag=(TH2D*)ftageff->Get((TString) ltname);
	bmatched=(TH2D*)ftageff->Get((TString) bmname);
	cmatched=(TH2D*)ftageff->Get((TString) cmname);
	lmatched=(TH2D*)ftageff->Get((TString) lmname);
	bpttag=(TH1D*)ftageff->Get((TString) btname+(TString)"pt");		
	cpttag=(TH1D*)ftageff->Get((TString) ctname+(TString)"pt");
	lpttag=(TH1D*)ftageff->Get((TString) ltname+(TString)"pt");
	bptmatched=(TH1D*)ftageff->Get((TString) bmname+(TString)"pt");
	cptmatched=(TH1D*)ftageff->Get((TString) cmname+(TString)"pt");
	lptmatched=(TH1D*)ftageff->Get((TString) lmname+(TString)"pt");
	getratio();
	
      }
  }
  TagEff(){ cout<<"Sample name did not set for tageff "<< endl;  raise (SIGABRT);}
  ~TagEff(){}
  vector<double>& getWeight();  
  void lastbin (int last=3) {lasttagbin=last; weight.assign(lasttagbin+1,0.);
    if(status){    tagW = new TH1D ((TString)btname+(TString)"weights","weights",last+1,-0.5,last+0.5);}
  }
  void finalize ();
  int JetLoop(vector<Jet*>& pureJets);
};


#endif
