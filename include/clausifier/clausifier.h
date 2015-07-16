#ifndef CLAUSIFIER_H
#define CLAUSIFIER_H

#include "formula.h"
#include "clauses/cclause.h"
#include "clauses/iclause.h"
#include <vector>

class Clausifier
{

public:
	virtual void clausify(Fptr&) = 0;

protected:
	virtual void simplify(Fptr&);
	virtual void introduceGoal(Fptr&);
	virtual bool isClassical(Formula&);
	virtual bool isImplication(Formula&);
	virtual bool soleOperatorFormula(Formula&,Operator);
};

#endif
