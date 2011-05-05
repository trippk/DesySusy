#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include "THTools.h"
#include "TString.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH1.h"
#include "CutFlow.h"

using namespace std;

double CutSet::global_event_weight = 1.;
TDirectory* CutSet::tdir = 0;
TFile* CutSet::tfile = 0;

CutSet::CutSet(const TString& n, const char* dlm ): delim(dlm), autoprint(false), autodump(false) {
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
	cout<<endl;

	if(allFlows.size()>0)cout<<"cut flow survivors:"<<endl;
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

};

void CutSet::setTFile(TFile *f){
	TDirectory* mainDir=0;
	if(f!=0)        mainDir = f->GetDirectory("/");
	if(mainDir!=0) 	   tdir = mainDir->mkdir("CutFlow");
	if(tdir==0) { 
		cout<<"CutSet::setTFile: cannot create directory in file!"<<endl;
		exit(0);
	}
	tfile=f;
};

void CutSet::dumpToHist(){

	int bins=allCuts.size();

	// remove spaces from name
	TString hname;
	for(Ssiz_t i=0;i<Name.Length();i++) hname.Append( Name[i]==' ' ? '_' : Name[i] );

	tdir->cd();
	autoSavePtr<TH1D> hallCutsWOut = new TH1D(hname+"_WOut",Name+" - single cut out weighted",bins,0,bins);
	autoSavePtr<TH1I> hallCutsIn   = new TH1I(hname+"_In"  ,Name+" - single cut in"          ,bins,0,bins);
	autoSavePtr<TH1I> hallCutsOut  = new TH1I(hname+"_Out" ,Name+" - single cut out"         ,bins,0,bins);

	TAxis* axisA =  hallCutsOut->GetXaxis();
	TAxis* axisR =   hallCutsIn->GetXaxis();
	TAxis* axisW = hallCutsWOut->GetXaxis();
	
	for(unsigned c=0; c<allCuts.size(); ++c) {
		axisA->SetBinLabel(c+1,allCuts[c]);
		axisR->SetBinLabel(c+1,allCuts[c]);
		axisW->SetBinLabel(c+1,allCuts[c]);
		 hallCutsOut->SetBinContent(c+1, survivors[allCuts[c]]);
		  hallCutsIn->SetBinContent(c+1, rejected[allCuts[c]]+survivors[allCuts[c]] );
		hallCutsWOut->SetBinContent(c+1, survivors_weighted[allCuts[c]]);
        }
	for(unsigned f=0; f<allFlows.size(); ++f) {

		TString rawCuts=allFlows[f];

		vector<TString> vcuts;
		TString cutFlow;
		int i=0;
		while(rawCuts.Tokenize(cutFlow,i,delim)) vcuts.push_back(cutFlow);

		bins=vcuts.size();
		autoSavePtr<TH1D> hallCutsWOutFlow = new TH1D(hname+"_WOut_cut_flow",Name+" - cut flow out weighted",bins,0,bins);
		autoSavePtr<TH1I> hallCutsOutFlow  = new TH1I(hname+"_Out_cut_flow" ,Name+" - cut flow out"         ,bins,0,bins);
		autoSavePtr<TH1I> hallCutsInFlow   = new TH1I(hname+"_In_cut_flow"  ,Name+" - cut flow in"          ,bins,0,bins);

		TAxis* axisR =   hallCutsInFlow->GetXaxis();
		TAxis* axisA =  hallCutsOutFlow->GetXaxis();
		TAxis* axisW = hallCutsWOutFlow->GetXaxis();
		
		for(unsigned c=0; c<bins; ++c) {
			axisR->SetBinLabel(c+1,vcuts[c]);
			axisA->SetBinLabel(c+1,vcuts[c]);
			axisW->SetBinLabel(c+1,vcuts[c]);
			hallCutsOutFlow->SetBinContent(c+1, flowSurvivors[rawCuts][vcuts[c]]);
			 hallCutsInFlow->SetBinContent(c+1, flowRejected[rawCuts][vcuts[c]]+flowSurvivors[rawCuts][vcuts[c]]);
		       hallCutsWOutFlow->SetBinContent(c+1, flowSurvivors_weighted[rawCuts][vcuts[c]]);
		}

	}

}
