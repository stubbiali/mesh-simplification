#ifndef BISECTION_HPP_
#define BISECTION_HPP_

#include "../core/shapes.hpp"

namespace geometry
{

/*! Classe che implementa il metodo di bisezione. L'intera classe è templatizzata rispetto a una classe F che determina la 
funzione che si deve utilizzare per valutare lo zero. Affinché ciò sia possibile la classe che rappresenta la funzione DEVE 
avere il metodo "evaluate_f" */
  
template<class F> class bisection
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
	    bisection();
    //
    // Metodo che cerca lo zero 
    //
    public:
	    /*! Metodo che trova lo zero 
		\param fun funzione di cui si deve valutare lo zero 
		\param a primo estremo 
		\param b secondo estremo*/
	    Real find_zero(F fun, Real a, Real b);
    //
    // Metodi per settare la tolleranza e max_iter
    //
    public:    
	    /*! Metodo che restituisce la tolleranza 
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
template<class F>  bisection<F>::bisection()
{
    // setto i valori base
    toll     = 1e-14;
    max_iter = 1000;
}

//
// Metodo che cerca lo zero 
//
template<class F> Real bisection<F>::find_zero(F fun, Real a, Real b)
{
    // Setting delle variabili 
    Real x=0.0, fc;
    Real a_tmp = a;
    Real b_tmp = b;
    Real err   = toll+1.0;
    int n_iter = -1;

    // inizio il ciclo fino a quando l'errore è più piccolo di tol e finché non supero il numero massimo di iterazioni 
    while(err>toll && n_iter<max_iter)
    {
	// incremento le iterazioni
	n_iter++;
	
	// prendo il punto medio
	x=(a_tmp+b_tmp)/2;

	// valuto la fiunzione
	fc=fun.evaluate_f(x);

	// calcolo l'errore  
	err=fabs(fc);
	
	// controllo se devo uscire 
	if(err<toll)			break;

	// prendo i due estremi 
	if(fc*fun.evaluate_f(a_tmp)>0)	a_tmp=x;
	else 				b_tmp=x;
    }

    // faccio le stampe
    if(n_iter<max_iter)  std::cout<<"Bisezione converge in "<<n_iter<<" iterazioni. Errore "<<err<<std::endl; 
    else
      std::cout<<"Attenzione: massismo di iterazioni ( "<<max_iter<<" ) per bisezione raggiunto. Errore "<<err<<std::endl;

    return x;
}


//
// Metodi di set/get
//
template<class F> inline void bisection<F>::setToll(Real _toll)
{
   toll=_toll;
}

template<class F> inline Real bisection<F>::getToll()
{
   return(toll);
}

template<class F> inline void bisection<F>::setMax_Iter(Real _max_iter)
{
   max_iter=_max_iter;
}

template<class F> inline UInt bisection<F>::getMax_Iter()
{
   return(max_iter);
}

}


#endif
