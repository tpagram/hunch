#include "clausifier/clausifier.h"
using namespace std;

/*
Simplifies a formula using a variety of TRUE/FALSE rules.
 */
void Clausifier::simplify(Fptr& formula) {

	// atom base cases
	if (formula->getOp() == Operator::ATOM) return;
	else if (formula->getOp() == Operator::TRUE) return;
	else if (formula->getOp() == Operator::FALSE) return;

	// recurse
	simplify(formula->getLeft());
	simplify(formula->getRight());

	// simplify a & b 
	if (formula->getOp() == Operator::AND) {
		if (formula->getLeft()->isEqual(formula->getRight().get())) {
			formula = move(formula->getLeft());
		}
		else if (formula->getLeft()->getOp() == Operator::TRUE) {
			formula = move(formula->getRight());
		}
		else if (formula->getRight()->getOp() == Operator::TRUE) {
			formula = move(formula->getLeft());
		}
		else if (formula->getLeft()->getOp() == Operator::FALSE) {
			formula = Fptr(new Formula (Operator::FALSE));
		}
		else if (formula->getRight()->getOp() == Operator::FALSE) {
			formula = Fptr(new Formula (Operator::FALSE));
		}
	}

	// simplify a | b
	else if (formula->getOp() == Operator::OR) {
		if (formula->getLeft()->isEqual(formula->getRight().get())) {
			formula = move(formula->getLeft());
		}
		else if (formula->getLeft()->getOp() == Operator::TRUE) {
			formula = Fptr(new Formula (Operator::TRUE));
		}
		else if (formula->getRight()->getOp() == Operator::TRUE) {
			formula = Fptr(new Formula (Operator::TRUE));
		}
		else if (formula->getLeft()->getOp() == Operator::FALSE) {
			formula = move(formula->getRight());
		}
		else if (formula->getRight()->getOp() == Operator::FALSE) {
			formula = move(formula->getLeft());
		}
	}

	// simplify a => b
	else if (formula->getOp() == Operator::IMPLIES) {
		if (formula->getLeft()->isEqual(formula->getRight().get())) {
			formula = Fptr(new Formula (Operator::TRUE));
		}
		else if (formula->getLeft()->getOp() == Operator::TRUE) {
			formula = move(formula->getRight());
		}
		else if (formula->getRight()->getOp() == Operator::TRUE) {
			formula = Fptr(new Formula (Operator::TRUE));
		}
		else if (formula->getLeft()->getOp() == Operator::FALSE) {
			formula = Fptr(new Formula (Operator::TRUE));
		}
		else if (formula->getLeft()->getOp() == Operator::FALSE) {
		}
	}

	// simplify a <=> b
	else if (formula->getOp() == Operator::EQUAL) {
		if (formula->getLeft()->isEqual(formula->getRight().get())) {
			formula = Fptr(new Formula (Operator::TRUE));
		}
		else if (formula->getLeft()->getOp() == Operator::TRUE) {
			formula = move(formula->getRight());
		}
		else if (formula->getRight()->getOp() == Operator::TRUE) {
			formula = move(formula->getLeft());
		}
		else if (formula->getLeft()->getOp() == Operator::FALSE) {
			formula->getLeft().swap(formula->getRight());
			formula->setOp(Operator::IMPLIES);
		}
		else if (formula->getRight()->getOp() == Operator::FALSE) {
			formula->setOp(Operator::IMPLIES);
		}
	}
	return;
}

/*
Introduce a goal variable to a formula Q in the form Q -> goal.
If Q is of the form a => b, we further simplify.
 */
void Clausifier::introduceGoal(Fptr& formula) {
	// If formula is of the form a => b, return a & introduceGoal(b).
	if (formula->getOp() == Operator::IMPLIES) {
		introduceGoal(formula->getRight());
		formula->setOp(Operator::AND);
	}
	//Otherwise return formula => goal.
	else {
		//Fptr temp = move(formula);
		//formula = Fptr(new Formula(nullptr, new Formula("#goal"), Operator::IMPLIES));
		//formula->setLeft(move(temp));
		Fptr temp = Fptr(new Formula(nullptr, new Formula("#goal"), Operator::IMPLIES));
		temp->setLeft(move(formula));
		formula = move(temp);
	}
}

/*
Returns true if for a given op the formula is of the form a op b op c op d ...
 */
bool Clausifier::soleOperatorFormula(Formula& formula, Operator op) {
	if (formula.isAtomic()) return true;
	else if (formula.getOp() == op) {
		bool sole = soleOperatorFormula(*formula.getLeft(),op);
		sole &= soleOperatorFormula(*formula.getRight(),op);
		return sole;
	}
	else return false;
}

/*
Returns the lowest subformulae that are connected to the root formula via a 
single operator.
For example, a & b & (c | d) & (e => f), becomes a, b, (c | d), (e => f).
 */
vector<Fptr> Clausifier::extractSubformulae(Fptr& formula, Operator op) {
	vector<Fptr> lowestSubs;
	if (formula->getOp() != op) return lowestSubs;
	queue<Fptr> subformulae;
	subformulae.push(move(formula->getLeft()));
	subformulae.push(move(formula->getRight()));
	while (!subformulae.empty()) {
		if (subformulae.front()->isAtomic()) {
			lowestSubs.push_back(move(subformulae.front()));
			subformulae.pop();
		}
		else if (subformulae.front()->getOp() == op) {
			subformulae.push(move(subformulae.front()->getLeft()));
			subformulae.push(move(subformulae.front()->getRight()));
			subformulae.pop();
		}
		else {
			lowestSubs.push_back(move(subformulae.front()));
			subformulae.pop();
		}
	}
	return lowestSubs;
}

/*
Constructs a formula out of the given subformula and operator.
For example, ([a,b,(c => d)], &) becomes a & b & (c => d)
 */
Formula* Clausifier::constructSoleOperatorFormula(std::vector<Fptr>& subs, Operator op) {
	if (subs.size() < 2) return nullptr;
	Formula *formula = new Formula (nullptr, nullptr, op);
	formula->setLeft(move(subs.back()));
	subs.pop_back();
	formula->setRight(move(subs.back()));
	subs.pop_back();
	while (!subs.empty()) {
		formula = new Formula (nullptr, formula, op);
		formula->setLeft(move(subs.back()));
		subs.pop_back();
	}
	return formula;
}

/*
Returns true if the formula is of the form a & b & ... => c | d | ...
 */
bool Clausifier::isClassical(Formula& formula) {
	if (formula.getOp() == Operator::IMPLIES &&
			soleOperatorFormula(*formula.getLeft(), Operator::AND) &&
			soleOperatorFormula(*formula.getRight(), Operator::OR)) {
		return true;
	} 
	else return false;
}

/*
Returns true if the formula is of the form (a => b) => c
 */
bool Clausifier::isImplication(Formula& formula) {
	if (formula.getOp() == Operator::IMPLIES &&
			formula.getRight()->isAtomic() &&
			formula.getLeft()->getOp() == Operator::IMPLIES &&
			formula.getLeft()->getLeft()->isAtomic() &&
			formula.getLeft()->getRight()->isAtomic()) {
		return true;
	} 
	else return false;
}

/*
Converts an appropriate formula to a classical clause.
 */
CClause Clausifier::formulaToClassical(Fptr& formula) {
	if (!isClassical(*formula)) {
		cerr << "ERROR: formulaToClassical recieved non-classical formula" << endl;
		exit(1);
	}
	vector<string> left, right;
	queue<Fptr> leftQueue, rightQueue;
	leftQueue.push(move(formula->getLeft()));
	while (!leftQueue.empty()) {
		if (leftQueue.front()->isAtomic()) {
			left.push_back(leftQueue.front()->getVar());
			leftQueue.pop();
		}
		else {
			leftQueue.push(move(leftQueue.front()->getLeft()));
			leftQueue.push(move(leftQueue.front()->getRight()));
			leftQueue.pop();
		}
	}
	rightQueue.push(move(formula->getRight()));
	while (!rightQueue.empty()) {
		if (rightQueue.front()->isAtomic()) {
			right.push_back(rightQueue.front()->getVar());
			rightQueue.pop();
		}
		else {
			rightQueue.push(move(rightQueue.front()->getLeft()));
			rightQueue.push(move(rightQueue.front()->getRight()));
			rightQueue.pop();
		}
	}
	return CClause(left,right);
}

/*
Converts an appropriate formula to a classical clause.
 */
IClause Clausifier::formulaToImplication(Fptr& formula) {
	if (!isImplication(*formula)) {
		cerr << "ERROR: formulaToImplication recieved non-implication formula" << endl;
		exit(1);
	}
	string left = formula->getLeft()->getLeft()->getVar();
	string mid = formula->getLeft()->getRight()->getVar();
	string right = formula->getRight()->getVar();
	return IClause(left,mid,right);
}