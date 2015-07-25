#ifndef CLAUSIFIER_FACTORY_H
#define CLAUSIFIER_FACTORY_H

#include <string>
#include <memory>
#include "clausifier/clausifier.h"
#include "clausifier/hunchClausifier.h"

class ClausifierFactory
{

public:
	static std::unique_ptr<Clausifier> getClausifier(std::string);
};

#endif // CLAUSIFIER_FACTORY_H
