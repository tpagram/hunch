#ifndef HUNCH_CLAUSIFIER_H
#define HUNCH_CLAUSIFIER_H

#include "clausifier/clausifier.h"
#include <queue>

class hunchClausifier : public Clausifier
{
public:
	virtual void clausify(Fptr&);

private:
	virtual void extractClauses(std::queue<Fptr>&);
};


#endif // HUNCH_CLAUSIFIER_H
