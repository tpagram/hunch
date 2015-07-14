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
	cout << this->toString() << " is being deleted" << endl;
	delete(left);
	delete(right);
}

/*
Public access to toString method.
 */
string Formula::toString() {
	return toString(0);
}

/*
Converts the formula to a readable string.
 */
string Formula::toString(int priority) {
	bool needBrackets = false;
	string str = "";
	if (priority >= (int) op && (int) op > 2) {
		needBrackets = true;
		str += "( ";
	}
	if (op == Operator::ATOM) str += var;
	else if (op == Operator::TRUE) str += "true";
	else if (op == Operator::FALSE) str += "false";
	else if (op == Operator::AND) str += left->toString(6) + " & "  + right->toString(6);
	else if (op == Operator::OR) str += left->toString(5) + " | "  + right->toString(5);
	else if (op == Operator::IMPLIES) str += left->toString(4) + " => "  + right->toString(4);
	else if (op == Operator::EQUAL) str += left->toString(3) + " <=> "  + right->toString(3);
	else {
		cerr << "ERROR: formula to string encounters invalid operator.\n";
		exit(1);
	}
	if (needBrackets) str += " )";
	return str;
}

/*
Return operator variable.
 */
Operator Formula::getOp() {
	return op;
}

/*
Return left subtree.
 */
Formula* Formula::getLeft() {
	return left;
}

/*
Return right subtree.
 */
Formula* Formula::getRight() {
	return right;
}

/*
Returns true if the formulae are equal.
 */
bool Formula::isEqual(Formula* other) {
	if (this->op == other->op) {
		if (this->op == Operator::ATOM) {
			if (this->var == other->var) return true;
			else return false;
		}
		else if (this->op == Operator::TRUE) return true;
		else if (this->op == Operator::FALSE) return true;
		else if (this->left->isEqual(other->left) && this->right->isEqual(other->right)) return true;
		else if (this->left->isEqual(other->right) && this->right->isEqual(other->left)) return true;
		else return false;
	} 
	else return false;
}