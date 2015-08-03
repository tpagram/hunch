#ifndef ICLAUSE_H
#define ICLAUSE_H

#include "clause.h"

class IClause : public Clause {

public:
	IClause(std::string, std::string, std::string);
	virtual std::string toString();
	virtual std::vector<std::string> getLiterals();
	virtual std::vector<int> toIntClause(std::unordered_map<std::string, int>&);
	virtual StringClause toStringClause();
private:
	std::array<std::string,3> clause;
};

#endif // ICLAUSE_H
