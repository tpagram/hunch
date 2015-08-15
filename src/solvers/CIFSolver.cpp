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
		internalSolver->addClause(make_pair(unordered_set<string>{lits[1]},unordered_set<string>{lits[2]}));
	}
	
	vector<Cptr> imps = implication;
	bool isValid = introduceImplications(imps,make_pair(unordered_set<string> {"#goal"}, unordered_set<string>()));
	
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

	unordered_set<string> truths = internalSolver->getTruths();
	if (true) { //todo verbose
		cout << "truths = ";
		for (string i : truths) cout << i << ", ";
		cout << endl;
	}
	vector<Cptr> triggeredImplications;
	for (Cptr i : implications) {
		vector<string> lits = i->getLiterals();
		if (!truths.count(lits[0]) && !truths.count(lits[2])) {
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
		StringClause assum = make_pair(unordered_set<string> {lits[1]}, unordered_set<string> {lits[0]});
		assum.second.insert(assumptions.second.begin(),assumptions.second.end());
		assum.second.insert(truths.begin(),truths.end()); 
		vector<Cptr> otherImplications = triggeredImplications;
		otherImplications.erase(otherImplications.begin()+i);
		if (introduceImplications(otherImplications,assum)) {
			unordered_set<string> conflictingAssumptions = internalSolver->getConflicts();
			conflictingAssumptions.erase(lits[0]);
			conflictingAssumptions.erase(lits[1]);
			StringClause newClause = make_pair(conflictingAssumptions,unordered_set<string>{lits[2]});
			internalSolver->addClause(newClause);
			addedClause = true;
			break;
		}

	}

	if (addedClause) return introduceImplications(implications,assumptions);
	return false;
}
