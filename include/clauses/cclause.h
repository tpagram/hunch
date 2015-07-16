#ifndef CCLAUSE_H
#define CCLAUSE_H

#include "clause.h"

class CClause : public Clause {

public:
	CClause(std::string,std::string);
	virtual std::string toString();
	void addLeft(std::string);
	void addRight(std::string);
private:
	std::array<std::vector<std::string>,2> clause;
};

#endif // CCLAUSE_H
