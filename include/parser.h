#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include "formula.h"

class Parser
{

public:
	virtual Formula* parse(std::string);

private:
	virtual std::vector<std::string>* tokenise(std::string);

};

#endif