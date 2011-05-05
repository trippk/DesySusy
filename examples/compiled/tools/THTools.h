// dk 5.5.2011
// - fetchHisto
// - autoSavePtr
#ifndef THTools_h
#define THTools_h

#include <iostream>
#include "TString.h"
#include "TDirectory.h"

using namespace std;

// helper function to fetch histogram + check whether it exists
template<typename T>
inline T* fetchHisto(const TString& name){

	T* h=dynamic_cast<T*>(gDirectory->Get(name));
	if(h==0) {
		cout<<"fetchHisto: could not find "<<name<<endl;
		exit(0);
	}
	return h;
}

// this can only be used with objects derived from TName which do have an Write() member function
// otherwise you will get an compile time error
/* Usage example:
#include "THTools.h"
#include "TH1.h"
int main()
{
     TFile *outfile = TFile::Open("out.root","RECREATE");
     autoSavePtr<TH1F> myHisto = new TH1F("myHisto","# of Susy evts.",50,0,100)
     ....
     // nothing to do at end of main - histograms will be written automatically  
}
*/

template <class T> class autoSavePtr {
public:
	typedef T element_type;
	autoSavePtr(T* p = 0) : itsCounter(0) {
		// allocate a new counter
        	if (p) itsCounter = new counter(p);
	}
	~autoSavePtr() {
		release();
	}
	autoSavePtr(const autoSavePtr& r) {
		acquire(r.itsCounter);
	}
	autoSavePtr& operator=(const autoSavePtr& r){
		if (this != &r) {
			release();
			acquire(r.itsCounter);
        	}
		return *this;
	}


	T& operator*()  const {return *itsCounter->ptr;}
	T* operator->() const {return itsCounter->ptr;}
	T* get()        const {return itsCounter ? itsCounter->ptr : 0;}
	bool unique()   const {return (itsCounter ? itsCounter->count == 1 : true);}

private:
	struct counter {
		counter(T* p = 0, unsigned c = 1) : ptr(p), count(c) {
			if(p!=0) name=p->GetName();
		}
		T*          ptr;
		unsigned    count;
		TString name;
	}* itsCounter;

	void acquire(counter* c){ // increment the count
		itsCounter = c;
		if (c) ++c->count;
	}

	void release(){ // decrement the count, delete if it is 0
		if (itsCounter) {
			if (--itsCounter->count == 0) {
				if(itsCounter->ptr==NULL){
					cout<<"autoSavePtr: pointer gone! for "<< itsCounter->name << "This should not happen!!!" <<endl;
					// exit(0);
				}
				itsCounter->ptr->Write();
				delete itsCounter->ptr;
				delete itsCounter;
			}
			itsCounter = 0;
		}
	}
};

#endif
