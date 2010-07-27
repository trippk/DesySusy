//---NtupleTools----------
//   Version 2.0
//   15.07.2010
//   dirk.kruecker@desy.de

#ifndef NtupleTools2_h
#define NtupleTools2_h

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wuninitialized"

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include "TTree.h"
#include "TChain.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TString.h"
#include "TStopwatch.h"
#include "Math/LorentzVector.h"
#include "Math/DisplacementVector3D.h"
#include "Math/GenVector/PositionVector3D.h"

using namespace std;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >            LorentzV;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double> > XYZVectorD;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double>  >     XYZPointD;


//---------- simple progress counter and timer -----------------------------
void progress(ostream& os=cout){
        static int cnt(0),step(1),next(1);
        cnt++;
        if(cnt==next){
                os<<cnt<<endl;
                next+=step;
        }
        if((cnt+1)/step==10)step*=10;
};
void progressT(int flush=0,ostream& os=cout){
	static TStopwatch timer;
        static int cnt(0),step(1),next(1);
	if(cnt==0) timer.Start();
        cnt++;
        if(cnt==next||flush==1){
        	timer.Stop();
                os<<setw(10)<<left<<cnt
                  <<" time (time/evt) - real: "
                  <<setw(10)<<timer.RealTime()
                  <<"("<<setw(10)<<timer.RealTime()/step
                  <<"), \tCPU: "<<setw(10)<<timer.CpuTime()
                  <<"("<<setw(10)<<timer.CpuTime()/step<<")"<<endl;
                timer.Start(false);
                next+=step;
        }
        if((cnt+1)/step==10)step*=10;
};

//---------- file handling -----------------------------
int GetResult(vector<string>& out, const string& command){
	TString line;
	FILE* pipe= gSystem->OpenPipe(command.c_str(),"r");
	if(!pipe){ 
		cerr<<"Did not work: "<<command<<endl;
	} else {
		while (line.Gets(pipe)) if(line!="") {
			
			out.push_back(string(line));
		}
		gSystem->ClosePipe(pipe);
	}
	return out.size();
}
int AllRootFilesIn(const string& dir,TChain* chain,const string& LScommand="ls",int max=1000){
	vector<string> files;
	int n=0;
	if(LScommand=="dcls"){
		n=GetResult(files,"dcls "+dir+" | grep \"\\.root\" ");
		n=n>max?max:n; 
		const string dcache_gate="dcap://dcache-ses-cms.desy.de:22125/";
		for(int i=0;i<n;++i) chain->Add((dcache_gate+dir+"/"+files[i]).c_str());
	} else if(LScommand=="rfdir") {
		n=GetResult(files,LScommand+" "+dir+" | grep \"\\.root\" | awk \'{print $9}\' ");	
		n=n>max?max:n; 
		for(int i=0;i<n;++i) chain->Add(("rfio:"+dir+"/"+files[i]).c_str());
	} else {
		n=GetResult(files,LScommand+" "+dir+" | grep \"\\.root\"");	
		n=n>max?max:n; 
		for(int i=0;i<n;++i) chain->Add((dir+"/"+files[i]).c_str());
	}
	return n;
};
void AllRootFilesIn(const string& dir,TChain* chain,int max=1000,const string& LScommand="ls"){
	AllRootFilesIn(dir,chain,LScommand,max);
};

//---------- chain and tree handling -----------------------------
// inline is redundant since this in a header files
class EasyChain: public TChain {
public:
	EasyChain(const char* tname) : TChain(tname), localEntry(0), localMax(0), off(0) {};

	// here all kinds of variables can be load from the chain
	// e.g.: vector<LorentzV>* electrons = tree->Get(&electrons,"electronP4Pat");
	//       electron->size()
	template<typename T> 
	inline T* Get(T** ppt, const char* name){
		*ppt=0;
		TBranch* branch;
		if( byName.find(name)==byName.end() ) branch=byName[name]= GetBranch( name );
		else branch=byName[name];
		if(branch==0) {
			cerr<<"Branch "<<name<<" is undefined in tree: "<<GetName()<<endl;
			exit(0);
		}
		branch->SetAddress( ppt );
		branch->GetEntry(localEntry,1);
		return *ppt;
	};
	template<typename T> 
	inline T* Get(T** ppt, const string& name){
		return Get(ppt,name.c_str());
	}
	// the same as above but the return type is a reference (same performance)
	// e.g.: vector<LorentzV>& electrons = tree->Get(&electrons,"electronP4Pat");
	//       Electron.size()
	template<typename T> 
	inline T& Get(T* leaf,const char* name) {
		T* pt=0;
		TBranch* branch;
		// This increases the performance since GetBranch seraches the full tree
		// byNames.find only the used names
		if( byName.find(name)==byName.end() ) branch=byName[name]= GetBranch( name );
		else branch=byName[name];
		if(branch==0) {
			cerr<<"Branch "<<name<<" is undefined in tree: "<<GetName()<<endl;
			exit(0);
		}
		branch->SetAddress( &pt );
		branch->GetEntry(localEntry,1);
		return *pt;
	};
	template<typename T> 
	inline T& Get(T* leaf, const string& name) {
		return Get(leaf,name.c_str());
	}
	// this is meant for simple data types as int,float,double i.e. splitlevel 0
	// e.g.: unsigned run = tree->Get(run,"run");
	//                          note: ^ no &
	template<typename T> 
	inline T Get(T& leaf,const char* name) {
		TBranch* branch;
		if( byName.find(name)==byName.end() ) branch=byName[name] = GetBranch( name );
		else branch=byName[name];
		if(branch==0) {
			cerr<<"Branch "<<name<<" is undefined in tree: "<<GetName()<<endl;
			exit(0);
		}
		branch->SetAddress(&leaf);
		branch->GetEntry(localEntry,1);
		return leaf;
	};
	template<typename T> 
	inline T Get(T& leaf, const string& name) {
		return Get(leaf,name.c_str() ) ;
	}

private:
	void FillBranchAddresses(TTree* t){
		for(map<const string,TBranch*>::iterator it=byName.begin();it!=byName.end();++it)
			it->second = t->GetBranch(  it->first.c_str() );
	}

public:
 	// get entry and check for new tree
	// sequential give best performance
	virtual inline Int_t GetEntry(Long64_t  entry, Int_t getall = 0){
		localEntry=entry-off;
		if(localEntry>=localMax||localEntry<0){
			localEntry=LoadTree(entry);
			if(fTree) localMax=fTree->GetEntries();
			off=fTreeOffset[fTreeNumber];
			FillBranchAddresses(fTree);
		}
		return localEntry>=0;
	}

private:
	int localEntry;
	int localMax;
	int off;
	// acts as booster for tree with many branches
	map<const string,TBranch*> byName;
};
#endif
