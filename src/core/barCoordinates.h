#ifndef BARCOORDINATES_H_
#define BARCOORDINATES_H_

#include <cassert>	
#include <iostream>
#include <numeric>	
#include <vector>
#include <cmath>

#include "../core/shapes.hpp"
#include "../core/point.h"

namespace geometry
{

using namespace std;

/*! Classe che implementa il calcolo delle coordinate baricentriche di un triangolo, una linea e un tetraedro, la classe è molto
semplice ed è stata implementa per poter rendere questa funzionalità portabili in TUTTE le classi della libreria */

class barCoordinates
{
	      //
	      // Variabili
	      //
	      public:		  
		      /*! Tolleranza*/
		      Real toll;
	      //
	      // Costruttore 
	      //
	      public:
		      /*! Costruttore vuoto */
		      barCoordinates();
	      
	      //
	      // Metodi che creano le coordinate baricentriche 
	      //	
	      public:
		      /*! Metodo che restituisce le coordinate barcentriche di un segmento
			  \param vertices puntatore a in vettore di nodi che contiene le coordinate del punto 
			  \param p punto di cui si vogliono stabilire le coordinate
			  \param bar vettore che conterrà le coordinate baricentriche */
		      void ofLine(vector<point> * vertices, point p, vector<Real> * bar);
		      
		      /*! Metodo che restituisce le coordinate barcentriche di un triangolo
			  \param vertices puntatore a in vettore di nodi che contiene le coordinate del punto 
			  \param p punto di cui si vogliono stabilire le coordinate
			  \param bar vettore che conterrà le coordinate baricentriche */
		      void ofTriangle(vector<point> * vertices, point p, vector<Real> * bar);
		      
		      /*! Metodo che restituisce le coordinate barcentriche di un tetraedro
			  \param vertices puntatore a in vettore di nodi che contiene le coordinate del punto 
			  \param p punto di cui si vogliono stabilire le coordinate
			  \param bar vettore che conterrà le coordinate baricentriche */
		      void ofTetra(vector<point> * vertices, point p, vector<Real> * bar);
		      
		      /*! Metodo che restutisce un punto con le coordinate baricentriche in input 
			  \param vertices puntatore a in vettore di nodi che contiene le coordinate del punto 
			  \param bar vettore con le coordinate baricentriche */
		      point pointOfLine(vector<point> * vertices, vector<Real> * bar);
		      
		      /*! Metodo che restutisce un punto con le coordinate baricentriche in input 
			  \param vertices puntatore a in vettore di nodi che contiene le coordinate del punto 
			  \param bar vettore con le coordinate baricentriche */
		      point pointOfTriangle(vector<point> * vertices, vector<Real> * bar);
		      
		      /*! Metodo che restutisce un punto con le coordinate baricentriche in input 
			  \param vertices puntatore a in vettore di nodi che contiene le coordinate del punto 
			  \param bar vettore con le coordinate baricentriche */
		      point pointOfTetra(vector<point> * vertices, vector<Real> * bar);
		      
		      /*! Metodo per stabilire se il punto è sul vertice, sull'edge, sulla faccia o nel volume
			  \param bar vettore con le coordinate baricentriche */
		      ReferenceGeometry whereIsNode(vector<Real> * bar);
		      
	      //
	      // Metodi per toll
	      //
	      public:
		      /*! Set della tolleranza 
			  \param _toll valore della tolleranza*/
		      void setToll(Real _toll);
		      
		      /*! Get della tolleranza */
		      Real getToll();
};






}

#endif
