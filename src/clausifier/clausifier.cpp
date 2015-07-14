#include "clausifier/clausifier.h"
using namespace std;

/*
Simplifies a formula using a variety of rules.
 */
Formula* Clausifier::simplify(Formula* formula) {
	// atom base cases
	if (formula->getOp() == Operator::ATOM) return formula;
	else if (formula->getOp() == Operator::TRUE) return formula;
	else if (formula->getOp() == Operator::FALSE) return formula;

	// recurse
	Formula* left = simplify(formula->getLeft());
	Formula* right = simplify(formula->getRight());

	// simplify a & b 
	if (formula->getOp() == Operator::AND) {
		if (left->isEqual(right)) return left;
		else if (left->getOp() == Operator::TRUE) return right;
		else if (right->getOp() == Operator::TRUE) return left;
		else if (left->getOp() == Operator::FALSE) return new Formula (Operator::FALSE);
		else if (right->getOp() == Operator::FALSE) return new Formula (Operator::FALSE);
		else return new Formula (left, right, Operator::AND);
	}
	// simplify a | b
	else if (formula->getOp() == Operator::OR) {
		if (left->isEqual(right)) return left;
		else if (left->getOp() == Operator::TRUE) return new Formula (Operator::TRUE);
		else if (right->getOp() == Operator::TRUE) return new Formula (Operator::TRUE);
		else if (left->getOp() == Operator::FALSE) return right;
		else if (right->getOp() == Operator::FALSE) return left;
		else return new Formula (left, right, Operator::OR);
	}
	// simplify a => b
	else if (formula->getOp() == Operator::IMPLIES) {
		if (left->isEqual(right)) return new Formula (Operator::TRUE);
		else if (left->getOp() == Operator::TRUE) return right;
		else if (right->getOp() == Operator::TRUE) return new Formula (Operator::TRUE);
		else if (left->getOp() == Operator::FALSE) return new Formula (Operator::TRUE);
		else if (right->getOp() == Operator::FALSE) return new Formula (left, right, Operator::IMPLIES);
		else return new Formula (left, right, Operator::IMPLIES);
	}
	// simplify a <=> b
	else if (formula->getOp() == Operator::EQUAL) {
		if (left->isEqual(right)) return new Formula (Operator::TRUE);
		else if (left->getOp() == Operator::TRUE) return right;
		else if (right->getOp() == Operator::TRUE) return left;
		else if (left->getOp() == Operator::FALSE) return new Formula (right, left, Operator::IMPLIES);
		else if (right->getOp() == Operator::FALSE) return new Formula (left, right, Operator::IMPLIES);
		else return new Formula (left, right, Operator::EQUAL);
	}
	else return new Formula (left, right, Operator::AND);
}

/*
Introduce a goal variable to a formula Q in the form Q -> goal.
If Q is of the form a => b, we further simplify.
 */
Formula* Clausifier::introduceGoal(Formula* formula) {
	Formula* goalFormula;
	// If formula is of the form a => b, return a & introduceGoal(b).
	if (formula->getOp() == Operator::IMPLIES) {
		goalFormula = introduceGoal(formula->getRight());
		goalFormula = new Formula (formula->getLeft(),goalFormula,Operator::AND);
	}
	//Otherwise return formula => goal.
	else {
		goalFormula = new Formula("#goal");
		goalFormula = new Formula(formula, goalFormula, Operator::IMPLIES);
	}
	return goalFormula;
}