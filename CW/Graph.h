#pragma once
#include <string>
#include <fstream>
#include <stdio.h>
#include <limits.h>
#include <exception>
#include "Map.h"
#include "List.h"
#include "Queue.h"

using namespace std;

struct Graph
{
	Graph(string file) {
	_readFile(file);
	makeGraph();
	};
	
	~Graph();

	int getFlowSize();
	void readFile(string);

private:
	class flowStruct
	{
	public:
		flowStruct() : checked(false) {}
		bool checked;
		Map <int, int> capacity;
		Map <int, int> bands;
	};

	flowStruct *flow;
	List<int> pathToEnd;
	Map <string, int> items;
	Queue <string> fLines;

	void _readFile(string);

	void makeGraph();

	int countFlow();

	bool makeWay(int);

	bool updateFlow(int*);

	string readWord(string&);

	void readLine(string, string&, string&, int*);
};