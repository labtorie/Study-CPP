#include <iostream>
#include <fstream>
#include "DijkstraAlg.h"

int main()
{
	std::ifstream file("routes.txt");

	List<std::string> lines;

	while (file)
	{
		std::string str;
		getline(file, str);

		lines.push_back(str);
	}
	lines.pop_back();

	List<std::string*> normalized_info = convert(lines);

	List<std::string> namesList = getUniqueNames(normalized_info);

	Graph G(namesList);

	G.makeGraphFromList(normalized_info);

	std::string start, end;
	cout << "Departure City: ";
	cin >> start;
	cout << "\nArrival City: ";
	cin >> end;
	cout << endl << endl;
	cout << G.getShortestPath(start, end);
	cout << endl << endl;

	for (size_t i = 0; i < normalized_info.getSize(); ++i)
	{
		delete[] normalized_info.at(i);
	}

	return 0;
}
