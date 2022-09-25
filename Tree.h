#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Queue.h"
#include "Stack.h"
#include "Vector.h"

using namespace std;

//Function to retrieve current date in DD/MM/YY Format
string getDate() { //CHANGE TO USE LOCALTIME INSTEAD OF LOCALTIME_S // LOCALTIME IS THE PROPER IMPLEMENTATION
	time_t ttime = time(NULL);   // get time now
	struct tm timeinfo; //struct of tm
	localtime_s(&timeinfo, &ttime); //pointer to struct tm
	stringstream a;
	a << timeinfo.tm_mday << '-' << (timeinfo.tm_mon + 1) << '-'; //adding MM-DD- to stringstream
	//getting last two digits of year
	string year = to_string(timeinfo.tm_year + 1900);
	year = year.substr(2, year.size() - 2);
	//adding YY to stringstream - complete now
	a << year << '\n';
	//adding from stringstream to string and returning the string
	string date;
	getline(a, date, '\n');
	return date;
}

//Function to determine whether a string is alphanumeric or not
bool isalphanum(string name) {
	for (int i = 0; name[i] != '\0'; i++)
	{
		if (!isalnum(name[i])) //isalnum returns true if the character is alphanumeric
		{
			if (name[i] == '.') //allowing the use of '.' for file extensions
				continue;
			else return false; //because name[i] is not alphanumeric and is not '.'
		}
	}
	return true; //the whole string was alphanumeric
}

void help() {
	cout << "\nList of commands:\n";
	cout << left << setw(25) << "help" << ": Shows the list of available commands\n";
	cout << left << setw(25) << "pwd" << ": Prints the path of the current inode\n";
	cout << left << setw(25) << "realpath filename" << ": Prints the absolute/full path of a given filename of a file within the current directory\n";
	cout << left << setw(25) << "ls" << ": Prints the children of the current inode\n";
	cout << left << setw(25) << "ls sort" << ": Prints the children of the current inode ordered by descending file size\n";
	cout << left << setw(25) << "mkdir foldername" << ": Creates a folder named foldername under the current folder\n";
	cout << left << setw(25) << "touch filename size" << ": Creates a file under the current inode location with the specified filename, size, and current datetime\n";
	cout << left << setw(25) << "cd foldername" << ": Changes current inode to the specified folder\n";
	cout << left << setw(25) << "cd .." << ": Changes current inode to its parent folder\n";
	cout << left << setw(25) << "cd -" << ": Changes current inode to the previous working directory\n";
	cout << left << setw(25) << "cd" << ": Changes current inode to root\n";
	cout << left << setw(25) << "cd /my/path/name" << ": Changes the current inode to the specified path if it exists\n";
	cout << left << setw(25) << "find name" << ": Returns the path of the file(s) or folder(s) with specified name if it exists\n";
	cout << left << setw(25) << "mv filename foldername" << ": Moves a file located under the current inode location, to the specified folder path\n";
	cout << left << setw(25) << "rm name" << ": Removes the specified folder or file from the current inode and puts it in a bin of maximum size 10\n";
	cout << left << setw(25) << "size name" << ": Returns the total size of the folder, including all its subfiles, or the size of the file\n";
	cout << left << setw(25) << "emptybin" << ": Empties the bin\n";
	cout << left << setw(25) << "showbin" << ": Shows the oldest inode of the bin, including its path\n";
	cout << left << setw(25) << "recover" << ": Reinstates the oldest inode back from the bin to its original position in the tree (if the path still exists)\n";
	cout << left << setw(25) << "Exit" << ": The program stops and the filesystem is saved in the file vfs.txt\n\n";
}//NOTES:

class Tree {

public:
	//declaration - list of iNodes
	class PositionList;

	//iNode class
	class Position { //iNode Class
	private:
		//file/folder details
		string name;
		int size; //size of the folder/file itself
		string date;
		bool isFile;
		int totalsize; //size of folder including all its children - initialized only if isFile == false


		//linking pointers
		Position* parentPosition; //pointer to parent position
		Position* next; //pointer to next sibling position
		Position* prev; //pointer to previous sibling position
		PositionList* childrenList; //list of children positions //WHY?

		friend class Tree;
		friend class PositionList;
		friend class MyVector<Position*>;

	public:
		Position() {
			parentPosition = NULL;
			next = NULL;
			prev = NULL;
			childrenList = NULL;
			name = "";
			size = 0;
		}//empty-constructor

		Position(string name, int size, string date, bool isitFile, Position* inode) { //constructor
			this->name = name;
			this->date = date;
			isFile = isitFile;
			parentPosition = inode;
			next = NULL;
			prev = NULL;
			if (!isitFile) {
				childrenList = new PositionList;
				this->size = 10;
				this->totalsize = 10;
			}
			else {
				this->size = size;
			}
			Position* ptr = inode;
			while (ptr != NULL) { //adding the size of new folder/file to its parents
				if (isitFile) {
					ptr->totalsize += size;
				}
				else ptr->totalsize += 10;
				ptr = ptr->parentPosition;
			}
		}

		Position(string foldername, Position* inode, bool isitFile, int size) {
			this->name = foldername;
			this->date = getDate();
			isFile = isitFile;
			parentPosition = inode;
			next = NULL;
			prev = NULL;
			if (!isFile) {
				childrenList = new PositionList;
				this->size = 10;
				this->totalsize = 10;
			}
			else {
				this->size = size;
			}

			Position* ptr = inode;
			while (ptr != NULL) { //adding the size of new folder/file to its parents
				if (isitFile) {
					ptr->totalsize += size;
				}
				else ptr->totalsize += 10;
				ptr = ptr->parentPosition;
			}
		}

		~Position() {
			if (!isFile) {
				delete childrenList;
			}
		}

		string& operator*() {
			return name; //CHANGE TO ELEM IF NEED BE
		}

		Position* parent() const {
			return parentPosition;
		}

		PositionList* children() const {
			return childrenList;
		}
		bool isRoot() const {
			return (parentPosition == NULL);
		}
		bool isExternal() const {
			return isFile;
		}
		string pwd(Position* inode) {
			string path = "";

			//if the path of the root is printed
			if (inode->name == "") {
				path += "/";
				return path;
			}
			//stack to hold names of inode and its parents till NULL is reached
			CStack<string> pwd;

			//adding current path to stack
			Position* ptr = inode;
			while (ptr != NULL) {
				if (ptr->name != "") {
					pwd.push(ptr->name);
					pwd.push("/");
				}

				ptr = ptr->parentPosition;
			}
			//adding path from stack to string
			while (!pwd.empty()) {
				path += pwd.top();
				pwd.pop();
			}
			//path += '\n';
			return path;
		}
	};
	//nested position class

	//definition
	class PositionList {

	private:
		Position* head;// pointer to head of list
		Position* tail; // pointer to tail of list
		friend class Tree;

	public:
		class iterator {
		private:
			Position* v; // pointer to the node/position
			friend class PositionList;
			friend class Tree;


		public:
			iterator(Position* u) { //constructor
				v = u;
			}
			//traits
			using value_type = string;
			using pointer = string*;
			using reference = string&;
			using iterator_category = bidirectional_iterator_tag;
			using difference_type = ptrdiff_t;

			//methods
			Position& operator*() { //dereferencing returns the address of the position
				return *v;
			}

			bool operator==(const iterator& p) const { //comparison
				return v == p.v;
			}

			bool operator!=(const iterator& p) const { //comparison
				return v != p.v;
			}

			iterator& operator++() { //increment so iterator points to next node in list
				v = v->next;
				return *this;
			}

			iterator& operator--() { //decrement so iterator points to previous node in list
				v = v->prev;
				return *this;

			}
			friend class PositionList;
		};

		PositionList() {
			head = new Position;
			tail = new Position;
			head->next = tail;
			tail->prev = head;
		} // empty list constructor

		~PositionList() {
			while (!empty()) {
				removeFront();
			}
			delete head;
			delete tail;
		} // destructor to clean up all nodes

		bool empty() const {
			if (head->next == tail)
				return true;
			else
				return false;
		} // is list empty?

		const string& front() const {
			if (empty())
				cout << "List is empty!\n";
			else
				return head->next->name;
		} // get the value (element) from front Position of list

		const string& back() const {
			if (empty())
				cout << "List is empty!\n";
			else
				return tail->prev->name;
		}  // get the value (element) from last Position of the List 

		void insert(Position* ptr, string name, int size, string date, bool isitFile, Position* inode) {
			Position* v = new Position(name, size, date, isitFile, inode);
			v->next = ptr;
			v->prev = ptr->prev;
			ptr->prev->next = v;
			ptr->prev = v;
		}	 //add a new node before ptr

		void addFront(string name, int size, string date, bool isitFile, Position* inode) {
			Position* v = new Position(name, size, date, isitFile, inode);
			v->next = head->next;
			v->prev = head;
			head->next->prev = v;
			head->next = v;
		} // add a new node to the front of list

		void addBack(string name, int size, string date, bool isitFile, Position* inode) {
			Position* v = new Position(name, size, date, isitFile, inode);
			v->next = tail;
			v->prev = tail->prev;
			tail->prev->next = v;
			tail->prev = v;
		} //add a new node to the back of the list

		void addBackmkdir(string foldername, Position* inode, bool isFile, int size) {
			Position* v = new Position(foldername, inode, isFile, size);
			v->next = tail;
			v->prev = tail->prev;
			tail->prev->next = v;
			tail->prev = v;
		}

		void addBackptr(Position* v) {
			v->next = tail;
			v->prev = tail->prev;
			tail->prev->next = v;
			tail->prev = v;

			Position* ptr = v->parentPosition;
			while (ptr != NULL) { //adding the size of new folder/file to its parents
				if (v->isFile) {
					ptr->totalsize += v->size;
				}
				else ptr->totalsize += v->totalsize;
				ptr = ptr->parentPosition;
			}
		}

		void removeFront() {
			if (!empty()) {
				Position* old = head->next;
				head->next = old->next;
				old->next->prev = head;
				delete old;
			}
			else
				cout << "List is empty!\n";
		} // remove front node from list

		void removeBack() {
			if (!empty()) {
				Position* old = tail->prev;
				tail->prev = old->prev;
				old->prev->next = tail;
				delete old;
			}
			else
				cout << "List is empty!\n";
		}  // remove last node from list

		void remove(Position* ptr) {
			if (!empty()) {
				Position* forw = ptr->next;
				Position* back = ptr->prev;
				forw->prev = back;
				back->next = forw;

				Position* v = ptr->parentPosition;
				while (v != NULL) { //subtracting the size of folder/file from its parents
					if (ptr->isFile) {
						v->totalsize -= ptr->size;
					}
					else v->totalsize -= ptr->totalsize;
					v = v->parentPosition;
				}
			}
			else
				cout << "List is empty!\n";
		}	 //remove ptr node

		void display() const {
			Position* curr = head;
			while (curr != NULL)
			{
				cout << "+------";
				curr = curr->next;
			}
			cout << "+" << endl << "|";
			curr = head;
			while (curr != NULL)
			{
				if (curr == head)		cout << " Head |";
				else if (curr == tail)	cout << " Tail |";
				else 					cout << setw(5) << curr->name << " |";

				curr = curr->next;
			}
			curr = head;
			cout << endl;
			while (curr != NULL)
			{
				cout << "+------";
				curr = curr->next;
			}
			cout << "+" << endl;
		}	// display all element of the list

		//iterator-related methods
		iterator begin() const {
			return iterator(head->next);
		}

		iterator end() const {
			return iterator(tail);
		}

		Position* frontPosition() {
			return head->next;
		}

		Position* lastPosition() {
			return tail->prev;
		}

		Position* find(string name, bool isFile) { //only finds the file or folder in the children of the current node
			Position* ptr = head->next;
			while (ptr != tail) {
				if (ptr->name == name && ptr->isFile == isFile) {
					return ptr;
				}
				ptr = ptr->next;
			}
			return NULL;
		}

		void printinorder() {
			if (!empty()) {
				Position* ptr = head->next;
				while (ptr != tail) {
					if (ptr->isFile)
						cout << "file ";
					else
						cout << "dir ";
					cout << ptr->name << " ";
					cout << ptr->date << " ";
					if (!ptr->isFile)
						cout << ptr->totalsize << " bytes";
					else
						cout << ptr->size << " bytes";
					cout << endl;
					ptr = ptr->next;
				}
			}
			else
				cout << "Current folder is empty\n";
		}

		void printsorted() {
			int i = 0;
			int j = 0;
			int n = 0;

			//filling up Positions into a vector and obtaining size of vector
			if (!empty()) {
				MyVector<Position*> vec;
				Position* ptr = head->next;
				while (ptr != tail) {
					vec.push_back(ptr);
					ptr = ptr->next;
					n++;
				}

				//Bubble sort
				bool swapped; //for efficiency
				for (i = 0; i < n - 1; i++) {
					swapped = false;
					for (j = 0; j < n - i - 1; j++) {
						if (vec[j]->isFile && vec[j + 1]->isFile) {
							if (vec[j]->size < vec[j + 1]->size) { //descending order
							//swapping
								Position* temp = vec[j];
								vec[j] = vec[j + 1];
								vec[j + 1] = temp;
								swapped = true;
							}
						}
						else if (!vec[j]->isFile && !vec[j + 1]->isFile) {
							if (vec[j]->totalsize < vec[j + 1]->totalsize) { //descending order
							//swapping
								Position* temp = vec[j];
								vec[j] = vec[j + 1];
								vec[j + 1] = temp;
								swapped = true;
							}
						}
						else if (vec[j]->isFile && !vec[j + 1]->isFile) {
							if (vec[j]->size < vec[j + 1]->totalsize) { //descending order
							//swapping
								Position* temp = vec[j];
								vec[j] = vec[j + 1];
								vec[j + 1] = temp;
								swapped = true;
							}
						}
						else if (!vec[j]->isFile && vec[j + 1]->isFile) {
							if (vec[j]->totalsize < vec[j + 1]->size) { //descending order
							//swapping
								Position* temp = vec[j];
								vec[j] = vec[j + 1];
								vec[j + 1] = temp;
								swapped = true;
							}
						}

					}
					if (swapped == false) //no swaps were made and so the vector is already sorted
						break;
				}

				//Printing positions in descending order of size	
				for (int x = 0; x < n; x++) {
					if (vec[x]->isFile)
						cout << "file ";
					else
						cout << "dir ";
					cout << vec[x]->name << " ";
					cout << vec[x]->date << " ";
					if (vec[x]->isFile)
						cout << vec[x]->size << " bytes";
					else
						cout << vec[x]->totalsize << " bytes";
					cout << endl;
				}
			}
			else
				cout << "Current folder is empty\n";
		}

		bool isUnique(string name) {
			if (!empty()) {
				Position* ptr = head->next;
				while (ptr != tail) {
					if (ptr->name == name) {
						return false;
					}
					ptr = ptr->next;
				}
				return true;
			}
			else return true;
		}

		Position* findany(string name) { //helper function for rm
			Position* ptr = head->next;
			while (ptr != tail) {
				if (ptr->name == name) {
					return ptr;
				}
				ptr = ptr->next;
			}
			return NULL;
		}

	};
	//nested position list class

private: //private attributes of tree class

	Position* rootNode;
	Position* inode; //current directory
	int totalsize; //????????????
	CStack <string> pathsfor_cd; //previous working directories for 'cd -' function
	Queue<Position*> bin; //the bin that holds Position* values (max = 10)

public:

	bool empty() const {
		return (rootNode == NULL);
	}

	Position* root() const {
		return rootNode;
	}

	Tree() {
		rootNode = new Position;
		rootNode->parentPosition = NULL;
		rootNode->size = 0;
		totalsize = 0;
		inode = rootNode;
	}

	~Tree() {
		//dumping filesystem data into file
		ofstream out;
		out.open("save.txt"); //change this name to dump data into some other file

		if (!out.is_open()) {
			perror("File could not be opened");
		}
		string path;
		string size;
		string date;
		string s = dumpHelper(rootNode); //holds data for every line for file with '\n' delimiter
		stringstream a(s);
		//adding line by line with every iteration of the while loop
		while (a.good()) {
			getline(a, path, ',');
			if (path == "") //base case
				break;
			getline(a, size, ',');
			getline(a, date, '\n');
			out << path << "," << size << "," << date << endl;
			path.clear();
			size.clear();
			date.clear();
		}

		out.close();

		//deallocating memory
		if (rootNode == NULL) return;
		else delete rootNode; //deleting rootNode autmoatically calls destructors for all children
		if (!bin.isEmpty()) { //deallocation memory from bin
			emptybin();
		}
	}

	string dumpHelper(Position* p) {
		string s;

		s = p->pwd(p) + "," + to_string(p->size) + "," + p->date + "\n";
		if (!p->isExternal()) {
			PositionList* ch = p->children(); // list of children
			for (PositionList::iterator q = ch->begin(); q != ch->end(); ++q) {
				s += dumpHelper(q.v);
			}
		}
		return s;
	}

	void preorderLoad(const Tree& T, Position& p, string names, string sizes, string dates, int Slash) {

		//base case
		if (names == "") { //if the string names still has names, because the last element is ',' //if names == ",", then saem for strings sizes and dates
			return;
		}
		//variables needed
		string name;
		string size;
		string date;
		bool isFile;
		int Size;
		int numSlash; //to adjust to the proper inode
		string s;

		if (names != ",") {
			stringstream a(names);
			getline(a, name, ',');
		}
		if (sizes != ",") {
			stringstream a(sizes);
			getline(a, size, ',');
		}
		if (dates != ",") {
			stringstream a(dates);
			getline(a, date, ',');
		}

		Size = stoi(size);
		if (Size == 10 || Size == 0) {
			isFile = false;
		}
		else {
			isFile = true;
		}

		//counting number of slashes in name
		s = name;
		s = s.substr(1, s.size() - 1); //removes '/'
		numSlash = 0;
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '/') {
				numSlash++;
			}
		}

		//to get the name for the position // DO PROPERLY
		string finalname;
		stringstream a(name);
		for (int j = 0; j <= numSlash; j++) {
			getline(a, finalname, '/');
		}
		a >> finalname;

		//if slashes in the name are greater than the slashes in the previous line of file, then this must be a child of the current inode
		if (numSlash > Slash) {
			if (inode->childrenList == NULL) {
				inode->childrenList = new PositionList;
			}
			inode->childrenList->addBack(finalname, Size, date, isFile, inode);
			inode = inode->childrenList->lastPosition();
		}
		//if slashes in the name are lesser than the slashes in the previous line of file, then this must be the parent of the current inode
		if (numSlash < Slash) {
			for (int i = 0; i <= (Slash - numSlash); i++) {
				inode = inode->parentPosition;
			}
			inode->childrenList->addBack(finalname, Size, date, isFile, inode);
			inode = inode->childrenList->lastPosition();
		}
		//if slashes in the name are equal to the slashes in the previous line of file, then this must be a sibling node of the children of inode
		if (numSlash == Slash) {
			inode = inode->parentPosition;
			inode->childrenList->addBack(finalname, Size, date, isFile, inode);
			inode = inode->childrenList->lastPosition();
		}

		//resizing names,sizes, and dates to account for the data already loaded
		names = names.substr(name.size() + 1, names.size() - name.size());
		sizes = sizes.substr(size.size() + 1, sizes.size() - size.size());
		dates = dates.substr(date.size() + 1, dates.size() - date.size());

		//recursive call
		preorderLoad(*this, *inode, names, sizes, dates, numSlash);
	}

	void loadData() {
		ifstream in;
		in.open("vfs.txt");

		if (!in.is_open()) {
			perror("File could not be opened");
		}
		//variables to read from file: eg /system,10,12-02-21
		string name;
		string Size;
		string date;
		string a;
		string b;
		string c;
		bool isRoot = true;

		while (in.peek() != EOF) {

			getline(in, a, ',');
			getline(in, b, ',');
			getline(in, c, '\n');

			//to handle the first line of file
			if (isRoot) {
				rootNode->date = c;
				isRoot = false;
				continue; //necessary, CHECK?
			}

			name += a + ",";
			Size += b + ",";
			date += c + ",";
		}
		inode = rootNode;
		preorderLoad(*this, *inode, name, Size, date, -1); //-1 because we start from root
		in.close();
		inode = rootNode; //system starts from root of tree
	}

	void preorderPrint(const Tree& T, Position& p) { //for testing purposes
		cout << *p; // print element
		if (p.children() != NULL) {
			PositionList* ch = p.children(); // list of children
			for (PositionList::iterator q = ch->begin(); q != ch->end(); ++q) {
				cout << " ";
				preorderPrint(T, *q);
			}
		}
	}

	void printPath() {
		cout << inode->pwd(inode) << endl;
	}

	void realpath(string filename, bool isFile) { //isFile always true when function is called
		Position* ptr = inode;
		if (!inode->isExternal()) {
			if (inode->childrenList->find(filename, isFile) != NULL) {
				ptr = inode->childrenList->find(filename, isFile); //find the file with filename
				cout << ptr->pwd(ptr) << endl; //print its path
			}
			else cout << "The specified file is not within the current directory\n";
		}
		else cout << "The current folder is empty\n";
	}

	void ls(string type) {
		if (type == "") {
			if (!inode->isFile)
				inode->childrenList->printinorder();
			else
				cout << "Current inode is not a directory\n";
		}
		else if (type == "sort") {
			if (!inode->isFile)
				inode->childrenList->printsorted();
			else
				cout << "Current inode is not a directory\n";
		}
		else
			cout << "Invalid command\n";
	}

	void mkdir(string foldername) {
		if (isalphanum(foldername)) { //check if foldername is alphanumeric
			if (inode->childrenList == NULL) { //check if childrenList does not point to NULL
				inode->childrenList = new PositionList;
			}
			if (inode->childrenList->isUnique(foldername)) { //check if foldername is unique within current directory
				inode->childrenList->addBackmkdir(foldername, inode, false, 10); //finally add it
			}
			else cout << "The specified foldername already exists within current directory\n";
		}
		else cout << "The specified foldername must be alphanumeric only\n";
	}

	void touch(string filename, int size) { //same as mkdir function, except for bool sent into addBackmkdir function
		if (isalphanum(filename)) {
			if (inode->childrenList == NULL) {
				inode->childrenList = new PositionList;
			}
			if (inode->childrenList->isUnique(filename)) {
				inode->childrenList->addBackmkdir(filename, inode, true, size);
			}
			else cout << "The specified filename already exits within current directory\n";
		}
		else cout << "The specified filename must be alphanumeric only\n";
	}

	void cd(string type) { //isFile always false when function is called

		if (type == "..") { //changes inode to parent directory
			if (inode->parentPosition != NULL) {
				pathsfor_cd.push(inode->pwd(inode)); //pushing current path to stack for later use in cd -
				inode = inode->parentPosition;
			}
		}
		else if (type == "") {
			pathsfor_cd.push(inode->pwd(inode));
			inode = rootNode; //if-else needed? NOT NEEDED
		}

		else if (type == "-") { //changes inode to previous working directory, make STACK of paths
			if (!pathsfor_cd.empty()) {
				cd(pathsfor_cd.top()); //changing path to the previous working directory
				pathsfor_cd.pop(); //popping used path from stack
			}
			else cout << "No previous working directory found\n";
		}

		else { //if the user specifies a path or a folder
			int numSlash = 0;
			string s = type;
			Position* ptr = inode;
			//counting number of slashes in type
			for (int i = 0; i < s.size(); i++) {
				if (s[i] == '/') {
					numSlash++;
				}
			}
			if (numSlash == 0) { //that is, the user specified a folder under the current inode
				if (ptr->childrenList != NULL) { //set ptr to the name found in ptr's children
					if (ptr->childrenList->find(type, false) != NULL) {
						ptr = ptr->childrenList->find(type, false);
						pathsfor_cd.push(inode->pwd(inode));
						inode = ptr;
						return;
					}
				}
			}
			//otherwise, the user must have specified a path

			ptr = FindInode(type, false); //trace the path to a directory
			if (ptr != NULL) {
				inode = ptr; //inode is now pointing to specified path
			}
			else cout << "The specified path does not exist\n";
		}
	}

	void Find(string name, Position& ptr) { //returns all matching paths of file/folder with name = name (ptr = rootNode on function call)

		if (ptr.name == name) {
			cout << ptr.pwd(&ptr) << endl;
		}

		if (!ptr.isExternal()) {
			PositionList* ch = ptr.children(); // list of children
			for (PositionList::iterator q = ch->begin(); q != ch->end(); ++q) {
				Find(name, *q);
			}
		}
	}

	Position* FindInode(string path, bool isFile) { //find the inode with the specified path, helper function for cd and mv and size

		string s;
		int numSlash = 0;
		Position* ptr = rootNode;
		bool flag = false;

		if (path == "/") { //return root if path is "/" - best case
			return rootNode;
		}

		//counting number of slashes in path
		s = path;
		s = s.substr(1, s.size() - 1); //removes first '/'
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '/') {
				numSlash++;
			}
		}

		//tracing the specified path
		string name;
		stringstream a(s);
		for (int j = 0; j <= numSlash; j++) {
			if (j == numSlash) { //because there will be no '/' left in a-stream
				getline(a, name);
				if (isFile == true) { //incase we are searching for a file
					flag = true;
				}
			}
			else getline(a, name, '/');

			if (name == "") { //if name is "", then we must be at root
				return rootNode;
			}
			if (ptr->childrenList != NULL) { //set ptr to the name found in ptr's children
				if (ptr->childrenList->find(name, flag) != NULL) {
					ptr = ptr->childrenList->find(name, flag);
				}
				else {
					return NULL;
				}
			}
			else {
				return NULL;
			}
		}
		return ptr;
	}

	void mv(string filename, string foldername) { //implemented on arbitray nodes too
		Position* file = NULL;
		Position* folder = NULL;
		int numSlash1 = 0;
		int numSlash2 = 0;
		string s = filename;
		string p = foldername;

		//counting number of slashes in filename
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '/') {
				numSlash1++;
			}
		}
		//counting number of slashes in foldername
		for (int i = 0; i < p.size(); i++) {
			if (p[i] == '/') {
				numSlash2++;
			}
		}

		if (numSlash1 == 0 && numSlash2 == 0) { //i.e. the user specified names of a folder and file within current directory
			if (inode->childrenList != NULL) {
				if (inode->childrenList->find(filename, true) != NULL) {
					file = inode->childrenList->find(filename, true);
				}
				else {
					cout << "The specified file does not exist in the current directory\n";
					return;
				}

				if (inode->childrenList->find(foldername, false) != NULL) {
					folder = inode->childrenList->find(foldername, false);
				}
				else {
					cout << "The specified folder does not exist in the current directory\n";
					return;
				}
			}
			else cout << "The current directory has no files or folders\n";
			//adding file to folder and then removing file from its original position
			folder->childrenList->addBack(file->name, file->size, file->date, file->isFile, folder);
			inode->childrenList->remove(file);
			delete file;
		}
		else if (numSlash1 != 0 && numSlash2 != 0) { //if the user has specified paths for foldername and filename
			file = FindInode(filename, true); //trace the path to the filename
			folder = FindInode(foldername, false); //trace the path to the foldername
			if (file != NULL && folder != NULL) {
				folder->childrenList->addBack(file->name, file->size, file->date, file->isFile, folder);
				Position* ptr = file->parentPosition;
				ptr->childrenList->remove(file);
				delete file;
			}
			if (file == NULL) {
				cout << "The specified file path does not exist\n";
			}
			if (folder == NULL) {
				cout << "The specified path does not exist\n";
			}
		}
		else cout << "Invalid command\n";
	}

	void rm(string name) {
		Position* ptr = NULL;
		Position* ptr2 = NULL;
		int numSlash = 0;
		string s = name;
		//counting number of slashes in name
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '/') {
				numSlash++;
			}
		}
		if (numSlash == 0) { //i.e. the user specified name of a file/folder within current directory
			if (inode->childrenList != NULL) {
				ptr = inode->childrenList->findany(name); //finding the file/folder
				if (ptr == NULL) {
					cout << "Specified file is not within current directory\n";
					return;
				}
			}
			else cout << "The current directory has no files or folders\n";

			bin.enqueue(ptr); //adding file/folder to bin
			inode->childrenList->remove(ptr); //removing ptr from tree
		}
		else if (numSlash != 0) { //if the user has specified a path for foldername/filename
			ptr = FindInode(name, true); //trace the path to the filename
			ptr2 = FindInode(name, false); //trace the path to the foldername

			if (ptr2 == NULL && ptr != NULL) {
				bin.enqueue(ptr); //adding file/folder to bin
				Position* temp = ptr;
				ptr = ptr->parentPosition;
				ptr->childrenList->remove(temp); //removing ptr from tree
				return;
			}
			else if (ptr == NULL && ptr2 != NULL) {
				bin.enqueue(ptr2); //adding file/folder to bin
				Position* temp = ptr2;
				ptr2 = ptr2->parentPosition;
				ptr2->childrenList->remove(temp); //removing ptr from tree
				return;
			}
			else if (ptr == NULL && ptr2 == NULL) {
				cout << "No file/folder with specified name found\n";
				return;
			}
			else {
				bin.enqueue(ptr); //adding file/folder to bin
				Position* temp = ptr;
				ptr = ptr->parentPosition;
				ptr->childrenList->remove(temp); //removing ptr from tree
				return;
			}
		}
		else cout << "Invalid command\n";
	}

	void emptybin() { //empty bin function, deallocates memory too
		while (!bin.isEmpty()) {
			Position* ptr = bin.top();
			bin.dequeue();
			delete ptr; //deallocating memory
		}
	}

	void showbin() {
		if (!bin.isEmpty()) {
			Position* ptr = bin.top();
			//Next Element to remove: /user/Data/exp1.txt (1886 bytes, 01-03-21)
			cout << "Next Element to remove: " << ptr->pwd(ptr) << " (" << ptr->size << " bytes, " << ptr->date << ")\n";
		}
		else cout << "The bin is empty\n";
	}

	void recover() {
		Position* temp;
		Position* ptr = bin.top();
		temp = FindInode(ptr->parentPosition->pwd(ptr->parentPosition), false);
		if (temp != NULL) { //checking if the path still exists
			temp->childrenList->addBackptr(ptr); //reinstating old position
		}
		else {
			cout << "File/Folder cannot be recovered because the original path does not exist\n";
			return;
		}
		bin.dequeue();
	}

	void size(string name) {
		Position* ptr = NULL;
		int numSlash = 0;
		string s = name;

		if (name == "/") {
			int size = 0;
			//cout << preorderSize(*rootNode, size) << endl;
			cout << rootNode->totalsize << endl;
			return;
		}

		//counting number of slashes in name
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '/') {
				numSlash++;
			}
		}
		if (numSlash == 0) { //i.e. the user specified name of a file/folder within current directory
			if (inode->childrenList != NULL) {
				ptr = inode->childrenList->findany(name); //finding the file/folder
				if (ptr == NULL) {
					cout << "Specified file/folder is not within current directory\n";
					return;
				}
			}
			else cout << "The current directory has no files or folders\n";

			if (ptr->isFile) {
				cout << ptr->size << " bytes\n";
			}
			else {
				int size = 0;
				//cout << preorderSize(*ptr, size) << " bytes\n";
				cout << ptr->totalsize << " bytes\n";
			}
		}

		else if (numSlash != 0) { //if the user has specified a path for foldername/filename
			ptr = FindInode(name, true); //trace the path to the file/folder name
			if (ptr == NULL) {
				ptr = FindInode(name, false); //if ptr == NULL, then name must have been a foldername
				if (ptr == NULL) {
					cout << "The specified file/folder path was not found\n";
					return;
				}
				int size = 0;
				//cout << preorderSize(*ptr, size) << " bytes\n";
				cout << ptr->totalsize << " bytes\n";
			}
			else {
				cout << ptr->size << " bytes\n";
			}
		}
		else cout << "Invalid command\n";
	}

};
