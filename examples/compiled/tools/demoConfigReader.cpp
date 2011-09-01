// g++ demoConfigReader.cpp ConfigReader.cpp -I$ROOTSYS/include -L$ROOTSYS/lib `root-config --libs`
#include <iostream>
#include "ConfigReader.h"
#include <fstream>
#include "TString.h"

using namespace std;

void sub();

int main(){

	// the first instance
	ConfigReader config("myConfig.txt");

	bool abool = config.getBool("aBool");
	cout<<"aBool="<<abool<<endl;

	int anint=config.getInt("anInt",456); // this default is ignored since anInt is defined in myConfig.txt
	cout<<"anInt="<<anint<<endl;

	float afloat=config.getFloat("aFloat");
	cout<<"aFloat="<<afloat<<endl;

	double adouble=config.getDouble("aDouble");
	cout<<"aDouble="<<adouble<<endl;

	double anotherDouble=config.getDouble("anotherDouble");
	cout<<"anotherDouble="<<anotherDouble<<endl;

	TString astring=config.getTString("aString");
	cout<<"aString="<<astring<<endl;

	TString tstring=config.getTString("anotherString");
	cout<<"anotherString="<<tstring<<"|< it ends here!"<<endl;

	config.getTString("aList","trigger_a trigger_b trigger_c");
	for(int i=0;i<config.ListSize("aList");i++){
		cout<<config.getTString("aList",i)<<endl;
	}

	int notdefined = config.getInt("notdefined",12); // this is not defined in myConfig.txt -> we use the default
	cout<<"notdefined="<<notdefined<<" < this is a default value"<<endl;

//	const TString a="ass";
//	const TString b="def";
//	int notdef = config.getInt(a+b,44);

	sub();

	//new 1.9.11
	cout<<"reading in vectors"<<endl;
	vector<double> aVec;
	for(int i=0;i<config.ListSize("doubleVector");i++){
		aVec.push_back( config.getDouble("doubleVector",i) );
	}
	// or for the lazy - uses push_back and adds to the end
	vector<double> aVec2;
	int N = config.getDouble("doubleVector",aVec2);
	for(int i=0;i<N;i++) cout<<aVec2[i]<<" ";cout<<endl;
	
	cout<<endl<<"A printout of the used configuration:"<<endl;
	// print used configuration to stdout
	config.printUsed(cout);
	cout<<endl;

	// print used configuration to file
	ofstream outfile;
	outfile.open("usedConfig.txt");
	config.printUsed(outfile);
	outfile.close();

	// now for inconsistent defaults
	cout<<"Now we crash deliberately :)"<<endl;
	int notdefined2 = config.getInt("notdefined",27);
	cout<<"notdefined="<<notdefined<<" < this is a default value but the one defined above!"<<endl;

}

void sub(){

	// a new instance
	ConfigReader config;

	cout<<endl<<"We are now in sub - the variables are still available"<<endl<<endl;
	bool b = config.getBool("aBool");
	cout<<"aBool="<<b<<endl;
	int i = config.getInt("anInt");
	cout<<"anInt="<<i<<endl;
	cout<<endl<<"back to main"<<endl;

}
