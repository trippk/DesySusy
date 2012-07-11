#ifndef SampleInformation_h
#define SampleInformation_h
#include <fstream>

class SampleInformation{

  ////////===================================
  ////////  Information about the sample in which
  ////////  the program runs
  ////////  It requires a file to read the information
  ////////  which should be provided by the external
  ////////  running script
  ////////====================================


 private:

  string WhatSample;
  string WhatSubSample;
  string WhatEstimation;
  string WhatTail;

 public:

  string GetSample(){
    return WhatSample;
  }
  string GetSubSample(){
    return WhatSubSample;
  }
  string GetEstimation(){
    return WhatEstimation;
  }
  string GetTail(){
    return WhatTail;
  }

  void SetSample(string sample_in){
    WhatSample=sample_in;
  }
  void SetSubSample(string subsample_in){
    WhatSubSample=subsample_in;
  }
  void SetEstimation(string estimation_in){
    WhatEstimation=estimation_in;
  }
  void SetTail(string tail_in){
    WhatTail=tail_in;
  }


  //=======================================
  //SET THE INFORMATION 
  //=======================================
  void SetInformation(string sample_in,string subsample_in, string estimation_in, string tail_in){
    SetSample(sample_in);
    SetSubSample(subsample_in);
    SetEstimation(estimation_in);
    SetTail(tail_in);
  }


  //=========================================
  // READ THE INFORMATION FROM A FILE
  //=========================================
  void ReadInformation(const char* infile){
    
    string whatsample;
    string whatsubsample;
    string whatestimation;
    string whattail;

    ifstream myfile;
    myfile.open(infile);
    if(myfile.is_open()){
      getline(myfile,whatsample);
      getline(myfile,whatsubsample);
      getline(myfile,whatestimation);
      getline(myfile,whattail);
      //myfile>>whatsample;
      //myfile>>whatsubsample;
      //myfile>>whatestimation;
      //myfile>>whattail;
      //Now Set the values
      SetInformation(whatsample,whatsubsample,whatestimation,whattail);
    }
    else{
      cout <<"No information about the sample. Cannot continue"<<endl;
      exit(1);
    }
  }


  void DumpInformation(){
    cout<<"==================================================="<<endl;
    cout<<"RUNNING OVER THE SAMPLE        "<<WhatSample<<endl;
    cout<<"AND THE SUBSAMPLE              "<<WhatSubSample<<endl;
    cout<<"==================================================="<<endl;
    
    cout<<"==================================================="<<endl;
    cout<<" THE ESTIMATION IS             "<<WhatEstimation      <<endl;
    cout<<"==================================================="<<endl;
    
    cout<<"==================================================="<<endl;
    cout<<" THE TAIL IS                   "<<WhatTail<<endl;
    cout<<"==================================================="<<endl;
  }

  

};


#endif
