#ifndef makeTracks_h
#define makeTracks_h

#include "NtupleTools2_h.h"
#include "francescoParticle.h"
#include "Particle.h"
vector<fParticle> makeAllTracks(EasyChain* tree);
void IsoTracksV4 ( vector<fParticle>& allTracks);
bool IsoTrackVetoV4 ( fParticle lepton, vector<fParticle>& allTracks);
bool IsoTrackVetoV4 ( Particle* lepton, vector<fParticle>& allTracks);
#endif
