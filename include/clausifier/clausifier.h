#ifndef CLAUSIFIER_H
#define CLAUSIFIER_H

#include "formula.h"

class Clausifier
{

public:
	virtual void clausify(Fptr&) = 0;

protected:
	Fptr mainFormula;
	virtual void simplify(Fptr&);
	virtual void introduceGoal(Fptr&);
};


#endif
