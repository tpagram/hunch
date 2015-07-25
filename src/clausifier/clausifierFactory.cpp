#include "clausifier/clausifierFactory.h"

using namespace std;

/*
Return a particular clausifier implentation.
 */
unique_ptr<Clausifier> ClausifierFactory::getClausifier(string clausifierType) {
	return unique_ptr<Clausifier>(new hunchClausifier);
}