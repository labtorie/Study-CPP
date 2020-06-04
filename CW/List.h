#pragma once
#include <iostream>
#include <exception>

using namespace std;
template<typename T>
class List
{
private:
	class Node
	{
	public:
		T index;
		Node *prev, *next;

		explicit Node(T index) : index(index), prev(nullptr), next(nullptr) {}
		Node(T index, Node* prev, Node* next) : index(index), prev(prev), next(next) {}
	};

	size_t size;
	Node *head, *tail;
public:
	List() : head(nullptr), tail(nullptr), size(0) {}

	~List()
	{
		this->clear();
	}

	void push_back(T index)
	{
		Node *temp = new Node(index);
		if (!isEmpty())
		{
			temp->prev = tail;
			tail->next = temp;
			tail = temp;
		}
		else
		{
			head = temp;
			tail = temp;
		}
		size++;
	}

	void push_front(T index)
	{
		Node *temp = new Node(index);
		if (!isEmpty())
		{
			head->prev = temp;
			temp->next = head;
			head = temp;
		}
		else
		{
			head = temp;
			tail = temp;
		}
		size++;
	}

	void pop_back()
	{
		if (head)
		{
			Node *temp = tail;

			if (head != tail)
			{
				tail = tail->prev;
				tail->next = nullptr;
				delete temp;
			}
			else
			{
				head = nullptr;
				tail = nullptr;
				delete temp;
			}
			size--;
		}
		else throw invalid_argument("Empty List!");
	}

	void pop_front() {
		if (head)
		{
			Node *temp = head;

			if (head != tail)
			{
				head = head->next;
				head->prev = nullptr;
				delete temp;
			}
			else
			{

				head = nullptr;
				tail = nullptr;
				delete temp;
			}
			size--;
		}
		else throw invalid_argument("Empty List!");
	}

	void map_insert(T index, size_t pos) {
		if (pos >= size || pos < 0)
			throw out_of_range("Invalid argument!");

		size_t i = 0;
		Node *insertion_node = head;

		if (pos == 0)
			push_front(index);
		else if (pos == size - 1)
			push_back(index);
		else
		{
			while (i < pos - 1)
			{
				insertion_node = insertion_node->next;
				i++;
			}
			Node *temp = new Node(index, insertion_node, insertion_node->next);
			insertion_node->next = temp;
			size++;
		}
	}

	T at(size_t pos)
	{
		if (pos >= size || pos < 0)
			throw out_of_range("Invalid Argument!");

		size_t i = 0;
		Node *temp = head;
		while (i < pos)
		{
			temp = temp->next;
			i++;
		}
		return temp->index;
	}

	void remove(size_t pos)
	{
		if (pos >= size || pos < 0)
			throw out_of_range("Invalid Argument!");

		size_t i = 0;
		Node *temp = head;
		while (i < pos)
		{
			temp = temp->next;
			i++;
		}

		if (temp == head)
		{
			head = head->next;
			head->prev = nullptr;
			delete temp;
		}
		else if (temp == tail)
		{
			tail = tail->prev;
			tail->next = nullptr;
			delete temp;
		}
		else
		{
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			delete temp;
		}
		size--;
	}

	size_t get_size()
	{
		return size;
	}

	void print_to_console()
	{
		Node *temp = head;
		while (temp) 
		{
			cout << temp->index << " ";
			temp = temp->next;
		}
		cout << endl;
	}

	void clear()
	{
		while (head)
			pop_front();
		size = 0;
	}

	void set(size_t pos, T index) 
	{
		if (pos >= size || pos < 0)
			throw out_of_range("Invalid Argument!");
		size_t i = 0;
		Node *temp = head;
		while (i < pos) 
		{
			temp = temp->next;
			i++;
		}
		temp->index = index;
	}

	bool isEmpty() 
	{
		return this->head == nullptr;
	}
};