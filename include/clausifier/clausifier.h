#ifndef CLAUSIFIER_H
#define CLAUSIFIER_H

#include "formula.h"
#include "clauses/cclause.h"
#include "clauses/iclause.h"
#include <vector>
#include <queue>

class Clausifier
{

public:
	virtual void clausify(Fptr&) = 0;

protected:
	virtual void simplify(Fptr&);
	virtual void introduceGoal(Fptr&);
	virtual bool soleOperatorFormula(Formula&,Operator);
	virtual std::vector<Fptr> extractSubformulae(Fptr&,Operator);
	virtual Formula* constructSoleOperatorFormula(std::vector<Fptr>&, Operator);
	virtual bool isClassical(Formula&);
	virtual bool isImplication(Formula&);
	virtual CClause formulaToClassical(Fptr&);
	virtual IClause formulaToImplication(Fptr&);

};

#endif
