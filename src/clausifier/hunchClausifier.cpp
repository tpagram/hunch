#include "clausifier/hunchClausifier.h"

using namespace std;

/*
Dismantles a formula into a set of classical and implication clauses.
 */
void hunchClausifier::clausify(Fptr& mainFormula) {

	introduceGoal(mainFormula);
	cout << "goal introduced: " << mainFormula->toString() << endl;

	debug = mainFormula->toString();

	simplify(mainFormula);
	cout << "simplified: " << mainFormula->toString() << endl;
	
	flattenEquivalence(mainFormula);
	cout << "equivalence removed: " << mainFormula->toString() << endl;

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
		cout << "current: " << currentFormula->toString() << endl;
		if (isClassical(*currentFormula)) {
			classical.push_back(formulaToClassical(currentFormula));
			continue;
		}
		else if (isImplication(*currentFormula)) {
			implication.push_back(formulaToImplication(currentFormula));
			continue;
		}
		switch (currentFormula->getOp()) {

			// atom a becomes [] -> [a]
			case Operator::ATOM:
				classical.push_back(CClause("",currentFormula->getVar()));
				break;

			// if a & b, extract clauses from a and b separately.
			case Operator::AND:
				formulae.push(move(currentFormula->getLeft()));
				formulae.push(move(currentFormula->getRight()));
				break;

			// a | b | A ... becomes [] -> [a,b,pA ...], adds [pA] -> [B], ...
			case Operator::OR: {
				vector<Fptr> orFormulae = extractSubformulae(currentFormula, Operator::OR);
				CClause clause = CClause("","");
				for (Fptr& f : orFormulae) {
					if (f->getOp() == Operator::ATOM) {
						clause.addRight(f->getVar());
					}
					else {
						pair<Formula*,string> newName = rename(move(f), Direction::implies);
						if (newName.first != NULL) formulae.push(Fptr(newName.first));
						clause.addRight(newName.second);
					}
				}
				classical.push_back(clause);
				break;
			}

			case Operator::IMPLIES:
				// (A => B) => C  becomes (pA => pB) => pC, pA => A, B => pB 
				if (currentFormula->getLeft()->getOp() == Operator::IMPLIES) {
					string left,mid,right;
					if (currentFormula->getLeft()->getLeft()->isAtomic()) {
						left = currentFormula->getLeft()->getLeft()->getVar();
					}
					else {
						pair<Formula*,string> newName = rename(
											move(currentFormula->getLeft()->getLeft()), Direction::implies);
						left = newName.second;
						if (newName.first != NULL) formulae.push(Fptr(newName.first));
					}
					if (currentFormula->getLeft()->getRight()->isAtomic()) {
						mid = currentFormula->getLeft()->getRight()->getVar();
					}
					else {
						pair<Formula*,string> newName = rename(
											move(currentFormula->getLeft()->getRight()), Direction::implied);
						mid = newName.second;
						if (newName.first != NULL) formulae.push(Fptr(newName.first));
					}
					if (currentFormula->getRight()->isAtomic()) {
						right = currentFormula->getRight()->getVar();
					}
					else {
						pair<Formula*,string> newName = rename(
											move(currentFormula->getRight()), Direction::implies);
						right = newName.second;
						if (newName.first != NULL) formulae.push(Fptr(newName.first));
					}
					//implication.push_back(IClause(left, mid, right));
					formulae.push(Fptr(new Formula(new Formula(new Formula (left), 
																new Formula(mid),
																Operator::IMPLIES),
													new Formula(right),
													Operator::IMPLIES)));
				}

				// A | B => C becomes A => C, B => C.
				else if (currentFormula->getLeft()->getOp() == Operator::OR) {
					Fptr newLeftFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					Fptr newRightFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					string oldName = checkForName(*currentFormula->getLeft()->getLeft(),Direction::implied);
					if (oldName == "") {
						newLeftFormula->setLeft(move(currentFormula->getLeft()->getLeft()));
					}
					else {
						newLeftFormula->setLeft(Fptr(new Formula(oldName)));
					}
					newLeftFormula->setRight(Fptr(new Formula(*currentFormula->getRight())));
					oldName = checkForName(*currentFormula->getLeft()->getRight(),Direction::implied);
					if (oldName == "") {
						newRightFormula->setLeft(move(currentFormula->getLeft()->getRight()));
					}
					else {
						newRightFormula->setLeft(Fptr(new Formula(oldName)));
					}
					newRightFormula->setRight(move(currentFormula->getRight()));
					formulae.push(move(newLeftFormula));
					formulae.push(move(newRightFormula));
				}

				// A => B & C becomes A => B, A => C.
				else if (currentFormula->getRight()->getOp() == Operator::AND) {
					Fptr newLeftFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					Fptr newRightFormula = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));

					newLeftFormula->setLeft(Fptr(new Formula(*currentFormula->getLeft())));
					string oldName = checkForName(*currentFormula->getRight()->getLeft(),Direction::implies);
					if (oldName == "") {
						newLeftFormula->setRight(move(currentFormula->getRight()->getLeft()));
					}
					else {
						newLeftFormula->setRight(Fptr(new Formula(oldName)));
					}

					newRightFormula->setLeft(move(currentFormula->getLeft()));
					oldName = checkForName(*currentFormula->getRight()->getRight(),Direction::implies);
					if (oldName == "") {
						newRightFormula->setRight(move(currentFormula->getRight()->getRight()));
					}
					else {
						newRightFormula->setRight(Fptr(new Formula(oldName)));
					}

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

				// A & B & c ... => D becomes pA & pB & c ... => D, with A => pA, ...
				else if (currentFormula->getLeft()->getOp() == Operator::AND &&
							!soleOperatorFormula(*currentFormula->getLeft(),Operator::AND)) {
					cout << "uhuh\n";
					vector<Fptr> newAndFormulae;
					vector<Fptr> andFormulae = extractSubformulae(currentFormula->getLeft(), Operator::AND);
					cout << "uhuh\n";
					for (Fptr& f : andFormulae) {
						if (f->isAtomic()) newAndFormulae.push_back(move(f));
						else {
							pair<Formula*,string> newName = rename(move(f), Direction::implied);
							if (newName.first != NULL) formulae.push(Fptr(newName.first));
							newAndFormulae.push_back(Fptr(new Formula(newName.second)));
						}
					}
					Fptr temp1 = Fptr(constructSoleOperatorFormula(newAndFormulae,Operator::AND));
					Fptr temp2 = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					temp2->setLeft(move(temp1));
					temp2->setRight(move(currentFormula->getRight()));
					formulae.push(move(temp2));
				}

				// A => B | C | d ... becomes A => pB | pC | d..., with pB => B, ...
				else if (currentFormula->getRight()->getOp() == Operator::OR &&
							!soleOperatorFormula(*currentFormula->getRight(),Operator::OR)) {
					vector<Fptr> newOrFormulae;
					vector<Fptr> orFormulae = extractSubformulae(currentFormula->getRight(), Operator::OR);
					for (Fptr& f : orFormulae) {
						if (f->isAtomic()) newOrFormulae.push_back(move(f));
						else {
							pair<Formula*,string> newName = rename(move(f), Direction::implies);
							if (newName.first != NULL) {
								cout << "imp or " << newName.first->toString() << endl;
								formulae.push(Fptr(newName.first));
							}
							newOrFormulae.push_back(Fptr(new Formula(newName.second)));
						}
					}
					Fptr temp1 = Fptr(constructSoleOperatorFormula(newOrFormulae,Operator::OR));
					Fptr temp2 = Fptr(new Formula(nullptr, nullptr, Operator::IMPLIES));
					temp2->setLeft(move(currentFormula->getLeft()));
					temp2->setRight(move(temp1));
					formulae.push(move(temp2));
				}
				else {
					cerr << "ERROR: Clausifier encountered an unrecognised form" << endl;
					exit(1);
				}
				break;

			case Operator::EQUAL: 
			case Operator::FALSE: 
			case Operator::TRUE:
				cerr << "ERROR: Unexpected operator in extractClauses." << endl;
				exit(1);
		}
	}
	cout << "VARIABLES INTRODUCED: " << renameCounter << endl;
	cout << "CLASSICAL CLAUSES (" << classical.size() << ") :" << endl;
	for (CClause i : classical) {
		cout << i.toString() << endl;
	}
	cout << "IMPLICATION CLAUSES (" << implication.size() << ") :" << endl;
	for (IClause i : implication) {
		cout << i.toString() << endl;
	}
	cout << "Ready for copypaste:" << endl;
	cout << "(";
	for (CClause i : classical) {
		cout << "(" << i.toString() << ") &";
	}
	for (int i = 0; i < implication.size(); i++) {
		if (i == (implication.size()-1)) cout << "(" << implication.at(i).toString() << ")";
		else cout << "(" << implication.at(i).toString() << ") &";
	}
	cout << ") => (" << debug << ")" <<endl;
}

string hunchClausifier::newName() {
	string newName = "r" + to_string(renameCounter);
	renameCounter++;
	return newName;
}

pair<Formula*,string> hunchClausifier::rename(Fptr formula, Direction dir) {
	pair<Formula*,string> renamed;
	auto i = nameMap.find(*formula);
	if (i == nameMap.end()) { 
		renamed.second = "r" + to_string(renameCounter);
		renameCounter++;
		nameMap[*formula] = make_pair(renamed.second, dir);
		//nameMap->emplace(*formula,make_pair(renamed.second, dir));
		renamed.first = createRenamedFormula(renamed.second, move(formula), dir);
	}
	else {
		renamed.second = i->second.first;
		if ((dir == Direction::implies && i->second.second == Direction::implied) ||
				(dir == Direction::implied && i->second.second == Direction::implies)) {
			i->second.second = Direction::equiv;
			renamed.first = createRenamedFormula(renamed.second, move(formula), dir);
		}
	}
	return renamed;
}

Formula* hunchClausifier::createRenamedFormula(string name, Fptr formula, Direction dir) {
	Formula* renamed;
	switch (dir) {
		case Direction::implies:
			renamed = new Formula (new Formula(name), nullptr, Operator::IMPLIES);
			renamed->setRight(move(formula));
			std::cout << renamed->toString() << endl;
			break;
		case Direction::implied:
			renamed = new Formula (nullptr, new Formula(name), Operator::IMPLIES);
			renamed->setLeft(move(formula));
			break;
		case Direction::equiv:
			Fptr clone = Fptr(new Formula(*formula));
			renamed = new Formula ( 
				createRenamedFormula(name, move(clone), Direction::implies),
				createRenamedFormula(name, move(formula), Direction::implied),
				Operator::AND );
			break;

	}
	return renamed;
}

string hunchClausifier::checkForName(Formula formula, Direction dir) {
	auto i = nameMap.find(formula);
	if (i == nameMap.end()) return "";
	else if (i->second.second != Direction::equiv && i->second.second != dir) return "";
	else return i->second.first;
}

