#include "parser.h"

using namespace std;

/*
Creates a Formula corresponding to the input string.
*/
Formula* Parser::parse(string input) {
	vector<string> *tokens = tokenise(input);
	//Formula* mainFormula = new Formula();
	//return mainFormula;
}

/*
Splits a string into tokens of ),(,~,&,|,=>,<=>,[0-9a-z]+.
*/
vector<string>* Parser::tokenise(string input) {
	vector<string>* tokens = new vector<string>();
	for (int i = 0; i < input.size(); ++i) {
		if(isspace(input.at(i))) continue;
		if (input.at(i) == '(') tokens->push_back("(");
		else if (input.at(i) == ')') tokens->push_back(")");
		else if (input.at(i) == '&') tokens->push_back("&");
		else if (input.at(i) == '|') tokens->push_back("|");
		else if (input.at(i) == '~') tokens->push_back("~");
		else if (isalnum(input.at(i))) {
			string atom = string(1,input.at(i));
			for (int j = i + 1; j < input.size(); ++j) {
				if (isalnum(input.at(j))) {
					atom.push_back(input.at(j));
					i++;
				}
				else break;
			}
			tokens->push_back(atom);
		}
		else if (input.at(i) == '=') {
			if (i + 1 < input.size() && input.at(i + 1) == '>') {
				tokens->push_back("=>");
				i = i + 1;
			}
			else {
				cerr << "ERROR: tokeniser expected > after =.\n";
				exit(1);
			}
		} 
		else if (input.at(i) == '<') {
			if (i + 2 < input.size() && input.at(i + 1) == '=' && input.at(i + 2) == '>') {
				tokens->push_back("<=>");
				i = i + 2;
			}
			else {
				cerr << "ERROR: tokeniser expected => after <.\n";
				exit(1);
			}
		} 
		else {
			cerr << "ERROR: tokeniser does not recognise character: '" << input.at(i) << "'.\n";
			exit(1);
		}
	}
	return tokens;
}