#ifndef HUNCH_CLAUSIFIER_H
#define HUNCH_CLAUSIFIER_H

#include "clausifier/clausifier.h"

class hunchClausifier : public Clausifier
{
	virtual void clausify(Formula*);
};


#endif // HUNCH_CLAUSIFIER_H
