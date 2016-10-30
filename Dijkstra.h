/*
 * Dijkstra.h
 * v1.0
 * Beskrivelse: Dijkstra.h lar deg regne ut kjappeste rute gitt nabomatrise og 
	start, slutt. Et Dijkstra objekt lages ved å oppgi en nabomatrise. En kan
	da regne ut kjappeste rute ved forskjellige (start, slutt) par.
 * Potensielle forbedringer:
	- bruke <algorithm> aktivt
	- mer kreativt bruk av containers (vektor--);
	- skriv om doTheThing til å jobbe direkte på nabomatrisa? (kutte ut advec)
	- skriv om til å la brukeren definere når to noder er naboer (tilate grid)
	- rydde opp :^)
 * <eksempelbruk 1>
 
	vector<vector<int>> matrix = {				// nabomatrisa
				{ 0, 0, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 1, 0, 0, 0 },
				{ 0, 1, 0, 0 }		
			};
	
	Dijkstra d(matrix);							// d er et Dijkstra-objekt
	
	vector<int> path = d.doTheThing(0, 3);		// finner sti
	
	for (int i : path)							// printer sti
		cout << i << " ";
	cout << endl;
	
   </eksempelbruk 1>
*/

#ifndef DJIKSTRAHEADER
#define DJIKSTRAHEADER

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

// Struct for å holde (navn, vekt) par. Brukt i kantlista 
struct Pair {
	int name;
	int weight;
};

class Dijkstra {
	
	// typedef for kantlistetypen
	typedef std::vector<std::vector<Pair>> adjacencyVec;
	
	private:
		const int INF = 9999;							// "evig"
		
		adjacencyVec advec;								// kantlista
		
		bool isIn(int value, std::vector<int> vec);		// om value er i vec
		
	public:
		Dijkstra(std::vector<std::vector<int>> vec);		// constructor
		std::vector<int> doTheThing(int start, int end);	// finn sti
		void printAdjacencyList();							// print advec
};

// Returnerer om value er i vec. Intern
//  (burde erstattes av <algorithm>
bool Dijkstra::isIn(int value, std::vector<int> vec) {
	for (int i : vec) {
		if (i == value)
			return true;
	}
	return false;
}

// Lager et nytt Dijkstra-objekt basert på en nabomatrise gitt som en vektor av
//  vektorer av int'er
Dijkstra::Dijkstra(std::vector<std::vector<int>> vec) {
	
	// vec må være m x m
	for (int i = 0; i < vec.size(); ++i) {					// hvis noden i er 
		advec.push_back(std::vector<Pair>());				// nabo til j, legg
		for (int j = 0; j < vec[i].size(); ++j) {			// til (navn, vekt)
			if (vec[j][i] != 0)								// par i advec[i]
				advec[i].push_back(Pair {j, vec[j][i]});	
		}
	}
}

// Utfører Dijkstras algoritme på advec med (start, end). Returnerer stien
//  dersom en slik finnes, en tom sti dersom ingen stier finnes. Stien blir 
//  returnert som en vektor av inter, hvor hver int representerer en node
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
		if (i != start)		// alle noder unntatt start regnes som ubesøkt
			unvisited.push_back(i);
	}
	
	// utfør loopen på én node av gangen. starter med start
	while (true) {
		
		// se på de ubesøkte naboene, og regn ut distanse fra noden til naboen.
		// hvis denne er mindre enn distansen som er lagret, oppdater
		for (int i = 0; i < advec[current].size(); ++i) {
			if (isIn(advec[current][i].name, unvisited)) {
				int tentDist = advec[current][i].weight + dist[current];
				if (tentDist < dist[advec[current][i].name]) {
					dist[advec[current][i].name] = tentDist;	// oppdater dist
					prev[advec[current][i].name] = current;	// oppdater parent
				}
			}
		}
		
		// slett noden fra ubesøkt
		auto toDelete = std::find(unvisited.cbegin(), unvisited.cend(), 
				current);
		if (toDelete != unvisited.end())
			unvisited.erase(toDelete);
		
		if (!isIn(end, unvisited)) {	// Hvis vi har besøkt slutt: ferdig
			break;
		}
		
		// Finner den ubesøkte noden med lavest dist
		Pair lowest = {unvisited[0], dist[unvisited[0]]};
		for (std::vector<int>::size_type i = 0; i < unvisited.size(); ++i) {
			if (dist[unvisited[i]] < lowest.weight) {
				lowest.weight = dist[unvisited[i]];
				lowest.name = unvisited[i];
			}
		}
		
		if (lowest.weight == INF) {		// Finnes ingen vei... gi opp
			return std::vector<int>();
		} else {						// sett current til noden med minst dist
			current = lowest.name;
		}
	}
	
	std::vector<int> rpath;		// stien (baklengs)
	std::vector<int> path;		// stien
	
	// regner ut stien (baklengs) utifa prev[] inn i rpath
	int i = end;
	while (i != start) {
		rpath.push_back(i);
		i = prev[i];
	}
	rpath.push_back(start);
	
	// reverserer stien og lagrer i path
	for (auto it = rpath.rbegin(); it != rpath.rend(); ++it) {
		path.push_back(*it);
	}
	
	return path;	// returner stien
}

// Printer advec
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
