#include "createFile.h"

using namespace geometry;

//
// Costruttore di default
//

createFile::createFile()
{
      toll = 1e-07;
}

//
// get e set di toll
//

Real createFile::getToll()
{
      return(toll);
}
		  
void createFile::setToll(Real _toll)
{
      toll = _toll;
}
	  
//
// Metodi per creare i file per Paraview
//

// ------------------------------
//           MESH0D
// ------------------------------
void createFile::fileForParaview(string s, mesh0d<simplePoint> * mesh)
{
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements()  << " 0 0 0" << endl;

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;
		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "pt ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << endl;
	}
	
	out.close();
}

// ------------------------------
//           MESH1D
// ------------------------------
void createFile::fileForParaview(string s, mesh1d<Line> * mesh)
{
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 0 0 0" << endl;

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;
		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "line ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << endl;
	}
	
	out.close();
}

void createFile::fileForParaviewNodePropriety(string s, mesh1d<Line> * mesh, vector<Real> * prop)
{
	// creo il file
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 1 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;
		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "line ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 <<endl;
	}
	
	// prorpietà
	out << "1 1" << endl;
	out << "none, adim" << endl;
	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << "  " << prop->at(i) << endl;
	}
} 

void createFile::fileForParaviewElementPropriety(string s, mesh1d<Line> * mesh, vector<Real>  * prop)
{	
	// creo il file
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 0 1 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;

		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "line ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 <<endl;
	}
	
	// prorpietà
	out << "1 1" << endl;
	out << "prop, null" << endl;
	for(UInt i=0; i<mesh->getNumElements(); ++i)	out << i+1 << "  " << prop->at(i) << endl;
	
}

// ------------------------------
//           MESH2D
// ------------------------------
void createFile::fileForParaview(string s, mesh2d<Triangle> * mesh)
{
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 0 0 0" << endl;	
	
	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	out.precision(16);

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;
		
		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << " ";
		out << "tri ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << " ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << " ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << endl;
	}
}

void createFile::fileForParaview(string s, mesh2d<Quad> * mesh)
{
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 0 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	out.precision(16);

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (mesh->getNode(i)).getX() << " ";
// 		out << (mesh->getNode(i)).getY() << " ";
// 		out << (mesh->getNode(i)).getZ() << endl;
		
		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "quad ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(3)+1 << endl;
	}
}

void createFile::fileForParaviewNodePropriety(string s, mesh2d<Triangle> * mesh, vector<Real> * prop)
{
	// creo il file
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 1 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;
		
		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
  
	}
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "tri ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << endl;
	}
	
	// prorpietà
	out << "1 1" << endl;
	out << "none, adim" << endl;
	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << "  " << prop->at(i) << endl;
	}
} 

void createFile::fileForParaviewElementPropriety(string s, mesh2d<Triangle> * mesh, vector<Real>  * prop)
{	
	// creo il file
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 0 1 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;

		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "tri ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << endl;
	}
	
	// prorpietà
	out << "1 1" << endl;
	out << "prop, null" << endl;
	for(UInt i=0; i<mesh->getNumElements(); ++i)	out << i+1 << "  " << prop->at(i) << endl;
	
}

void createFile::fileForParaviewNodePropriety(string s, mesh2d<Triangle> * mesh, vector<point> * prop)
{
	// creo il file
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 3 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;

		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "tri ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << endl;
	}
	
	// prorpietà
	out << "1 3" << endl;
	out << "none, adim" << endl;
	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << "  " << prop->at(i).getX() << "  " << prop->at(i).getY() << "  " << prop->at(i).getZ() << endl;
	}
}

// ------------------------------
//           MESH3D
// ------------------------------

void createFile::fileForParaview(string s, mesh3d<Tetra> * mesh)
{
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 0 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;

		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "tet ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(3)+1 <<endl;
	}
}

void createFile::fileForParaview(string s, mesh3d<Hexa> * mesh)
{
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 0 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;

		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}	
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "hex ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(3)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(4)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(5)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(6)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(7)+1 << endl;
	}
}

void createFile::fileForParaviewNodePropriety(string s, mesh3d<Tetra> * mesh, vector<point> * prop)
{
	// creo il file
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 3 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;

		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "tet ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(3)+1 << endl;
	}
	
	// prorpietà
	out << "1 3" << endl;
	out << "none, adim" << endl;
	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << "  " << prop->at(i).getX() << "  " << prop->at(i).getY() << "  " << prop->at(i).getZ() << endl;
	}
}

void createFile::fileForParaviewNodePropriety(string s, mesh3d<Tetra> * mesh, vector<Real> * prop)
{
	// creo il file
	ofstream out(s.c_str());
		
	out << mesh->getNumNodes() << " " << mesh->getNumElements() << " 1 0 0" << endl;	

	// N.B. il fattore 1.00001 è stato introdotto per far funzionare meglio paraview
	//      per il problema dei boundingbox

	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getX() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getY() << " ";
// 		out << (1.00001)*(mesh->getNode(i)).getZ() << endl;

		out << (mesh->getNode(i)).getX() << " ";
		out << (mesh->getNode(i)).getY() << " ";
		out << (mesh->getNode(i)).getZ() << endl;
	}
	
	// passo alle facce
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << i+1 << " ";
		out << (mesh->getElement(i)).getGeoId() << "  ";
		out << "tet ";
		out << (mesh->getElement(i)).getConnectedId(0)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(1)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(2)+1 << "  ";
		out << (mesh->getElement(i)).getConnectedId(3)+1 << endl;
	}
	
	// prorpietà
	out << "1 1" << endl;
	out << "none, adim" << endl;
	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
		out << i+1 << "  " << prop->at(i) << endl;
	}
} 

//
// file per medit
//
void createFile::fileForMedit(string s, mesh3d<Tetra> * mesh, mesh2d<Triangle> * surf)
{
	ofstream out(s.c_str());
		
	out << "MeshVersionFormatted 1" << endl;
	out << endl;
	out << "Dimension 3" << endl;
	out << endl;
	out << "Vertices" << endl;
	out << mesh->getNumNodes() << endl;
	
	for(UInt i=0; i<mesh->getNumNodes(); ++i)
	{
	    	out << mesh->getNode(i).getX() << " ";
		out << mesh->getNode(i).getY() << " ";
		out << mesh->getNode(i).getZ() << " ";
		out << mesh->getNode(i).getBoundary() << endl;
	}
	
	out << endl;
	out << "Tetrahedra" << endl;
	out << mesh->getNumElements() << endl;
	for(UInt i=0; i<mesh->getNumElements(); ++i)
	{
		out << mesh->getElement(i).getConnectedId(0)+1 << "  ";
		out << mesh->getElement(i).getConnectedId(1)+1 << "  ";
		out << mesh->getElement(i).getConnectedId(2)+1 << "  ";
		out << mesh->getElement(i).getConnectedId(3)+1 << "  ";
		out << mesh->getElement(i).getGeoId() << endl;
	}
	
	out << endl;
	out << "Triangles" << endl;
	out << surf->getNumElements() << endl;
	for(UInt i=0; i<surf->getNumElements(); ++i)
	{
		out << surf->getElement(i).getConnectedId(0)+1 << "  ";
		out << surf->getElement(i).getConnectedId(1)+1 << "  ";
		out << surf->getElement(i).getConnectedId(2)+1 << "  ";
		out << surf->getElement(i).getGeoId() << endl;
	}

}

//
// file .off
//
void createFile::fileOFFFormat(string s, mesh2d<Triangle> * surf)
{
	ofstream out(s.c_str());
	out << "OFF" << endl;
	out << surf->getNumNodes() << "  " << surf->getNumElements() << " 0" << endl;
	
	// stampo tutti i nodi 
	for(UInt i=0; i<surf->getNumNodes(); ++i)
	{
	    out << surf->getNode(i).getX() << "  ";
	    out << surf->getNode(i).getY() << "  ";
	    out << surf->getNode(i).getZ() << endl;
	}
	
	// stampo gli elementi 
	for(UInt i=0; i<surf->getNumElements(); ++i)
	{
	    out << "3 ";
	    out << surf->getElement(i).getConnectedId(0) << "  ";
	    out << surf->getElement(i).getConnectedId(1) << "  ";
	    out << surf->getElement(i).getConnectedId(2) << endl;
	}
	
	// chiudo 
	out.close();
	
}
