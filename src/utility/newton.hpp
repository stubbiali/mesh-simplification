#ifndef NEWTON_HPP_
#define NEWTON_HPP_


#include "../core/shapes.hpp"

namespace geometry
{
  
/*! Classe che implementa il metodo di newton. L'intera classe è templatizzata rispetto a una classe F che determina la 
funzione che si deve utilizzare per valutare lo zero. Affinché ciò sia possibile la classe che rappresenta la funzione DEVE 
avere il metodo "evaluate" e "evaluate_df" */

template<class F> class newton
{
    //
    // Variabili di classe 
    //
    public:
	    /*! Tolleranza*/  
	    Real toll;

	    /*! Numero massimo di iterazioni */  
	    int max_iter;
    //
    // Costruttore 
    //
    public:
	    /*! Costruttore vuoto */
	    newton();
    //
    // Metodo che cerca lo zero 
    //
    public:
	    /*! Metodo che trova lo zero 
		\param fun funzione di cui si trova lo zero
		\param x0 punto di partenza */
	    Real find_zero(F fun, Real x0);
    //
    // Metodi per settare la tolleranza e max_iter
    //
    public:    
	    /*! Metodo che setta la tolleranza 
		\param _toll valore della tolleranza */
	    inline void setToll(Real _toll);
		  
	    /*! Metodo che restituisce la tolleranza */
	    inline Real getToll();
	    
	    /*! Metodo che setta il numero massimo di iterazioni 
		\param _max_iter valore della tolleranza */
	    inline void setMax_Iter(Real _max_iter);
		  
	    /*! Metodo che restituisce la tolleranza */
	    inline UInt getMax_Iter();
};

//-------------------------------------------------------------------------------------------------------
// IMPLEMENTATION
//-------------------------------------------------------------------------------------------------------

//
// Costruttori
//
template<class F>  newton<F>::newton()
{
    // setto i valori base
    toll     = 1e-14;
    max_iter = 1000;
}

//
// Metodo che cerca lo zero 
//
template<class F> Real newton<F>::find_zero(F fun, Real x0)
{
    // Variabili in uso 
    Real err    = toll+1.0;
    int  n_iter = 0;
    Real x_old  = x0;

    // variabili che si usano 
    Real x_new=0.0,df;

    // ciclo per trovare il valore 
    while(err>toll && n_iter<max_iter)
    {
	// calcolo della derivata
	df=fun.evaluate_df(x_old);
	
	// controllo la derviata della funzione 
	if(fabs(df)<toll)
	{
	    cout<<"Arresto per annullamento derivata del metodo di Newton"<<endl;
	    return(-1.);
	}
	
	// prendo il nuovo nodo
	x_new=x_old-fun.evaluate_f(x_old)/df;

	// calcolo l'errore
	err=fabs(x_new-x_old);
      
	// incremento i passi 
	n_iter++;

	// resetto old
	x_old=x_new;

    }

    // stampo i dati 
    if(n_iter<max_iter)		     std::cout<<"Newton converge in "<<n_iter<<" iterazioni. Errore "<<err<<std::endl; 
    else	
      std::cout<<"Newton non converge nel numero massimo di iterazioni ( "<<max_iter<<" ). Errore "<<err<<std::endl; 
    
    // ritorno 
    return x_new;
}

//
// Metodi di set/get
//
template<class F> inline void newton<F>::setToll(Real _toll)
{
   toll=_toll;
}

template<class F> inline Real newton<F>::getToll()
{
   return(toll);
}

template<class F> inline void newton<F>::setMax_Iter(Real _max_iter)
{
   max_iter=_max_iter;
}

template<class F> inline UInt newton<F>::getMax_Iter()
{
   return(max_iter);
}




}


#endif
