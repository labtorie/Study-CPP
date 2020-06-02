#pragma once
#include "List.h"
#include "Map.h"
#include "PriorityQueue.h"
#include <string>
#include <stdexcept>

const uint8_t ROUTES_STRING_LENGTH = 4;
const uint8_t DEPARTURE = 0;
const uint8_t DESTINATION = 1;
const uint8_t PRICE_AtoB = 2;
const uint8_t PRICE_BtoA = 3;


template<class T>
List<T> revList(List<T> lst);


class Graph
{
private:
	Map<uint16_t, uint64_t>* adjList; //adjacency list that stores vertices number and edge weight
	List<std::string> namesList; //represents link between city name and vertices number
	PriorityQueue<uint16_t> markList; //stores vertices number and its mark
	size_t size;

	//creates from start point to the destination point using marked graph
	List<std::string> makePath(PriorityQueue<uint16_t>& visitedMarksList, const uint16_t& departure, const uint16_t& destination)
	{
		List<std::string> path;

		//checks if the straight route from start to destination is the cheapest
		if (adjList[departure].contains(destination))
		{
			if (adjList[departure].find(destination) == visitedMarksList.get_priority(destination))
			{
				path.push_front(namesList.at(destination));
				path.push_front(namesList.at(departure));
				return path;
			}

		}

		//checks if destination point is reachable from the start
		if (visitedMarksList.get_priority(destination) == UINT64_MAX)
		{
			path.push_front("There is no way to get to "+ namesList.at(destination) + " from " + namesList.at(departure) );
			return path;
		}

		path.push_front(namesList.at(destination));
		auto current = destination;
		auto cur_mark = visitedMarksList.get_priority(destination);

		//the algorithm makes path from the destination point to the start
		while (current != departure)
		{
			std::string city;

			//getting all current's neighbors
			auto neighbors_list = adjList[current].get_keys();
			uint16_t neighbor;

			//checks every neighbor's mark if (current's mark - edge weight) equals it
			for (size_t i = 0; i < neighbors_list.getSize(); ++i)
			{
				neighbor = neighbors_list.at(i);

				if (cur_mark - adjList[neighbor].find(current) == visitedMarksList.get_priority(neighbor))
				{
					city = namesList.at(neighbor);
					break;
				}
			}

			path.push_front(city);
			current = neighbor;
			cur_mark = visitedMarksList.get_priority(neighbor);
		}

		return path;
	}

public:

	//default graph constructor
	explicit Graph(List<std::string>& namesList)
		: size(namesList.getSize())
	{
		this->namesList = namesList;
		adjList = new Map<uint16_t, uint64_t>[size];

		for (size_t i = 0; i < size; ++i)
			markList.insert(i, UINT64_MAX);
	}


	~Graph()
	{
		delete[] adjList;
	}

	//creates graph from list of words arrays
	void makeGraphFromList(List<std::string*>& info) const
	{
		for (size_t i = 0; i < info.getSize(); ++i)
		{
			const auto line = info.at(i);

			uint16_t departureCity = namesList.find(line[DEPARTURE]);
			uint16_t destinationCity = namesList.find(line[DESTINATION]);
			uint64_t priceAToB = std::stoull(line[PRICE_AtoB]);
			uint64_t priceBToA = std::stoull(line[PRICE_BtoA]);

			//filling the adjacency list from input info
			for (auto j = 1; j <= 2; ++j)
			{

				adjList[departureCity].insert(destinationCity, priceAToB);

				std::swap(departureCity, destinationCity);
				std::swap(priceAToB, priceBToA);
			}

		}
	}

	//finding the cheapest/shortest way with dijkstra algorithm 
	std::string getShortestPath(const std::string& start_point, const std::string& destination_point)
	{
		if (start_point == destination_point)
			return "Looks like you don't need a flight.";

		if (!namesList.contains(start_point) || !namesList.contains(destination_point))
			return "There's no such cities.";

		PriorityQueue<uint16_t> visitedMarksList; //stores visited vertices with their final marks
		const uint16_t start = namesList.find(start_point); //beginning of the required path
		const uint16_t destination = namesList.find(destination_point); //end of the required path

		markList.update(start, 0);

		while (markList.getSize())
		{
			const auto current_mark = markList.get_priority();
			const auto current = markList.extract_min(); // vertices that is currently being processed			
			const auto neighbors_list = adjList[current].get_keys(); //getting all current's neighbors

			for (size_t j = 0; j < neighbors_list.getSize(); ++j)
			{
				const auto neighbor = neighbors_list.at(j); //getting current being processed neighbor
				const auto visited = !markList.contains(neighbor); //find out if this neighbor has been already visited
				const auto edge_weight = adjList[current].find(neighbor);

				if (!visited && edge_weight != 0 && current_mark != UINT64_MAX)
				{
					const auto mark = current_mark + edge_weight;

					if (markList.get_priority(neighbor) > mark)
						markList.update(neighbor, mark);
				}

			}

			visitedMarksList.insert(current, current_mark);
		}

		//getting route from marked graph
		const auto way = makePath(visitedMarksList, start, destination);
		std::string result;

		if (way.getSize() > 1)
		{
			result = "The most affordable route: ";

			for (size_t i = 0; i < way.getSize() - 1; ++i)
				result += way.at(i) + " -> ";

			result += way.at(way.getSize() - 1) + "\nTotal cost: " + std::to_string(visitedMarksList.get_priority(destination));
		}
		else
			result = way.at(0);

		return result;

	}
};

//converts input list of lines into list of words arrays
inline List<std::string*> convert(List<std::string>& routes)
{
	if (routes.getSize() == 0)
		throw std::length_error("Input was empty!");

	List<std::string*> separated_info;

	for (size_t i = 0; i < routes.getSize(); ++i)
	{
		const auto line = new std::string[ROUTES_STRING_LENGTH];
		auto word_number = 0;

		for (auto ch : routes.at(i))
		{
			if (ch == ';')
				word_number++;
			else
				line[word_number] += ch; 
		}

		if (word_number > ROUTES_STRING_LENGTH)
			throw std::length_error("Invalid input format.");

		if (line[PRICE_AtoB] == "N/A")
			line[PRICE_AtoB] = "0";

		if (line[PRICE_BtoA] == "N/A")
			line[PRICE_BtoA] = "0";

		if (line[PRICE_AtoB] == "0" && line[PRICE_BtoA] == "0")
			throw std::logic_error("Flight was unavailable in both directions!");

		separated_info.push_back(line);
	}

	return separated_info;
}

//returns list of all cities names (without repeats)
inline List<std::string> getUniqueNames(List<std::string*>& info)
{
	List<std::string> namesList;

	for (size_t i = 0; i < info.getSize(); ++i)
	{
		if (!namesList.contains(info.at(i)[DEPARTURE]))
			namesList.push_back(info.at(i)[DEPARTURE]);

		if (!namesList.contains(info.at(i)[DESTINATION]))
			namesList.push_back(info.at(i)[DESTINATION]);
	}

	return namesList;
}

template<class T>
List<T> revList(List<T> lst)
{
	List<T> reversed;

	for (size_t i = 1; i <= lst.getSize(); ++i)
		reversed.push_back(lst.at(lst.getSize() - i));

	return reversed;
}
