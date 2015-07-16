#include "clausifier/hunchClausifier.h"

using namespace std;

/*
Dismantles a formula into a set of classical and implication clauses.
 */
void hunchClausifier::clausify(Fptr& mainFormula) {
	introduceGoal(mainFormula);
	cout << "goal introduced " << mainFormula->toString() << endl;
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
		formulae.pop();
		switch (currentFormula->getOp()) {

			case (Operator::ATOM): {
				classical.push_back(CClause("",currentFormula->getVar()));
				//CClause clause = CClause(NULL,NULL);
				//clause.addRight(currentFormula->getVar());
				break;
			}
			case (Operator::TRUE):
				//todo
				break;

			case (Operator::FALSE):
				//todo
				break;

			case (Operator::AND):
				formulae.push(move(currentFormula->getLeft()));
				formulae.push(move(currentFormula->getRight()));
				break;

			// a | b | A ... becomes [] -> [a,b,pA ...], adds [pA] -> [B], ...
			case (Operator::OR): {
				cout << "op is or\n";
				CClause clause = CClause("","");
				queue<Fptr> orFormulae;
				orFormulae.push(move(currentFormula->getLeft()));
				orFormulae.push(move(currentFormula->getRight()));
				while (!orFormulae.empty()) {
					Fptr currentOr = move(orFormulae.front());
					orFormulae.pop();
					cout << "current Or" << currentOr->toString() << endl;
					if (currentOr->getOp() == Operator::ATOM) {
									cout << "before\n";
						clause.addRight(currentOr->getVar());
									cout << "after\n";
					}
					else if (currentOr->getOp() == Operator::OR) {
						orFormulae.push(move(currentOr->getLeft()));
						orFormulae.push(move(currentOr->getRight()));
					}
					else {
						//todo rename
					}
				}
				classical.push_back(clause);
				break;
			}

			case (Operator::IMPLIES):
				//todo
				formulae.push(move(currentFormula->getLeft()));
				formulae.push(move(currentFormula->getRight()));
				break;

			case (Operator::EQUAL):
				//todo
				break;
		}
	}
	cout << "CLAUSES:" << endl;
	for (CClause i : classical) {
		cout << i.toString() << endl;
	}
}