#ifndef DJIKSTRAHEADER
#define DJIKSTRAHEADER

#include <iostream>
#include <vector>
#include <fstream>

struct Pair {
	int name;
	int weight;
};

class Dijkstra {
	
	typedef std::vector<std::vector<Pair>> adjacencyVec;
	
	private:
		const int INF = 9999;
		
		adjacencyVec advec;
		
		bool isIn(int value, std::vector<int> vec);
		
	public:
		Dijkstra(std::vector<std::vector<int>> vec);
		std::vector<int> doTheThing(int start, int end);
		void printAdjacencyList();
};

bool Dijkstra::isIn(int value, std::vector<int> vec) {
	for (int i : vec) {
		if (i == value)
			return true;
	}
	return false;
}

Dijkstra::Dijkstra(std::vector<std::vector<int>> vec) {
	
	// vec må være m x m
	for (int i = 0; i < vec.size(); ++i) {
		advec.push_back(std::vector<Pair>());
		for (int j = 0; j < vec[i].size(); ++j) {
			if (vec[j][i] != 0)
				advec[i].push_back(Pair {j, vec[j][i]});
		}
	}
}

std::vector<int> Dijkstra::doTheThing(int start, int end) {
	
	int current = start;	// noden som evalueres. settes til start
	std::vector<int> dist;		// distanse fra start til [index]
	std::vector<int> prev;		// den forrigen noden til [index]
	std::vector<int> unvisited;	// ubesøkte noder
	
	for (adjacencyVec::size_type i = 0; i < advec.size(); ++i) {
		dist.push_back(INF);	// sett alle distanser til 'evig'
		prev.push_back(-1);			// sett alle parents til -1 (ingenting)
	}
	
	dist[start] = 0; 		// distansen til start fra start er 0 (duh)
	
	for (adjacencyVec::size_type i = 0; i < advec.size(); ++i) {
		if (i != start)
			unvisited.push_back(i);
	}
	
	while (true) {
		for (int i = 0; i < advec[current].size(); ++i) {
			if (isIn(advec[current][i].name, unvisited)) {
				int tentDist = advec[current][i].weight + dist[current];
				if (tentDist < dist[advec[current][i].name]) {
					dist[advec[current][i].name] = tentDist;
					prev[advec[current][i].name] = current;
				}
			}
		}
		
		int pos;
		for (pos = 0; pos < unvisited.size(); ++pos) {
			if (current == unvisited[pos])
				break;
		}
		if (pos < unvisited.size()) {
			unvisited.erase(unvisited.begin()+pos);
		}
		
		if (!isIn(end, unvisited)) {
			break;
		}
		
		Pair lowest = {unvisited[0], dist[unvisited[0]]};
		for (std::vector<int>::size_type i = 0; i < unvisited.size(); ++i) {
			if (dist[unvisited[i]] < lowest.weight) {
				lowest.weight = dist[unvisited[i]];
				lowest.name = unvisited[i];
			}
		}
		
		if (lowest.weight == INF) {
			// finnes ingen vei
			return std::vector<int>();
		} else {
			current = lowest.name;
		}
	}
	
	std::vector<int> path;
	std::vector<int> rpath;
	
	int i = end;
	while (i != start) {
		path.push_back(i);
		i = prev[i];
	}
	path.push_back(start);
	
	for (auto it = path.rbegin(); it != path.rend(); ++it) {
		rpath.push_back(*it);
	}
	
	return rpath;
}

void Dijkstra::printAdjacencyList() {
	
	std::cout << "Printing adjacency list: \n";
	for (adjacencyVec::size_type i = 0; i < advec.size(); ++i) {
		std::cout << "\t" << i << ": ";
		for (Pair p : advec[i]) {
			std::cout << "(" << p.name << ", " << p.weight << "), ";
		}
		std::cout << std::endl;
	}
}

#endif
