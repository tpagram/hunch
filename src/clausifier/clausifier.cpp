#include "clausifier/clausifier.h"
using namespace std;

/*
Simplifies a formula using a variety of rules.
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