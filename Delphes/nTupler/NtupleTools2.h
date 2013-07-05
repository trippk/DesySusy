//---NtupleTools----------
//   Version 2.0
//   15.07.2010
//   19.11.2010 duplicate check in AllRootFilesNoDup
//   29.11.2010 clash removed in case of multiple opened EasyChains
//   15.02.2011 conditionals for pure header use if __NTHEADER___ defined.
//              This aproach allows the use of NtupleTools2.h with command line macros
//              since the complete implementation is in one file
//     2.5.2011 - Improved performance in case of multiple acces of a branch for same entry
//              - new dcache door 
//     8.6.2013 - simple TClonesArray to vector interface
//              - no duplicates default -> false
//              - unique name base argument
//   dirk.kruecker@desy.de

#ifndef NtupleTools2_h
#define NtupleTools2_h

#ifdef __GNUC__
// only visible for the gnu pre-compiler
//#warning __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TString.h"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TStopwatch.h"
#include "Math/LorentzVector.h"
#include "Math/DisplacementVector3D.h"
#include "Math/GenVector/PositionVector3D.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "Math/GenVector/PtEtaPhiE4D.h"
#include "Math/GenVector/PxPyPzM4D.h"
#include "Math/GenVector/PtEtaPhiM4D.h"

using namespace std;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >            LorentzV;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >          LorentzM;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<float> >          LorentzE;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> >          LorentzMD;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<double> >          LorentzED;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double> > XYZVectorD;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<double>  >     XYZPointD;
typedef ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<float> > XYZVectorF;
typedef ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>  >     XYZPointF;


#ifdef __CINT__ 
// for the command line ACLIC macro
// compiled during rootcint
//#warning __CINT__
#pragma link C++ class vector<LorentzV>+;
#pragma link C++ class vector<LorentzM>+;
#pragma link C++ class vector<XYZVectorD>+;
#pragma link C++ class vector<XYZPointD>+;
#pragma link C++ class vector<XYZVectorF>+;
#pragma link C++ class vector<XYZPointF>+;
#pragma link C++ class map<string,bool>+;
#pragma link C++ class map<string,string>+;
//
#pragma link C++ class pair<string,bool>+;
#pragma link C++ class pair<string,string>+;
#pragma link C++ class map<string,int>+;
#pragma link C++ class map<string,string>+;
//
#pragma link C++ class ROOT::Math::Cartesian3D<float>+;
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> >+;
#pragma link C++ class ROOT::Math::PositionVector3D<ROOT::Math::Cartesian3D<float>  >+;
#pragma link C++ class ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<float> >+;
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> >+;
#pragma link C++ class ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<float> >+;
#pragma link C++ class ROOT::Math::PtEtaPhiM4D<float>+;
#endif

//namespace ROOT {
#ifndef __CINT__ 
// following functions should not be processed by rootcint
//---------- simple progress counter and timer -----------------------------
void progress(ostream& os=cout,const TString& pref="",const TString& postf="")
#ifndef __NTHEADER___
{
        static int cnt(0),step(1),next(1);
        cnt++;
        if(cnt==next){
                os<<pref<<cnt<<postf<<endl;
                next+=step;
        }
        if((cnt+1)/step==10)step*=10;
}
#endif
;
void progressT(int flush=0,ostream& os=cout)
#ifndef __NTHEADER___
{
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
}
#endif
;
void timer(ostream& os=cout,int stp=10)
#ifndef __NTHEADER___
{
	static TStopwatch timer;
        static int cnt(0),step(stp),next(stp);
	if(cnt==0) timer.Start();
        cnt++;
        if(cnt==next){
        	timer.Stop();
                os<<"Evts: "<<setw(10)<<right<<cnt
                  <<" | time [sec] - real: "
                  <<left<<setprecision(4)<<setw(10)<<timer.RealTime()
                  <<" \tCPU: "<<setw(10)<<left<<setprecision(4)<<timer.CpuTime()<<endl;
                timer.Start(false);
                next+=step;
        }
        if((cnt+1)/step==10)step*=10;
}
#endif
;

//---------- file handling -----------------------------
int GetResult(vector<string>& out, const TString& command,bool nodup)
#ifndef __NTHEADER___
{
	TString line;
	FILE* pipe= gSystem->OpenPipe(command,"r");
	if(!pipe){
		cerr<<"Did not work: "<<command<<endl;
	} else {
		while (line.Gets(pipe)) if(line!="") {
			out.push_back(string(line));
		}
		gSystem->ClosePipe(pipe);
	}
	if(nodup){
		map<string, pair<unsigned,string> > singleOut;
		map<string, pair<unsigned,string> >::iterator it;
		unsigned i;
		for(i=0;i<out.size();i++){
			//check format
			if(count(out[i].begin(),out[i].end(),'_') < 3) break;
			//let's hope it fits
			unsigned pos =out[i].rfind("_");
			unsigned pos2=out[i].substr(0,pos).rfind("_")+1;
			unsigned n=atoi(out[i].substr(pos2,pos-pos2).c_str());
			it = singleOut.find(out[i].substr(0,pos2));
			if(it!=singleOut.end()) {
				if(it->second.first<n) {
					it->second.first=n;
					it->second.second=out[i].substr(pos2);
				}
			} else singleOut[out[i].substr(0,pos2)]=pair<unsigned,string>(n,out[i].substr(pos2));
		}
		if(i==out.size()){
			if (out.size()!=singleOut.size()) cout<< out.size()-singleOut.size() <<" duplicates ignored!"<<endl;;
			out.clear();
			for(it = singleOut.begin();it!=singleOut.end();it++)
			 		out.push_back(it->first+it->second.second);
		} else cout<<"File name format not appropriate for duplicate check!"<<endl;
	}
	return out.size();
}
#endif
;
//AllRootFilesIn(dir,tree)           // any 'ls'-able directory
//AllRootFilesIn(dir,tree,10)        // any 'ls'-able directory, first 10 files
//AllRootFilesIn(dir,tree,"dcls")    // dcache -  needs proxy & dctools !
//AllRootFilesIn(dir,tree,"dcls",10) //first 10 files in dir
int AllRootFilesIn(const TString& dir,TChain* chain,const TString& LScommand,int max,bool nodup=false)
#ifndef __NTHEADER___
{
	vector<string> files;
	int n=0;
	if(LScommand=="dcls"){
		n=GetResult(files,"dcls "+dir+" | grep \"\\.root\" ",nodup);
		n=n>max?max:n;
//old		const string dcache_gate="dcap://dcache-ses-cms.desy.de:22125/";
		const string dcache_gate="dcap://dcache-cms-dcap.desy.de:22125/";
		for(int i=0;i<n;++i) chain->Add((dcache_gate+dir+"/"+files[i]));
	} else if(LScommand=="rfdir") {
		n=GetResult(files,LScommand+" "+dir+" | grep \"\\.root\" | awk \'{print $9}\' ",nodup);	
		n=n>max?max:n;
		for(int i=0;i<n;++i) chain->Add(("rfio:"+dir+"/"+files[i]));
	} else {
		n=GetResult(files,LScommand+" "+dir+" | grep \"\\.root\"",nodup);
		n=n>max?max:n;
		for(int i=0;i<n;++i) chain->Add((dir+"/"+files[i]));
	}
	return n;
}
#endif
;
// no duplicate check
int AllRootFilesIn(const TString& dir,TChain* chain)
#ifndef __NTHEADER___
{
	return AllRootFilesIn(dir,chain,"ls",1000,false);
}
#endif
;
int AllRootFilesIn(const TString& dir,TChain* chain,const TString& LScommand)
#ifndef __NTHEADER___
{
	return AllRootFilesIn(dir,chain,LScommand,1000,false);
}
#endif
;
int AllRootFilesIn(const TString& dir,TChain* chain,int max)
#ifndef __NTHEADER___
{
	return AllRootFilesIn(dir,chain,"ls",max,false);
}
#endif
;
// duplicate check
int AllRootFilesNoDup(const TString& dir,TChain* chain)
#ifndef __NTHEADER___
{
	return AllRootFilesIn(dir,chain,"ls",1000,true);
}
#endif
;
int AllRootFilesNoDup(const TString& dir,TChain* chain,const TString& LScommand)
#ifndef __NTHEADER___
{
	return AllRootFilesIn(dir,chain,LScommand,1000,true);
}
#endif
;
int AllRootFilesNoDup(const TString& dir,TChain* chain,int max)
#ifndef __NTHEADER___
{
	return AllRootFilesIn(dir,chain,"ls",max,true);
}
#endif
;
string file_base(const string& nam)
#ifndef __NTHEADER___
{
	int dot=nam.rfind(".");
	int slash=nam.rfind("/")+1;
	return nam.substr(slash,dot-slash);
}
#endif
;
TString file_base(const TString& nam)
#ifndef __NTHEADER___
{
	const string namstr(nam.Data());
	return file_base(namstr).c_str();
}
#endif
;
#endif
//---------- chain and tree handling -----------------------------
// inline is redundant since this is a header files
class EasyChain: public TChain {
public:
	EasyChain(const char* tname) : TChain(tname), localEntry(0), localMax(0), off(0), dcache(false) {
		fileWeight=0;
	};

	// here all kinds of variables can be load from the chain
	// e.g.: vector<LorentzV>* electrons = tree->Get(&electrons,"electronP4Pat");
	//       electron->size()
	template<typename T>
	inline T* Get(T** ppt, const char* name){
		TBranch* branch;
		// This increases the performance since GetBranch searches the full tree
		// byNames.find only the used names
		if( localByName.find(name)==localByName.end() ) {
			branch = byName[name] = GetBranch( name );
			localByName[name].second = 0;
			localByName[name].first = -1;
		}
		else branch=byName[name];
		if(branch==0) {
			cerr<<"Branch "<<name<<" is undefined in tree: "<<GetName()<<endl;
			exit(0);
		}
		if(localByName[name].first==localEntry && localByName[name].second!=0) 
			return static_cast<T*>(localByName[name].second);
		if(localByName[name].second!=0){
			T* toDelete = static_cast<T*>(localByName[name].second);
			delete toDelete;
			localByName[name].first=-1;
		}
		*ppt=0;
		branch->SetAddress( ppt );
		branch->GetEntry(localEntry,1);
		localByName[name].second=*ppt;
		localByName[name].first=localEntry;
		return *ppt;
	};
	template<typename T>
	inline T* Get(T** ppt, const TString& name){
		return Get(ppt,name.Data());
	}
	// the same as above but the return type is a reference (same performance)
	// e.g.: vector<LorentzV>& electrons = tree->Get(&electrons,"electronP4Pat");
	//       Electron.size()
	template<typename T>
	inline T& Get(T* leaf,const char* name) {
		leaf=leaf;//just to get rid of the  unused warning
		TBranch* branch;
		if( localByName.find(name)==localByName.end() ) {
			branch = byName[name] = GetBranch( name );
			localByName[name].second = 0;
			localByName[name].first = -1;
		}
		else branch=byName[name];
		if(branch==0) {
			cerr<<"Branch "<<name<<" is undefined in tree: "<<GetName()<<endl;
			exit(0);
		}
		if(localByName[name].first==localEntry && localByName[name].second!=0) return *static_cast<T*>(localByName[name].second);
		if(localByName[name].second!=0) {
			T* toDelete = static_cast<T*>(localByName[name].second);
			delete toDelete;
			localByName[name].first=-1;
		}
		T* pt=0;
		branch->SetAddress( &pt );
		branch->GetEntry(localEntry,1);
		localByName[name].second=pt;
		localByName[name].first=localEntry;
		return *pt;
	};
	template<typename T>
	inline T& Get(T* leaf, const TString& name) {
		return Get(leaf,name.Data());
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
	inline T Get(T& leaf, const TString& name) {
		return Get(leaf,name.Data() ) ;
	}

	template<typename T>
	inline void GetCA(vector<T*>& some ,int N, const char* name){ // not optimal efficient but simple syntax
		TClonesArray* ar = Get(&ar,name);
		some.clear();
		for(int i = 0;i < N;i++) {
			some.push_back( (T*)ar->At(i) );
		}
	};
	template<typename T>
	inline void GetCA(vector<T*>& some ,int N,const TString& name){
		GetCA(some,N,name.Data());
	};

	template<typename T>
	inline void Get4(vector<T*>& some ,int N, const char* name){ // not optimal efficient but simple syntax
		TClonesArray* ar = Get(&ar,name);
		some.clear();
		for(int i = 0;i < N;i++) {
			((T*)(ar->At(i)))->Fill4();
			some.push_back( (T*)ar->At(i) );
		}
	};
	template<typename T>
	inline void Get4(vector<T*>& some ,int N,const TString& name){
		Get4(some,N,name.Data());
	};

private:
	void FillBranchAddresses(TTree* t){
		for(map<const string,TBranch*>::iterator it=byName.begin();it!=byName.end();++it)
			it->second = t->GetBranch(  it->first.c_str() );
	}

public:
 	// get entry and check for new tree
	// sequential gives best performance
	virtual inline Int_t GetEntry(Long64_t  entry, Int_t getall = 0){
		getall=getall;//just to get rid of unused warning
		localEntry=entry-off;
		if(localEntry>=localMax||localEntry<0){
			localEntry=LoadTree(entry);
			if(fTree) localMax=fTree->GetEntries();
			else cout<<"hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"<<endl;
			off=fTreeOffset[fTreeNumber];
			FillBranchAddresses(fTree);
		}
		return localEntry>=0;
	}
 	// get entry and check for new tree + weight
	// sequential gives best performance
	inline double GetEntryW(Long64_t  entry, Int_t getall = 0){
		getall=getall;//just to get rid of unused warning
		localEntry=entry-off;
		if(localEntry>=localMax||localEntry<0){
			localEntry=LoadTree(entry);
			if(fTree) localMax=fTree->GetEntries();
			else cout<<"hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm"<<endl;
			off=fTreeOffset[fTreeNumber];
			FillBranchAddresses(fTree);
			TString filename = GetFile()->GetName();
			TString nam( filename(0,filename.Index("//")+1) );
			if( weights.find(nam)==weights.end() ) {
				cout<<"NtupleTools2:GetEntry don't find a weight for "<<filename<<endl;
				exit(0);
			}
			fileWeight=weights[nam];
		}
		return fileWeight;
	}
	// helper function for AddSmart below
	int AddSmartSingle(const TString& name,int max,bool nodup){
		string dcache_gate="dcap://dcache-cms-dcap.desy.de:22125/";
		vector<string> files;
		int n=1;
		if (name.Index("/pnfs/")==0 || name.Index("/store/")==0){
			if(name.Index("/store/")==0) dcache_gate+="/pnfs/desy.de/cms/tier2/";
			if(name.EndsWith("/")){
				n=GetResult(files,"dcls "+name+" | grep \"\\.root$\" ",nodup);
				n=n>max?max:n;
				for(int i=0;i<n;++i) Add((dcache_gate+name+"/"+files[i]));
			} else  Add(dcache_gate+name);
			dcache=true;
		} else {
			if(name.EndsWith("/")){
				n=GetResult(files,"ls "+name+" | grep \"\\.root$\"",nodup);
				n=n>max?max:n;
				for(int i=0;i<n;++i) Add((name+"/"+files[i]));
			} else Add(name);
		}
		return n;
	}
	// Add single file or all files in directory to this tree
	// assumes /pnfs or /store indicates dcache
	// takes trailing / as directory
	// checks for dir on normal filesystem
	// if name contains ' ' or ',' assume a list of filenames
	int AddSmart(const TString& longname,int max=10000,bool nodup=false){
		// check if name contains ' ' or ','
		TObjArray*  arr = longname.Tokenize(',');
		if(arr->GetEntries()==1) arr = longname.Tokenize(' ');
		if(arr->GetEntries()==1)
			// just one name
			return  AddSmartSingle(longname,max,nodup);
		int k=0;
		for(int idx=0;idx<arr->GetEntries();idx++){
			TObjString* tok = (TObjString*) (*arr)[idx];
		        k+=AddSmartSingle(tok->GetName(),max,nodup);
		        max-=k;
		}
		return k;
	}
	int AddSmartW(const TString& longname,double w,int max=10000,bool nodup=false){
		weights[longname]=w;
		return AddSmart(longname,max,nodup);
	}
	// return an (almost) unique file name depending on all files added to this chain
	// if only 1 file use basename
	string GetUniqeName(const char* base="SusyCAF_Tree"){
		stringstream sstr;
		sstr<<base;
		TObjArray* trees = GetListOfFiles();
		if(trees==0) {
			cout<<"EasyChain::GetUniqeName no files in tree"<<endl;
			exit(0);
		} else if(trees->GetEntries()==1) {
			string s((*trees)[0]->GetTitle());
			return file_base(s)+"_out.root";
		} else {
			// we create a hash number depending on all names
			sstr<<"_n"<<trees->GetEntries();
			TString longname;
			TIter next(trees);
			TChainElement *chEl=0;
			while (( chEl=(TChainElement*)next() )) longname.Append(chEl->GetTitle());
			sstr<<"_"<<longname.Hash()<<"_out.root";
			return sstr.str();
		}
	};
	void GetAll(){
		cout<<"EasyChain::GetAll not implemented"<<endl;
	}
	double fileWeight;
private:
	int localEntry;
	int localMax;
	int off;
	// files are from dcache
	bool dcache;
	// acts as booster for tree with many branches
	map<const string,TBranch*> byName;
	map<string, pair<int,void*> > localByName; // a pointer to the branch and the localEntry number for which it had been read
	map<TString,double>  weights;// weights depending on filename or dirname in AddSmartW
#ifdef  __NTHEADER___
	ClassDef(EasyChain, 1);
#endif
};
//}
#endif
