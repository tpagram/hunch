#ifndef CLAUSIFIER_H
#define CLAUSIFIER_H

#include <vector>
#include <queue>
#include "formula.h"
#include "options.h"
#include "clauses/cclause.h"
#include "clauses/iclause.h"
#include "clauses/clausalForm.h"

class Clausifier
{

public:
	Clausifier(Options options): options(options) {};
	virtual ClausalForm* clausify(Fptr&) = 0;

protected:
	Options options;
	virtual void simplify(Fptr&);
	virtual void introduceGoal(Fptr&);
	virtual void flattenEquivalence(Fptr&);
	virtual bool soleOperatorFormula(Formula&,Operator);
	virtual std::vector<Fptr> extractSubformulae(Fptr&,Operator);
	virtual Formula* constructSoleOperatorFormula(std::vector<Fptr>&, Operator);
	virtual bool isClassical(Formula&);
	virtual bool isImplication(Formula&);
	virtual CClause* formulaToClassical(Fptr&);
	virtual IClause* formulaToImplication(Fptr&);

};

#endif
