// see header file for usage
#include "ConfigReader.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include "TObjString.h"
#include "TObjArray.h"
#include <iomanip>

using namespace std;

namespace {
	bool comp(ConfigReader::myMapType::value_type& a,ConfigReader::myMapType::value_type& b)
		{ return a.first.Length() < b.first.Length(); };
}

ConfigReader::ConfigReader(const TString& filename,int argc,char** argv) {
	if(filename!="") { // default in ConfigReader.h is ""
		if(usedFilename!="") {
			cout<<"ConfigReader::ConfigReader - ConfigReader already initialized with "<<usedFilename<<" !"<<endl;
			exit(0);
		}
		// read config file
		load(filename);
		// read command line
		for(int i=1;i<argc;i++) {
			TString arg(argv[i]);
			Ssiz_t pos=0;
			TString left,right;
			if(!arg.Tokenize(left,pos,"=")) continue;
			Ssiz_t l=arg.Length();
			configMap[left]=arg(pos,l-pos);
		}
	}
}

ConfigReader::~ConfigReader() {}
// static member initialization
ConfigReader::myMapType  ConfigReader::configMap=myMapType();
ConfigReader::myMapType ConfigReader::defaultMap=myMapType();
TString ConfigReader::usedFilename="";

unsigned ConfigReader::ListSize(const TString& property,const TString delim){
	myMapType::const_iterator key = configMap.find(property);
	if(key == configMap.end()) {
		cout<<"ConfigReader::getTString - "<<property<<" is not defined in "<<usedFilename<<endl;
		exit(0);
	}
	TObjArray*  arr = key->second.Tokenize( delimiter );
	int n =  arr->GetEntries();
	delete arr;
	return n;
}

// We implement functions for reading: string, int, bool, float, double and TString
TString  ConfigReader::getTString(const TString& property,const TString& defaultValue){
	return getstring(property,defaultValue);
}
TString  ConfigReader::getTString(const TString& property,int idx,const TString delim){
	myMapType::const_iterator key = configMap.find(property);
	if(key == configMap.end()) {
		cout<<"ConfigReader::getTString - "<<property<<" is not defined in "<<usedFilename<<endl;
		exit(0);
	}
	if(idx<0) return key->second;
	TObjArray*  arr = key->second.Tokenize(delim);
	if(idx>=arr->GetEntries()){
		cout<<"ConfigReader::getTString - "<<idx<<" is out of range "<<endl;
		exit(0);
	}
	TObjString* tok = (TObjString*) (*arr)[idx];
	TString name =  tok->GetName();
	delete arr;
	return name;
}
string ConfigReader::getString(const TString& property,const TString& defaultValue){
	return string(getTString(property,defaultValue));
}
string ConfigReader::getString(const TString& property){
	return string(getTString(property));
}
int ConfigReader::getInt(const TString& property, int defaultValue) {
	return atoi(getTString(property,toString(defaultValue)));
}
int ConfigReader::getInt(const TString& property) {
		myMapType::const_iterator key = configMap.find(property);
		if(key == configMap.end()) {
			cout<<"ConfigReader::getInt - '"<<property<<"' is not defined in "<<usedFilename<<endl;
			exit(0);
		}
		return atoi(key->second);
}
long ConfigReader::getLong(const TString& property, long defaultValue) {
	return atol(getTString(property,toString(defaultValue)));
}
long ConfigReader::getLong(const TString& property) {
		myMapType::const_iterator key = configMap.find(property);
		if(key == configMap.end()) {
			cout<<"ConfigReader::getLong - '"<<property<<"' is not defined in "<<usedFilename<<endl;
			exit(0);
		}
		return atol(key->second);
}
bool ConfigReader::getBool(const TString& property,bool defaultValue){
	TString def = defaultValue ? "true" : "false";
	myMapType::const_iterator key = configMap.find(property);
	if(key == configMap.end()) {
		//fill default into maps
		configMap[property]=defaultMap[property] = def;
	} else {
		// check if default is consisitent
		myMapType::const_iterator keyD = defaultMap.find(property);
		// a default has been defined previously - check if consistent
		if(keyD!=defaultMap.end() && keyD->second!= def ){
			// inconsistent
			cout<<"ConfigReader::getBool - '"<<key->first<<"' had been used with a different default value!"<<endl;
			cout<<"                         now: "<<def<<", before: "<<key->second<<endl;
			exit(0);
		}
	}
	return configMap[property] == "true";
};
bool ConfigReader::getBool(const TString& property) {
	myMapType::const_iterator key = configMap.find(property);
	if(key == configMap.end()) {
		cout<<"ConfigReader::getBool - "<<property<<" is not defined in "<<usedFilename<<endl;
		exit(0);
	} else {
		TString str=key->second;
		str.ToLower();
		str.ReplaceAll(" ","");
		str.ReplaceAll("\t","");
		return str == "true";
	}
}
double ConfigReader::getDouble(const TString& property, double defaultValue) {
	return atof(getTString(property,toString(defaultValue)));
}
double ConfigReader::getDouble(const TString& property) {
	myMapType::const_iterator key = configMap.find(property);
	if(key == configMap.end()) {
		cout<<"ConfigReader::getDouble - "<<property<<" is not defined in "<<usedFilename<<endl;
		exit(0);
	}
	return atof(key->second);
}
double    ConfigReader::getDouble(const TString& property, const TString& defaultValue){
	getString(property,defaultValue);
	return atof( getTString(property,0) );
};
double    ConfigReader::getDouble(const TString& property, int i, const TString delim){
	 return atof( getTString(property,i,delim) );
};
unsigned    ConfigReader::getDouble(const TString& property, vector<double>& aVec, const TString delim){
	unsigned N=ListSize(property);
	for(int i=0;i<N;i++) aVec.push_back( getDouble(property,i,delim) );
	return N;
};
float ConfigReader::getFloat(const TString& property, float defaultValue) {
	return getDouble(property,defaultValue);
}
float ConfigReader::getFloat(const TString& property) {
	return getDouble(property);
}

// config file reading
void ConfigReader::load(const TString& initFileName, bool newmap) {

	ifstream in( initFileName );

        if (!in) {
		cout<<"ConfigReader::load - File " + initFileName + " does not exist!"<<endl;
		exit(0);
        }
	usedFilename=initFileName;

        if(newmap) configMap.clear();
        TString fullLine, command;
        TString leftSide, rightSide;
        char line[10000];
        Ssiz_t length;
	myMapType::const_iterator key;

        while (in.getline(line, 10000)) {

		fullLine = line;
		// if the line starts a # then it is a comment
		// if we find it anywhere other than the beginning, then we assume
		// there is a command on that line, and it we don't find it at all
		// we assume there is a command on the line (we test for valid
		// command later) if neither is true, we continue with the next line
		length = fullLine.First('#');
		if (length == kNPOS) {
			command = fullLine;
		} else if (length > 0) {
			command = fullLine(0, length);
		} else  continue;

		// check the command and handle it
		length = command.Index('=');
		if (length != kNPOS) {
			leftSide = command(0, length);
			leftSide.ReplaceAll(" ","");
			leftSide.ReplaceAll("\t","");
			rightSide = command(length + 1, command.Length() - length);
			unsigned off=0;
			if(rightSide(0,1)==" ") off = 1; // remove first space
			key = configMap.find(leftSide);
			if(key == configMap.end()){
				Ssiz_t l = rightSide.Length();
				configMap[leftSide]=rightSide(off,l);
			} else {
				cout<<"ConfigReader::load - Multiple definition of \'"<<leftSide<<"\'. Fix your config file!"<<endl;
				exit(0);
			}
		} else  continue;

	}
}

void ConfigReader::printUsed(ostream& out){
	cout<<endl<<"_____________________ Used configuration _________________________"<<endl;
	int l=max_element(configMap.begin(),configMap.end(),comp)->first.Length();

	for(myMapType::const_iterator i = configMap.begin(); i != configMap.end(); ++i) {
		out << setw(l) << i->first << " = " << i->second << endl;
	} 
}
