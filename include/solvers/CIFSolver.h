#ifndef CIF_SOLVER_H
#define CIF_SOLVER_H

#include "solver.h"
#include "structures/mini.h" //temp
#include <unordered_map>

class CIFSolver : public Solver
{
public:
	bool solve(CFptr&);
	
};

#endif // CIF_SOLVER_H
