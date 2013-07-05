#include "NtupleTools2.h"
#include<iostream>
#include<vector>
#include<TLorentzVector.h>
using namespace std;

void reader(){

	EasyChain* tree = new EasyChain("delphTree");

	tree->Add("o.root");

	int N=tree->GetEntries();

	N=100;

	for(int i=0;i<N;i++){
		tree->GetEntry(i);

		int    Nel = tree->Get(   Nel,   "Nel");
		int looNel = tree->Get(looNel,"looNel");
		int rawNel = tree->Get(rawNel,"rawNel");

		vector<int>&          ElCh = tree->Get(&ElCh, "ElectronCh");
		vector<TLorentzVector>& El = tree->Get(&El,   "Electrons");
		vector<TLorentzVector>& Mu = tree->Get(&Mu,   "Muons");
		vector<int>&          MuCh = tree->Get(&MuCh, "MuonCh");

		
//		if(ElCh.size()!=2) continue;
		cout<<"rawNel: "<<rawNel<<" Nel: "<<Nel<<" looNel:"<<looNel<<endl;
		cout<<"#el ";
		for(unsigned ie=0;ie<El.size();ie++){
			cout<<ie<<" ";
		}
		cout<<endl<<"Ch: ";
		for(unsigned ie=0;ie<ElCh.size();ie++){
			cout<<ElCh[ie]<<" ";
		}
		cout<<endl;
	}

}
