#include "simplification2dCostFunctionBased.h"

namespace geometry
{
    
simplification2dCostFunctionBased::simplification2dCostFunctionBased(costFunction * _costFunctionPointer,
                                                                     mesh2d<Triangle> * _meshPointer,
                                                                     vector<UInt> & _pointMaterialId) 
  : doctor2d<Triangle>(_meshPointer), 
    costFunctionPointer(_costFunctionPointer),
    pointMaterialId(_pointMaterialId)
{
    // set the trick pointer 
    costFunctionPointer->setTrickyClassPointer(this);
}

//
// Method to run the simplification 
//
void simplification2dCostFunctionBased::simplificateGreedy(UInt numNodesMax)
{
    // variabili in uso
    UInt counter=0;
    UInt numNode=meshPointer->getNumNodes();
    UInt numNodeStart=meshPointer->getNumNodes();
    time_t start,end;
    Real dif;
    sortList<geoElementSize<simplePoint> > listOfPossiblePointRemoval;

    // check if I already reached the limit on the point 
    if(numNodesMax>=meshPointer->getNumNodes())
    {
        cout << "The mesh has " << meshPointer->getNumNodes();
        cout << " which is under the limit " << numNodesMax << endl;
        return;
    }

    // print
    cout << "Greedy Simplification process..." << endl;
    time(&start);

    // build the list 
    createElementList(listOfPossiblePointRemoval);
    
    // while loop to remove points 
    while(numNode>numNodesMax && counter<numNodeStart)
    {
        // get the id
        UInt pointId = getPointToRemoveAndTakeOffItFromTheList(listOfPossiblePointRemoval);
        
        // check 
        if(pointId==meshPointer->getNumNodes())
            break;
        
        // get the edge to remove and the point where I contract the edge 
        std::vector<UInt> edge;
        point newCoordinates;
        costFunctionPointer->getEdgeToRemove(pointId, edge, newCoordinates);
        
        // check if I can contract the edge 
        if(control(&edge, newCoordinates))
        {
            // add the new point 
            meshPointer->insertNode(newCoordinates);
            UInt newAddedPointId = meshPointer->getNumNodes()-1;
            
            // contract the edge 
            collEdge(&edge);
            
            // method to fill the material Id 
            fillTheMaterialIdAfterContraction(pointId, edge);
            
            // update the list 
            updateAndAddElementList(listOfPossiblePointRemoval, newAddedPointId);
            
            // scale numNode
            numNode--;
        }
        
        // incremento counter 
        ++counter;
            
        cout << numNode << " nodes over the max limit of " << numNodesMax << "                            \r";
    }

    // faccio un refresh 
    refresh();

    time(&end);
    dif = difftime(end,start);
    cout << "The process started with " << numNodeStart << " and arrives to " << meshPointer->getNumNodes() << " points" << endl;
    cout << "Time of the whole process: " <<  dif << " sec." << endl;
}

//
// Internal methods to manage the mesh 
//
bool simplification2dCostFunctionBased::control(vector<UInt> * edge, point pNew)
{
      // controllo il tipo di punti con cui stiamo lavorando
      graphItem			      grafTmp,grafTmpPnew;
      set<UInt>                                 coinvolti;
      set<UInt>::iterator                             it1;
      vector<UInt>            common,elem,stellata,tmpEle;
      UInt                                        id1,id2;
      point                  oldId1,oldId2,nPrima,nDopo,p;
      bool                                       inverted;
      
      // controllo che pNew non sia degenere 
      if((pNew-pNull).norm2()<toll) return(false);
     
      // setto le variabili per comodità
      id1 = edge->at(0);
      id2 = edge->at(1);
      
      // trovo i triangoli adiacenti al lato
      elementOnEdge(id1, id2, &elem);
      
      // trovo TUTTI gli elementi coinvolti
      for(UInt s=0; s<edge->size(); ++s)
	    for(UInt j=0; j<conn.getNodeToElementPointer(edge->at(s))->getNumConnected(); ++j)
		  coinvolti.insert(conn.getNodeToElementPointer(edge->at(s))->getConnectedId(j));
      
      // elimino dalla lista dei coinvolti quelli in elem
      for(UInt s=0; s<elem.size(); ++s)
      {    
	    // li cerco 
	    it1 = coinvolti.find(elem[s]);
	    
	    // li DEVO TROVARE!!
	    assert(it1!=coinvolti.end());
	    
	    // li elimino
	    coinvolti.erase(it1);
      }
      
      // controllo che TUTTE le condizioni che ammettano il collasso siano verificate:
      // inv      = non devono esserci elementi invertiti
      
      // inserisco le coordinate di id1 e id2 in questi due punti
      // id1 ...
      oldId1.setX(meshPointer->getNode(id1).getX());
      oldId1.setY(meshPointer->getNode(id1).getY());
      oldId1.setZ(meshPointer->getNode(id1).getZ());
      // ... e id2
      oldId2.setX(meshPointer->getNode(id2).getX());
      oldId2.setY(meshPointer->getNode(id2).getY());
      oldId2.setZ(meshPointer->getNode(id2).getZ());
      
      // setto queste due variabili vere e poi nel ciclo le aggiorno
      inverted  = false;
      
      for(it1 = coinvolti.begin(); it1!=coinvolti.end(); ++it1)
      {
	    // calcolo la normale prima del collasso 
	    nPrima = getTriangleNormal(*it1);
	    
	    // sostituisco il punto
	    // per sicurezza sostituisco sia a id1 ...
	    meshPointer->getNodePointer(id1)->setX(pNew.getX());
	    meshPointer->getNodePointer(id1)->setY(pNew.getY());
	    meshPointer->getNodePointer(id1)->setZ(pNew.getZ());
	    // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
	    meshPointer->getNodePointer(id2)->setX(pNew.getX());
	    meshPointer->getNodePointer(id2)->setY(pNew.getY());
	    meshPointer->getNodePointer(id2)->setZ(pNew.getZ());
	            
	    // calcolo la normale dopo il collasso 
	    nDopo = getTriangleNormal(*it1);
	    
	    // vedo i due controlli
	    inverted  = ((nPrima*nDopo)<sqrt(3.)/2.);
	    
	    // ripristino le vecchie coordinate
	    // per sicurezza sostituisco sia a id1 ...
	    meshPointer->getNodePointer(id1)->setX(oldId1.getX());
	    meshPointer->getNodePointer(id1)->setY(oldId1.getY());
	    meshPointer->getNodePointer(id1)->setZ(oldId1.getZ());
	    // ...che a id2, lo so che è sovrabbondante come cosa ma non costa nulla
	    meshPointer->getNodePointer(id2)->setX(oldId2.getX());
	    meshPointer->getNodePointer(id2)->setY(oldId2.getY());
	    meshPointer->getNodePointer(id2)->setZ(oldId2.getZ());
	    
	    // controllo preventivo sull'inversione
	    if(inverted)	  return(false);
      }
      
      // prendo la stellata degli estremi 
      createStellataEdgeNode(edge, &stellata);
      
      // creo il grafo con i connessi del punto in cui ho collassato tutto 
      for(it1=coinvolti.begin(); it1!=coinvolti.end(); ++it1)		grafTmpPnew.connectedPushBack(*it1);
      
      // controllo che dopo il collasso ogni edge abbia solamente due triangolo adiacenti 
      for(UInt i=0; i<stellata.size(); ++i)
      {
	  // lo azzero 
	  grafTmp.clear();
	  tmpEle.clear();
	  
	  // creo il grafo 
	  for(UInt j=0; j<conn.getNodeToElementPointer(stellata[i])->getNumConnected(); ++j)	
	    grafTmp.connectedPushBack(conn.getNodeToElementPointer(stellata[i])->getConnectedId(j));
	  
	  // rimuovo quelli che saranno eliminati dal collasso 
	  for(UInt s=0; s<elem.size(); ++s)		  
	    grafTmp.remove(elem[s]);
	  
	  // guardo i comuni
	  grafTmp.common(grafTmpPnew, &tmpEle);
	  
	  // se dopo non hanno in comune due triangoli mi fermo 
	  if(tmpEle.size()!=2) 	return(false);
      }
      
      // ritorno vero se passa tutti i test
      return(true);
        
}

void simplification2dCostFunctionBased::fillTheMaterialIdAfterContraction(UInt pointToBeRemoved, const std::vector<UInt> & edge)
{
    if(edge[0]==pointToBeRemoved)
        pointMaterialId.push_back(pointMaterialId[edge[1]]);
    else
        pointMaterialId.push_back(pointMaterialId[edge[0]]);
}

void simplification2dCostFunctionBased::refresh()
{
      
      // variabili in uso
      graphItem 		    tmpGraph;
      vector<point>                    tmpPt;
      vector<geoElement<Triangle> >    tmpTr;
      point                                p;
      geoElement<Triangle>              tria;
      vector<bool>    		      active;
      vector<UInt>       	       newId;
      UInt              tmp,cont,id1,id2,id3;
      Real                             x,y,z;
      vector<geoElement<Line> >  lineListTmp;
      vector<graphItem>		tmpTrackList;
      
      // faccio un reserve
      active.reserve(meshPointer->getNumNodes());
      newId.reserve(meshPointer->getNumNodes());
      tmpPt.reserve(meshPointer->getNumNodes());
      tmpTr.reserve(meshPointer->getNumElements());
      
      // setto il vettore active che mi dice quali nodi sono attivi
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {
	    active.push_back(false);
	    newId.push_back(-1);
      }
      
      // vedo quali sono gli elementi attivi
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	    if(!isTriangleDegenerate(i))
	    {
		  id1 = meshPointer->getElement(i).getConnectedId(0);
		  id2 = meshPointer->getElement(i).getConnectedId(1);
		  id3 = meshPointer->getElement(i).getConnectedId(2);
		  
		  active[id1] = true;
		  active[id2] = true;
		  active[id3] = true;
	    }
      }
            
      // ciclo sui nodi pre creare la nuova lista
      cont = 0;
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
      {
	    if(active[i])
	    {
		    // prendo le coordinate
		    x   = meshPointer->getNode(i).getX();
		    y   = meshPointer->getNode(i).getY();
		    z   = meshPointer->getNode(i).getZ();
		    tmp = meshPointer->getNode(i).getBoundary();
		    
		    // setto una variabile p temporanea
		    p.setX(x);	  p.setY(y);	p.setZ(z);
		    p.setBoundary(tmp);         p.setId(cont);
		    
		    // metto a posto gli id
		    newId[i] = cont;
		    
		    // aggiorno cont
		    ++cont;
		    
		    // inserisco nella lista temporanea dei nodi
		    tmpPt.push_back(p);
	    }
      }
      
      // 
      // Set the materialId  
      // 
      std::vector<UInt> newPointMaterialId(tmpPt.size());
      for(UInt i=0; i<meshPointer->getNumNodes(); ++i)
         if(newId[i]!=static_cast<UInt>(-1))
             newPointMaterialId[newId[i]] = pointMaterialId[i];
      pointMaterialId = newPointMaterialId;
      
      
      // ciclo sugli elementi per sistemare gli id dei nodi e metterli nella lista temporanea
      cont = 0;
      for(UInt i=0; i<meshPointer->getNumElements(); ++i)
      {
	    if(!isTriangleDegenerate(i))
	    {
		  id1 = newId[meshPointer->getElement(i).getConnectedId(0)];
		  id2 = newId[meshPointer->getElement(i).getConnectedId(1)];
		  id3 = newId[meshPointer->getElement(i).getConnectedId(2)];
		  tmp = meshPointer->getElement(i).getGeoId();
		  
		  // creo la mia nuova variabile triangolo
		  tria.setConnectedId(0,id1);
		  tria.setConnectedId(1,id2);
		  tria.setConnectedId(2,id3);
		  tria.setGeoId(tmp);
		  tria.setId(cont);
		  
		  // metto nella lista dei triangoli
		  tmpTr.push_back(tria);
	    }
      }
      
      
      // libero le liste
      meshPointer->clear();
      
      // riempio i nodi
      meshPointer->insertNode(&tmpPt);
      
      // riempio gli elementi
      meshPointer->insertElement(&tmpTr);
        
      // metto a posto gli id
      meshPointer->setUpIds();
      
      // faccio un setUp
      setUp();
      
}

//
// Methods to manage the list
//
void simplification2dCostFunctionBased::createElementList(sortList<geoElementSize<simplePoint> > & sortedList)
{
    std::set<geoElementSize<simplePoint> >  listaTmp;
    
    // loop over the point and create the list 
    for(UInt pointId=0; pointId<meshPointer->getNumNodes(); ++pointId)
    {
        geoElementSize<simplePoint> tmpPointWithCost;
        computeCostOfAPoint(pointId, tmpPointWithCost);
        
        listaTmp.insert(tmpPointWithCost);
    }
    
    // metto nella lista
    sortedList.setElementVector(&listaTmp);
}

void simplification2dCostFunctionBased::updateAndAddElementList(sortList<geoElementSize<simplePoint> > & sortedList, 
                                                                UInt contractedPoint)
{
    std::vector<UInt> listOfPoints;
    costFunctionPointer->getNeightbourPoints(contractedPoint, listOfPoints);
    
    for(UInt i=0; i<listOfPoints.size(); ++i)
    {
        geoElementSize<simplePoint> tmpPointWithCost;
        computeCostOfAPoint(listOfPoints[i], tmpPointWithCost);
        
        // if a point is inside I change otherwise I add it, sice it may be 
        // a previous try that was not removed from the mesh 
        if(sortedList.isIn(listOfPoints[i]))
            sortedList.change(listOfPoints[i], tmpPointWithCost.getGeoSize());
        else
            sortedList.add(&tmpPointWithCost);
    }
}

void simplification2dCostFunctionBased::computeCostOfAPoint(UInt pointId, geoElementSize<simplePoint> & pointWithCost)
{
    point actualPoint = meshPointer->getNode(pointId);
    Real costValue = costFunctionPointer->computeCost(pointId, actualPoint);
    
    pointWithCost.setConnectedId(0, pointId);
    pointWithCost.setGeoSize(costValue);
    pointWithCost.setId(pointId);
}

UInt simplification2dCostFunctionBased::getPointToRemoveAndTakeOffItFromTheList(sortList<geoElementSize<simplePoint> > & listOfPossiblePointRemoval)
{
    point pt = pNull;
    UInt pointId = meshPointer->getNumNodes();
    while(pt==pNull)
    {
        // get the point 
        pointId = costFunctionPointer->getPointToRemove(listOfPossiblePointRemoval);
        pt = meshPointer->getNode(pointId);
        // remove from the list 
        listOfPossiblePointRemoval.remove(pointId);
    }
    
    return(pointId);
}

//
// Internal methods to print 
//
void simplification2dCostFunctionBased::printCostFunction(std::string filename)
{
    std::vector<Real> costValues(meshPointer->getNumNodes());
    for(UInt i=0; i<costValues.size(); ++i)
    {
        point tmpPoint;
        costValues[i] = costFunctionPointer->computeCost(i, tmpPoint);
    }
    
    createFile file;
    file.fileForParaviewNodePropriety(filename, meshPointer, &costValues);
}
    
    
}