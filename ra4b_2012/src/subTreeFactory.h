#ifndef subTreeFactory_h
#define subTreeFactory_h

#include "TString.h" 
#include "subTree.h"
#include "defaultTree.h"
#include "Nminus1Tree.h"

#include <iostream>

class subTreeFactory
{
 public:
  
  static subTree *NewTree(const char* description="default") {
    TString desc=description;
    return subTreeFactory::NewTree(desc);
  }
  
  static subTree *NewTree(TString &description)
    {
      if(description == "default") {
	std::cout<<"default subTree will be filled."<<std::endl;
	return new defaultTree;
      }
      if(description == "Nminus1") {
 	std::cout<<"Nminus1 subTree will be filled."<<std::endl;
	return new Nminus1Tree;
      }
      std::cout<<"No matching for subTreeType: defaultTree will be used"<<std::endl;
      return new defaultTree;
    }
 };

#endif
