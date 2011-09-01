// dk 25.4.2011
// a few usage examples
#include <iostream>
#include <cmath>

#include "CutFlow.h"
#include "TFile.h"

using namespace std;

void goodEvents(){

}
// since we do not have real data here
class elect {
public:
	elect(double pp,double ee): Pt(pp),Eta(ee){}
	elect(): Pt(0),Eta(0){}
	double pt(){return Pt;}
	double eta(){return Eta;}
private:
	double Pt;
	double Eta;
};
int main(){

	TFile* out = new TFile("histo.root","RECREATE");

	CutSet electronSelection("selecting good electrons");
	CutSet    eventSelection("selecting good events");

	electronSelection.quick=true;

	// cuts
	double elePtThreshold  =  10;
	double eleEtaThreshold = 2.4;

	// a test electron collection
	vector<elect> ele;
	// 6 'electrons'
	// 4 with pt  >  10
	//   from which 2 are at eta < 2.4
	// 4 with eta < 2.4
	//   from which 2 are at pt > 10
	ele.push_back(elect(1,1));
	ele.push_back(elect(5,1));
	ele.push_back(elect(15,1));
	ele.push_back(elect(20,3));
	ele.push_back(elect(15,3));
	ele.push_back(elect(20,3));
bool quick=true;
//	electronSelection.quick=false;
	for(int iev=0;iev<10;iev++){
		double ew=0.5; // event weight
		double q=iev/10.;

		CutSet::global_event_weight=0.5;

		if( !eventSelection.keepIf("quality", q>0.4) ) continue;

		for(vector<elect>::iterator el=ele.begin();el!=ele.end(); el++){
			if(electronSelection.keepIf("Pt",       el->pt()   > elePtThreshold)  && quick ) continue;
			if(electronSelection.keepIf("Eta", fabs(el->eta()) < eleEtaThreshold) && quick ) continue;
			electronSelection.applyCuts("Pt Eta");
		}
	}

	eventSelection.printAll();
	electronSelection.printAll();
	CutSet::dumpHist();

}
