#ifndef CREATEFILE_H_
#define CREATEFILE_H_

#include <cassert>
#include <iostream>
#include <vector>
#include <fstream>

#include "../geometry/mesh0d.hpp"
#include "../geometry/mesh1d.hpp"
#include "../geometry/mesh2d.hpp"
#include "../geometry/mesh3d.hpp"

namespace geometry
{

using namespace std;

/*! Classe che implementa una serie di metodi che permettono la visualizzazione delle mesh in formato:

    <ol>
    <li> paraview;
    <li> medit;
    </ol> -
*/

class createFile
{
	  //
	  // Variabili utilizzate
	  //
	  public:
		  /*! Tolleranza */
		  Real toll;
	  //
	  // Costruttore di default
	  //
	  public: 
		  /*! Costruttore di default */
		  createFile();
	  //
	  // get e set di toll
	  //
	  public: 
		  /*! get */
		  Real getToll();
		  
		  /*! set */
		  void setToll(Real _toll);
	  //
	  // Metodi per creare i file per Paraview
	  //
	  public:		  
		  // ------------------------------
		  //           MESH0D
		  // ------------------------------
		  /*! File per creare un file compatibile per una mesh0d 
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia */
		  void fileForParaview(string s, mesh0d<simplePoint> * mesh);
		  
		  // ------------------------------
		  //           MESH1D
		  // ------------------------------
		  /*! File per creare un file compatibile per una mesh1d 
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia */
		  void fileForParaview(string s, mesh1d<Line> * mesh);
		  
		  /*! File per creare un file compatibile per una mesh1d con la prorpietà dei nodi data da un vettore
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param prop proprietà legata al punto */
		  void fileForParaviewNodePropriety(string s, mesh1d<Line> * mesh, vector<Real> * prop);
		  
		  /*! File per creare un file compatibile per una mesh1d con la prorpietà degli elementi data da un vettore
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param prop proprietà legata all'elemento */
		  void fileForParaviewElementPropriety(string s, mesh1d<Line> * mesh, vector<Real> * prop);
		  		  
		  // ------------------------------
		  //           MESH2D
		  // ------------------------------
		  /*! File per creare un file compatibile per una mesh2d 
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia */
		  void fileForParaview(string s, mesh2d<Triangle> * mesh);
		  
		  /*! File per creare un file compatibile per una mesh2d 
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia */
		  void fileForParaview(string s, mesh2d<Quad> * mesh);
		  
		  /*! File per creare un file compatibile per una mesh2d con la prorpietà dei nodi data da un vettore
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param prop proprietà legata al punto */
		  void fileForParaviewNodePropriety(string s, mesh2d<Triangle> * mesh, vector<Real> * prop);
		  
		  /*! File per creare un file compatibile per una mesh2d con la prorpietà dei nodi data da un vettore
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param prop proprietà legata al punto */
		  void fileForParaviewNodePropriety(string s, mesh2d<Triangle> * mesh, vector<point> * prop);
		  
		  /*! File per creare un file compatibile per una mesh2d con la prorpietà degli elementi data da un vettore
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param prop proprietà legata all'elemento */
		  void fileForParaviewElementPropriety(string s, mesh2d<Triangle> * mesh, vector<Real> * prop);
		    
		  // ------------------------------
		  //           MESH3D
		  // ------------------------------		  		  
		  /*! File per creare un file compatibile per una mesh3d 
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia */
		  void fileForParaview(string s, mesh3d<Tetra> * mesh);
		  
		  /*! File per creare un file compatibile per una mesh3d 
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia */
		  void fileForParaview(string s, mesh3d<Hexa> * mesh);
		  
		  /*! File per creare un file compatibile per una mesh3d con la prorpietà dei nodi data da un vettore
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param prop proprietà legata al punto */
		  void fileForParaviewNodePropriety(string s, mesh3d<Tetra> * mesh, vector<point> * prop);
		  
		  /*! File per creare un file compatibile per una mesh3d con la prorpietà dei nodi data da un vettore
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param prop proprietà legata al punto */
		  void fileForParaviewNodePropriety(string s, mesh3d<Tetra> * mesh, vector<Real> * prop);
	//
	// file per medit
	//
	public:
		  /*! File per creare un file compatibile per una mesh3d 
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   \param surf puntatore alla griglia che definisce il bordo del volume 
		   N.B. devono avere la stessa numerazione */
		  void fileForMedit(string s, mesh3d<Tetra> * mesh, mesh2d<Triangle> * surf);
	//
	// file .off
	//
	public:
		  /*! File per creare un file OFF
		   \param s stringa che contiene l'indirizzo del file
		   \param mesh puntatore alla griglia 
		   N.B. devono avere la stessa numerazione */
		  void fileOFFFormat(string s, mesh2d<Triangle> * surf);
};

}

#endif

