#ifndef CLAUSE_H
#define CLAUSE_H

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

typedef std::pair<std::vector<std::string>,std::vector<std::string>> StringClause;

class Clause {

public:
	virtual std::string toString() = 0;
	virtual std::vector<std::string> getLiterals() = 0;
	virtual std::vector<int> toIntClause(std::unordered_map<std::string, int>&) = 0;
	virtual StringClause toStringClause() = 0;
};

#endif // CLAUSE_H
