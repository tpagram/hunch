#include "structures/mini.h"

using namespace std;


Mini::Mini() {
	internalSolver= unique_ptr<Minisat::Solver>(new Minisat::Solver);
	Minisat::Lit falseLit = Minisat::mkLit(internalSolver->newVar());
	nameToLitMap["false"] = falseLit;
	litToNameMap[toInt(falseLit)] = "false";
	internalSolver->addClause(~falseLit);

	Minisat::Lit a = Minisat::mkLit(internalSolver->newVar());
	Minisat::Lit b = Minisat::mkLit(internalSolver->newVar());
	Minisat::vec<Minisat::Lit> litClause;
	litClause.push(a);
	litClause.push(b);
	internalSolver->addClause(litClause);
	cout << "test sat = " << internalSolver->solve() << endl;
	cout << "a = " << (bool) !toInt(internalSolver->modelValue(a)) << endl;
	cout << "b = " << (bool) !toInt(internalSolver->modelValue(b)) << endl;
}

/*
Create literals for each unique name.
 */
void Mini::makeLiterals(unordered_set<string> uniqueNames) {
	for (string name : uniqueNames) {
		if (name == "false") continue;
		Minisat::Lit newLit = Minisat::mkLit(internalSolver->newVar());
		nameToLitMap[name] = newLit;
		litToNameMap[toInt(newLit)] = name;
	}
}

/*
Add clause to the sat solver.
 */
void Mini::addClause(StringClause clause) {
	if (true) { //todo verbose
		cout << "adding clause:";
		for (string i : clause.first) cout << " ~" << i;
		for (string i : clause.second) cout << " " << i;
		cout << endl;
	}
	internalSolver->addClause(*toLitClause(clause));
}

/*
Establish whether current set of clauses are satisfiable given an assumption clause.
 */
bool Mini::isSatisfiable(StringClause assumptions) {
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
Minisat::vec<Minisat::Lit>* Mini::toLitClause(StringClause stringClause) {
	if (true) { //todo verbose
	}
	Minisat::vec<Minisat::Lit> *litClause = new Minisat::vec<Minisat::Lit>();
	for (string name : stringClause.first) litClause->push(~ getLit(name));
	for (string name : stringClause.second) litClause->push(getLit(name));
	return litClause;
}

/*
Returns true if the literal was proven (true in every model) in the last solve call.
 */
bool Mini::isModel(string name) {
	// Minisat uses 0 as true and 1 as false.
	bool temp = !toInt(internalSolver->modelValue(getLit(name)));
	return temp;
}

/*
Returns all proven literals in the last solve call.
 */
unordered_set<string> Mini::getTruths() {
	unordered_set<string> truths;
	for (auto i : nameToLitMap) {
		if (isModel(i.first)) truths.insert(i.first);
	}
	return truths;
}

unordered_set<string> Mini::getConflicts() {
	unordered_set<string> conflictingLits;
	for (int i = 0; i < internalSolver->conflict.size(); i++) {
		conflictingLits.insert(getName(internalSolver->conflict[i]));
	}
	return conflictingLits;
}

Minisat::Lit Mini::getLit(string name) {
	auto j = nameToLitMap.find(name);
	if (j == nameToLitMap.end()) {
		cerr << "Asked for literal of unknown name: " << name << endl;
		exit(1);
	}
	else return j->second;
}

string Mini::getName(Minisat::Lit lit) {
	int litID = toInt(lit);
	if (litID % 2) litID--;
	auto j = litToNameMap.find(litID);
	if (j == litToNameMap.end()) {
		cerr << "Asked for name of unknown literal: " << toInt(lit) << endl;
		exit(1);
	}
	else return j->second;
}
