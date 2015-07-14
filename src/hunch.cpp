#include "hunch.h"

using namespace std;

int main(int argc, char const *argv[]) {

	//Read input.
	string input;
	getline(cin,input);
	if (input.empty()) {
		cout << "Empty formula is valid.\n";
		return 0;
	}

	//Parse input to formula.
	Parser parser;
	Formula* mainFormula = parser.parse(input);

	//Clausify formula.
	unique_ptr<Clausifier> clausifier = clausifierFactory::getClausifier("hunch");
	clausifier->clausify(mainFormula);


	cout << mainFormula->toString() << endl;
	delete(mainFormula);
	

}