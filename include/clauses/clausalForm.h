#ifndef CLAUSAL_FORM_H
#define CLAUSAL_FORM_H

#include "clause.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>

typedef std::unique_ptr<Clause> Cptr;

class ClausalForm
{
public:
	void addClause(Clause*, std::string);
	std::vector<Clause> getNameClauses(std::string);
	void printClauses(std::string);
private:
	std::unordered_map<std::string, std::vector<Cptr>> clauseMap;
	std::unordered_map<std::string, int> literalMap;
	int literalCount = 0;
	bool verbose;
};

typedef std::unique_ptr<ClausalForm> CFptr;

#endif // CLAUSAL_FORM_H
