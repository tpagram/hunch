#ifndef CCLAUSE_H
#define CCLAUSE_H

#include "clause.h"

class CClause : public Clause {

public:
	CClause(std::string,std::string);
	CClause(std::vector<std::string>,std::vector<std::string>);
	virtual std::string toString();
	void addLeft(std::string);
	void addRight(std::string);
	virtual std::vector<std::string> getLiterals();
	virtual std::vector<int> toIntClause(std::unordered_map<std::string, int>&);
	virtual StringClause toStringClause();
private:
	std::array<std::vector<std::string>,2> clause;
};

#endif // CCLAUSE_H
