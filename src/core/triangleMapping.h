#ifndef TRIANGLEMAPPING_H_
#define TRIANGLEMAPPING_H_

#include <cassert>	
#include <iostream>
#include <numeric>	
#include <vector>
#include <cmath>

#include "../core/shapes.hpp"
#include "../core/point.h"
#include "../core/tensor.h"

namespace geometry
{

using namespace std;

/*! Classe che implementa le trasformazioni base con triangoli
    - mappa da un triangolo 3d a uno isometrico sul piano 
    - calcolo del circocentro sul piano 
    - mappa fra due triangoli sul piano 
    Questa classe non contriene 
*/

class triangleMapping
{
      //
      // costruttore 
      //
      public:
	      /*! Costruttore vuoto che non fa nulla */
	      triangleMapping();	
      // 
      // Metodi che creano i triangoli
      // 
      public:
	      /*! Metodo che costruisce i nodi del triangolo con determinate lunghezze dei lati e angoli 
		  \param edgesLen lunghezza degli edge 
		  \param angles angoli opposit agli edge->at(i) DEVONO ESSERE IN GRADI!!
		  \param nodes vettore che conterrà i nodi 
	      N.B. il triangolo avrà il primo nodo nell'origine */
	      void createTriangle(vector<Real> * edgesLen, vector<Real> * angles, vector<point> * nodes);	  
      //
      // Metodi che legano un triangolo in 3d con uno in 2d
      //
      public:
	      /*! Metodo che fa la mappa isometrica 3d->2d
		  \param nodes vettore che contiene i vertici del triangolo in 3d
		  \param map2dTo3d vettore che conterrà la mappa */
	      void create2dTo3dMapIso(vector<point> * nodes, vector<Real> * map2dTo3d);
	      
	      /*! Metodo che crea la mappa isometrica inversa 2d->3d 
		  \param nodes vettore che contiene i vertici del triangolo in 3d 
		  \param map3dTo2d vettore che conterrà la mappa 
	      N.B. il triangolo è lo stesso del metodo precedente ma la mappa risultate sarà l'inversa*/
	      void create3dTo2dMapIso(vector<point> * nodes, vector<Real> * map3dTo2d);
	      
	      /*! Metodo che crea la mappa isometrica inversa 2d->3d e 3d->2d 
		  \param nodes vettore che contiene i vertici del triangolo in 3d 
		  \param map3dTo2d vettore che conterrà la mappa 3d->2d 
		  \param map2dTo3d vettore che conterrà la mappa 2d->3d*/
	      void createBothMapIso(vector<point> * nodes, vector<Real> * map3dTo2d, vector<Real> * map2dTo3d);
	      
	      /*! Metodo che applica la mappa 3d->2d 
		  \param pt punto su cu applicare la mappa 
		  \param map3dTo2d mappa da applicare */
	      point applyMap3dTo2d(point p, vector<Real> * map3dTo2d);
	      
	      /*! Metodo che applica la mappa 2d->3d 
		  \param pt punto su cu applicare la mappa 
		  \param map2dTo3d mappa da applicare */
	      point applyMap2dTo3d(point p, vector<Real> * map2dTo3d); 
      //
      // Metodi per fare le mappe fra due triangoli in 2d
      //
      public:
	      /*! Metodo per fare la mappa fra due triangoli in 2d 
		  \param nodes1 coordinate del primo   triangolo 
		  \param nodes2 coordinate del secondo triangolo 
		  \param tria1ToTria2 mappa per passare da tria1 a tria2 */
	      void createTria1ToTria2Map2d(vector<point> * nodes1, vector<point> * nodes2, vector<Real> * tria1ToTria2);
	      
	      /*! Metodo che applica la mappa 
		  \param pt punto su cui applicare la mappa 
		  \param tria1ToTria2 mappa da applicare */
	      point applyMap2dTo2d(point pt, vector<Real> * tria1ToTria2);
      //
      // Metodo per calcolare quantità utili per triangoli in 2d
      //
      public:
	      /*! Metodo che permette di calcolare il circocentro di un triangolo dato in input 
		  N.B. tutto è fatto in 2d!
		  \param nodes nodi che definiscono il triangolo */
	      point getCircumcenter(vector<point> * nodes);
	      
	      /*! Metodo che permette di calcolare il circocentro di un triangolo nello spazio 3d 
		  N.B. il circocentro sarà sul piano identificato dal triangolo 
		  \param nodes nodi che definiscono il triangolo */
	      point getCircumcenterOnTriaPlane(vector<point> * nodes);
};












}



#endif
