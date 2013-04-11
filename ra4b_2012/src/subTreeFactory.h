#ifndef subTreeFactory_h
#define subTreeFactory_h

#include "TString.h" 
#include "TFile.h"
#include "subTree.h"
#include "defaultTree.h"
#include "Nminus1Tree.h"
#include "anDiLep.h"
#include "trigStudyTree.h"
#include "defaultTree.h"
#include "objectsTree.h"
#include <iostream>

class subTreeFactory
{
 public:
  
  static TDirectory * getDir(TFile * tfile, const TString newdir) {

    if (!tfile) {
      return NULL;
    }

    TDirectory * dir = (TDirectory*) tfile;  
    
    TObjArray* arr=newdir.Tokenize("/");
    const Int_t numsep=arr->GetEntries();
    for(Int_t i=0; i<numsep;++i){
      
      TObjString* tok=(TObjString*)(*arr)[i];    
      TString newsubdir=tok->GetName();
      
      if ( !dir ) {
	cout << "ERROR : Could not create the subdirectory " << newdir << endl;
	break; //Protect against NULL 
      }

      //Try to get the new subdir, to see if it exists
      TDirectory * newdir=(TDirectory*)dir->Get(newsubdir); //Get the new subdir.
      if (newdir == 0) {
	//Subdir does not exist. Try to create it.
	bool okmkdir=dir->mkdir(newsubdir); 
	newdir=(TDirectory*)dir->Get(newsubdir); 

	//If subdir not created, exit
	if (newdir == 0) {
	  //<<<<<<< subTreeFactory.h
	  cout << "ERROR: Problem creating the directory: " << newdir << endl;
	  break;
	  //=======
	  //cout << "Creating dir: " << newsubdir << endl;
	  bool okmkdir=dir->mkdir(newsubdir); //Create the new subdir. If it exists, returns false.
	  //cout << "DONE!" << endl;
	  dir=(TDirectory*)dir->Get(newsubdir); //Get the new subdir.
	}
	else {
	  //cout << "Found exisiting directory!"<< endl;
	  dir = newdir;
	  //>>>>>>> 1.7.2.1
	}
      }

      //If here, subdir was either found or successfully created.
      dir = newdir;
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

    if(description == "objectsTree") {
      std::cout<<"objectsTree will be filled."<<std::endl;
      if (tfile!=0){
	return new objectsTree(tfile, dir);
      }else{
	return new objectsTree;
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
