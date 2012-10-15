#ifndef subTreeFactory_h
#define subTreeFactory_h

#include "TString.h" 
#include "TFile.h"
#include "subTree.h"
#include "defaultTree.h"
#include "Nminus1Tree.h"

#include <iostream>

class subTreeFactory
{
 public:
  
  static subTree* NewTree(TString description="default") {
    //    TString desc=description;
    TFile* tfile=0;
    string dir="";
    //NewTree(description,tfile,dir);
    return subTreeFactory::NewTree(description,tfile,dir);
  }

  static subTree* NewTree(TString description, TFile* tfile, TString dir)
    {
      if(description == "default") {
	std::cout<<"default subTree will be filled."<<std::endl;
	if (tfile!=0){
	  //std::cout<<"CALLING NEW TREE WITH PARAMETERS "<<std::endl;
	  return new defaultTree(tfile, dir);
	}else{
	  //	  std::cout<<"CALLING NEW TREE WITH NO PARAMETERS "<<std::endl;
	  return new defaultTree;	
	}
      }
      else if(description == "Nminus1") {
 	std::cout<<"Nminus1 subTree will be filled."<<std::endl;
	return new Nminus1Tree;
      }
      else{
	std::cout<<"No matching for subTreeType: defaultTree will be used"<<std::endl;
      }
    }
};

#endif
