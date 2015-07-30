#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <vector>

class Structure {

public:
	virtual void addClause(std::vector<int>) = 0;
	virtual bool isSatisfiable(std::vector<int>) = 0;
	virtual void simplify() = 0;
private:
};

#endif // STRUCTURE_H
