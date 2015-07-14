#include "clausifier/hunchClausifier.h"

using namespace std;

void hunchClausifier::clausify(Formula* mainFormula) {
	mainFormula = introduceGoal(mainFormula);
	cout << "goal introduced " << mainFormula->toString() << endl;
	mainFormula = simplify(mainFormula);
	cout << "simplified " << mainFormula->toString() << endl;
	return;
}