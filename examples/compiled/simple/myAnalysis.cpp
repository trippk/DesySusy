#include <map>
#include <iostream>
#include "TSystem.h"
#include "NtupleTools2.h"

using namespace std;


void myAnalysis(){
	EasyChain* tree = new EasyChain("/susyTree/tree");
	tree->Add("/scratch/hh/current/cms/user/schettle/mc/Fall10-START38_V12-v1/LM12_SUSY_sftsht_7TeV-pythia6/SusyCAF_Tree_19_1_iwD.root");
	tree->GetEntry(0);
	int N = tree->GetEntries();

	int cntHlt=0;
	int cntMus=0;
	for(int i=0;i<N;++i){

		tree->GetEntry(i);
                map<string,bool>& HLTtrigger = tree->Get(&HLTtrigger, (string("trigg")+"ered").c_str());
		if( HLTtrigger.size()!=0&&!HLTtrigger["HLT_Mu9"] ) continue;
		cntHlt++;
		vector<LorentzV>& Muons = tree->Get(&Muons, "muonP4Pat");		
		int im = Muons.size();
		if (im>2) cntMus++;

	}
	cout<<"HLT_Mu9 found in "<<cntHlt<<" of "<<N<<" events"<<endl;
	cout<<cntMus<<" of the triggered events contains more than 2 muons"<<endl;
	
}

#ifndef __CINT__
int main(){
        myAnalysis();
        return 0;
}
#endif
