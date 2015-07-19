#include "clauses/cclause.h"

using namespace std;

/*
Initialise CClause with [left] -> [right]
 */
CClause::CClause(string left,string right) {
	if (!left.empty()) clause[0].push_back(left);
	if (!right.empty()) clause[1].push_back(right);
}

/*
Initialise CClause with left -> right
 */
CClause::CClause(vector<string> left,vector<string> right) {
	clause[0] = left;
	clause[1] = right;
}

/*
Returns a readable string of the clause.
 */
string CClause::toString() {
	/*string str = "[";
	for (string i : clause[0]) str += i + ", ";
	if (str.back() == ' ') str.erase(str.end()-2, str.end());
	str += "] => [";
	for (string i : clause[1]) str += i + ", ";
	if (str.back() == ' ') str.erase(str.end()-2, str.end());
	str += "]";	
	return str;*/
	string str;
	for (string i : clause[0]) str += i + " & ";
	if (str.back() == ' ') str.erase(str.end()-2, str.end());
	if (clause[0].size() > 0 && clause[1].size() > 0) str += " => ";
	for (string i : clause[1]) str += i + " | ";
	if (str.back() == ' ') str.erase(str.end()-2, str.end());	
	return str;
}

/*
Adds a literal to the left side of the implication.
 */
void CClause::addLeft(string newLit) {
	clause[0].push_back(newLit);
}

/*
Adds a literal to the right side of the implication.
 */
void CClause::addRight(string newLit) {
	clause[1].push_back(newLit);
}