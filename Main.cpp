#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include "Queue.h"
#include "Stack.h"
#include "Vector.h"
#include "Tree.h"

using namespace std;

int main() {

	Tree tree;
	tree.loadData(); 
	string command;
	string func;
	string type1;
	string type2;

	cout << "Welcome to the Virtual Filesystem\n";
	while (true) {
		cout << ">";
		getline(cin, command);
		stringstream a(command);
		a >> func >> type1 >> type2;

		if (func == "help") {
			help();
		}
		else if (func == "pwd") {
			tree.printPath();
		}
		else if (func == "realpath") {
			tree.realpath(type1, true);
		}
		else if (func == "ls") {
			tree.ls(type1);
		}
		else if (func == "mkdir") {
			tree.mkdir(type1);
		}
		else if (func == "touch") {
			tree.touch(type1, stoi(type2));
		}
		else if (func == "cd") {
			tree.cd(type1);
		} //error if filename input??????
		else if (func == "find") {
			tree.Find(type1, *tree.root());
		}
		else if (func == "mv") {
			tree.mv(type1, type2);
		}
		else if (func == "rm") {
			tree.rm(type1);
		}
		else if (func == "size") {
			tree.size(type1);
		}
		else if (func == "emptybin") {
			tree.emptybin();
		}
		else if (func == "showbin") {
			tree.showbin();
		}
		else if (func == "recover") {
			tree.recover();
		}
		else if (func == "exit" || func == "Exit") {
			break;
		}

		else cout << "Invalid command\n";

		//To clear the memory of all variables for potential next iteration of while loop
		func.clear();
		type1.clear();
		type2.clear();
	} 

	return 0;
}

