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

/*
Returns a vector of all literals used in the clause.
 */
vector<string> CClause::getLiterals() {
	vector<string> literals;
	for (string i : clause[0]) literals.push_back(i);
	for (string i: clause[1]) literals.push_back(i);
	return literals;
}

/*
Returns the clause in the form of an int vector according to certain mapping
of literal names to integers.
 */
vector<int> CClause::toIntClause(std::unordered_map<std::string, int>& litMap) {
	vector<int> intClause;
	for (string i : clause[0]) {
		intClause.push_back(-litMap[i]);
	}
	for (string i : clause[1]) {
		intClause.push_back(litMap[i]);
	}
	return intClause;
}

StringClause CClause::toStringClause() {
	return make_pair(clause[0],clause[1]);
}
