#ifndef SOLVER_H
#define SOLVER_H

#include "clauses/clause.h"
#include "clausifier/clausifier.h"
#include "structures/structure.h"

class Solver {

public:
	virtual bool solve(ClausalForm) = 0;

private:
};

#endif // SOLVER_H
