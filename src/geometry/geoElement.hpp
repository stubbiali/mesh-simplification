#ifndef GEOELEMENT_HPP_
#define GEOELEMENT_HPP_

#include <iostream>


#include "../core/graphItem.h"
#include "../core/shapes.hpp"
#include "../core/point.h"

namespace geometry {

    /*! Classe derivata da GraphItem aggiunge un Id relativo al sottodominio geometrico a cui appartiene l'elemento. Il numero di
    punti connessi all'elemento sono noti in quanto si ereditano tali informazioni da GEOSHAPE. */

    template<typename GEOSHAPE> class geoElement : public graphItem, public GEOSHAPE {
        //
        // Variabili di classe
        //
    public:
        /*! Number of the vertices */
        static const UInt numVertex = GEOSHAPE::numVertices;

        /*! The geometrical id */
        UInt geoId;

        UInt Idx_I;
        UInt Idx_J;
        UInt Idx_K;
        //
        // Costruttori
        //
    public:
        /*! Constructor */
        geoElement();

        /*! Costruttore di copia */
        geoElement<GEOSHAPE> (const geoElement<GEOSHAPE> &E);

        /*! Operatore uguaglianza */
        geoElement<GEOSHAPE> operator=(const geoElement<GEOSHAPE> &E);

        /*! Operatore uguaglianza */
        bool operator==(const geoElement<GEOSHAPE> &E) const;

        /*! Operatore minore */
        bool operator<(const geoElement<GEOSHAPE> &E) const;

        /*! Set I index */
        inline void setI(UInt idx_i) {
            Idx_I = idx_i;
        };

        /*! Set J index */
        inline void setJ(UInt idx_j) {
            Idx_J = idx_j;
        };

        /*! Set K index */
        inline void setK(UInt idx_k) {
            Idx_K = idx_k;
        };

        /*! Get I index */
        inline UInt getI() const {
            return Idx_I;
        };

        /*! Get J index */
        inline UInt getJ() const {
            return Idx_J;
        };

        /*! Get K index */
        inline UInt getK() const {
            return Idx_K;
        };
        //
        // Setting del geoId
        //
    public:
        /*! Return the geometrical Id */
        inline UInt getGeoId() const;

        /*! Set id geometrico */
        inline void setGeoId(UInt Id);
        //
        // Stampa
        //
    public:
        /*! Print to screen */
        void print();
    };

    //-------------------------------------------------------------------------------------------------------
    // IMPLEMENTATION
    //-------------------------------------------------------------------------------------------------------

    //
    // Costruttori
    //

    template<typename GEOSHAPE> geoElement<GEOSHAPE>::geoElement() : graphItem(numVertex) {
        geoId = 0;
    }

    template<typename GEOSHAPE> geoElement<GEOSHAPE>::geoElement(const geoElement<GEOSHAPE> &E) : graphItem(E) {
        geoId = E.geoId;
        Idx_I = E.Idx_I;
        Idx_J = E.Idx_J;
        Idx_K = E.Idx_K;
    }

    template<typename GEOSHAPE> geoElement<GEOSHAPE> geoElement<GEOSHAPE>::operator=(const geoElement<GEOSHAPE> &E) {
        this->graphItem::operator=(E);
        geoId = E.geoId;
        Idx_I = E.Idx_I;
        Idx_J = E.Idx_J;
        Idx_K = E.Idx_K;
        return *this;
    }

    template<typename GEOSHAPE> bool geoElement<GEOSHAPE>::operator==(const geoElement<GEOSHAPE> &E) const {
        return (this->graphItem::operator==(E));
    }

    //
    // Get/Set
    //

    template<typename GEOSHAPE> inline UInt geoElement<GEOSHAPE>::getGeoId() const {
        return (geoId);
    }

    template<typename GEOSHAPE> inline void geoElement<GEOSHAPE>::setGeoId(UInt Id) {
        geoId = Id;
    }

    //
    // Stampa
    //

    template<typename GEOSHAPE> void geoElement<GEOSHAPE>::print() {
        std::cout << "Id         : " << this->getId() << std::endl;
        std::cout << "GeoId      : " << this->getGeoId() << std::endl;
        std::cout << "Num Points : " << this->getNumConnected() << std::endl;
        std::cout << "Nodes Id's : ";

        for (UInt i = 0; i < connected.size(); ++i) {
            std::cout << this->getConnectedId(i) << " ";
        }
        std::cout << std::endl;
    }

    template<typename GEOSHAPE> bool geoElement<GEOSHAPE>::operator<(const geoElement<GEOSHAPE> &E) const {

        // if we want a sorted container, why not to use set?
        // proposed implementation
        multiset<UInt> thisConnectedIds(this->getConnectedIds().begin(), this->getConnectedIds().end());
        multiset<UInt> otherConnectedIds(E.getConnectedIds().begin(), E.getConnectedIds().end());

        for (multiset<UInt>::iterator itThis = thisConnectedIds.begin(), itOther = otherConnectedIds.begin();
                itThis != thisConnectedIds.end() && itOther != otherConnectedIds.end();
                itThis++, itOther++) { // this prevents an array out of bound
            if (*itThis < *itOther) {
                return true; //Esce se è minore
            } else if (*itThis > *itOther) {
                return false;
            } //Esce se è maggiore
        }

        return false; //Qui esce se son uguali

    }


}

#endif
