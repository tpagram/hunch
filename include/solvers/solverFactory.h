#ifndef SOLVER_FACTORY_H
#define SOLVER_FACTORY_H

#include <memory>
#include "solver.h"
#include "CIFSolver.h"
#include "options.h"

class SolverFactory
{
public:
		static std::unique_ptr<Solver> getSolver(Options options);
};

#endif // SOLVER_FACTORY_H
