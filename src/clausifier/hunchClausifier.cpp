#include "clausifier/hunchClausifier.h"

using namespace std;

/*
Dismantles a formula into a set of classical and implication clauses.
 */
void hunchClausifier::clausify(Fptr& mainFormula) {
	//introduceGoal(mainFormula);
	//cout << "goal introduced " << mainFormula->toString() << endl;
	simplify(mainFormula);
	cout << "simplified " << mainFormula->toString() << endl;
	queue<Fptr> formulae;
	formulae.push(move(mainFormula));
	extractClauses(formulae);
	return;
}

void hunchClausifier::extractClauses(queue<Fptr>& formulae) {
	vector<CClause> classical;
	vector<IClause> implication;
	while (!formulae.empty()) {
		Fptr currentFormula = move(formulae.front());
		cout << "current" << currentFormula->toString() << endl;
		formulae.pop();
		cout << "one\n";
		if (isClassical(*currentFormula)) {
			classical.push_back(formulaToClassical(currentFormula));
			continue;
		}
		else if (isImplication(*currentFormula)) {
			implication.push_back(formulaToImplication(currentFormula));
			continue;
		}
		cout << "one\n";
		switch (currentFormula->getOp()) {

			// atom a becomes [] -> [a]
			case (Operator::ATOM):
				classical.push_back(CClause("",currentFormula->getVar()));
				break;

			case (Operator::TRUE):
				//todo
				break;

			case (Operator::FALSE):
				//todo
				break;

			// if a & b, extract clauses from a and b separately.
			case (Operator::AND):
				formulae.push(move(currentFormula->getLeft()));
				formulae.push(move(currentFormula->getRight()));
				break;

			// a | b | A ... becomes [] -> [a,b,pA ...], adds [pA] -> [B], ...
			case (Operator::OR): {
				vector<Fptr> orFormulae = extractSubformulae(currentFormula, Operator::OR);
				CClause clause = CClause("","");
				for (Fptr& f : orFormulae) {
					if (f->getOp() == Operator::ATOM) {
						clause.addRight(f->getVar());
					}
					else {
						string newName = "r" + to_string(renameCounter);
						renameCounter++;
						Fptr newFormula = Fptr(new Formula(new Formula(newName), nullptr, Operator::IMPLIES));
						newFormula->setRight(move(f));
						formulae.push(move(newFormula));
						clause.addRight(newName);
					}
				}
				classical.push_back(clause);
				break;
			}

			case (Operator::IMPLIES):
				// A | B => C becomes A => C, B => C.
				if (currentFormula->getLeft()->getOp() == Operator::OR) {
					Fptr newLeftFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					Fptr newRightFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					newLeftFormula->setLeft(move(currentFormula->getLeft()->getLeft()));
					newLeftFormula->setRight(Fptr(new Formula(*currentFormula->getRight())));
					newRightFormula->setLeft(move(currentFormula->getLeft()->getRight()));
					newRightFormula->setRight(move(currentFormula->getRight()));
					formulae.push(move(newLeftFormula));
					formulae.push(move(newRightFormula));
				}
				// A => B & C becomes A => B, A => C.
				else if (currentFormula->getRight()->getOp() == Operator::AND) {
					Fptr newLeftFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					Fptr newRightFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					newLeftFormula->setLeft(Fptr(new Formula(*currentFormula->getLeft())));
					newLeftFormula->setRight(move(currentFormula->getRight()->getLeft()));
					newRightFormula->setLeft(move(currentFormula->getLeft()));
					newRightFormula->setRight(move(currentFormula->getRight()->getRight()));
					formulae.push(move(newLeftFormula));
					formulae.push(move(newRightFormula));
				}
				// A => (B => C) becomes A & B => C
				else if (currentFormula->getRight()->getOp() == Operator::IMPLIES) {
					Fptr temp = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					temp->setLeft(move(currentFormula->getLeft()));
					temp->setRight(move(currentFormula->getRight()->getLeft()));
					temp->setOp(Operator::AND);
					currentFormula->setLeft(move(temp));
					currentFormula->setRight(move(currentFormula->getRight()->getRight()));
					formulae.push(move(currentFormula));
				}
				// A & B & c ... => Z becomes pA & pB & c ... => Z, with A => pA, ...
				else if (currentFormula->getLeft()->getOp() == Operator::AND) {
					vector<Fptr> newAndFormulae;
					vector<Fptr> andFormulae = extractSubformulae(currentFormula->getLeft(), Operator::AND);
					for (Fptr& f : andFormulae) {
						if (f->getOp() == Operator::ATOM) newAndFormulae.push_back(move(f));
						else {
							string newName = "r" + to_string(renameCounter);
							renameCounter++;
							newAndFormulae.push_back(Fptr(new Formula(newName)));
							Fptr renamedFormula = Fptr(new Formula (nullptr,new Formula(newName), Operator::IMPLIES));
							renamedFormula->setLeft(move(f));
							formulae.push(move(renamedFormula));
						}
					}
					Fptr temp1 = Fptr(constructSoleOperatorFormula(newAndFormulae,Operator::AND));
					Fptr temp2 = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					temp2->setLeft(move(temp1));
					temp2->setRight(move(currentFormula->getRight()));
					formulae.push(move(temp2));
				}
				else {
					formulae.push(move(currentFormula->getLeft()));
					formulae.push(move(currentFormula->getRight()));
				}
				break;

			case (Operator::EQUAL):
				//todo
				break;
		}
	}
	cout << "CLASSICAL CLAUSES:" << endl;
	for (CClause i : classical) {
		cout << i.toString() << endl;
	}
	cout << "IMPLICATION CLAUSES:" << endl;
	for (IClause i : implication) {
		cout << i.toString() << endl;
	}
}