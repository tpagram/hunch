#ifndef SOLVER_H
#define SOLVER_H

#include "clauses/clause.h"
#include "clauses/clausalForm.h"
#include "structures/structure.h"
#include "memory"

class Solver {

public:
	virtual bool solve(CFptr&) = 0;

protected:
	std::unique_ptr<Structure> internalSolver;
};

#endif // SOLVER_H
