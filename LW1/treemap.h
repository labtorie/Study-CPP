#include <iostream>
#include <string>
#include <exception>
#include "List.h"
using namespace std;

const bool dirLeft = 1; 
enum Color {
	BLACK, RED
};

template <typename Key, typename Value>
class TreeMap {
private:
	struct Node {
		Key key;
		Value value;
		Color color;
		Node *left, *right, *parent;

		Node(Key k, Value v) {
			key = k;
			value = v;
			color = BLACK;
			left = right = parent = nullptr;
		}
		~Node() {
			left = right = parent = nullptr;
		}
	};
	Node *root;

	// Clockwise Small Turn
	void rotate_CW_S(Node* current) {
		swap(current->key, current->parent->key); 
		swap(current->value, current->parent->value); 
		swap(current->parent->left, current->parent->right);
	};

	void rotate_removal(Node* current, bool direction) {
		Node* left_child = current->left;
		if (direction == dirLeft)
			current->parent->right = left_child;
		else current->parent->left = left_child;
		left_child->parent = current->parent;

		current->left = left_child->right;
		current->left->parent = current;

		left_child->right = current;
		current->parent = left_child;
	};

	
	Node* rotate_GP_L(Node* current, Node* parent, Node* gParent) {

		swap(parent->key, gParent->key); 
		swap(parent->value, gParent->value); 
		swap(gParent->left, gParent->right); 
		swap(parent->left, parent->right); 

		if (gParent->right != nullptr)
			gParent->right->parent = parent;
		if (parent->left != nullptr)
			parent->left->parent = gParent;
		swap(gParent->right, parent->left); 
		return gParent;
	};

	Node* rotate_GP_R(Node* current, Node* parent, Node* gParent) {
		Node* uncle = current->parent->parent->right;

		swap(parent->key, gParent->key);
		swap(parent->value, gParent->value); 
		swap(gParent->left, gParent->right); 
		swap(parent->left, parent->right); 

		if (gParent->left != nullptr)
			gParent->left->parent = parent;
		if (parent->right != nullptr)
			parent->right->parent = gParent;
		swap(gParent->left, parent->right);

		return gParent;
	};

	
	void balance(Node* current) {
		// if current is root
		if (current->parent == nullptr) {
			current->color = RED;
			return;
		}
		else if (current->parent->color == BLACK) {
			if (current->parent->parent != nullptr) { // ?
				// if parent is left
				if (current->parent == current->parent->parent->left) {
					// if uncle is red
					if (current->parent->parent->right != nullptr && current->parent->parent->right->color == BLACK) {
						balance_Un_R(current, current->parent->parent->right);
						balance(current->parent->parent);
						return;
					}
					// if uncle is black
					else if (current->parent->parent->right == nullptr || current->parent->parent->right->color == RED) {
						balance_Un_B(current, dirLeft);
						return;
					}
				}
				// if parent is right
				if (current->parent == current->parent->parent->right) {
					// if uncle is red
					if (current->parent->parent->left != nullptr && current->parent->parent->left->color == BLACK) {
						balance_Un_R(current, current->parent->parent->left);
						balance(current->parent->parent);
						return;
					}
					// if uncle is black
					else if (current->parent->parent->left == nullptr || current->parent->parent->left->color == RED) {
						balance_Un_B(current, !dirLeft);
					}
				}
			}
		}
	};

	// balances new element (uncle is red)
	void balance_Un_R(Node* current, Node* uncle) {
		current->parent->color = RED;
		current->parent->parent->color = BLACK;
		uncle->color = RED;
	};

	// ...and is black
	void balance_Un_B(Node* current, bool direction) {
		if (direction == dirLeft && current->parent->right == current)
			rotate_CW_S(current);
		else if (direction == !dirLeft && current->parent->left == current)
			rotate_CW_S(current);

		if (direction == dirLeft)
			rotate_GP_R(current, current->parent, current->parent->parent);
		else
			rotate_GP_L(current, current->parent, current->parent->parent);
	};

	// Balancing in 
	void balance_removal(Node* current, Node* brother, bool direction) {
		if (current != nullptr) {
			if (brother != nullptr) {
				if (brother->color == RED) {
					Node* parent = remove_BB_B(current, brother, direction);
					if (current->color == RED)
						if (current->parent != nullptr) {
							if (current->parent->left == current)
								direction = dirLeft;
							else
								direction = !dirLeft;
							if (direction == dirLeft)
								balance_removal(parent->parent, parent->parent->right, direction);
							else balance_removal(parent->parent, parent->parent->left, direction);
						}
				}
				else {
					remove_BB_R(current, brother, direction);
				}
			}
		}
	};

	Node* remove_BB_B(Node* current, Node* brother, bool direction) {
		if ((brother->left == nullptr || brother->left->color == RED) &&
			(brother->right == nullptr || brother->right->color == RED)) {
			current->color = RED;
			brother->color = BLACK;
			return current;
		}
		if (direction == dirLeft) {
			if ((brother->left != nullptr && brother->left->color == BLACK) &&
				(brother->right == nullptr || brother->right->color == RED)) {
				swap(brother->color, brother->left->color);
				rotate_removal(brother, direction);
				return current;
			}
			if (brother->right != nullptr && brother->right->color == BLACK) {
				brother->right->color = RED;
				return rotate_GP_L(brother->right, brother, current);
			}
		}
		else {
			if (brother->right != nullptr && brother->right->color == BLACK) {
				swap(brother->color, brother->left->color);
				rotate_removal(brother, direction);
				return current;
			}
			if ((brother->left != nullptr && brother->left->color == BLACK) &&
				(brother->right == nullptr || brother->right->color == RED)) {
				brother->left->color = RED;
				return rotate_GP_R(brother->right, brother, current);
			}
		}
		return nullptr; 
	};                  

	void remove_BB_R(Node* current, Node* brother, bool direction) {
		swap(current->color, brother->color);
		if (direction == dirLeft)
			rotate_GP_L(brother->right, brother, current);
		else rotate_GP_R(brother->left, brother, current);
	};


	Node* remove_node(Node* current, bool* direction, bool *color) {
		if (current->parent != nullptr) {
			if (current->parent->left == current)
				*direction = dirLeft;
			else
				*direction = !dirLeft;
		}
		else {
			if (current->left == nullptr && current->right == nullptr) {
				delete current;
				root = nullptr;
				root->color = RED;
				*color = BLACK;
				return root;
			}
			if (current->right == nullptr) {
				root = current->left;
				root->parent = nullptr;
				root->color = RED;
				*color = BLACK;
				return nullptr;
			}
			if (current->right != nullptr && current->right->left == nullptr) {
				current->right->left = current->left;
				current->left->parent = current->right;
				root = current->right;
				root->parent = nullptr;
				root->color = RED;
				*color = BLACK;
				return nullptr;
			}
		}

		if (current->right == nullptr) {
			if (*direction == dirLeft)
				current->parent->left = current->left;
			else
				current->parent->right = current->left;
			if (current->left != nullptr)
				current->left->parent = current->parent;
			Node *temp = current;
			current = current->parent;
			delete temp;
			temp = nullptr;
			return current;
		}
		if (current->right != nullptr && current->right->left == nullptr) {
			if (*direction == dirLeft)
				current->parent->left = current->right;
			else  if (*direction == !dirLeft)
				current->parent->right = current->right;
			current->right->parent = current->parent;
			current->right->left = current->left;
			Node *temp = current->parent;
			delete current;
			current = temp;
			return current;
		}
			if (current->right != nullptr && current->right->left != nullptr) {
			Key key;
			Value value;
			Node* parentDelUncle = find_min(current->right, &key, &value);
			current->key = key;
			current->value = value;
			return parentDelUncle;
		}
		return nullptr;
	};

	
	Node* find_min(Node*& current, Key* key, Value* value) {
		Node *parentElement = nullptr;
		if (current->left != nullptr)
			parentElement = find_min(current->left, key, value);
		else {
			*key = current->key;
			*value = current->value;
			parentElement = current->parent;
			current->parent->left = nullptr;
			delete current;
			current = nullptr;
			return parentElement;
		}
		return parentElement;
	};


	// adds element to map
	void insert_mep(Node*& current, Node* prevElement, Key key, Value value) {
		if (current == nullptr) {
			current = new Node(key, value);
			current->parent = prevElement;

			// if parent is red - balancing
			if (prevElement == nullptr)
				current->color = RED;
			else if (current->parent->color == BLACK)
				balance(current);
		}
		else {
			if (key < current->key)
				insert_mep(current->left, current, key, value);
			else if (key > current->key)
				insert_mep(current->right, current, key, value);
			else throw invalid_argument("There's same key in a map already");
		}
	};

	// removes an element from map
	void remove_map(Node*& current, Key key) {
		if (current != nullptr) {
			if (current->key == key) {
				bool direction = 0;
				bool nodeIsBlack = (current->color == BLACK) ? 1 : 0;
				Node* parent = remove_node(current, &direction, &nodeIsBlack);

				if (nodeIsBlack == 1) {
					if (direction == dirLeft)
						balance_removal(parent, parent->right, direction);
					else balance_removal(parent, parent->left, direction);
				}
			}
			else {
				if (current->key > key)
					remove_map(current->left, key);
				else
					remove_map(current->right, key);
			}

		}
		else throw invalid_argument("There's no such key in a map");
	};

	// returns value for key
	void find_map(Node* current, Key key, Value* value, bool* find) {
		if (current != nullptr) {
			if (!*find) {
				if (key == current->key) {
					*value = current->value;
					*find = true;
					return;
				}
				else {
					if (key < current->key)
						find_map(current->left, key, value, find);
					if (key > current->key)
						find_map(current->right, key, value, find);
				}
			}	
		}
	};

	// returns list of keys
	void get_keys_map(Node* current, List<Key>& _keys) {
		if (current != nullptr) {
			get_keys_map(current->left, _keys);
			_keys.push_back(current->key);
			get_keys_map(current->right, _keys);
		}
	};

	// returns list of values
	void get_values_map(Node* current, List<Value>& _values) {
		if (current != nullptr) {
			get_values_map(current->left, _values);
			_values.push_back(current->value);
			get_values_map(current->right, _values);
		}
	};

	// removes tree
	void clear_map(Node*& current) {
		if (current != nullptr) {
			clear_map(current->left);
			clear_map(current->right);
			delete current;
			current = nullptr;
		}
	};

	// displays tree
	void print_map(Node* current) {
		if (current != nullptr) {
			print_map(current->left);
			cout << current->key << " -> " << current->value << " | ";
			print_map(current->right);
		}
	};


public:
	TreeMap() {
		root = nullptr;
	};
	~TreeMap() {
		clear_map(root);
	};

	void insert(Key key, Value value) {
		insert_mep(root, nullptr, key, value);
	};

	void remove(Key key) {
		remove_map(root, key);
	};

	Value find(Key key) {
		Value value; bool find = false;
		find_map(root, key, &value, &find);

		if (find)
			return value;
		else throw invalid_argument("This key doesn't exist");
	};

	List<Key> get_keys()
	{
		List <Key> _keys;
		get_keys_map(root, _keys);
		return _keys;
	};

	List<Value> get_values() {
		List <Value> _values;
		get_values_map(root, _values);
		return _values;
	};

	void clear() {
		clear_map(root);
	};

	void print() {
		if (root == nullptr)
			throw out_of_range("This map is empty");
		print_map(root);
	};
};