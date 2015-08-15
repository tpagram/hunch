#ifndef SOLVER_H
#define SOLVER_H

#include "clauses/clause.h"
#include "clauses/clausalForm.h"
#include "structures/structure.h"
#include "memory"
#include "options.h"

class Solver {

public:
	virtual bool solve(CFptr&) = 0;
	Solver(Options options) : options(options) {};

protected:
	std::unique_ptr<Structure> internalSolver;
	Options options;
};

#endif // SOLVER_H
