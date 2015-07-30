#include "clauses/clausalForm.h"

using namespace std;

/*
Add a clause with a string identifying the type of clause.
 */
void ClausalForm::addClause(Clause *clause, string type) {

	//Check if literal exists for variable name.
	//If not, assign literal.
	for (string lit: clause->getLiterals()) {
		auto i = literalMap.find(lit);
		if (i == literalMap.end()) {
			literalMap[lit] = literalCount;
			literalCount++;
		}
	}

	//Add clause to a vector of clauses of similar type.
	auto i = clauseMap.find(type);
	if (i == clauseMap.end()) {
		clauseMap[type]= vector<Cptr>();
		clauseMap[type].push_back(Cptr(clause));
	}
	else {
		clauseMap[type].push_back(Cptr(clause));
	}
}

/*
Prints clauses of a certain type.
 */
void ClausalForm::printClauses(string type) {
	auto clauseType = clauseMap.find(type);
	cout << type + "clauses (" << clauseType->second.size() << ") :" << endl;
	if (verbose) {
		for (int i = 0; i < clauseType->second.size(); i++) {
			cout << clauseType->second.at(i)->toString() << endl;
		}
	}
}


/*
Returns a string of all clauses contained in specific clause type.
 */


