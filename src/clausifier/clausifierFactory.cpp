#include "clausifier/clausifierFactory.h"

using namespace std;

/*
Return a particular clausifier implentation.
 */
unique_ptr<Clausifier> ClausifierFactory::getClausifier(Options options) {
	if (options.clausifier == "hunch") return unique_ptr<Clausifier>(new hunchClausifier(options));
	else {
		cerr << "ClausifierFactory asked for unknown clausifier: " << options.clausifier << endl;
		exit(1);
	}
}