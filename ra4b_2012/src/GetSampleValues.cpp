//#include "TSystem.h"
//#include "Math/VectorUtil.h"
//#include "NtupleTools2_h.h"
// #include "NtupleTools2.h"
 #include "ConfigReader.h"
 #include <iostream>
// #include "../tools/THTools.h"
// #include "../tools/eventselection.h"
using namespace std;
ConfigReader config;

double GetFilterEfficiency(string WhatSample, string WhatSubSample){
  double FilterEfficiency=0.0;
  
  FilterEfficiency=config.getDouble("FE_"+WhatSample+"_"+WhatSubSample);
  return FilterEfficiency;
}


double GetCrossSection(string WhatSample, string WhatSubSample){
  double CrossSection=0.0;
  cout<<"xs_"+WhatSample+"_"+WhatSubSample<<endl;
  CrossSection=config.getDouble("xs_"+WhatSample+"_"+WhatSubSample);
    //=======================================
  return CrossSection;
}



long GetTotalNumberOfEntries(string WhatSample, string WhatSubSample){
  cout<<"----------------------------------------------------------------------"<<endl;
  cout<<"GetTotaNumberOfEntries has been called with the values "<<WhatSample<<" and "<<WhatSubSample<<endl;
  cout<<"----------------------------------------------------------------------"<<endl;
  cout<<"TNoE_"+WhatSample+"_"+WhatSubSample<<endl;
  long TNOE=1;   
  TNOE=config.getLong("TNoE_"+WhatSample+"_"+WhatSubSample);
  return TNOE;
}





