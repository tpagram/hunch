#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <iostream>

enum class Operator { FALSE, TRUE, ATOM, EQUAL, IMPLIES, OR, AND };

class Formula
{

public:
	Formula(Formula*, Formula*, Operator);
	Formula(std::string var);
	Formula(Operator);
	~Formula();
	std::string toString();
	Operator getOp();
	Formula* getLeft();
	Formula* getRight();
	bool isEqual(Formula*);

private:
	Formula* left;
	Formula* right;
	Operator op;
	std::string var;
	std::string toString(int priority);
};	

#endif