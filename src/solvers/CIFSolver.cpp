#include "solvers/CIFSolver.h"

using namespace std;

/*
Returns whether a set of classical clauses of the form a_1 & ... & a_n => b_1 | ... | b_m
and implication clauses of the form (a => b) => c are valid or not. 
 */
bool CIFSolver::solve(CFptr& clausalform) {

	//todo factory
	unique_ptr<Structure> satSolver = unique_ptr<Structure>(new Mini());

	// 

	return true;
}

/*
Turns clauses of strings into clauses of integers.
 */
pair<vector<int>,vector<int>> extractLiterals(ClausalForm) {
	unordered_map<string,int> litMap;
	

}