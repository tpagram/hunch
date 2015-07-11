#include "formula.h"

using namespace std;

/*
Create a binary operator branch.
 */
Formula::Formula(Formula* left, Formula* right, Operator op) {
	this->left = left;
	this->right = right;
	this->op = op;
	var.clear();
}

/*
Create a variable atom.
 */
Formula::Formula(string var) {
	left = NULL;
	right = NULL;
	op = Operator::ATOM;
	this->var = var;
}

/*
Create a true or false atom.
 */
Formula::Formula(Operator op) {
	left = NULL;
	right = NULL;
	this->op = op;
	var.clear();
}

/*
Destroy entire formula.
 */
Formula::~Formula(void) {
	cout << var << "is being deleted" << endl;
	delete(left);
	delete(right);
}

/*
Converts the formula to a readable string.
 */
/*string Formula::toString() {
	if (op == Operator::ATOM) return var;
	if (op == Operator.AND) 
}*/