#include "hunch.h"

using namespace std;

int main(int argc, char const *argv[]) {

	string input;
	getline(cin,input);

	Parser parser;
	Formula* mainFormula = parser.parse(input);
	delete(mainFormula);
	

}