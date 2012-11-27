#ifndef EventInfo_h
#define EventInfo_h

struct EventInfo{

  int Event;
  int Run;

  float xs;
  int   NEvents;
  float FE; 
  float GlobalWeight;
  float PUWeight;
  float EventWeight;

  float mY;
  float mLsp;
};

#endif
