#ifndef HUNCH_CLAUSIFIER_H
#define HUNCH_CLAUSIFIER_H

#include "clausifier/clausifier.h"
#include <queue>
#include <unordered_map>
#include <memory>

enum class Direction { implies, equiv, implied }; 

typedef std::unordered_map<Formula,std::pair<std::string,Direction>,Formula::FormulaHash> FormulaMap;

class hunchClausifier : public Clausifier
{
public:
	virtual ClausalForm* clausify(Fptr&);

private:
	int renameCounter = 0;
	std::unique_ptr<FormulaMap> nameMap = std::unique_ptr<FormulaMap>(new FormulaMap());
	bool verbose = false;

	ClausalForm* extractClauses(Fptr&);
	std::pair<Formula*,std::string> rename(Fptr, Direction);  
	Formula* createRenamedFormula(std::string, Fptr, Direction);
	std::string checkForName(Formula,Direction);
	void removeRedundantImplications(ClausalForm& clausalform);
};


#endif // HUNCH_CLAUSIFIER_H
