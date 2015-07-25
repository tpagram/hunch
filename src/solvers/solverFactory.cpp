#include "solvers/solverFactory.h"

using namespace std;

unique_ptr<Solver> SolverFactory::getSolver(string) {
	return unique_ptr<Solver>(new CIFSolver());
}