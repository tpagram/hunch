#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <vector>
#include <unordered_set>
#include "options.h"
#include "clauses/clause.h"

class Structure {

public:
	Structure(Options options): options(options) {};
	virtual void makeLiterals(std::unordered_set<std::string>) = 0;
	virtual void addClause(StringClause) = 0;
	virtual bool isSatisfiable(StringClause) = 0;
	virtual void simplify() = 0;
	virtual bool isModel(std::string name) = 0;
	virtual std::unordered_set<std::string> getTruths() = 0;
	virtual std::unordered_set<std::string> getConflicts() = 0;
protected:
	Options options;
};

#endif // STRUCTURE_H
