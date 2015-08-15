#ifndef MINISAT_H
#define MINISAT_H

#include "structure.h"
#include "third_party/Solver.h"
#include <memory>
#include <iostream>
#include <unordered_map>

class Mini : public Structure
{
public:
	Mini(Options options);
	void makeLiterals(std::unordered_set<std::string>);
	void addClause(StringClause);
	bool isSatisfiable(StringClause);
	void simplify();
	bool isModel(std::string name);
	std::unordered_set<std::string> getTruths();
	std::unordered_set<std::string> getConflicts();
private:
	std::unique_ptr<Minisat::Solver> internalSolver;
	std::unordered_map<std::string,Minisat::Lit> nameToLitMap;
	std::unordered_map<int,std::string> litToNameMap;
	Minisat::Lit getLit(std::string);
	std::string getName(Minisat::Lit);
	Minisat::vec<Minisat::Lit>* toLitClause(StringClause);
};



#endif // MINISAT_H
