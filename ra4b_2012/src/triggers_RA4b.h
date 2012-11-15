#ifndef triggers_RA4b_h
#define triggers_RA4b_h
bool triggers_RA4b(EasyChain* tree, vector<const char*>&, double&);
bool triggersFired_RA4b(EasyChain* tree, std::map<std::string, bool>&);
bool isMuEGPlateau(const std::map<std::string, bool>& trigsFiredMap, double MuPt, double ElPt);
#endif
