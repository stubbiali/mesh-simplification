#ifndef DOWNLOADMESH_H_
#define DOWNLOADMESH_H_

#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map> 
#include <cmath>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/graphItem.h"

#include "../geometry/geoElement.hpp"
#include "../geometry/mesh0d.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/mesh3d.hpp"

#include "../doctor/meshHandler.hpp"

namespace geometry
{

using namespace std;

/*! Classe che implementa una serie di metodi che permettono la visualizzazione delle mesh in formato:

    <ol>
    <li> formato gts;
    <li> paraview;
    <li> TSurf di gOcad;
    <li> netgen.
    </ol> 
*/

class downloadMesh
{
	  //
	  // Variabili utilizzate
	  //
	  public:  
		  /*! Oggetto fstream che verrà utilizzato per downlodare */
		  fstream file;
	  //
	  // Costruttore di default
	  //
	  public: 
		  /*! Costruttore di default */
		  downloadMesh();
	  //
	  // Metodi 
	  //
	  public:
		  // ---------------------
		  // download da PARAVIEW
		  // ---------------------
		  /*! Download una mesh di paraview 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromParaview(string s, mesh0d<simplePoint> * mesh);
		  
		  /*! Download una mesh di paraview 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromParaview(string s, mesh1d<Line> * mesh);
		  
		  /*! Download una mesh di paraview 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromParaview(string s, mesh2d<Triangle> * mesh);
		  
		  /*! Download una mesh di paraview 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromParaview(string s, mesh3d<Tetra> * mesh);
		  
		  /*! Download una mesh di paraview 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromParaviewNodePropriety(string s, mesh2d<Triangle> * mesh, vector<Real> * ris);
		  
		  // ----------------
		  //  download .off
		  // ----------------
		  /*! Download una mesh nel formato .off
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromOffFormat(string s, mesh2d<Triangle> * mesh);
		  
		  // ---------------------
		  //  download da NETGEN
		  // ---------------------
		  /*! Download da un file in formato neutrale di netgen estrae SOLO la mesh di superficie 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromNetgen(string s, mesh2d<Triangle> * mesh);
		  
		  /*! Download da un file in formato neutrale di netgen estrae SOLO la mesh di superficie con quel geoId 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromNetgen(string s, mesh2d<Triangle> * mesh, UInt geoId);
		  
		  /*! Download da un file in formato neutrale di netgen estrae SOLO la mesh di superficie 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromNetgen(string s, mesh3d<Tetra> * mesh);
		  
		  /*! Download da un file in formato neutrale di netgen estrae SOLO la mesh di superficie con quel geoId 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromNetgen(string s, mesh3d<Tetra> * mesh, UInt geoId);
		  
		  // ------------------
		  //  download da gOcad
		  // ------------------
		  
		  /*! Download di un file in formato gOcad 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromGocad(string s, mesh2d<Triangle> * mesh);
		  
		  // ------------------
		  //  download da medit
		  // ------------------
		  
		  /*! Download di un file in formato medit (.mesh) 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromMedit(string s, mesh2d<Triangle> * mesh);
		  
		  /*! Download di un file in formato medit (.mesh) 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromMedit(string s, vector<UInt> * flagPt, mesh2d<Triangle> * surf, mesh3d<Tetra> * vol);
		  
		  // ----------------
		  //  download da VTK
		  // ----------------
		  
		  /*! Download di un file in formato vtk (.vtk) 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni */
		  void fileFromVTK(string s, mesh2d<Triangle> * mesh);		  
		  		  
		  // -------------------
		  //  download da TXT
		  // -------------------
		  /*! Download di un file che contiene solo le coordinate x,y e z dei punti 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni 
		      ritorna 1 se non ha trovato il file 0 se tutto è andato per il meglio */
		  void fileNodes(string s, mesh0d<simplePoint> * mesh);
		  
		  // --------------------
		  //  download da freeFem
		  // --------------------
		  
		  /*! Download di un file in formato medit (.msh) 
		      \param s stringa che identifica il file 
		      \param mesh oggetto mesh in cui ricopiare le informazioni 
		      N.B. si presuppone che sia una mesh piana la z è settata a 0*/
		  void fileFromPlaneMSH(string s, mesh2d<Triangle> * mesh);

};

}

#endif
