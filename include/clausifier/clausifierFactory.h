#ifndef CLAUSIFIER_FACTORY_H
#define CLAUSIFIER_FACTORY_H

#include <string>
#include <memory>
#include "options.h"
#include "clausifier/clausifier.h"
#include "clausifier/hunchClausifier.h"

class ClausifierFactory
{

public:
	static std::unique_ptr<Clausifier> getClausifier(Options);
};

#endif // CLAUSIFIER_FACTORY_H
