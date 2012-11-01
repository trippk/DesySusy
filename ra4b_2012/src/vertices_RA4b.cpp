#include "NtupleTools2_h.h"
#include "ConfigReader.h"
#include "CutFlow.h"
#include "THTools.h"
#include "eventselection.h"

// incomplete !!!
//      keepIf("FracOfHPtracks", Ntracks <=10 ? true : (((double) NHPtracks/Ntracks)>=0.25) );

bool vertices_RA4b(EasyChain* tree, vector<int>& goodVert){

  ConfigReader config;
  bool isData=config.getBool("isData");

  float vertexZ_mc = config.getFloat("VertexZ_MC", 24.0);

  extern bool pcp;


  //	static TH1F* num_vert    = fetchHisto<TH1F>("num_vert") ;
  if(pcp)cout<<"================INSIDE THE VERTICES PROGRAM===================="<<endl;

  if(pcp)cout<<"getting the Vertices from the tree"<<endl;

  vector<XYZPointF>& Vertices       = tree->Get( &Vertices,      "vertexPosition");
  if( Vertices.size() == 0 ) return false;

  vector<int>&       Vertex_isFake  = tree->Get( &Vertex_isFake, "vertexIsFake"  );
  vector<float>&     Vertex_ndof    = tree->Get( &Vertex_ndof,   "vertexNdof"    );
  vector<int>&       Vertex_nTracks = tree->Get( &Vertex_nTracks,"vertexNtrks"   );

  if(pcp)cout<<"loop over the vertices"<<endl;
  for( unsigned vx=0; vx < Vertices.size(); ++vx ) {

    if( Vertex_isFake[vx] ) continue;

    if( isData ){
      // data
      if( fabs(Vertices[vx].z()) > 24 ) continue;
      if( fabs(Vertices[vx].rho()) > 2 ) continue;
      if( Vertex_ndof[vx] <= 4 )         continue;
      goodVert.push_back(vx);
    } else {
      // MC
      if( fabs(Vertices[vx].z()) > vertexZ_mc ) continue;
      if( Vertices[vx].rho() > 2 )       continue;
      if( Vertex_ndof[vx] <= 4 )         continue;


      goodVert.push_back(vx);
    }
  }

	
  return goodVert.size()>0;

}
