// dk 20.4.2011
#ifndef ConfigReader_h
#define ConfigReader_h

#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <TString.h>

/*
Usage: principle structure of the config file: key = value.
       Comments are marked by #
       Trailing comments are allowed for all types but strings.
       TABs are allowed.
       For strings: the first space after the '=' is removed
       the rest is taken literally including trailing spaces etc.

       Use:

           ConfigReader config("filename");

       only in your main program. This reads the config file and fills a static map.
       The static map makes the content available everywhere.
       In any other place of your code use:

           ConfigReader config;

       The value from the config file is used by (if it is a double):

           double var = config.getDouble("variableName");

       or:

           double var = config.getDouble("variableName",defaultValue);

       The config file value overwrites the defaultValue!

       The values that had been used can be dumped:

            config.printUsed(cout);

       Check the example.cpp for more examples.

       A third way to set values is the command line (No spaces allowed):

       	   myAnalysis variableName=Value
       The command line overwrites the config file.
*/

// the default delimiter
#define delimiter " "

using namespace std;

class ConfigReader {
public:
	ConfigReader(const TString& filename="",int argc=0,char** argv=0);
	~ConfigReader();

	typedef map<TString, TString> myMapType;

	 TString  getTString(const TString& property ,int i=-1,const TString delim=delimiter);
	 TString  getTString(const TString& property, const TString& defaultValue);
	 int      getInt(   const TString& property);
	 int      getInt(   const TString& property, int defaultValue);
	 bool     getBool(  const TString& property);
	 bool     getBool(  const TString& property, bool defaultValue);
	 float    getFloat( const TString& property);
	 float    getFloat( const TString& property, float defaultValue);
	 double   getDouble(const TString& property);
	 double   getDouble(const TString& property, double defaultValue);
	 string	getString(const TString& property);
	 string	getString(const TString& property, const TString& defaultValue);
	 string	getString(const TString& property,int i=-1,const TString delim=delimiter){
	 	return string(getTString(property,i,delim).Data());
	 };
	//
	unsigned ListSize(const TString&,const TString delim=delimiter);

	// print used variables
	void printUsed(ostream& out=cout);

private:
	TString  getstring(const TString& property, const TString& defaultValue);
	void load(const TString& initFileName);
	template<typename T> TString toString(T val){
		stringstream sstr;
		sstr.precision(10);
		sstr<<val;
		return sstr.str();
	};
	// static members for cross instance communication
	// and default value consistency check -  the bool is true for a value read from the config file
	static myMapType  configMap;
	static myMapType defaultMap;
	static TString usedFilename;
};
//inlines
inline TString  ConfigReader::getstring(const TString& property,const TString& defaultValue=""){
	myMapType::const_iterator key = configMap.find(property);
	if(key == configMap.end()) {
		// no value in config file
		if(defaultValue==""){
			cout<<"ConfigReader::get... - '"<<property<<"' is not defined in "<<usedFilename<<endl;
			exit(0);
		}
		//fill default into map
		configMap[property] =defaultValue;
		defaultMap[property]=defaultValue;
	} else {
		// check if default is consisitent
		myMapType::const_iterator keyD = defaultMap.find(property);
		// a default has been defined previously - check if consistent
		if(keyD!=defaultMap.end() && keyD->second!=defaultValue){
			// inconsistent
			cout<<"ConfigReader::get... - '"<<key->first<<"' had been used with a different default value!"<<endl;
			cout<<"                       now: "<<defaultValue<<", before: "<<key->second<<endl;
			exit(0);
		}
	}
	return configMap[property];
};

#endif

