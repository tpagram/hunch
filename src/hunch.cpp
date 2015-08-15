#include "hunch.h"

using namespace std;

int main(int argc, char **argv) {

	//Parse options
	Options options = Options(argc,argv);

	//Read formula from standard input.
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
	unique_ptr<Clausifier> clausifier = ClausifierFactory::getClausifier(options);
	CFptr clausalForm = CFptr(clausifier->clausify(mainFormula));

	//Solve formula.
	unique_ptr<Solver> solver = SolverFactory::getSolver(options);
	bool isValid = solver->solve(clausalForm);
	if (isValid) cout << "===== VALID =====" << endl;
	else cout << "===== INVALID =====" << endl;

	return 0;
}