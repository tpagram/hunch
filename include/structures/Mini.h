#ifndef MINISAT_H
#define MINISAT_H

#include "structure.h"
#include "third_party/Solver.h"
#include <memory>

class Mini : public Structure
{
public:
	Mini();
	void addClause(std::vector<int>);
	bool isSatisfiable(std::vector<int> assumptions);
	void simplify();
private:
	std::unique_ptr<Minisat::Solver> internalSolver;
	Minisat::vec<Minisat::Lit>* toLitClause(std::vector<int>);
};



#endif // MINISAT_H
