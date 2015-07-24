#include "structures/mini.h"

using namespace std;


Mini::Mini() {
	internalSolver= unique_ptr<Minisat::Solver>(new Minisat::Solver);
}

/*
Add clause to the sat solver.
 */
void Mini::addClause(vector<int> clause) {
	internalSolver->addClause(*toLitClause(clause));
}

/*
Establish whether current set of clauses are satisfiable given an assumption clause.
 */
bool Mini::isSatisfiable(vector<int> assumptions) {
	return internalSolver->solve(*toLitClause(assumptions));
}

/*
Simplify current set of clauses.
 */
void Mini::simplify() {
	internalSolver->simplify();
}

/*
Converts a clause (int vector) to minisat-specific types.
 */
Minisat::vec<Minisat::Lit>* Mini::toLitClause(std::vector<int> intClause) {
	Minisat::vec<Minisat::Lit> *litClause = new Minisat::vec<Minisat::Lit>(intClause.size());
	for (int i : intClause) {
		litClause->push(Minisat::toLit(i));
	}
	return litClause;
}