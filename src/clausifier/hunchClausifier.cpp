#include "clausifier/hunchClausifier.h"

using namespace std;

/*
Dismantles a formula into a set of clauses ready for solving.
 */
ClausalForm* hunchClausifier::clausify(Fptr& mainFormula) {

	introduceGoal(mainFormula);
	if (verbose) cout << "goal introduced: " << mainFormula->toString() << endl;

	simplify(mainFormula); //todo fix b & (a & b) not simplifying to a & b.
	if (verbose) cout << "simplified: " << mainFormula->toString() << endl;

	std::cout << "Extracting clauses... ";
	ClausalForm* clauses = extractClauses(mainFormula);
	std::cout << "Done!\n";

	cout << "VARIABLES INTRODUCED: " << renameCounter << endl;
	clauses->printClauses("classical");
	clauses->printClauses("implication");

	return clauses;
}


/*
Extracts classical and implication clauses from formula.
 */
ClausalForm* hunchClausifier::extractClauses(Fptr& mainFormula) {
	ClausalForm *clauseform = new ClausalForm();
	queue<Fptr> formulae;
	formulae.push(move(mainFormula));
	while (!formulae.empty()) {

		Fptr currentFormula = move(formulae.front());
		formulae.pop();
		if (isClassical(*currentFormula)) {
			clauseform->addClause(formulaToClassical(currentFormula),"classical");
			continue;
		}
		else if (isImplication(*currentFormula)) {
			clauseform->addClause(formulaToImplication(currentFormula),"implication");
			continue;
		}
		switch (currentFormula->getOp()) {

			// atom a becomes [] -> [a]
			case Operator::ATOM:
				clauseform->addClause(new CClause("",currentFormula->getVar()),"classical");
				break;

			// if a & b, extract clauses from a and b separately.
			case Operator::AND:
				formulae.push(move(currentFormula->getLeft()));
				formulae.push(move(currentFormula->getRight()));
				break;

			// a | b | A ... becomes [] -> [a,b,pA ...], adds [pA] -> [B], ...
			case Operator::OR: {
				vector<Fptr> orFormulae = extractSubformulae(currentFormula, Operator::OR);
				CClause *clause = new CClause("","");
				for (Fptr& f : orFormulae) {
					if (f->getOp() == Operator::ATOM) {
						clause->addRight(f->getVar());
					}
					else {
						pair<Formula*,string> newName = rename(move(f), Direction::implies);
						if (newName.first != NULL) formulae.push(Fptr(newName.first));
						clause->addRight(newName.second);
					}
				}
				clauseform->addClause(clause,"classical");
				break;
			}

			// todo
			case Operator::EQUAL: {
				Fptr temp = Fptr(new Formula(*currentFormula));
				Fptr temp2 = move(temp->getLeft());
				temp->setLeft(move(temp->getRight()));
				temp->setRight(move(temp2));
				temp->setOp(Operator::IMPLIES);
				formulae.push(move(temp));

				currentFormula->setOp(Operator::IMPLIES);
				formulae.push(move(currentFormula));
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

				// A => (B <=> C) becomes A & B => C, A & C => B
				else if (currentFormula->getRight()->getOp() == Operator::EQUAL) {
					Fptr newLeftFormula = Fptr(new Formula(nullptr,nullptr, Operator::IMPLIES));
					newLeftFormula->setLeft(Fptr(new Formula(nullptr,nullptr,Operator::AND)));
					newLeftFormula->getLeft()->setLeft(Fptr(new Formula(*currentFormula->getLeft())));
					newLeftFormula->getLeft()->setRight(Fptr(new Formula(*currentFormula->getRight()->getLeft())));
					newLeftFormula->setRight(Fptr(new Formula(*currentFormula->getRight()->getRight())));
					formulae.push(move(newLeftFormula));

					Fptr newRightFormula = Fptr(new Formula(nullptr,nullptr, Operator::IMPLIES));
					newRightFormula->setLeft(Fptr(new Formula(nullptr,nullptr,Operator::AND)));
					newRightFormula->getLeft()->setLeft(move(currentFormula->getLeft()));
					newRightFormula->getLeft()->setRight(move(currentFormula->getRight()->getRight()));
					newRightFormula->setRight(move(currentFormula->getRight()->getLeft()));
					formulae.push(move(newRightFormula));
				}

				// A & B & c ... => D becomes pA & pB & c ... => D, with A => pA, ...
				else if (currentFormula->getLeft()->getOp() == Operator::AND &&
							!soleOperatorFormula(*currentFormula->getLeft(),Operator::AND)) {
					vector<Fptr> newAndFormulae;
					vector<Fptr> andFormulae = extractSubformulae(currentFormula->getLeft(), Operator::AND);
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

				// (A <=> B) => C) becomes ((A => B) & (B => A)) => C)
				else if (currentFormula->getLeft()->getOp() == Operator::EQUAL) {
					cout << "ok\n";
					Fptr temp = Fptr(new Formula(nullptr, nullptr, Operator::AND));
					temp->setLeft(Fptr(new Formula(nullptr,nullptr, Operator::IMPLIES)));
					temp->setRight(Fptr(new Formula(nullptr,nullptr, Operator::IMPLIES)));

					temp->getLeft()->setLeft(Fptr(new Formula(*currentFormula->getLeft()->getLeft())));
					temp->getLeft()->setRight(Fptr(new Formula(*currentFormula->getLeft()->getRight())));

					temp->getRight()->setLeft(move(currentFormula->getLeft()->getRight()));
					temp->getRight()->setRight(move(currentFormula->getLeft()->getLeft()));

					currentFormula->setLeft(move(temp));
					formulae.push(move(currentFormula));
				}
				else {
					cerr << "ERROR: Clausifier encountered an unrecognised form" << endl;
					exit(1);
				}
				break;

			case Operator::FALSE: 
			case Operator::TRUE:
				cerr << "ERROR: Unexpected operator in extractClauses." << endl;
				exit(1);
		}
	}
	return clauseform;
}

/*
Renames a formula with a particular implication direction, if not already renamed.
 */
pair<Formula*,string> hunchClausifier::rename(Fptr formula, Direction dir) {
	pair<Formula*,string> renamed;
	auto i = nameMap->find(*formula);
	if (i == nameMap->end()) { 
		renamed.second = "r" + to_string(renameCounter);
		renameCounter++;
		//nameMap[*formula] = make_pair(renamed.second, dir);
		nameMap->emplace(*formula,make_pair(renamed.second, dir));
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

/*
Given a name pA, formula A and implication directin, returns a formula pA =>/<=/<=> A.
 */
Formula* hunchClausifier::createRenamedFormula(string name, Fptr formula, Direction dir) {
	Formula* renamed;
	switch (dir) {
		case Direction::implies:
			renamed = new Formula (new Formula(name), nullptr, Operator::IMPLIES);
			renamed->setRight(move(formula));
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

/*
Checks whether a formula has already been renamed.
 */
string hunchClausifier::checkForName(Formula formula, Direction dir) {
	auto i = nameMap->find(formula);
	if (i == nameMap->end()) return "";
	else if (i->second.second != Direction::equiv && i->second.second != dir) return "";
	else return i->second.first;
}

