#ifndef SOLVER_FACTORY_H
#define SOLVER_FACTORY_H

#include <memory>
#include "solver.h"
#include "CIFSolver.h"

class SolverFactory
{
public:
		static std::unique_ptr<Solver> getSolver(std::string);
};

#endif // SOLVER_FACTORY_H
