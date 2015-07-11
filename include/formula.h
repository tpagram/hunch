#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <iostream>

enum class Operator { FALSE, TRUE, ATOM, AND, OR, IMPLIES, EQUAL };

class Formula
{

public:
	Formula(Formula*, Formula*, Operator);
	Formula(std::string var);
	Formula(Operator);
	~Formula();
	std::string toString();

private:
	Formula* left;
	Formula* right;
	Operator op;
	std::string var;
};	

#endif