#include "clausifier/clausifierFactory.h"

using namespace std;

unique_ptr<Clausifier> clausifierFactory::getClausifier(string clausifierType) {
	return unique_ptr<Clausifier>(new hunchClausifier);
}