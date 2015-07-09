#ifndef FORMULA_H
#define FORMULA_H value

class Formula
{
	enum class Operator { FALSE, TRUE, ATOM, AND, OR, IMPLIES, EQUAL };

public:
	Formula(Formula*, Formula*, Operator);

private:
	Formula* left;
	Formula* right;
	Operator op;
};	int variable;

#endif