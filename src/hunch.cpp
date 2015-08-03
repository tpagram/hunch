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
	unique_ptr<Parser> parser = unique_ptr<Parser>(new Parser);
	Fptr mainFormula = Fptr(parser->parse(input));
	
	//Clausify formula.
	unique_ptr<Clausifier> clausifier = ClausifierFactory::getClausifier("hunch");
	CFptr clausalForm = CFptr(clausifier->clausify(mainFormula));

	//Solver formula.
	unique_ptr<Solver> solver = SolverFactory::getSolver("CIF");
	bool isValid = solver->solve(clausalForm);

	if (isValid) cout << "===== VALID =====" << endl;
	else cout << "===== INVALID =====" << endl;
}