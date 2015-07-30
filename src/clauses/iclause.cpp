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