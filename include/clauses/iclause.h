#ifndef ICLAUSE_H
#define ICLAUSE_H

#include "clause.h"

class IClause : public Clause {

public:
	virtual std::string toString();
private:
	std::array<std::string,3> clause;
};

#endif // ICLAUSE_H
