#ifndef plotSet_h
#define plotSet_h

#include "TH1.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TTree.h"
#include "TPaveStats.h"
#include <algorithm>


using namespace std;

class plotSet {
 protected:
  TString Name;

  bool autoSaveRoot, autoSavePDF;

  TFile* tfile;
  TDirectory* tdir;

  map<TString, vector<TH1*> > pTitleToHistos;
  map<TString, TCanvas*>      pTitleToCanvas;

  map<TString, TString>       pTitleToXaxisTitle;
  map<TString, TString>       pTitleToYaxisTitle;

  float hist_maximum(TH1*);
  float hist_minimum(TH1*);

  bool isNewCanvas(TString);
  bool isNewHisto(TString,TString);
  bool isNewHisto(TString h) { return isNewHisto(h,h); }

  vector<TString>                   tNames;
  map<TString, TTree*>              tNameToTree;
  map<TString, bool>                tNameToIsFilled;
  map<TString, map<TString, bool> > tNameTobNameToNotFilledValue;

  map<TString, map<TString, double> > tNameTobNameToDouble;
  map<TString, map<TString,    int> > tNameTobNameToInt;
  map<TString, map<TString, double> >& GetValueMap( double v ) { return tNameTobNameToDouble; }
  map<TString, map<TString,    int> >& GetValueMap(    int v ) { return tNameTobNameToInt   ; }

  bool fillTree(TString);

 public:

  static double global_event_weight;

  plotSet(TString n = "", bool asroot = false, bool aspdf = false) {
    Name = n;
    TH1F* se = new TH1F();
    se->SetDefaultSumw2(kTRUE);

    autoSaveRoot = asroot;
    autoSavePDF  = aspdf;
  }

  ~plotSet() {
    if(autoSaveRoot) {
      saveRootFile(Name);
    }
    if(autoSavePDF) {
      savePSFile(Name+".pdf");
    }
  }

  //returns pointer to histogram by name;
  TH1* Get(TString);

  /*different possibilities to add and fill a 1D plot:
    addPlot( TString uniqueName,
            [TString title,]
	     int    numberOfBins,
	     double Xmin,
	     double Xmax,
	     double value,
	    [double weight,]
	    [TString XAxisTitle,]
	    [TString YAxisTitle ]  )
  */
  void addPlot( TString,   TString,   int,   double,   double,   double,   double   );
  void addPlot( TString h, TString c, int b, double l, double u, double v           ) {
    addPlot(h,c,b,l,u,v,plotSet::global_event_weight); }

  void addPlot( TString h,            int b, double l, double u, double v, double w ) { 
    addPlot(h,h,b,l,u,v,w); }
  void addPlot( TString h,            int b, double l, double u, double v           ) { 
    addPlot(h,h,b,l,u,v,plotSet::global_event_weight); }

  void addPlot( TString h, TString c, int b, double l, double u, double v, double w, TString XT, TString YT ) {
    if( isNewHisto(h,c) ) SetAxesTitles(c,XT,YT);
    addPlot(h,c,b,l,u,v,w); }
  void addPlot( TString h, TString c, int b, double l, double u, double v,           TString XT, TString YT ) {
    addPlot(h,c,b,l,u,v,plotSet::global_event_weight,XT,YT); }

  void addPlot( TString h,            int b, double l, double u, double v, double w, TString XT, TString YT ) {
    addPlot(h,h,b,l,u,v,w,XT,YT); }
  void addPlot( TString h,            int b, double l, double u, double v,           TString XT, TString YT ) {
    addPlot(h,h,b,l,u,v,plotSet::global_event_weight,XT,YT); }


  void addPlot_withUnweighted( TString h, TString c, int b, double l, double u, double v ) {
    addPlot(h,c,b,l,u,v,plotSet::global_event_weight);
    addPlot(h+"_unweighted",c+"_unweighted",b,l,u,v,1);
  }

  void addPlot_withUnweighted( TString h,            int b, double l, double u, double v ) {
    addPlot_withUnweighted(h,h,b,l,u,v);
  }
  // or add an existing plot (both 1D and 2D)
  void addPlot( TH1*, TString, TString );
  void addPlot( TH1* h, TString t) {
    addPlot(h,t,t);
  }

  /*different possibilities to add and fill a 2D plot:
    addPlot( TString uniqueName,
            [TString title,]
	     int    numberOfXBins,
	     double Xmin,
	     double Xmax,
	     double Xvalue,
	     int    numberOfYBins,
	     double Ymin,
	     double Ymax,
	     double Yvalue,
	    [double weight,]
	    [TString XAxisTitle,]
	    [TString YAxisTitle ]  )
  */
  void addPlot( TString,   TString,
		int,    double,    double,    double,
		int,    double,    double,    double,    double  );
  void addPlot( TString h, TString c,
		int xb, double xl, double xu, double xv,
		int yb, double yl, double yu, double yv          ) {
    addPlot(h,c,
	    xb,xl,xu,xv,
	    yb,yl,yu,yv,plotSet::global_event_weight); }

  void addPlot( TString h,
		int xb, double xl, double xu, double xv,
		int yb, double yl, double yu, double yv, double w ) {
    addPlot(h,h,
	    xb,xl,xu,xv,
	    yb,yl,yu,yv,w); }
  void addPlot( TString h,
		int xb, double xl, double xu, double xv,
		int yb, double yl, double yu, double yv) {
    addPlot(h,h,
	    xb,xl,xu,xv,
	    yb,yl,yu,yv,plotSet::global_event_weight); }

  void addPlot( TString h, TString c,
		int xb, double xl, double xu, double xv,
		int yb, double yl, double yu, double yv, double w, TString XT, TString YT ) {
    addPlot(h,c,xb,xl,xu,xv,
	    yb,yl,yu,yv,w);
    if( isNewHisto(h,h) ) SetAxesTitles(h,XT,YT);
  }
  void addPlot( TString h, TString c,
		int xb, double xl, double xu, double xv,
		int yb, double yl, double yu, double yv,           TString XT, TString YT ) {
    addPlot(h,c,xb,xl,xu,xv,
	    yb,yl,yu,yv,plotSet::global_event_weight);
    if( isNewHisto(h,h) ) SetAxesTitles(h,XT,YT);
  }

  void addPlot( TString h,
		int xb, double xl, double xu, double xv,
		int yb, double yl, double yu, double yv, double w, TString XT, TString YT ) {
    addPlot(h,h,xb,xl,xu,xv,
	    yb,yl,yu,yv,w);
    if( isNewHisto(h,h) ) SetAxesTitles(h,XT,YT);
  }
  void addPlot( TString h,
		int xb, double xl, double xu, double xv,
		int yb, double yl, double yu, double yv,           TString XT, TString YT ) {
    addPlot(h,h,xb,xl,xu,xv,
	    yb,yl,yu,yv,plotSet::global_event_weight);
    if( isNewHisto(h,h) ) SetAxesTitles(h,XT,YT);
  }

  void SetAxesTitles(TString, TString, TString);

  //until now possible to fill doubles or integers to a branch:
  template<typename T>
  void addLeaf(TString, TString, T);

  void printAll(TString);
  void printAll() {printAll("");}

  TCanvas* print(map<TString, vector<TH1*> >::iterator);

  void setTFile(TFile*);

  void saveRootFile(TString);
  void savePSFile(TString);

};

template<typename T>
void plotSet::addLeaf(TString TreeName, TString BranchName, T value) {
  bool isNewTree = true;
  for(size_t tn=0; tn<tNames.size() && isNewTree; ++tn)
    if( TreeName == tNames.at(tn) )
      isNewTree = false;
  if(isNewTree) {
    TTree* ntr = new TTree(TreeName,TreeName);
    tNames.push_back(TreeName);
    tNameToTree[TreeName]=ntr;
    tNameToIsFilled[TreeName]=false;
  }

  if( GetValueMap(value)[TreeName].find(BranchName) == GetValueMap(value)[TreeName].end() ) {
    if( tNameToIsFilled[TreeName] ) cout<<" --- ------- ---"<<endl
				    <<" --- WARNING --- You are adding branch \'"
				    <<BranchName<<"\' to tree \'"<<TreeName
				    <<"\' which has already been filled in another loop! You will get nonsense numbers!"
				    <<endl<<" --- ------- ---"<<endl;
    T valuePointer;
    GetValueMap(value)[TreeName][BranchName] = valuePointer;
    tNameToTree[TreeName]->Branch(BranchName, &GetValueMap(value)[TreeName][BranchName]);
    tNameTobNameToNotFilledValue[TreeName][BranchName] = false;
  }
  else if( tNameTobNameToNotFilledValue[TreeName][BranchName] )
    fillTree(TreeName);

  GetValueMap(value)[TreeName][BranchName] = value;
  tNameTobNameToNotFilledValue[TreeName][BranchName] = true;
}

#endif


