#ifndef subTreeFactory_h
#define subTreeFactory_h

#include "TString.h" 
#include "TFile.h"
#include "subTree.h"
#include "defaultTree.h"
#include "Nminus1Tree.h"
#include "anDiLep.h"
#include "trigStudyTree.h"

#include <iostream>

class subTreeFactory
{
 public:

  static TDirectory * getDir(TFile * tfile, const TString newdir) {

    if (!tfile) {
      //NULL pointer to tfile. Return null;
      return NULL;
    }

    TObjArray* arr=newdir.Tokenize("/");
    TIter next(arr);
    TString* token;
    //arr->Print();
    Int_t numsep=arr->GetEntries();
  
    TObjString* tok;

    TDirectory * dir = (TDirectory*) tfile;
    
    TString newpath="";
    for(Int_t i=0; i<numsep;++i){
      
	tok=(TObjString*)(*arr)[i];    
	TString newsubdir=tok->GetName();
	newpath=newpath+newsubdir+(TString)+"/";

	if ( !dir ) {
	  cout << "ERROR : Could not create the subdirectory " << newdir << endl;
	  break; //Protect against NULL 
	}

	//Try to get the new subdir, to see if it exists
	TDirectory * newdir=(TDirectory*)dir->Get(newsubdir); //Get the new subdir.
	if (newdir == 0) {
	  //cout << "Creating dir: " << newsubdir << endl;
	  bool okmkdir=dir->mkdir(newsubdir); //Create the new subdir. If it exists, returns false.
	  //cout << "DONE!" << endl;
	  dir=(TDirectory*)dir->Get(newsubdir); //Get the new subdir.
	}
	else {
	  //cout << "Found exisiting directory!"<< endl;
	  dir = newdir;
	}

 	//      cout<<"result of mkdir is "<<okmkdir<<endl; 
 	//      cout<<"the current path is "<<newpath<<endl; 
 	//if (!okmkdir){ 
 	  //======IT EXISTS, SO CD INTO IT AND REPEAT 
 	  //	cout<<"so now I'm here"<<endl; 
 	  //directory aleady exists!, go to it 
 	  //	cout<<"trying to get "<<(TString)tok->GetName()<<endl; 
 	  //cout<<"the directory already existed. Now I'm in "<<dir->GetName()<<endl; 
 	//} 
	//dir=(TDirectory*)dir->Get(newsubdir); //Get the new subdir.
    }
    
    //Clear-up memory
    if (arr) {
      delete arr;
      arr = 0;
    }

    return dir;
  }

  
  static subTree* NewTree(TString description="default") {
    //    TString desc=description;
    TFile* tfile=0;
    string dir="";
    //NewTree(description,tfile,dir);
    return subTreeFactory::NewTree(description,tfile,dir);
  }

  static subTree* NewTree(TString description, TFile* tfile, TString dir) {

    TDirectory * originalDir = gDirectory; //Save the current directory

    TDirectory * treeDir = subTreeFactory::getDir(tfile, dir);    
    //If treeDir is NULL, use currentDir
    if (!treeDir) {
      cout << "Creating subTree in current directory! " << endl;
      treeDir = originalDir;
    }
    treeDir->cd(); //cd into treeDir. Trees and histograms will be stored here.
    
    if(description == "default") {
      std::cout<<"default subTree will be filled."<<std::endl;
      if (tfile!=0){
	//std::cout<<"CALLING NEW TREE WITH PARAMETERS "<<std::endl;
	return new defaultTree(tfile, treeDir);
      }else{
	//	  std::cout<<"CALLING NEW TREE WITH NO PARAMETERS "<<std::endl;
	return new defaultTree;
      }
    }
    else if(description == "Nminus1") {
      std::cout<<"Nminus1 subTree will be filled."<<std::endl;
      return new Nminus1Tree;
    }
    else if(description == "anDiLep") {
      std::cout<<"anDiLep subTree will be filled."<<std::endl;
      return new anDiLep;
    }
    if(description == "trigStudyTree") {
      std::cout<<"TrigStudyTree subTree will be filled."<<std::endl;
      if (tfile!=0){
	//std::cout<<"CALLING NEW TREE WITH PARAMETERS "<<std::endl;
	return new trigStudyTree(tfile, dir);
      }else{
	//	  std::cout<<"CALLING NEW TREE WITH NO PARAMETERS "<<std::endl;
	return new trigStudyTree;
      }
    }
    else{
      std::cout<<"No matching for subTreeType: defaultTree will be used"<<std::endl;
    }

    //Return to the original dir
    originalDir->cd();

  }
};

#endif
