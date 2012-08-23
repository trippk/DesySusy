#ifndef subTreeFactory_h
#define subTreeFactory_h

#include "TString.h" 
#include "subTree.h"
#include "defaultTree.h"

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
      if(description == "default") return new defaultTree;
      std::cout<<"No matching for subTreeType: defaultTree will be used"<<std::endl;
      return new defaultTree;
    }
 };

#endif
