#ifndef HUNCH_CLAUSIFIER_H
#define HUNCH_CLAUSIFIER_H

#include "clausifier/clausifier.h"
#include <queue>
#include <unordered_map>

enum class Direction { implies, equiv, implied }; 


class hunchClausifier : public Clausifier
{
public:
	virtual void clausify(Fptr&);

private:
	int renameCounter = 0;
	std::unordered_map<Formula,std::pair<std::string,Direction>,Formula::FormulaHash> nameMap;
	std::string debug;

	virtual void extractClauses(std::queue<Fptr>&);
	std::string newName();
	std::pair<Formula*,std::string> rename(Fptr, Direction);  
	Formula* createRenamedFormula(std::string, Fptr, Direction);
	std::string checkForName(Formula,Direction);
};


#endif // HUNCH_CLAUSIFIER_H
