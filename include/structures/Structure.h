#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <vector>
#include "clauses/clause.h"
#include <unordered_set>

class Structure {

public:
	virtual void makeLiterals(std::unordered_set<std::string>) = 0;
	virtual void addClause(StringClause) = 0;
	virtual bool isSatisfiable(StringClause) = 0;
	virtual void simplify() = 0;
	virtual bool isModel(std::string name) = 0;
	virtual std::unordered_set<std::string> getTruths() = 0;
	virtual std::unordered_set<std::string> getConflicts() = 0;
private:
};

#endif // STRUCTURE_H
