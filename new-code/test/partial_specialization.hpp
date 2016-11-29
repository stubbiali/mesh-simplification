#ifndef HH_PARTIALSPECIALIZATION_HH
#define HH_PARTIALSPECIALIZATION_HH

#include <iostream>

using namespace std;

enum class Type{BASIC, EXTENDED};

class B1
{
	public:
		virtual void print() const
		{
			cout << "This is the first base class." << endl;
		};
};


class B2
{
	public:
		virtual void print() const
		{
			cout << "This is the second base class." << endl;
		};
};


template<typename B, Type T>
class D : public B
{
	private:
		const int NB = 5;
		
	public:
		void print() const
		{
			cout << "This is the derived class. BASIC and EXTENDED mode available." << endl;
		};
};


template<typename B>
class D<B,Type::EXTENDED>
{
	private:
		const int NE = 10;
		
	public:
		void print() const
		{
			cout << "This is the derived class in EXTENDED mode. NE = " << NE << "." << endl;
		};
};

#endif
