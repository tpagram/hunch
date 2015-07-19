#ifndef FORMULA_H
#define FORMULA_H

#include <string>
#include <iostream>
#include <memory>

enum class Operator { FALSE, TRUE, ATOM, EQUAL, IMPLIES, OR, AND };

class Formula
{

public:
	Formula(Formula*, Formula*, Operator);
	Formula(std::string var);
	Formula(Operator);
	Formula(const Formula&);
	~Formula();
	bool operator==(const Formula& other) const;
	std::string toString();
	Operator getOp() const;
	std::string getVar() const;
	std::unique_ptr<Formula>& getLeft();
	std::unique_ptr<Formula>& getRight();
	void setLeft(std::unique_ptr<Formula>);
	void setRight(std::unique_ptr<Formula>);
	void setOp(Operator);
	bool isEqual(Formula*);
	bool isAtomic();
	struct FormulaHash {size_t operator()(const Formula& formula) const;};

private:
	std::unique_ptr<Formula> left;
	std::unique_ptr<Formula> right;
	Operator op;
	std::string var;
	std::string toString(int priority);
};	

typedef std::unique_ptr<Formula> Fptr;
#endif