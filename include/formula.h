#ifndef FORMULA_H
#define FORMULA_H

class Formula
{
	enum class Operator { FALSE, TRUE, ATOM, AND, OR, IMPLIES, EQUAL };

public:
	Formula(Formula*, Formula*, Operator);
	Formula();

private:
	Formula* left;
	Formula* right;
	Operator op;
	int variable;
};	

#endif