#include "clausifier/hunchClausifier.h"

using namespace std;

void hunchClausifier::clausify(Fptr& mainFormula) {
	introduceGoal(mainFormula);
	cout << "goal introduced " << mainFormula->toString() << endl;
	simplify(mainFormula);
	cout << "simplified " << mainFormula->toString() << endl;
	return;
}