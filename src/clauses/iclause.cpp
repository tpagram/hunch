#include "clauses/iclause.h"

using namespace std;

/*
Initialise IClause with (left -> mid) -> right
 */
IClause::IClause(string left, string mid, string right) {
	clause[0] = left;
	clause[1] = mid;
	clause[2] = right;
}

/*
Returns a readable string of the clause.
 */
string IClause::toString() {
	return "(" + clause[0] + " => " + clause[1]  + ") => " + clause[2];
}

/*
Returns a vector of all literals used in the clause.
 */
vector<string> IClause::getLiterals() {
	vector<string> literals;
	for (int i = 0; i < 3; i++) literals.push_back(clause[i]);
	return literals;
}

/*
Returns the clause in the form of an int vector according to certain mapping
of literal names to integers.
 */
vector<int> IClause::toIntClause(std::unordered_map<std::string, int>& litMap) {
	vector<int> intClause;
	for (string i : clause) {
		intClause.push_back(litMap[i]);
	}
	return intClause;
}

StringClause IClause::toStringClause() {
	return make_pair(unordered_set<string>(),unordered_set<string>());
}
