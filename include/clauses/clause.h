#ifndef CLAUSE_H
#define CLAUSE_H

#include <string>
#include <array>
#include <vector>

class Clause {

public:
	virtual std::string toString() = 0;
	virtual std::vector<std::string> getLiterals() = 0;
};

#endif // CLAUSE_H
