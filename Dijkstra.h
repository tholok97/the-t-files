/*
 * Dijkstra.h
 * Laget av: Thomas Løkkeborg
 * Beskrivelse: Lar deg finne kjappeste rute gjennom et nettverk ved Dijkstras-
	algoritme. Klassen er templetert for å støtte mange typer nodenavn. Et 
	Dijkstra<string> objekt bruker for.eks. string-navn som nodenavn og et 
	Dijkstra<pair<int, int>> objekt bruker par av int-verdier (r, c). For å 
	danne et Dijkstra-objekt oppgir man nodenavn-typen, sammen med enten en
	nodeliste og en nabomatrise, eller en nodeliste og en nabofunksjon. Når
	objektet er klargjort kan en finne kjappeste sti fra ulike start- og slutt-
	par. Stier returneres som vektorer av nodenavn-type.
	Et Dijkstra-objekt kan også dannes basert på et (m x n) grid av boolske 
	verdier ved hjelp av en global funksjon. Noder får da navn at typen
	pair<int, int>, og med verdier basert på (r, c)-koordinatene sine i gridet.
 * Potensielle forbedringer:
	- bruke <algorithm> "enda" mer aktivt
	- unvisited burde være priority queue?
	- bruke array istedefor vector (templetere størrelsen) ?
	- constructor som tar liste med nodenavn burde bruke set? (bruker vector)
	- ::nodetype, ::pathtype    (?) :D
	- Teste eksempla...
	- Erstatte vektor med set det vektor e unødvendig
	- "evig" burde regnes ut, ikke bare være INT_MAX
 * <eksempelbruk 1>
 
	vector<vector<bool>> grid = {	{1,1,1,0,1,1,1,1},		// "kartet"
									{0,0,1,0,1,1,1,1},		// sann: lovlig
									{1,1,1,0,1,0,0,1},		// usann: ulovlig
									{1,0,0,0,1,1,1,1},
									{1,1,1,0,1,0,1,1},
									{0,0,1,0,1,1,1,1},
									{1,1,1,0,1,0,0,1},
									{1,0,0,0,1,1,0,1},
									{1,1,1,1,1,1,0,1},
									{1,1,1,1,1,1,1,1}};
	
	// Dijkstra-objekt basert på grid. Navn blir valgt etter koordinater i grid
	Dijkstra<pair<int, int> d = gridToDijkstra(grid);
	vector<string> path = d.findPath({0,0}, {0,7});	// rute fra (0,0) til (0,7)
	
	cout << "Best path from (0,0) to (0,7) is: ";			// print rute
	for (pair<int, int> step : path)
		cout << "(" << step.first << ", " << step.second << ") > ";
	
   </eksempelbruk 1>
 * <eksempelbruk 2>
 
	vector<string> nodes = {"A", "B", "C", "D"};	// nodeliste
	vector<vector<int>> admatrix = {	{0,0,0,0},	// nabomatrise
										{0,0,1,0},
										{1,0,0,0},
										{4,1,0,0}};
	
	Dijkstra<string> d(nodes, admatrix);	// Dijkstra-objekt
	
	vector<string> path = d.findPath("A", "D");	// finn sti fra A til D
	
	for (string step : path)	// print sti
		cout << step << " > ";
	
 
   </eksempelbruk 2>
*/

#ifndef DIJKSTRAHEADER
#define DIJKSTRAHEADER

#include <vector>		// vector
#include <map>			// map
#include <set>			// set
#include <utility>		// pair
#include <algorithm>	// reverse
#include <climits>		// INT_MAX

// T er nodenavn-type
template<typename T>
class Dijkstra {
	
	private:
		// advec er kantlista (en map fra T til vektor av (T, int)-par)		  :p
		std::map<T, std::set<std::pair<T, int>>> advec;
		std::set<T> nodes;	// navnene til nodene
		const int INF = INT_MAX;	// "evig"	TODO: burde regnes ut
		bool broken = false;	// flagg som blir true hvis constructor feiler
		
	public:

		// typer
		typedef T node_type;

		// constructorer
		Dijkstra(std::set<T> nodes,	// nodeliste + nabofunksjon 
				int (*neighboor)(T a, T b));
		Dijkstra(std::vector<T> nodeVector, 	// nodeliste + nabomatrise
				std::vector<std::vector<int>> adjacencyMatrix);

		// funksjoner
		std::vector<T> findPath(T start, T end);  // finner sti ved dijkstra
		// Samme som over ^ , men lagrer besøkte nodenavn i set
		std::vector<T> findPathSaveVisited(T start, T end, 
				std::set<T> &visited);
		bool isBroken() const;			// getter for 'broken'

		// Overloaded conversion to bool, til bruk i conditions
		explicit operator bool() const { return !broken; }
};

// Globale funksjoner:

// Returnerer et Dijkstra objekt med pair<int, int> som nodenavn basert på
//	et grid. Nodene får navn utifra posisjonen sin i gridet (r,c)
Dijkstra<std::pair<int, int>> gridToDijkstra(std::vector<
		std::vector<bool>> grid);

//---------------------------------CONSTRUCTORS---------------------------------

// Lager et nytt objekt basert på en liste med nodenavn og en funskjon som kan
//  brukes for å sjekke hvilke av nodene i listen som er naboer. Kantliste blir
//  basert på denne informasjonen.
template<typename T>
Dijkstra<T>::Dijkstra(std::set<T> nodes, 
		int (*neighboor)(T a, T b)) {

	// om nodelista er tom gir vi opp..
	if (nodes.empty()) {
		broken = true;
		return;
	}
	
	this->nodes = nodes;	// lagrer nodenavnene i this->nodes
	
	for (T a : nodes)		// loop gjennom nodes x nodes
		for (T b : nodes)			// Hvis a og b er naboer: legg til
			if (neighboor(a, b))	// (b, (a->b)-vekt) i a 
				advec[a].emplace(b, neighboor(a, b));
}

// Lager et nytt objekt basert på en liste med nodenavn og en nabomatrise. 
//	Matrisen må være (antall nodenavn) x (antall nodenavn)!!!!!
template<typename T>
Dijkstra<T>::Dijkstra(std::vector<T> nodeVector, 
		std::vector<std::vector<int>> adjacencyMatrix) {

	// Sjekk om infoen er ugyldig. Isåfall: gi opp (BRUKER SHORT-CIRCUIT
	if (nodeVector.empty() || adjacencyMatrix.empty() ||	// EGENSKAPEN TIL
			adjacencyMatrix[0].empty() ||					// || -OPERATOREN
			(nodeVector.size() != adjacencyMatrix.size()) || 
			(nodeVector.size() != adjacencyMatrix[0].size())) {
		broken = true;
		return;
	}

	for (T node : nodeVector)		// Lagrer nodenavn fra nodeVector i nodes
		nodes.emplace(node);

	
	// Går igjennom nabomatrisa og lager advec basert på den
	for (std::vector<std::vector<int>>::size_type i = 0; 
			i < adjacencyMatrix.size(); ++i) {
		for (std::vector<int>::size_type j = 0; j < adjacencyMatrix[i].size();
				++j) {
			if (adjacencyMatrix[i][j])
				advec[nodeVector[j]].insert(std::pair<T, int>(nodeVector[i], 
						adjacencyMatrix[i][j]));
		}
	}
}

//--------------------------------FUNKSJONER------------------------------------

// Utfører Dijkstras algoritme på advec med (start, end). Returnerer stien som 
//  en vektor av nodenavn hvis denne finnes, eller en tom sti dersom ingen stier
//  finnes.
template<typename T>
std::vector<T> Dijkstra<T>::findPath(T start, T end) {

	// Om broken, gi opp..
	if (broken)
		return std::vector<T>();
	
	// Om start eller slutt ikke finnes i nodelista, returner en tom sti (feil)
	if (nodes.find(start) == nodes.end() || nodes.find(end) == nodes.end()) {
		return std::vector<T>();
	}
		
	T current;					// noden som evalueres.
	std::map<T, int> dist;		// distanse fra start til [node]	
	std::map<T, T> prev;		// parent til [node]
	std::set<T> unvisited;		// set av ubesøkte noder
	
	for (T n : nodes) {				// Sett dist til alle noder til INF,
		dist[n] = INF;				// prev til alle noder til T() og
		prev[n] = T();				// legg alle noder unntatt start til
		if (n != start)				// unvisited
			unvisited.insert(n);
	}
	
	dist[start] = 0;	// distansen fra start til seg selv er 0;
	
	current = start;	// vi begynner med å evaluere start
	
	// utfører loopen på én node av gangen. Fortsetter til kjappest sti blir 
	// funnet, eller til det blir konkludert med at ingen stier finnes.
	while (true) {
		
		// Se på de ubesøkte nodene til current, foreslå ny dist via current,
		//	hvis denne er mindre enn dist som er lagret, oppdater
		for (std::pair<T, int> neighboor : advec[current]) {
			if (unvisited.find(neighboor.first) != unvisited.end()) {
				int tentDist = dist[current] + neighboor.second;
				if (tentDist < dist[neighboor.first]) {	// Hvis ny er mindre:
					dist[neighboor.first] = tentDist;	// oppdater dist
					prev[neighboor.first] = current;	// oppdtaer prev
				}
			}
		}
		
		// har "besøkt" current. fjern fra ubesøkt
		unvisited.erase(current);
		
		// hvis vi har besøkt unvisited: FERDIG
		if (unvisited.find(end) == unvisited.end())	
			break;
		
		// Finn den ubesøkte noden med lavest dist
		std::pair<T, int> lowest(*(unvisited.begin()), 
				dist[*(unvisited.begin())]);
				
		
		for (T n : unvisited) {
			if (dist[n] < lowest.second) {
				lowest = std::make_pair(n, dist[n]);
			}
		}
		
		if (lowest.second == INF) {		// Finnes ingen sti... gi opp :(
			return std::vector<T>();	// returner en tom sti (FERDIG)
		} else {
			current = lowest.first;			// sett current til noden med
		}									// minst dist
	}
	std::vector<T> rpath;				// stien (baklengs)
	
	// regn ut stien (baklengs) utifra prev[] inn i rpath
	T s = end;
	while (s != start) {
		rpath.push_back(s);
		s = prev[s];
	}
	rpath.push_back(start);
	
	reverse(rpath.begin(), rpath.end());	// reverser for å få start -> end
		
		
	return rpath; // returner sti
}

// Utfører Dijkstras algoritme på advec med (start, end). Returnerer stien som 
//  en vektor av nodenavn hvis denne finnes, eller en tom sti dersom ingen stier
//  finnes. Legger besøkte noder til i 'visited'
template<typename T>
std::vector<T> Dijkstra<T>::findPathSaveVisited(T start, T end,	
		std::set<T> &visited) {

	// Om broken, gi opp..
	if (broken)
		return std::vector<T>();
	
	// Om start eller slutt ikke finnes i nodelista, returner en tom sti (feil)
	if (nodes.find(start) == nodes.end() || nodes.find(end) == nodes.end()) {
		return std::vector<T>();
	}
		
	T current;					// noden som evalueres.
	std::map<T, int> dist;		// distanse fra start til [node]	
	std::map<T, T> prev;		// parent til [node]
	std::set<T> unvisited;		// set av ubesøkte noder
	
	for (T n : nodes) {				// Sett dist til alle noder til INF,
		dist[n] = INF;				// prev til alle noder til T() og
		prev[n] = T();				// legg alle noder unntatt start til
		if (n != start)				// unvisited
			unvisited.insert(n);
	}
	
	dist[start] = 0;	// distansen fra start til seg selv er 0;
	
	current = start;	// vi begynner med å evaluere start
	
	// utfører loopen på én node av gangen. Fortsetter til kjappest sti blir 
	// funnet, eller til det blir konkludert med at ingen stier finnes.
	while (true) {
		
		// Se på de ubesøkte nodene til current, foreslå ny dist via current,
		//	hvis denne er mindre enn dist som er lagret, oppdater
		for (std::pair<T, int> neighboor : advec[current]) {
			if (unvisited.find(neighboor.first) != unvisited.end()) {
				int tentDist = dist[current] + neighboor.second;
				if (tentDist < dist[neighboor.first]) {	// Hvis ny er mindre:
					dist[neighboor.first] = tentDist;	// oppdater dist
					prev[neighboor.first] = current;	// oppdtaer prev
				}
			}
		}
		
		// har "besøkt" current. fjern fra ubesøkt. legg til i besøkt
		unvisited.erase(current);
		visited.insert(current);
		
		// hvis vi har besøkt unvisited: FERDIG
		if (unvisited.find(end) == unvisited.end())	
			break;
		
		// Finn den ubesøkte noden med lavest dist
		std::pair<T, int> lowest(*(unvisited.begin()), 
				dist[*(unvisited.begin())]);
				
		
		for (T n : unvisited) {
			if (dist[n] < lowest.second) {
				lowest = std::make_pair(n, dist[n]);
			}
		}
		
		if (lowest.second == INF) {		// Finnes ingen sti... gi opp :(
			return std::vector<T>();	// returner en tom sti (FERDIG)
		} else {
			current = lowest.first;			// sett current til noden med
		}									// minst dist
	}
	std::vector<T> rpath;				// stien (baklengs)
	
	// regn ut stien (baklengs) utifra prev[] inn i rpath
	T s = end;
	while (s != start) {
		rpath.push_back(s);
		s = prev[s];
	}
	rpath.push_back(start);
	
	reverse(rpath.begin(), rpath.end());	// reverser for å få start -> end
		
		
	return rpath; // returner sti
}

template <typename T>
bool Dijkstra<T>::isBroken() const {
	return broken;
}

//------------------------------GLOBALE FUNKSJONER------------------------------

// Tar et 2-dimensjonelt grid av boolske verdier og danner nodeliste og 
//	nabofunksjon for dette. Returnerer et Dijkstra-objekt hvor nodenavn er 
//	pair<int, int>. Noder får navn basert på (r, c)-koordinatene sine i gridet.
Dijkstra<std::pair<int, int>> gridToDijkstra(std::vector<
		std::vector<bool>> grid) {

	// Bygger nodelista til grid
	std::set<std::pair<int, int>> nodes;
	for (std::vector<std::vector<bool>>::size_type i = 0; i < grid.size(); ++i)
		for (std::vector<bool>::size_type j = 0; j < grid[i].size(); ++j)
			if (grid[i][j])
				nodes.emplace(i,j);
	
	// Funksjon som bruker (r, c)-koordinatene til å bestemme om to noder er
	//	naboer
	auto neighboor = [](std::pair<int, int> a, std::pair<int, int> b) -> int {
		int r_diff = a.first - b.first;
		int c_diff = a.second - b.second;	// FORKLARING:
											// To noder a og b er naboer
		if (r_diff < 0)						//  hvis distansen mellom dem 
			r_diff *= -1;					//	i rader + -||- i søyler er 
											//	mindre enn 2. a er altså nabo
		if (c_diff < 0)						//	med b om den befinner seg i
			c_diff *= -1;					//	en av x'ene:
													//  x
		return (r_diff + c_diff < 2) ? 1 : 0;		// xax
	};												//	x

	// Returner et Dijkstra-objekt basert på nodelista og nabofunksjonen
	return Dijkstra<std::pair<int, int>>(nodes, neighboor);
}

#endif

// 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 9
//  923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 
// 3 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923
// 23 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 92
// 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 9
//  923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 
// 3 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923
// 23 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 92
// 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 9
//  923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 
// 3 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923
// 23 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 92
// 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 9
//  923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 
// 3 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923
// 23 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 923 92

	
