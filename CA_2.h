/*
 * CA_2.h
 * Laget av: Thomas Løkkeborg
 * Beskrivelse: CA_2 er en implementasjon av et elementært cellular-automata-
	system. Bruker bitset til å representere systemet.
 * Defaultverdier:
	- Constructor:
		> System: Én aktivert celle i midten (for. eks: 0000001000000)
		> Regelsett: 30 (altså 00011110)
	- PrintCA():
		> Output: std::cout;
		> "På"-noder: 219 (altså █)
		> "Av"-noder: 176 (altså ░)
 * <eksempelbruk1>

	// Et system med bredde 50 (systemet og regelsettet får defaultverdier)
	CA<50> ca;			
	while (true) {							// Evig loop	
		cout << ca.getIteration() << '\t';	// Vis iterasjon
		printCA(ca++) << endl;		// Print systemet med den globale utilit-
	}								//	-funksjonen	

   </eksempelbruk1>
*/

#ifndef CABITSETEDHEADER
#define CABITSETEDHEADER

#include <iostream>
#include <bitset>
#include <string>

//-------------------------- (FORWARD) DEKLARASJONER ---------------------------

template <std::size_t N> class CA;
template <std::size_t N> std::ostream& operator<<(std::ostream& os, 
		const CA<N>& ca);
template <std::size_t N> std::ostream& printCA(const CA<N>& ca, 
		std::ostream& = std::cout, const char on = 219, const char off = 176); 

//----------------------------- KLASSE-DEFINISJON ------------------------------

template <std::size_t N>
class CA {
	// Friends: (<<-operatoren og en utility-funksjon for å printe systemet)
	friend std::ostream& operator<< <N>(std::ostream&, const CA<N>&);
	friend std::ostream& printCA<N>(const CA& ca, std::ostream& os, 
			const char on, const char off);

	private:
		std::bitset<N> system;	// systemet
		std::bitset<8> ruleset;	// regelsettet (alltid bredde 8)
		int iteration;			// iterasjonen
		bool rules(bool a, bool b, bool c) const;	
			// ^ regner ut ny verdi for en celle basert på tre naboer

	public:
		// Constructor:
		CA(const std::bitset<N> sys =
				std::bitset<N>(std::string(N, '0').replace(N/2, 1, "1")), 
				const std::bitset<8> rule = 30);

		// Overloadede operatorer:
		constexpr bool operator[](std::size_t pos) const;
		typename std::bitset<N>::reference operator[](size_t pos);
		CA& operator++();
		CA operator++(int);

		// Gettere og settere:
		std::bitset<N> getSystem() const;
		std::bitset<8> getRuleset() const;
		int getIteration() const;
		void setSystem(const std::bitset<N> sys);
		void setRuleset(const std::bitset<8> rule);

		void step();	// Tar ett steg
};

//---------------------------- MEMBER DEFINISJONER -----------------------------

// Lager et nytt CA<N>-objekt basert på størrelse, utgangssystem og regelsett.
//	Om regelsettet ikke er oppgitt får dette verdi 30, om utganssystemet ikke er
//	oppgitt får det én aktivert node i midten (000010000). Iteration begynner på
//	null. Størrelse på systemet MÅ oppgis (som template variabel)
template <std::size_t N>
CA<N>::CA(const std::bitset<N> sys, const std::bitset<8> rule): system(sys), 
		ruleset(rule), iteration(0) { }

// Overloadet []-operator på const-objekter. Constexpr-versjon pga. bitset har 
//	dette. Returnerer resultatet av []-operatoren på bitsettet
template <std::size_t N>
constexpr bool CA<N>::operator[](std::size_t pos) const {
	return system[pos];
}

// Overloadet []-operator. Returnerer en verdi en kan nå tak i systemet med :)
//	Skal fungere på samme måte som bitset::operator[](...)
template <std::size_t N>
typename std::bitset<N>::reference CA<N>::operator[](std::size_t pos) {
	return system[pos];
}

// Overloadet ++-operator (suffix). Tar et steg og returnerer en referense til
//	systemet
template <std::size_t N>
CA<N>& CA<N>::operator++() {
	step();
	return *this;
}

// Overloadet ++-operator (postfix). Tar et steg og returnerer en kopi av den
//	forrige iterasjonen til systemet
template <std::size_t N>
CA<N> CA<N>::operator++(int) {
	CA tmp(*this);
	operator++();
	return tmp;
}

// Returnerer systemet
template <std::size_t N>
std::bitset<N> CA<N>::getSystem() const {
	return system;	
}

// Returnerer regelsettet
template <std::size_t N>
std::bitset<8> CA<N>::getRuleset() const {
	return ruleset;
}

// Returnerer hvilke iterasjon systemet er i
template <std::size_t N>
int CA<N>::getIteration() const {
	return iteration;
}

// Setter systemet til 'sys'
template <std::size_t N>
void CA<N>::setSystem(const std::bitset<N> sys) {
		system = sys;
}

// Setter regelsettet til 'rule'
template <std::size_t N>
void CA<N>::setRuleset(const std::bitset<8> rule) {
	ruleset = rule;
}

// Returnerer ny verdi for en celle basert på verdien til de tre naboene. Hvor
//	a er verdien til venstre nabo, b er verdien til noden selv og c er verdien
//	til høyre nabo. Bruker en verdi fra 'ruleset' regnet utifra a, b og c til å
//	bestemme ny verdi til cellen
template <std::size_t N>
bool CA<N>::rules(bool a, bool b, bool c) const {
	std::size_t pos = (a ? 1 : 0)*4 + (b ? 1 : 0)*2 + (c ? 1 : 0)*1;
	return ruleset[pos];
}

// Ta et steg. Regner ut ny verdi for alle cellene og lagre dem i et midlertidig 
//	system. Setter så system lik dette nye systemet og øker 'iteration'.
// NB! Legg merke til at systemet "wrapper". Naboene til celle 0 blir for. eks:
//	(a: bredde-1, b: 0, c: 1)
template <std::size_t N>
void CA<N>::step() {
	std::bitset<N> next;	// midlertidig bitset
	for (std::size_t i = 0; i < system.size(); ++i) {
		bool a = (i == 0) ? system[system.size() - 1] : system[i - 1];
		bool b = system[i];
		bool c = (i == system.size() - 1) ? system[0] : system[i + 1];

		next[i] = rules(a, b, c);	// bruker 'rules' til å regne ut ny verdi
	}
	system = next;		// setter system til det nye systemet
	++iteration;		// øker iteration
}

//---------------------------- GLOBALE DEFINISJONER ----------------------------

// Overloadet <<-operator. Skriver ut via <<-operatoren på bitset
template <std::size_t N>
std::ostream& operator<<(std::ostream& os, const CA<N>& ca) {
	os << ca.system;
	return os;
}

// Printer ut et gitt system på en gitt ostream med gitte bokstaver for "på"- 
//	og "av"-celler. Om bokstavene ikke oppgis får "på"-celler bokstav 219 og 
//	"av"-celler verdi 176. Om ostream ikke oppgis brukes std::cout
template <std::size_t N>
std::ostream& printCA(const CA<N>& ca, std::ostream& os, const char on, 
		const char off) {
	for (size_t i = 0; i < ca.system.size(); ++i)
		os << char(ca.system[i] ? on : off);
	return os;
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
