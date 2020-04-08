#include <iostream>

template <class T>
class List {
	class Node {
	public:
		Node() : next(nullptr) {}
		~Node() {}
		Node *next;
		T info;
	};
	Node *head;
	Node *tail;
	size_t size;
	void first(T info) {
		head = new Node;
		tail = head;
		head->info = info;
		size++;
	}
public:
	List() : head(nullptr), tail(nullptr), size(0) {}
	~List() {
		clear();
	}
	void push_back(T info) {  // adding node in the tail
		if (head != nullptr) {
			Node *adding = new Node;
			adding->info = info;
			tail->next = adding;
			tail = adding;
			size++;
		}
		else
			first(info);
	}

	void push_front(T info) { // in the front
		if (head != nullptr) {
			Node *adding = new Node;
			adding->info = info;
			adding->next = head;
			head = adding;
			size++;
		}
		else first(info);
	}

	void pop_back() { // removing from the tail
		if (head != tail) {
			Node *temp = head;
			while (temp->next != tail) {
				temp = temp->next;
			}
			delete tail;
			tail = temp;
			tail->next = nullptr;
			size--;
		}
		else pop_front();
	}

	T pop_front() {  // from the front
		auto re = head->info;
		if (head != tail) {
			Node *temp = head->next;
			delete head;
			head = temp;
			size--;
		}
		else {
			delete head;
			head = nullptr;
			tail = nullptr;
			size = 0;
		}
		return(re);
	}

	void insert(T info, size_t index) { // inserting element anywhere we want
		if (head == nullptr)
			first(index);
		else {
			if ((index < 0) || (index >= size)) {    // check if index in the available range
				throw std::out_of_range("Out of range!");
			}
			else if (index == 0) push_front(info); //if index = 0 then we can use more simple function
				//there's no  (index == size - 1) push_back(info) because this function can't do this according to task (adding element before index -- there's no element with number index)
			else { // if it is somewhere else, we use this
				Node *adding = new Node;
				Node *temp = head;
				adding->info = info;
				for (size_t i = 0; i < index - 1; i++)
					temp = temp->next;
				adding->next = temp->next;
				temp->next = adding;
				size++;
			}
		}
	}

	T at(size_t checking) {
		if ((checking < 0) || (checking >= size))  // as usual checking if index in available range
			throw std::out_of_range("Out of range!");
		else {
			Node *temp = head;
			for (size_t i = 0; i < checking; i++) { // going to the checking, it'll be that guy, who we're looking for
				temp = temp->next;
			}
			return temp->info;
		}
	}
	void remove(size_t index) {
		if ((index < 0) || (index >= size))
			throw std::out_of_range("Out of range!");
		else if (index == 0)
			pop_front(); // using more simple specialized functions on border elements
		else if (index == size - 1)
			pop_back();
		else { // when it is anywhere but first and last
			Node *removing = head;
			Node *temp = removing; // actually, can't explain if we need this, probably not
			for (size_t i = 0; i < index; i++) {  //making temp one before removing one so we can point previous element on the next element after removing
				temp = removing;
				removing = removing->next;
			}
			temp->next = removing->next;
			delete removing;
			size--;
		}
	}
	size_t get_size() { // no comments here
		return size;
	}
	void print_to_console() { // here we're just walking from head to tail and outputting info
		Node *outputting = head;
		while (outputting != nullptr) {
			std::cout << outputting->info << " ";
			outputting = outputting->next;
		};
		std::cout << std::endl;
	}
	void clear() {
		while (head != nullptr) {
			pop_front();
		}
		size = 0;
	}
	void set(size_t index, T change) {
		if ((index < 0) || (index >= size)) {
			throw std::out_of_range("Out of range!");
		}
		else {
			Node *changing = head;
			for (size_t i = 0; i < index; i++) { // going to the necessary element
				changing = changing->next;
			}
			changing->info = change;
		}
	}
	bool isEmpty() {
		if (size == 0) return true; //seems logical to me
		else return false;
	}
	void push_front(List adding) {
		for (int i = (adding.get_size() - 1); i >= 0; i--) {     // Minus one for using i as a pointer on adding list's element
			push_front(adding.at(i));
		}
	}
};