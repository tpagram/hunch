#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <queue>
#include <string>
#include "formula.h"

class Parser
{

public:
	virtual Formula* parse(std::string);

private:
	virtual std::queue<std::string>* tokenise(std::string);
	Formula* parseEquality(std::queue<std::string>*);
	Formula* parseImplication(std::queue<std::string>*);
	Formula* parseDisjunction(std::queue<std::string>*);
	Formula* parseConjunction(std::queue<std::string>*);
	Formula* parseNegation(std::queue<std::string>*);
	Formula* parseTerm(std::queue<std::string>*);


};

#endif