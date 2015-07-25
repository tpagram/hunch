#ifndef CIF_SOLVER_H
#define CIF_SOLVER_H

#include "solver.h"

class CIFSolver : public Solver
{
public:
	bool solve(ClausalForm);
	
};

#endif // CIF_SOLVER_H
