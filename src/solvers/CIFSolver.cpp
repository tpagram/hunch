#include "solvers/CIFSolver.h"

using namespace std;

/*
Returns whether a set of classical clauses of the form a_1 & ... & a_n => b_1 | ... | b_m
and implication clauses of the form (a => b) => c are valid or not. 
 */
bool CIFSolver::solve(CFptr& clausalform) {

	//todo factory
	//Initialise internal solver.
	internalSolver = unique_ptr<Structure>(new Mini());

	//Add literals to solver.
	internalSolver->makeLiterals(clausalform->getNames());

	//Add classical clauses to the internal solver.
	vector<Cptr>& classical = clausalform->getClauses("classical");
	for (const Cptr& clause : classical) internalSolver->addClause(clause->toStringClause());

	//For each implication clause (a => b) => c, add b => c to the solver.
	vector<Cptr>& implication = clausalform->getClauses("implication");
	for (const Cptr& clause : implication) {
		vector<string> lits = clause->getLiterals();
		internalSolver->addClause(make_pair(vector<string>{lits[1]},vector<string>{lits[2]}));
	}
	
	vector<Cptr> imps = implication;
	bool isValid = introduceImplications(imps,make_pair(vector<string> {"#goal"}, vector<string>()));
	
	return isValid;

}

bool CIFSolver::introduceImplications(vector<Cptr> implications, StringClause assumptions) {

	//Is the solver currently unsatisfiable? If so, we know validity.
	if (true) { //todo verbose
		cout << "testing satisfiability with assumptions:";
		for (string i : assumptions.first) cout << " ~" << i;
		for (string i : assumptions.second) cout << " " << i;
		cout << endl;
	}
	bool isSat = internalSolver->isSatisfiable(assumptions);
	if (isSat) cout << "initial sat" << endl;
	else cout << "initial unsat" << endl;
	if (!isSat) return true;

	//
	vector<Cptr> triggeredImplications;
	for (Cptr i : implications) {
		vector<string> lits = i->getLiterals();
		if (!internalSolver->isModel(lits[0]) &&
				!internalSolver->isModel(lits[1]) &&
				!internalSolver->isModel(lits[2])) {
			triggeredImplications.push_back(i);
		}
	}
	if (true) { //todo verbose
		cout << "triggeredImplications = ";
		for (Cptr i : triggeredImplications) cout << i->toString() << ", ";
		cout << endl;
	}

	bool addedClause = false;
	for (int i = 0; i < triggeredImplications.size(); i++) {
		vector<string> lits = triggeredImplications[i]->getLiterals();
		StringClause assum = make_pair(vector<string> {lits[1]}, vector<string> {lits[0]});
		assum.second.insert(assum.second.end(),assumptions.second.begin(),assumptions.second.end());
		vector<Cptr> otherImplications = triggeredImplications;
		otherImplications.erase(otherImplications.begin()+i);
		if (introduceImplications(otherImplications,assum)) {
			vector<string> assumedTruths = assum.second;
			assumedTruths.erase(remove(assumedTruths.begin(), assumedTruths.end(), lits[0]), assumedTruths.end() );
			assumedTruths.erase(remove(assumedTruths.begin(), assumedTruths.end(), lits[1]), assumedTruths.end() );
			StringClause newClause = make_pair(assumedTruths,vector<string>{lits[2]});
			internalSolver->addClause(newClause);
			addedClause = true;
		}

	}

	if (addedClause) return introduceImplications(implications,assumptions);
	return false;
}
