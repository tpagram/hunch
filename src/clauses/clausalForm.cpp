#include "clauses/clausalForm.h"

using namespace std;

ClausalForm::ClausalForm() {
}

/*
Add a clause with a string identifying the type of clause.
 */
void ClausalForm::addClause(Clause *clause, string type) {
	for (string name : clause->getLiterals()) uniqueNames.insert(name);
	auto i = clauseMap.find(type);
	if (i == clauseMap.end()) clauseMap[type] = vector<Cptr>{Cptr(clause)};
	else i->second.push_back(Cptr(clause));
}

/*
Prints clauses of a certain type.
 */
void ClausalForm::printClauses(string type) {
	auto clauseType = clauseMap.find(type);
	if (clauseType == clauseMap.end()) {
		cout << "    " << type + " clauses (0):" << endl;
		return;
	}

	cout << "    " << type + " clauses (" << clauseType->second.size() << "):" << endl;
	if (verbose) {
		for (int i = 0; i < clauseType->second.size(); i++) {
			cout << "        (" << i << ") " << clauseType->second.at(i)->toString() << endl;
		}
	}
}

vector<Cptr>& ClausalForm::getClauses(string type) {
	auto clauseType = clauseMap.find(type);
	if (clauseType == clauseMap.end()) return nullclause;
	else return clauseType->second;
}

/*
Returns an unordered set of all names inside the form.
 */
unordered_set<string> ClausalForm::getNames() {
	return uniqueNames;
}

