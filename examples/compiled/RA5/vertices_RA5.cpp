#include "NtupleTools2_h.h"
#include "../tools/ConfigReader.h"
#include "../tools/CutFlow.h"

// incomplete !!!
//      keepIf("FracOfHPtracks", Ntracks <=10 ? true : (((double) NHPtracks/Ntracks)>=0.25) );

bool vertices_RA5(EasyChain* tree, vector<int>& goodVert){

	ConfigReader config;
	bool isData=config.getBool("isData");


	vector<XYZPointF>& Vertices       = tree->Get( &Vertices,      "vertexPosition");
	if( Vertices.size() == 0 ) return false;

	vector<int>&       Vertex_isFake  = tree->Get( &Vertex_isFake, "vertexIsFake"  );
	vector<float>&     Vertex_ndof    = tree->Get( &Vertex_ndof,   "vertexNdof"    );
	vector<int>&       Vertex_nTracks = tree->Get( &Vertex_nTracks,"vertexNtrks"   );

        for( unsigned vx=0; vx < Vertices.size(); ++vx ) {

		if( Vertex_isFake[vx] ) continue;

		if( isData ){
			// data
			if( fabs(Vertices[vx].z()) >= 24 ) continue;
			goodVert.push_back(vx);
		} else {
			// MC
			if( fabs(Vertices[vx].z()) >= 15 ) continue;
			if( Vertices[vx].rho() > 2 )       continue;
			if( Vertex_ndof[vx] <= 4 )         continue;
			goodVert.push_back(vx);
		}
	}
	return goodVert.size() >= 1 ;

}
