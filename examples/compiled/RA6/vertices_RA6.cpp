#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"
#include "../tools/THTools.h"
#include "TDirectory.h"

using namespace std;

bool vertices_RA6(EasyChain* tree, vector<unsigned>& selVx, CutSet& selCut) {


  ConfigReader config;
  static bool  quick           = config.getBool("quick",false);
  static bool  isOldNtuple     = config.getBool("isOldNtuple",false);

  typedef XYZPointF XYZp;
  typedef float df;
  //if(isOldNtuple) {
  //typedef XYZPointD XYZp;
  //typedef double df;
  //}

  vector<XYZp>& Vertices       = tree->Get( &Vertices,      "vertexPosition");

  for( unsigned vx=0; vx < Vertices.size(); ++vx ) {
    vector<int>&       Vertex_isFake  = tree->Get( &Vertex_isFake, "vertexIsFake"  );

    if( !selCut.keepIf( "IsNoFake", !Vertex_isFake.at(vx)             ) && quick ) continue;
    if( !selCut.keepIf( "Z<24"    , fabs(Vertices.at(vx).z())   < 24. ) && quick ) continue; // MC:15, Data:24
    if( !selCut.keepIf( "Rho<2"   , fabs(Vertices.at(vx).rho()) <  2. ) && quick ) continue;
    
    vector<df>&    Vertex_ndof    = tree->Get( &Vertex_ndof,   "vertexNdof"    );
    if( !selCut.keepIf( "Ndof>4"  , Vertex_ndof.at(vx)          >  4  ) && quick ) continue;
    
    if( quick || selCut.applyCuts("RA6 Vertex selection","IsNoFake Z<24 Rho<2 Ndof>4") )
      selVx.push_back(vx);

  }

    
}
