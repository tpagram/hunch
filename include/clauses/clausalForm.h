#ifndef CLAUSAL_FORM_H
#define CLAUSAL_FORM_H

#include "clause.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <unordered_set>

typedef std::shared_ptr<Clause> Cptr;

class ClausalForm
{
public:
	ClausalForm();
	void addClause(Clause*, std::string);
	void printClauses(std::string);
	std::vector<Cptr>& getClauses(std::string);
	std::unordered_set<std::string> getNames();
private:
	std::unordered_map<std::string, std::vector<Cptr>> clauseMap;
	std::unordered_set<std::string> uniqueNames {"#goal"};
	bool verbose = true;
	std::vector<Cptr> nullclause;
};

typedef std::unique_ptr<ClausalForm> CFptr;

#endif // CLAUSAL_FORM_H
