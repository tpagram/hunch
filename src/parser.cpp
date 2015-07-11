#include "parser.h"

using namespace std;

/*
Creates a Formula corresponding to an input string.
*/
Formula* Parser::parse(string input) {
	queue<string> *tokens = tokenise(input);
	return parseEquality(tokens);
}

/*
Splits a string into tokens of ),(,~,&,|,=>,<=>,[0-9a-z]+.
*/
queue<string>* Parser::tokenise(string input) {
	queue<string>* tokens = new queue<string>();
	for (int i = 0; i < input.size(); i++) {
		if(isspace(input.at(i))) continue;
		if (input.at(i) == '(') tokens->push("(");
		else if (input.at(i) == ')') tokens->push(")");
		else if (input.at(i) == '&') tokens->push("&");
		else if (input.at(i) == '|') tokens->push("|");
		else if (input.at(i) == '~') tokens->push("~");
		else if (isalnum(input.at(i))) {
			string atom = string(1,input.at(i));
			for (int j = i + 1; j < input.size(); ++j) {
				if (isalnum(input.at(j))) {
					atom.push_back(input.at(j));
					i++;
				}
				else break;
			}
			tokens->push(atom);
		}
		else if (input.at(i) == '=') {
			if (i + 1 < input.size() && input.at(i + 1) == '>') {
				tokens->push("=>");
				i = i + 1;
			}
			else {
				cerr << "ERROR: tokeniser expected > after =.\n";
				exit(1);
			}
		} 
		else if (input.at(i) == '<') {
			if (i + 2 < input.size() && input.at(i + 1) == '=' && input.at(i + 2) == '>') {
				tokens->push("<=>");
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

	Formula* Parser::parseEquality(queue<string>* tokens) {
		Formula* left = parseImplication(tokens);
		if (!tokens->empty() && tokens->front() == "<=>") {
			tokens->pop();
			Formula* right = parseEquality(tokens);
			return new Formula(left, right, Operator::EQUAL);
		}
		else return left;
	}

	Formula* Parser::parseImplication(queue<string>* tokens) {
		Formula* left = parseDisjunction(tokens);
		if (!tokens->empty() && tokens->front() == "=>") {
			tokens->pop();
			Formula* right = parseImplication(tokens);
			return new Formula(left, right, Operator::IMPLIES);
		}
		else return left;
	}

	Formula* Parser::parseDisjunction(queue<string>* tokens) {
		Formula* left = parseConjunction(tokens);
		if (!tokens->empty() && tokens->front() == "|") {
			tokens->pop();
			Formula* right = parseDisjunction(tokens);
			return new Formula(left, right, Operator::OR);
		}
		else return left;
	}

	Formula* Parser::parseConjunction(queue<string>* tokens) {
		Formula* left = parseTerm(tokens);
		if (!tokens->empty() && tokens->front() == "&") {
			tokens->pop();
			Formula* right = parseConjunction(tokens);
			return new Formula(left, right, Operator::AND);
		}
		else return left;
	}

	Formula* Parser::parseTerm(queue<std::string>* tokens) {
		if (tokens->empty()) {
			cerr << "ERROR: parser finds empty term. \n";
			exit(1);
		}
		else if (tokens->front() == "true") {
			Formula* formula = new Formula(Operator::TRUE);
			tokens->pop();
			return formula;
		}
		else if (tokens->front() == "false") {
			Formula* formula = new Formula(Operator::FALSE);
			tokens->pop();
			return formula;
		}
		else if (isalnum(tokens->front()[0])) {
			Formula* formula = new Formula(tokens->front());
			tokens->pop();
			return formula;
		}
		else if (tokens->front() == "(") {
			tokens->pop();
			Formula* formula = parseEquality(tokens);
			if (tokens->front() == ")") {
				tokens->pop();
				return formula;
			}
			else {
				cerr << "ERROR: parser detected no closing bracket ')'\n";
				exit(1);
			}
		}
		else {
			cerr << "ERROR: parser detected unexpected symbol: " << tokens->front() << endl;
			exit(1);
		}
	}