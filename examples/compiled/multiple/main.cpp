#include <map>
#include <iostream>
#include "TSystem.h"
#include "NtupleTools2.h"
#include "checkMuons.h"
#include "checkJets.h"

using namespace std;

int main(){

	EasyChain* tree = new EasyChain("/susyTree/tree");
	string dir="/scratch/hh/current/cms/user/schettle/mc/Fall10-START38_V12-v1/LM12_SUSY_sftsht_7TeV-pythia6/";
	tree->Add((dir+"SusyCAF_Tree_19_1_iwD.root").c_str());
	//AllRootFilesIn(dir,tree);
	tree->GetEntry(0);
	int N = tree->GetEntries();

	int cntHlt=0;
	int cntMus=0;
	int totGoodJets=0;
	int Mu2GoodJets=0;
	for(int i=0;i<N;++i){

		progress(cout,"Events ");
		
		tree->GetEntry(i);

                map<string,bool>& HLTtrigger = tree->Get(&HLTtrigger, "triggered");
		if( HLTtrigger.size()!=0&&!HLTtrigger["HLT_Mu9"] ) continue;
		cntHlt++;

		int im = checkMuons(tree);
		if (im>2) 	cntMus++;
		else continue;
		
		totGoodJets+=checkJets(tree);

	}
	cout<<"HLT_Mu9 found in "<<cntHlt<<" of "<<N<<" events"<<endl;
	cout<<cntMus<<" of the triggered events contains more than 2 muons"<<endl;
	cout<<"In total "<<totGoodJets<<" good jets have been found in the triggered events."<<endl;
	
	return 0;
}
