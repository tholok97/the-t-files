/*
 * Dijkstra.h
 * v4.0
 * Beskrivelse: Dijkstra.h lar deg regne ut kjappeste rute gjennom en graf. Et 
	Dijkstra objekt lages ved å oppgi enten en liste med nodenavn og en 
	nabo(node1, node2)-funksjon for å avgjøre hvilke nodepar som er naboer,
	en liste med nodenavn og en nabomatrise	eller en grid av bool-verdier, hvor 
	sann er lovlig terreng og usann er ulovlig terreng (kan brukes til å lage 
	graf direkte fra "kart"). Etter grafen er laget kan en regne ut kjappeste 
	rute ved Dijkstras algoritme for flere (start, slutt)-par. 
* NB! Om objektet lages fra grid vil hver node få navn utifra posisjonen sin i 
	grid. Foreks. vil objekt ved posisjon (2, 13) i et 102 x 53 grid få navn 
	"002013" (string-navn er en midlertidig løsning til  jeg lærer meg 
	template'ering
 * Potensielle forbedringer:
	- bruke <algorithm> "enda" mer aktivt
	- unvisited burde være priority queue?
	- nodes burde være set (??)
	- nodenavntypen burde kunne velges (template) (eks: 
		Dijkstra<pair<int,int>> (...) <- nodenavn er par av int'er (x,y))
	- rydde opp :^)
	- OBS! koden burde bruke utility-funksjoene som konverterer fra name til
		(r,c) og omvendt.
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
	
	// Dijkstra-objekt basert på grid. Navn blir valgt etter posisjon i grid
	Dijkstra d(grid);
	vector<string> path = d.findPath("00", "07");	// rute fra (0,0) til (0,7)
	
	cout << "Best path from 00 to 07 is: ";			// print rute
	for (string step : path)
		cout << step << " ";
	
   </eksempelbruk 1>
 * <eksempelbruk 2>
 
	vector<string> nodes = {"A", "B", "C", "D"};	// nodeliste
	vector<vector<int>> admatrix = {	{0,0,0,0},	// nabomatrise
										{0,0,1,0},
										{1,0,0,0},
										{4,1,0,0}};
	
	Dijkstra d(nodes, admatrix);	// Dijkstra-objekt
	
	vector<string> path = d.findPath("A", "D");	// finn sti fra A til D
	
	for (string step : path)	// print sti
		cout << step << endl;
	
 
   </eksempelbruk 2>
*/

#ifndef DIJKSTRAHEADER
#define DIJKSTRAHEADER

#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <string>
#include <set>

class Dijkstra {
	
	private:
		// advec er kantlista
		std::map<std::string, std::vector<std::pair<std::string, int>>> advec;
		std::vector<std::string> nodes;	// navnene til nodene
		const int INF = 999999;			// "evig"
		bool broken = false;	// flagg som blir true hvis constructor feiler
		
		// init basert på noder og nabofunskjon
		void init(std::vector<std::string> nodes, 
				int (*neighboor)(std::string a, std::string b));
		
	public:
		// constructorer
		Dijkstra(std::vector<std::string> nodes,	// nodeliste + nabofunksjon 
				int (*neighboor)(std::string a, std::string b));
		Dijkstra(std::vector<std::string> nodes, 	// nodeliste + nabomatrise
				std::vector<std::vector<int>> adjacencyMatrix);
		Dijkstra(std::vector<std::vector<bool>> grid);	// grid (navn blir r,c)
		
		// funksjoner
		std::vector<std::string> findPath(std::string start,	// finner sti
				std::string end);								// ved dijkstra
		// Samme som over ^ , men lagrer bes�kte nodenavn i set
		std::vector<std::string> findPathSaveVisited(std::string start,	  
				std::string end, std::set<std::string> &visited);
};

// Globale funksjoner:
std::string rcToNodeName(int r, int c, int digits); 	// (r,c) -> name
std::pair<int, int> nodeNameToRCPair(std::string name);	// name -> (r,c)

//---------------------------------CONSTRUCTORS---------------------------------

// Lager et nytt objekt basert på en liste med nodenavn og en funskjon som kan
//  brukes for å sjekke hvilke av nodene i listen som er naboer. Kantliste blir
//  basert på denne informasjonen. Bruker init
Dijkstra::Dijkstra(std::vector<std::string> nodes, 
		int (*neighboor)(std::string a, std::string b)) {

	// om nodelista er tom gir vi opp..
	if (nodes.empty()) {
		broken = true;
		return;
	}
	
	init(nodes, neighboor);
}

// Lager et nytt objekt basert på en liste med nodenavn og en nabomatrise. 
//	Matrisen MÅ være (antall nodenavn) x (antall nodenavn)!!!!!
Dijkstra::Dijkstra(std::vector<std::string> nodes, 
		std::vector<std::vector<int>> adjacencyMatrix) {

	// Sjekk om infoen er gyldig. Hvis ikke: gi opp..
	if (nodes.empty()) {
		broken = true;
		return;
	} else if (adjacencyMatrix.empty()) {
		broken = true;
		return;
	} else if (adjacencyMatrix[0].empty()) {
		broken = true;
		return;
	} else if (nodes.size() != adjacencyMatrix.size() || 
			nodes.size() != adjacencyMatrix[0].size()) {
		broken = true;
		return;
	}

	this->nodes = nodes;	// lagrer nodenavnene i this->nodes
	
	// Går igjennom nabomatrisa lager advec basert på den
	for (std::vector<std::vector<int>>::size_type i = 0; 
			i < adjacencyMatrix.size(); ++i) {
		for (std::vector<int>::size_type j = 0; j < adjacencyMatrix[i].size();
				++j) {
			if (adjacencyMatrix[i][j])
				advec[nodes[j]].push_back(std::pair<std::string, int>(nodes[i], 
						adjacencyMatrix[i][j]));
		}
	}
}

// Lager et nytt objekt basert på en grid av sann / usann -verdier. Finner på
//  egne navn til nodene og lager en nabofunskjon som baserer seg på måten
//  navnene blir gitt på. Navnene til nodene blir (r,c) (foreks: "13" = noden
//  på rad 1, søyle 3 i griden. sann blir tolket som lovlig node, usann blir
//  tolket som mangel på node (vegg)
// NB! noder får automatisk navn basert på posisjon i grid
Dijkstra::Dijkstra(std::vector<std::vector<bool>> grid) {
													
	// om grid er tom, gi opp
	if (grid.empty()) {
		broken = true;
		return;
	} else if (grid[0].empty()) {
		broken = true;
		return;
	}
													
	std::vector<std::string> nodes;		// nodenavn				
													
	// regner ut hvor mange siffer hver r og c trenger
	int max_size = (grid.size()>grid[0].size()) ? grid.size() : grid[0].size();
	int digits = std::to_string(max_size-1).size();
													
	// noder får navn etter koordinatene sine i grid
	for (std::vector<std::vector<bool>>::size_type i = 0; 
			i < grid.size(); ++i) {
		for (std::vector<bool>::size_type j = 0; j < grid[i].size(); ++j) {
			if (grid[i][j]) {
				// navnet blir r + c (foreks: "0001 + 0312" == "00010312"
				nodes.push_back(rcToNodeName(i, j, digits));
			}
		}
	}

	// nabofunsksjon som baserer seg på navngivingen til nodene til å finne ut
	//  om to noder er naboer. To noder er naboer hvis (abs(r1 - r2) + 
	//  abs(c1 + c2)) er mellom 0 og 2
	auto neighboor = [](std::string a, std::string b) -> int {

					std::pair<int, int> arcPair = nodeNameToRCPair(a);
					std::pair<int, int> brcPair = nodeNameToRCPair(b);
					
					int ar = arcPair.first, ac = arcPair.second;
					int br = brcPair.first, bc = brcPair.second;
					
					// regner ut r-differanse og c-differanse
					int dr = ar - br, dc = ac - bc;
					
					if (dr < 0)		// absoluttverdi
						dr *= -1;
					
					if (dc < 0)		// absoluttverdi
						dc *= -1;

					int d = dr + dc;	// totaldifferanse

					// returner true hvis totaldifferanse er mellom 0 og 2
					// false hvis ikke
					return (d < 2 && d > 0) ? 1 : 0;
				};
	
	init(nodes, neighboor);		// initialiser med nodene og nabofunksjonen
}

//--------------------------------FUNKSJONER------------------------------------

// Lager kantlista basert på en vektor av nodenavn og en funksjon får å sjekke
//  om to noder er naboer
void Dijkstra::init(std::vector<std::string> nodes, 		// funker bare for
		int (*neighboor)(std::string a, std::string b)) {	// max 10x10 (0-9)
			
	this->nodes = nodes;	// lagrer nodenavnene i this->nodes
	
	for (std::string sa : nodes)		// loop gjennom nodes x nodes
		for (std::string sb : nodes)
			if (neighboor(sa, sb))		// Hvis sa og sb er naboer: legg til 
				advec[sa].push_back(	// (sb, (sa->sb)-vekt) par til sa
						std::pair<std::string, int>(sb, neighboor(sa, sb)));
}

// Utfører Dijkstras algoritme på advec med (start, end). Returnerer stien som 
//  en vektor av nodenavn hvis denne finnes, eller en tom sti dersom ingen stier
//  finnes.
std::vector<std::string> Dijkstra::findPath(std::string start, 
		std::string end) {

	// Om broken, gi opp..
	if (broken)
		return std::vector<std::string>();
	
	// Om start eller slutt ikke finnes i nodelista, returner en tom sti (feil)
	if (find(nodes.begin(), nodes.end(), start) == nodes.end() ||
			find(nodes.begin(), nodes.end(), end) == nodes.end()) {
		return std::vector<std::string>();	
	}
		
	std::string current;	// noden som evalueres.
	std::map<std::string, int> dist;			// distanse fra start til [node]	
	std::map<std::string, std::string> prev;	// parent til [node]
	std::set<std::string> unvisited;			// set av ubesøkte noder
	
	for (std::string n : nodes) {		// Sett dist til alle noder til INF,
		dist[n] = INF;					// prev til alle noder til "" og
		prev[n] = "";					// legg alle noder unntatt start til
		if (n != start)					// unvisited
			unvisited.insert(n);
	}
	
	dist[start] = 0;	// distansen fra start til seg selv er 0;
	
	current = start;	// vi begynner med å evaluere start
	
	// utfører loopen på én node av gangen. Fortsetter til kjappest sti blir 
	// funnet, eller til det blir konkludert med at ingen stier finnes.
	while (true) {
		
		// Se på de ubesøkte nodene til current, foreslå ny dist via current,
		// hvis denne er mindre enn dist som er lagret, oppdater
		for (std::pair<std::string, int> neighboor : advec[current]) {
			if (unvisited.find(neighboor.first) != unvisited.end()) {
				int tentDist = dist[current] + neighboor.second;
				if (tentDist < dist[neighboor.first]) {	// Hvis ny er mindre:
					dist[neighboor.first] = tentDist;		// oppdater dist
					prev[neighboor.first] = current;		// oppdtaer prev
				}
			}
		}
		
		// har "besøkt" current. fjern fra ubesøkt
		unvisited.erase(current);
		
		// hvis vi har besøkt unvisited: FERDIG
		if (unvisited.find(end) == unvisited.end())	
			break;
		
		// Finn den ubesøkte noden med lavest dist
		std::pair<std::string, int> lowest(*(unvisited.begin()), 
				dist[*(unvisited.begin())]);
				
		
		for (std::string n : unvisited) {
			if (dist[n] < lowest.second) {
				lowest = std::pair<std::string, int>(n, dist[n]);
			}
		}
		
		if (lowest.second == INF) {				// Finnes ingen sti... gi opp :(
			return std::vector<std::string>();	// returner en tom sti (FERDIG)
		} else {
			current = lowest.first;				// sett current til noden med
		}										// minst dist
	}
	std::vector<std::string> rpath;				// stien (baklengs)
	
	// regn ut stien (baklengs) utifra prev[] inn i rpath
	std::string s = end;
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
//  finnes. Legger ogs� til bes�kte noder i visited settet
std::vector<std::string> Dijkstra::findPathSaveVisited(std::string start, 
		std::string end, std::set<std::string> &visited) {

	// Om broken, gi opp..
	if (broken)
		return std::vector<std::string>();
	
	// Om start eller slutt ikke finnes i nodelista, returner en tom sti (feil)
	if (find(nodes.begin(), nodes.end(), start) == nodes.end() ||
			find(nodes.begin(), nodes.end(), end) == nodes.end()) {
		return std::vector<std::string>();	
	}
		
	std::string current;	// noden som evalueres.
	std::map<std::string, int> dist;			// distanse fra start til [node]	
	std::map<std::string, std::string> prev;	// parent til [node]
	std::set<std::string> unvisited;			// set av ubesøkte noder
	
	for (std::string n : nodes) {		// Sett dist til alle noder til INF,
		dist[n] = INF;					// prev til alle noder til "" og
		prev[n] = "";					// legg alle noder unntatt start til
		if (n != start)					// unvisited
			unvisited.insert(n);
	}
	
	dist[start] = 0;	// distansen fra start til seg selv er 0;
	
	current = start;	// vi begynner med å evaluere start
	
	// utfører loopen på én node av gangen. Fortsetter til kjappest sti blir 
	// funnet, eller til det blir konkludert med at ingen stier finnes.
	while (true) {
		
		// Se på de ubesøkte nodene til current, foreslå ny dist via current,
		// hvis denne er mindre enn dist som er lagret, oppdater
		for (std::pair<std::string, int> neighboor : advec[current]) {
			if (unvisited.find(neighboor.first) != unvisited.end()) {
				int tentDist = dist[current] + neighboor.second;
				if (tentDist < dist[neighboor.first]) {	// Hvis ny er mindre:
					dist[neighboor.first] = tentDist;		// oppdater dist
					prev[neighboor.first] = current;		// oppdtaer prev
				}
			}
		}
		
		// har "besøkt" current. fjern fra ubesøkt
		unvisited.erase(current);
		visited.insert(current);
		
		// hvis vi har besøkt unvisited: FERDIG
		if (unvisited.find(end) == unvisited.end())	
			break;
		
		// Finn den ubesøkte noden med lavest dist
		std::pair<std::string, int> lowest(*(unvisited.begin()), 
				dist[*(unvisited.begin())]);
				
		for (std::string n : unvisited) {
			if (dist[n] < lowest.second) {
				lowest = std::pair<std::string, int>(n, dist[n]);
			}
		}
		
		if (lowest.second == INF) {				// Finnes ingen sti... gi opp :(
			return std::vector<std::string>();	// returner en tom sti (FERDIG)
		} else {
			current = lowest.first;				// sett current til noden med
		}										// minst dist
	}
	std::vector<std::string> rpath;				// stien (baklengs)
	
	// regn ut stien (baklengs) utifra prev[] inn i rpath
	std::string s = end;
	while (s != start) {
		rpath.push_back(s);
		s = prev[s];
	}
	rpath.push_back(start);
	
	reverse(rpath.begin(), rpath.end());	// reverser for å få start -> end
		
		
	return rpath; // returner sti
}

//----------------------------GLOBALE FUNKSJONER--------------------------------

// Tar et (r,c) par og returnerer et nodenavn med minimumslengde 2 * digits
// Midlertidig l�sning mens jeg l�rer meg kunsten � skrive template-typer
std::string rcToNodeName(int r, int c, int digits) {

	if (r < 0 || c < 0) 	// returner en tom string ved ugyldige verdier
		return "";
	
	std::string rs = std::to_string(r);		// Oversetter til string
	std::string cs = std::to_string(c);

	for (;rs.size() < digits; rs = "0" + rs); 		// legger til 0 slik at  
	for (;cs.size() < digits; cs = "0" + cs);		// lengden blir digits

	return rs + cs; 	// returnerer den samlede stringen rs + cs
}

// Tar et nodenavn p� formen "rc" og returnerer r og c som et par av inter
// NB! bruk bare p� navn p� formen "rc". Midltertidig l�sning mens jeg l�rer meg
// kunsten � skrive template-typer
std::pair<int, int> nodeNameToRCPair(std::string name) {

	if (name.empty()) {		// ved ugyldig nanv, return ugyldig string
		return std::pair<int, int>(-1, -1);
	} else if (name.size() % 2 != 0) {
		return std::pair<int, int>(-1, -1);
	}

	// henter ut r, c informasjonen fra name
	std::string rs = name.substr(0,name.size()/2);
	std::string cs = name.substr(name.size()/2, name.size()/2);

	int r = stoi(rs); 	// oversetter til int
	int c = stoi(cs);

	return std::pair<int, int>(r, c);	// returnerer et par med (r, c)
}

#endif
