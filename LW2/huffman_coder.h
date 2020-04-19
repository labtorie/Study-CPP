#pragma once
#include <string>
#include <iostream>
using namespace std;

class HuffmanCoder {

	string input;

	// A node that can be inserted either into the Tree and into the List (I find it useful)
	struct Symbol {
		char key;
		size_t weight;
		Symbol *prev, *next;
		Symbol *L, *R;

		// Constructor for the list insertion
		Symbol(char _key, size_t _weight, Symbol * _prev) {
			key = _key;
			weight = _weight;
			prev = _prev;
			if (_prev != nullptr)
				_prev->next = this;
			next = R = L = nullptr;
		}

		// Constructor for the tree node
		Symbol(Symbol* _L, Symbol* _R) {
			key = '\0';
			weight = _L->weight + _R->weight;
			L = _L;
			R = _R;
			next = prev = nullptr;
		}


	};

	// A list that holds <char, code> pair.
	// Not the best solution due to its O(n) Time Complexity (further T.C.),
	// I promise I'll use a binary tree next time :(
	class CodeList {
		friend class HuffmanCoder;
		struct CodeUnit {
			char symbol;
			string code;
			CodeUnit* next, *prev;
			CodeUnit(char _sym, string _code) {
				symbol = _sym;
				code = _code;
				next = prev = nullptr;
			}
		};

		CodeUnit* head, *tail;

		// Obvious one
		void add(char _sym, string _code) {
			if (head == nullptr) {
				head = tail = new CodeUnit(_sym, _code);
				return;
			}
			tail->next = new CodeUnit(_sym, _code);
			tail->next->prev = tail;
			tail = tail->next;
		}

		// Returns the code of a symbol, but with O(n) T.C.
		string getCode(char _sym) {
			try {
				CodeUnit* current = head;
				while (current->symbol != _sym && current->next != nullptr) 
					current = current->next;
				if (current->symbol == _sym)
					return current->code;
			}
			catch (exception _e) {
				cout << _e.what();
			}
		}

	public:

		CodeList() {
			head = tail = nullptr;
		}


		// Prints the [symbol : code] table, in case you need it
		// Also called from the encode() method
		void print() {
			if (head != nullptr) {
				CodeUnit* current = head;
				cout << endl << "[CODE TABLE]: " << endl;
				while (current != nullptr) {
					cout << current->symbol << " : " << current->code << endl;
					current = current->next;
				}
			}
		}
	};

	// A list class (wow) that I used most for the queues
	// Contains Symbol classes that are described above
	class List {

		// "Everyone needs a friend" - Bob Ross
		friend class HuffmanCoder;

		Symbol* root, *tail;

		// Adds the <symbol, frequency> pair to the end of List - O(n) T.C.
		void add(char _k, size_t _v) {
			if (!tail)
			{
				root = tail = new Symbol(_k, _v, nullptr);
				return;
			}
			tail->next = new Symbol(_k, _v, tail);
			tail = tail->next;
		}

		// Basically, counts the frequency of each symbol in the string - O(n) T.C.
		void collect(char ch) {
			if (root == nullptr) {
				root = tail = new Symbol(ch, 1, nullptr);
				return;
			}

			Symbol *cur = root;

			while (cur != tail) {
				if (cur->key == ch) {
					cur->weight++;
					return;
				}
				cur = cur->next;
			}
			if (tail->key == ch) {
				tail->weight++;
				return;
			}
			if (tail->key != ch) {
				tail->next = new Symbol(ch, 1, tail);
				tail = tail->next;
				return;
			}
		}

		// Shows the first one in the queue - O(1) T.C. I guess
		Symbol* peek() {
			return root;
		}

		// Shows the second one in the queue - O(1) T.C.
		Symbol* doublePeek() {
			if (root == nullptr)
				return root;
			return root->next;
		}

		// That's what everyone in any queue is waiting for - O(1) T.C.
		Symbol* dequeue() {
			Symbol* tmp = root;
			if (tmp == tail) {
				root = tail = nullptr;
				return tmp;
			}
			root->next->prev = nullptr;
			root = root->next;
			return tmp;
		}

		// "Who's the last to the therapist?" - O(1) T.C.
		void enqueue(Symbol* _sym) {
			if (root == nullptr) {
				root = tail = _sym;
				return;
			}
			tail->next = _sym;
			_sym->prev = tail;
			tail = tail->next;
		}

		// Aux for the getCodeTable()
		void _table(string pref, Symbol* sym, CodeList* _list) {
			if (sym->key != '\0')
			{
				_list->add(sym->key, pref);
				return;
			}
			if (sym->R != nullptr)
				_table(pref + "1", sym->R, _list);
			if (sym->L != nullptr)
				_table(pref + "0", sym->L, _list);
		}

		// Aux for the printTree()
		void print(string indent, Symbol* node, bool isRight)
		{
			if (node)
			{
				cout << indent;
				cout << (isRight ? "|---" : "'---");
				if (node->key == '\0')
					cout << (isRight ? "1" : "0") << "*" << endl;
				else
					cout << (isRight ? "1" : "0 ") << " [" << node->key << "]" << endl;

				print(indent + (isRight ? "|    " : "     "), node->R, true);
				print(indent + (isRight ? "|    " : "     "), node->L, false);
			}
		}
		
		// Frequency ascending sorting - O(n*n) T.C.
		List getSortedList() {
			List sortedList;
			Symbol *min = root, *current = root;
			while (root != tail) {
				while (current != tail) {
					if (current->weight < min->weight)
						min = current;
					current = current->next;
				}
				if (tail->weight < min->weight)
					min = tail;
				if (min != tail && min != root) {
					min->prev->next = min->next;
					min->next->prev = min->prev;
				}
				if (min == root)
					root = min->next;
				if (min == tail)
					tail = min->prev;
				sortedList.add(min->key, min->weight);
				min = root;
				current = root;
			}
			sortedList.add(tail->key, tail->weight);
			return sortedList;
		}

	public:

		/*
		A constructor is a member function of a class which initializes objects of a class.
		In C++, Constructor is automatically called when object(instance of class) create.
		As you can see below, this one equates the root and the tail pointers to the 
		nullpointers. I find this practice useful. 
		By the way, on April 17 I saw a couple of cute Shiba-inu doggos. 
		They were really sweet OwO. They looked like smol buns!!
		I like this breed of doggos!
		*/
		List() {
			root = tail = nullptr;
		}

		/// O(n) T.C.
		// Gets the table of [symbol: code] rows
		CodeList getCodeTable() {
			CodeList* _codeList = new CodeList;
			_table("\0", root, _codeList);
			return *_codeList;
		}

		/// Prints something like a frequency analysis table in case you need it. Why not? O(n)
		// ONLY FOR THE PRIORITY LIST!
		void printPriorityList() {
			if (root != nullptr) {
				cout << endl << "[FREQUENCY ANALYSIS]: " << endl;
				Symbol* current = root;
				int rows = 0;
				
				while (current != nullptr) {
					cout << "[" << current->key << " : " << current->weight << "] ";
					rows++;
					if (rows % 10 == 0)
						cout << endl;
					current = current->next;
				}

			}
		}

		/// Visualizes the tree if you want to. - O(n) T.C.
		// ONLY FOR THE TREE!
		void printTree() {
			cout << endl << endl << "[CODE TREE]: " << endl;
			print("", root, false);
		}

	};

public:

	// Initializes the Coder with the _input
	HuffmanCoder(string _input) {
		input = _input;
	}

	// Returns the [character : frequency] list - O(n)
	List getPriorityList() {
		List priorityList;
		for (size_t i = 0; i < input.length(); i++) {
			priorityList.collect(input[i]);
		}
		return priorityList.getSortedList();
	}

	// Returns the Huffman's tree - O(log n)
	List makeTree() { 
		List nodes;
		List symbols = getPriorityList();
		while (symbols.peek() != nullptr || nodes.doublePeek() != nullptr) {

			// (s) + s
			if (symbols.doublePeek() != nullptr && symbols.peek() != nullptr) {
				if (nodes.peek() == nullptr || nodes.peek()->weight > symbols.doublePeek()->weight) {
					nodes.enqueue(new Symbol(symbols.dequeue(), symbols.dequeue()));
					continue;
				}
			}

			// (s)+(n)
			if (symbols.peek() != nullptr && nodes.peek() != nullptr) {
				if ((symbols.doublePeek() == nullptr || nodes.doublePeek() == nullptr) || (
					symbols.peek()->weight <= nodes.doublePeek()->weight &&
					nodes.peek()->weight <= symbols.doublePeek()->weight)) {

					nodes.enqueue(new Symbol(nodes.dequeue(), symbols.dequeue()));
					continue;
				}
			}

			// (n) + n
			if (nodes.doublePeek() != nullptr && nodes.peek() != nullptr) {
				if (symbols.peek() == nullptr || symbols.peek()->weight > nodes.doublePeek()->weight) {

					nodes.enqueue(new Symbol(nodes.dequeue(), nodes.dequeue()));
					continue;
				}
			}

		}
		return nodes;
	}

	// Decodes a binary string coded with the Tree - O(log n)
	string decode(string _in) {
		string result;
		List tree = makeTree();
		Symbol* current = tree.root;
		for (size_t i = 0; i <= _in.length(); i++) {
			if (_in[i] == '0') {
				if (current->L != nullptr) {
					current = current->L;
					continue;
				}
				else {
					result += current->key;
					current = tree.root;					
						i--;
					continue;
				}
			}
			else 
			if (_in[i] == '1') {
				if (current->R != nullptr) {
					current = current->R;
					continue;
				}
				else {
					result += current->key;
					current = tree.root;
						i--;
					continue;
				}

			}
			else if (_in[i] == '\0') { //Detects end of line to add the last character
				result += current->key;
			}
		}
		return result;
	}

	// Encodes an input string to 011010100101011101000101011011101............ - O(n)
	string encode() {
		CodeList _list = makeTree().getCodeTable();
		_list.print();
		string encodedString;
		for (size_t i = 0; input[i] != '\0'; i++) {
			encodedString += _list.getCode(input[i]);
		}

		// Outputs before/after memory cosuption and compression ratio
		cout << endl << "[ENCODED TEXT]: " << endl << encodedString << endl;
		float ratio = (8 * float(input.length())) /  float(encodedString.length())  ;
		cout << endl << "[INITIAL STRING SIZE]: " << input.length() * 8 << " bits (" << input.length() << " bytes)" << endl;
		cout << "[COMPRESSED STRING SIZE]: " << encodedString.length() << " bits (" << float(encodedString.length()) / 8 << " bytes)"<<endl<<"[COMPRESSION RATIO]: "<<ratio<< endl;
		
		return encodedString;
	}
};