#include "formula.h"

using namespace std;

/*
Create a binary operator branch.
 */
Formula::Formula(Formula* left, Formula* right, Operator op): 
	left(left),
	right(right),
	op(op) {
	var.clear();
}

/*
Create a variable atom.
 */
Formula::Formula(string var) {
	left = nullptr;
	right = nullptr;
	op = Operator::ATOM;
	this->var = var;
}

/*
Create a true or false atom.
 */
Formula::Formula(Operator op) {
	left = nullptr;
	right = nullptr;
	this->op = op;
	var.clear();
}

/*
Clone existing formula.
 */
Formula::Formula(const Formula& other) {
	op = other.op;
	if (op == Operator::ATOM) {
		var = other.var;
		return;
	}
	else if (op == Operator::TRUE) return;
	else if (op == Operator::FALSE) return;
	else {
		left = Fptr(new Formula(*other.left));
		right = Fptr(new Formula(*other.right));
	}
}

/*
Destroy entire formula.
 */
Formula::~Formula(void) {
		//cout << "DELETING: " << this->toString() << endl;
}

bool Formula::operator==(const Formula& other) const {
	if (this->op == other.op) {
		if (this->op == Operator::ATOM) {
			if (this->var == other.var) return true;
			else return false;
		}
		else if (this->op == Operator::TRUE) return true;
		else if (this->op == Operator::FALSE) return true;
		if (this->op == Operator::IMPLIES) {
			if (*this->left == *other.left && *this->right == *other.right) return true;
			else return false;
		}
		else if (*this->left == *other.left && *this->right == *other.right) return true;
		else if (*this->left == *other.right && *this->right == *other.left) return true;
		else return false;
	} 
	else return false;
}

/*
Public access to toString method.
 */
string Formula::toString() const {
	return toString(0);
}

/*
Converts the formula to a readable string.
 */
string Formula::toString(int priority) const {
	bool needBrackets = false;
	string str = "";
	if (priority >= (int) op && (int) op > 2) {
		needBrackets = true;
		str += "( ";
	}
	if (op == Operator::ATOM) str += var;
	else if (op == Operator::TRUE) str += "true";
	else if (op == Operator::FALSE) str += "false";
	else if (left == nullptr && right == nullptr) return "NULL, NULL";
	else if (left == nullptr) return "NULL, " + right->toString();
	else if (right == nullptr) return left->toString() + ", NULL";
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
Operator Formula::getOp() const {
	return op;
}

/*
Return atomic variable.
 */
string Formula::getVar() const {
	if (op == Operator::TRUE) return "true";
	else if (op == Operator::FALSE) return "false";
	else return var;
}

/*
Return left subtree.
 */
Fptr& Formula::getLeft() {
	return left;
}

/*
Return right subtree.
 */
Fptr& Formula::getRight() {
	return right;
}

/*
Sets left subtree.
 */
void Formula::setLeft(Fptr left) {
	this->left = move(left);
}

/*
Sets right subtree.
 */
void Formula::setRight(Fptr right) {
	this->right = move(right);
}

/*
Sets operator.
 */
void Formula::setOp(Operator op) {
	this->op = op;
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
		else if (this->op == Operator::IMPLIES) {
			if (this->left->isEqual(other->left.get()) && this->right->isEqual(other->right.get())) {
				return true;
			}
			else return false;
		}
		else {
			if ((this->left->isEqual(other->left.get()) && this->right->isEqual(other->right.get())) ||
				(this->left->isEqual(other->right.get()) && this->right->isEqual(other->left.get()))) {
				return true;
			}
			else return false;
		}
	}
	else return false;
}

/*
Returns true if the formula is atomic.
 */
bool Formula::isAtomic() {
	if (op == Operator::TRUE ||
			op == Operator::FALSE ||
			op == Operator::ATOM)
		return true;
	else return false;
}

/*
Hasher of formulae for use in unordered_map.
 */
size_t Formula::FormulaHash::operator()(const Formula& formula) const {
	if (formula.op == Operator::ATOM) {
		return hash<std::string>()(formula.var);
	}
	else if (formula.op == Operator::TRUE || formula.op == Operator::FALSE) {
		return hash<int>()((int) formula.op);
	}
	else if (formula.op == Operator::IMPLIES) {
		return hash<int>()((int) formula.op) ^
 				(operator()(*formula.left) << 1) >> 1 ^
				operator()(*formula.right) << 1;
	}
	else {
		size_t left = operator()(*formula.left);
		size_t right = operator()(*formula.right);
		if (left >= right) {
			return hash<int>()((int) formula.op) ^
 				(left << 1) >> 1 ^
				right << 1;
		}
 		else {
 			return hash<int>()((int) formula.op) ^
 				(right << 1) >> 1 ^
				left << 1;
 		}
	}
}
