#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include "TString.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"
#include "CutFlow.h"

using namespace std;

double CutSet::global_event_weight = 1.;
TDirectory* CutSet::tdir = 0;
TFile* CutSet::tfile = 0;

CutSet::CutSet(const TString& n, const char* dlm ): delim(dlm),autoprint(false) {
	Name = n;
}
void CutSet::printAll() {

	cout<<endl
	<<"__________________________________________________________________"<<endl
	<<"CutSet: __ "<<Name<<" __"<<endl<<endl
	<<"single cut survivors:"<<endl;
	cout
	<<" | "<<setiosflags(ios::left)<<setw(30)
	<<"cut name:"<<setiosflags(ios::right)<<setw(10)
	<<"in"<<"    "<<setw(10)
	<<"out"<<setw(3)<<" "<<setw(7)
	<<"eff"
	<<setw(17)<<"weighted out"
	<<endl;
	for(unsigned c=0; c<allCuts.size(); ++c) {

	//cout<<setfill('.');
	cout
	<<" | "<<setiosflags(ios::left)<<setw(30)
	<<allCuts[c]<<setiosflags(ios::right)<<setw(10)
	<<rejected[allCuts[c]]+survivors[allCuts[c]]
	<<" -> "<<setw(10)
	<<survivors[allCuts[c]]<<setw(3)<<"="<<setw(6)
	<<setprecision(4)<<(double) 100*survivors[allCuts[c]]/(rejected[allCuts[c]]+survivors[allCuts[c]])<<"%  |  "
	<<setw(12)<<survivors_weighted[allCuts[c]]
	<<endl;
	}
	cout<<endl

	<<"cut flow survivors:"<<endl;
//	for(unsigned c=0; c<allCuts.size(); ++c) {
//		cout<<allCuts[c]<<" "<<survivors[allCuts[c]]<<endl;
//dk	}
	for(unsigned f=0; f<allFlows.size(); ++f) {
	cout
	<<">>> "<<allFlowNames[allFlows[f]]<<endl;
	printFlow(allFlows[f]);
	}

}

void CutSet::printFlow(const TString& rawCuts) {

	if( rawCuts == "" ) return;

	// header
	cout<<"   "<<setiosflags(ios::left)<<"flow steps:"
	   <<setiosflags(ios::right)
	   <<setw(31)<<"in"
	   <<setw(14)<<"out"
	   <<setw(10)<<"step eff"
	   <<setw(10)<<"tot eff"
	   <<setw(14)<<"weighted out"
	   <<endl;
	Ssiz_t i=0;
	TString cutFlow;
	TString first;
	rawCuts.Tokenize(first,i,delim);
	i=0;
	while(rawCuts.Tokenize(cutFlow,i,delim)) {
	cout<<"   + "<<setiosflags(ios::left)<<setw(30)
	   //name:
	   <<cutFlow<<setiosflags(ios::right)<<setw(10)
	   //total number:
	   <<flowRejected[rawCuts][cutFlow]+flowSurvivors[rawCuts][cutFlow]
	   <<" -> "<<setw(10)
	   //number of survivors:
	   <<flowSurvivors[rawCuts][cutFlow]<<setw(3)<<"="<<setw(6)
	   //survivor relative in this step:
	   <<setprecision(4)<<(double)
	   100*flowSurvivors[rawCuts][cutFlow]/(flowRejected[rawCuts][cutFlow]+flowSurvivors[rawCuts][cutFlow])<<"%"
	   //survivor rel to total
	   <<" | "<<setw(6)<<(double)
	   100*flowSurvivors[rawCuts][cutFlow]/(flowRejected[rawCuts][first]+flowSurvivors[rawCuts][first])<<"% |  "
	   //surv. weighted:
	   <<setw(10)<<flowSurvivors_weighted[rawCuts][cutFlow]
	   <<endl;
	}
	cout<<endl;

//dk	i=0;
//	cout<<"in "<<flowSurvivors[rawCuts][first]+flowRejected[rawCuts][first]<<endl;;
//	while(rawCuts.Tokenize(cutFlow,i,delim)) {
//		cout<<cutFlow<<" "<<flowSurvivors[rawCuts][cutFlow]<<endl;
//	}

};
void CutSet::setTFile(TFile *f){
	tfile=f;
};
void CutSet::dumpHist(){
	cout<<"To be done: histogram cut flow"<<endl;
/*
	hi = new TH1I("hi",n);
	hf = new TH1F("hf",n);
	for(vector<TH1*>::const_iterator it=histos.begin();it!=histos.end();it++) it->Write()
*/
}
