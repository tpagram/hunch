#include "options.h"

using namespace std;

Options::Options(int argc, char **argv) {
    clausifier = "hunch";
    solver = "cif";
    structure = "minisat";
    verbosity = 0;
    
	for (int i = 1; i < argc; ++i) {
		if (strncmp(argv[i], "-help", 5) == 0) printHelp();
		else if (strncmp(argv[i], "-c", 2) == 0) {
			i++;
			if (i >= argc || strncmp(argv[i], "-", 1) == 0) {
				cerr << "ERROR: -c needs argument" << endl;
				printHelp();
			}
			else if (strncmp(argv[i], "hunch", 5) == 0) clausifier = "hunch"; 
			else {
				cerr << "ERROR: Unknown option for -c: " << argv[i] << endl;
				printHelp();
			}
		} 
		else if (strncmp(argv[i], "-s", 2) == 0) {
			i++;
			if (i >= argc || strncmp(argv[i], "-", 1) == 0) {
				cerr << "ERROR: -s needs argument" << endl;
				printHelp();
			}
			else if (strncmp(argv[i], "cif", 3) == 0) solver = "cif"; 
			else {
				cerr << "ERROR: Unknown option for -s: " << argv[i] << endl;
				printHelp();
			}
		} 
		else if (strncmp(argv[i], "-d", 2) == 0) {
			i++;
			if (i >= argc || strncmp(argv[i], "-", 1) == 0) {
				cerr << "ERROR: -d needs argument" << endl;
				printHelp();
			}
			else if (strncmp(argv[i], "minisat", 7) == 0) solver = "minisat";
			else if (strncmp(argv[i], "bdd", 3) == 0) solver = "bdd";
			else if (strncmp(argv[i], "sdd", 3) == 0) solver = "sdd"; 
			else {
				cerr << "ERROR: Unknown option for -d: " << argv[i] << endl;
				printHelp();
			}
		} 
		else if (strncmp(argv[i], "-v0", 3) == 0) verbosity = 0;
		else if (strncmp(argv[i], "-v1", 3) == 0) verbosity = 1;
		else if (strncmp(argv[i], "-v2", 3) == 0) verbosity = 2;
		else {
			cerr << "ERROR: Unknown option: " << argv[i] << endl;
            printHelp();
		}
	}
}
void Options::printHelp() {
	cout << "help placeholder" << endl;
    exit(1);
}