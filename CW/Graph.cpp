#include "Graph.h"

Graph::~Graph() {
	delete[] flow;
	items.clear();
	fLines.clear();
	pathToEnd.clear();
};

void Graph::readFile(string file) {
	_readFile(file);
	makeGraph();
};
// Returns the widest flow size
int Graph::getFlowSize() {
	int flow = countFlow();
	cout << "The widest flow: " << flow << endl << endl;
	return flow;
}
// Reads file.....
void Graph::_readFile(string pathToFile) {
	ifstream file(pathToFile);

	if (!file.is_open())
		throw ios_base::failure("Unable to read the file!");

	string line;
	string source = "", sink = "";
	int num = 1; 

	while (getline(file, line)) {
		fLines.enqueue(line);
		string edge_1 = readWord(line);
		string edge_2 = readWord(line);

		if (edge_1 == "S") {
			source = edge_1;
			continue;
		}

		if (edge_2 == "T") {
			sink = edge_2;
			continue;
		}

		if (edge_1 == "T" || edge_2 == "S" || edge_1 == edge_2) throw invalid_argument("Make sure your file has no loops");

		if (!items.key_check(edge_1)) items.insert(edge_1, num++);
		if (!items.key_check(edge_2)) items.insert(edge_2, num++);
	}

	if (!items.get_size())
		throw invalid_argument("Empty file!");

	if (source != "S") throw logic_error("Cannot find Source!");
	else items.insert(source, 0);

	if (sink != "T") throw logic_error("Cannot find Sink!");
	else items.insert(sink, items.get_size());

	file.close();
};

void Graph::makeGraph() {
	string edge_1, edge_2; 
	int capacity; 

	flow = new flowStruct[items.get_size()];
	bool first = true;
	while (!fLines.isEmpty()) {
		readLine(fLines.dequeue(), edge_1, edge_2, &capacity);

		int index_1 = items.find_value(edge_1);
		int index_2 = items.find_value(edge_2);

        flow[index_1].bands.insert(flow[index_1].bands.get_size(), index_2);

		if (!flow[index_2].capacity.key_check(index_1))
			flow[index_2].capacity.insert(index_1, capacity);
		else
			flow[index_2].capacity.Value_Add(index_1, capacity);
	}
};

int Graph::countFlow() {
	int maxID;
	int maxCapacity;
	int sourceID = 0;

	int MAX_FLOW = INT_MIN;
	do {
		_searchForAnyWay:
			maxID = INT_MIN;
			maxCapacity = INT_MIN;

			for (int i = 0; i < flow[sourceID].bands.get_size(); i++) {
				int ID = flow[sourceID].bands.find_value(i);
				int capacity = flow[ID].capacity.find_value(sourceID);

				if (capacity > maxCapacity && !flow[ID].checked) {
					maxID = ID;
					maxCapacity = capacity;
				}
			}

			if (maxID != INT_MIN){
				if (!makeWay(maxID)) goto _searchForAnyWay;

				if (updateFlow(&MAX_FLOW)) break; // В случае если нет пути в сток
			}

		
	} while (maxID != INT_MIN);
	if (MAX_FLOW == INT_MIN)
		throw invalid_argument("There's no path between Souce and Sink");
	return MAX_FLOW;
}

bool Graph::makeWay(int startID){
	if (startID == items.get_size() - 1){
		pathToEnd.push_back(startID);
		return true;
	}
	bool searchForAnyPath = true;
	while (searchForAnyPath){
		int maxID = INT_MIN;
		int maxCapacity = INT_MIN;

		for (int i = 0; i < flow[startID].bands.get_size(); i++){

			int ID = flow[startID].bands.find_value(i);
			int capacity = flow[ID].capacity.find_value(startID);

			if (capacity > maxCapacity && !flow[ID].checked){
				maxID = ID;
				maxCapacity = capacity;
			}
		}

		flow[startID].checked = true;

		if (maxID != INT_MIN){
			if (!makeWay(maxID)) searchForAnyPath = true;
			else{
				searchForAnyPath = false;
				pathToEnd.push_back(startID);
				return true;
			}
		}
		else return false;
	}
}

bool Graph::updateFlow(int* MAX_FLOW) {
	bool isChannelEmpty = false;
	int minCapacity = INT_MAX;

	// Creates an array of the checked routes
	int prevID = 0;
	int* IDs = new int[pathToEnd.get_size()];

	for (int i = pathToEnd.get_size() - 1; i >= 0; i--) {
		IDs[i] = pathToEnd.at(i);

		int capacity = flow[IDs[i]].capacity.find_value(prevID);
		if (capacity < minCapacity && capacity)
			minCapacity = capacity;

		prevID = IDs[i];
	}

	prevID = 0;

	for (int i = pathToEnd.get_size() - 1; i >= 0; i--) {
		flow[IDs[i]].capacity.Value_Sub(prevID, minCapacity);

		if ((IDs[i] == items.get_size() - 1) && !flow[IDs[i]].capacity.find_value(prevID))
			isChannelEmpty = true;

		if (flow[IDs[i]].checked && flow[IDs[i]].capacity.find_value(prevID))
			flow[IDs[i]].checked = false;

		prevID = IDs[i];
	}

	if (minCapacity > 0 && *MAX_FLOW == INT_MIN)
		*MAX_FLOW = 0;

	*MAX_FLOW = *MAX_FLOW + minCapacity;

	pathToEnd.clear();

	return isChannelEmpty;
}

string Graph::readWord(string& route) {
	int i = 0;
	while (route[i] != ' ' && route[i] != '\0') i++;

	string word = route.substr(0, i);
	route.replace(0, word.length() + 1, "");

	return word;
};

void Graph::readLine(string line, string& item_one, string& item_two, int* channel_width) {
	item_one = readWord(line); 
	item_two = readWord(line); 

	string bw = readWord(line); 

	*channel_width = stoi(bw); 

	if (*channel_width <= 0) throw invalid_argument("Capacity must be greater than 0");
};