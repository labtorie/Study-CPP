#pragma once
#include <iostream>
#include "List.h"
#include <windows.h>

using namespace std;

enum Color { BLACK, RED, };

template <typename TKey, typename TValue>
class Map
{
private:
	class Node
	{
	private:
		Color color;
		Node* left, *right, *parent;
		TKey key;
		TValue value;
	public:
		Node() : color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
		Node(TKey key, TValue value) : color(RED), left(nullptr), right(nullptr), parent(nullptr), key(key), value(value) {}

		bool isOnLeft()

		{
			return this == parent->getLeft();
		}

		Node* sibling()
		{
			if (parent == nullptr)
				return nullptr;

			if (isOnLeft())
				return parent->right;

			return parent->left;
		}

		bool hasRedChild()
		{
			return (left != nullptr && left->getColor() == RED) || (right != nullptr && right->getColor() == RED);
		}

		void setColor(Color color)
		{
			this->color = color;
		}

		Color getColor()
		{
			if (this == nullptr)
				return BLACK;
			return color;
		}

		void setLeft(Node* ptr)
		{
			left = ptr;
		}

		Node* getLeft()
		{
			return left;
		}

		void setRight(Node* ptr)
		{
			right = ptr;
		}

		Node* getRight()
		{
			return right;
		}

		void setParent(Node* parent)
		{
			this->parent = parent;
		}

		Node* getParent()
		{
			return parent;
		}

		void setKey(TKey key)
		{
			this->key = key;
		}

		TKey getKey()
		{
			return key;
		}

		void setValue(TValue value)
		{
			this->value = value;
		}

		TValue getValue()
		{
			return value;
		}
	};
	Node* root;
	int size;

	void SetColor(int text, int background)
	{
		HANDLE hConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsoleHandle, (WORD)((background << 4) | text));
	}

	void fixInsertion(Node*& node)
	{
		if (root == node) 
		{
			node->setColor(BLACK);
			return;
		}
		Color c;
		Node* parent = nullptr, *grandparent = nullptr, *uncle = nullptr;
		while (node != root && node->getParent()->getColor() == RED)
		{
			parent = node->getParent();
			grandparent = parent->getParent();
			if (parent == grandparent->getLeft())
			{
				uncle = grandparent->getRight();
				if (uncle->getColor() == RED)
				{
					parent->setColor(BLACK);
					grandparent->setColor(RED);
					uncle->setColor(BLACK);
					node = grandparent;
				}
				else
				{
					if (node == parent->getRight())
					{
						leftRotate(parent);
						node = parent;
						parent = node->getParent();
					}
					rightRotate(grandparent);
					c = parent->getColor();
					parent->setColor(grandparent->getColor());
					grandparent->setColor(c);
					node = parent;
				}
			}
			else
			{
				uncle = grandparent->getLeft();
				if (uncle->getColor() == RED)
				{
					parent->setColor(BLACK);
					grandparent->setColor(RED);
					uncle->setColor(BLACK);
					node = grandparent;
				}
				else
				{
					if (node == parent->getLeft())
					{
						rightRotate(parent);
						node = parent;
						parent = node->getParent();
					}
					leftRotate(grandparent);
					c = parent->getColor();
					parent->setColor(grandparent->getColor());
					grandparent->setColor(c);
					node = parent;
				}
			}
		}
		root->setColor(BLACK);
	}

	void leftRotate(Node* node)
	{
		Node* right_child = node->getRight();
		node->setRight(right_child->getLeft());

		if (node->getRight() != nullptr)
			node->getRight()->setParent(node);

		right_child->setParent(node->getParent());

		if (node->getParent() == nullptr)
			root = right_child;
		else if (node == node->getParent()->getLeft())
			node->getParent()->setLeft(right_child);
		else
			node->getParent()->setRight(right_child);

		right_child->setLeft(node);
		node->setParent(right_child);
	}

	void rightRotate(Node* node)
	{
		Node* left_child = node->getLeft();
		node->setLeft(left_child->getRight());

		if (node->getLeft() != nullptr)
			node->getLeft()->setParent(node);

		left_child->setParent(node->getParent());

		if (node->getParent() == nullptr)
			root = left_child;
		else if (node == node->getParent()->getLeft())
			node->getParent()->setLeft(left_child);
		else
			node->getParent()->setRight(left_child);

		left_child->setRight(node);
		node->setParent(left_child);
	}

	void deleteSubTree(Node* node)
	{
		while (node != nullptr) 
		{
			deleteSubTree(node->getLeft());
			deleteSubTree(node->getRight());
			delete node;
			node = nullptr;
		}
		root = nullptr;
	}

	void map_ValueAdd(Node* node,TKey key,TValue addValue)
	{
		if (node != nullptr)
		{
			if (key == node->getKey())
			{
				node->setValue(node->getValue() + addValue);
				fixInsertion(node);
				return;
			}
			else
			{
				if (key < node->getKey())
				{
					map_ValueAdd(node->getLeft(), key, addValue);
				}
				if (key > node->getKey())
				{
					map_ValueAdd(node->getRight(), key, addValue);
				}
			}

		}
	}
	
	void map_ValueSub(Node* node, TKey key, TValue subValue)
	{
		if (node != nullptr)
		{
			if (key == node->getKey())
			{
				node->setValue(node->getValue() - subValue);
				fixInsertion(node);
				return;
			}
			else
			{
				if (key < node->getKey())
				{
					map_ValueSub(node->getLeft(), key, subValue);
				}
				if (key > node->getKey())
				{
					map_ValueSub(node->getRight(), key, subValue);
				}
			}

		}
	}
	Node* search(TKey key)
	{
		Node* temp = root;
		while (temp != NULL && key != temp->getKey())
		{
			if (key < temp->getKey())
			{
				temp = temp->getLeft();
			}
			else
			{
				temp = temp->getRight();
			}
		}
		return temp;
	}

	void map_insert(Node* node, Node* prev, TKey key, TValue value)
	{
        if (node == nullptr)
		{
			size++;
			node = new Node(key, value);
			node->setParent(prev);

			if (prev = nullptr)
			{
				node->setColor(BLACK);
			}
			else if (node->getParent()->getColor() == RED)
			{
				fixInsertion(node);
			}
		}
		else
		{
			if (key < node->getKey())
			{
				map_insert(node->getLeft(), node, key, value);
			}
			else if (key > node->getKey())
			{
				map_insert(node->getRight(), node, key, value);
			}
			else return;
		}

	}

public:
	Map() 
	{
		size = 0;
		root = nullptr;
	}

	~Map() 
	{
		deleteSubTree(root);
	}

	void insert(TKey key, TValue value)
	{
        map_insert(this->root, nullptr, key, value);
	}

	bool key_check(TKey key)
	{
		if (search(key) != NULL) return true;
		else return false;
	}
	TValue find_value(TKey key)
	{
		if (root = nullptr)
			throw invalid_argument("Empty Map!");

		Node* temp = root;
		while (temp && temp->getKey() != key)
		{
			if (temp->getKey() < key)
				temp = temp->getRight();
			else
				temp = temp->getLeft();
		}
		if (!temp)
			throw invalid_argument("Invalid value!");
		return temp->getValue();

	}
	void clear() 
	{
		if (root == nullptr)
			throw runtime_error("Empty Map!");
		deleteSubTree(root);
		size = 0;
	}

	void Value_Add(TKey key, TValue addValue)
	{
		map_ValueAdd(this->root, key, addValue);
	}

	void Value_Sub(TKey key, TValue subValue)
	{
		map_ValueSub(this->root, key, subValue);
	}

	int get_size()
	{
		return size;
	}	
};