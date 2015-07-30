#ifndef SOLVER_H
#define SOLVER_H

#include "clauses/clause.h"
#include "clauses/clausalForm.h"
#include "structures/structure.h"


class Solver {

public:
	virtual bool solve(CFptr&) = 0;

private:
};

#endif // SOLVER_H
