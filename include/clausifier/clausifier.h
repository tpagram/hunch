#ifndef CLAUSIFIER_H
#define CLAUSIFIER_H

#include "formula.h"

class Clausifier
{

public:
	virtual void clausify(Formula*) = 0;

protected:
	virtual Formula* simplify(Formula*);
	virtual Formula* introduceGoal(Formula*);
};


#endif
