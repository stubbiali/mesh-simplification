#include"geoElementSearch.h"

using namespace geometry;

//
// Costruttori
//
geoElementSearch::geoElementSearch()
{
	id = 0;
	active  = false; 
	rapp[0] = 0.0;
	rapp[1] = 0.0;
	rapp[2] = 0.0;
	boxMax[0] = 0.0;
	boxMax[1] = 0.0;
	boxMax[2] = 0.0;
	boxMin[0] = 0.0;
	boxMin[1] = 0.0;
	boxMin[2] = 0.0;
	
}

geoElementSearch::geoElementSearch(const geoElementSearch &E)
{
	id = E.id;
	rapp[0] = E.rapp[0];
	rapp[1] = E.rapp[1];
	rapp[2] = E.rapp[2];
	boxMax[0] = E.boxMax[0];
	boxMax[1] = E.boxMax[1];
	boxMax[2] = E.boxMax[2];
	boxMin[0] = E.boxMin[0];
	boxMin[1] = E.boxMin[1];
	boxMin[2] = E.boxMin[2];
	active  = E.active;
}
			
geoElementSearch & geoElementSearch::operator=(const geoElementSearch &E)
{
	id = E.id;
	rapp[0] = E.rapp[0];
	rapp[1] = E.rapp[1];
	rapp[2] = E.rapp[2];
	boxMax[0] = E.boxMax[0];
	boxMax[1] = E.boxMax[1];
	boxMax[2] = E.boxMax[2];
	boxMin[0] = E.boxMin[0];
	boxMin[1] = E.boxMin[1];
	boxMin[2] = E.boxMin[2];
	active  = E.active;
	return *this;
}

//
// Settaggio delle variabili 
//
void geoElementSearch::setRapp(point _rapp)
{
	rapp[0] = _rapp.getX();
	rapp[1] = _rapp.getY();
	rapp[2] = _rapp.getZ();
}

void geoElementSearch::setBoxMax(point _boxMax)
{
	boxMax[0] = _boxMax.getX();
	boxMax[1] = _boxMax.getY();
	boxMax[2] = _boxMax.getZ();
}

void geoElementSearch::setBoxMin(point _boxMin)
{
	boxMin[0] = _boxMin.getX();
	boxMin[1] = _boxMin.getY();
	boxMin[2] = _boxMin.getZ();
}

//
// get delle variabili
//
point geoElementSearch::getRapp()
{
	point tmp;
	tmp.setX(rapp[0]);
	tmp.setY(rapp[1]);
	tmp.setZ(rapp[2]);
	return(tmp);
}

point geoElementSearch::getBoxMax()
{
	point tmp;
	tmp.setX(boxMax[0]);
	tmp.setY(boxMax[1]);
	tmp.setZ(boxMax[2]);
	return(tmp);
}

point geoElementSearch::getBoxMin()
{
	point tmp;
	tmp.setX(boxMin[0]);
	tmp.setY(boxMin[1]);
	tmp.setZ(boxMin[2]);
	return(tmp);
}

//
// print
//
void geoElementSearch::print()
{
  	std::cout << "Id             : " << this->getId() << std::endl;
	std::cout << "Rappresentante : ";
	for(UInt i=0; i<3; ++i)
	{
		std::cout << rapp[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "BoxMax : ";
	for(UInt i=0; i<3; ++i)
	{
		std::cout << boxMax[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "BoxMin : ";
	for(UInt i=0; i<3; ++i)
	{
		std::cout << boxMin[i] << " ";
	}
	std::cout << std::endl;
	
}
