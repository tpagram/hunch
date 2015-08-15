#include "solvers/solverFactory.h"

using namespace std;

unique_ptr<Solver> SolverFactory::getSolver(Options options) {
	if (options.solver == "cif") return unique_ptr<Solver>(new CIFSolver(options));
	else {
		cerr << "SolverFactory asked for unknown solver: " << options.solver << endl;
		exit(1);
	}
}