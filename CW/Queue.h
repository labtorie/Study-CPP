#pragma once
#include <iostream>
#include <exception>

using namespace std;
template<typename T>
class Queue
{
private:
	class Node
	{
	public:
		T index;
		Node *next;

		explicit Node(T value) : index(value), next(nullptr) {}
		Node(T value, Node* next) : index(value), next(next) {}
	};

	size_t size;
	Node *head, *tail;
public:
	Queue() : head(nullptr), tail(nullptr), size(0) {}

	~Queue()
	{
		this->clear();
	}

	void enqueue(T value)
	{
		Node *temp = new Node(value);
		if (!isEmpty())
		{
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

	T dequeue()
	{
		if (head)
		{
			Node *temp = head;
			T tempdata = temp->index;

			if (head != tail)
			{
				head = head->next;
				delete temp;
			}
			else
			{

				head = nullptr;
				tail = nullptr;
				delete temp;
			}
			size--;
			return tempdata;
		}
		else throw out_of_range("Queue is Empty!");
	}

	T front()
	{
		if (this->head == nullptr)
			throw out_of_range("Queue is Empty!");
		return this->head->index;
	}

	size_t get_size()
	{
		return size;
	}

	bool isEmpty()
	{
		return this->head == nullptr;
	}

	void clear()
	{
		if (size)
		{
			Node* temp = head;
			while (head)
			{
				head = head->next;
				delete temp;
				temp = head;
			}
			size = 0;
		}
	}
};