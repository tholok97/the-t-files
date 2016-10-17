/*
 * CA.h
 * v1.0
 * Beskrivelse: CA er en implementasjon av et elementært cellular automata 
	system. Kan bruke CA til å simulere steg av et gitt system. CA inneholder 
	funksjoner for å printe generasjonene med cout, men det er også mulig å ta 
	systemet direkte å lage egen output. Et system er i denne klassen beskrevet 
	som en vektor av booler, typdef'et til vec.
 * Der input er valgfritt brukes disse defaultverdiene:
	- Regel: 		30
	- Bakgrunn: 	'0'
	- Ink: 			'1'
 * <eksempelbruk1>
 
	CA ca(200);			// Nytt CA med vidde 200. Default regler brukes (30)
	while (true) {		
		ca.printf();	// Print systemet på egen linje med default verdier
		ca.step();		// Ta et steg
	}					// Repeter^. En får da se generasjonene av et CA i cmd
	
   </eksempelbruk1>
 * <eksempelbruk2>
 
	bool tmp[] = {0,0,1,0,1,1,1,1,1,0};		// tmp variabel til utgangssystem
	CA ca(vector<bool>(tmp, tmp+10), 82);	// nytt CA med tmp og regel 82
	for (int i = 0; i < 30; ++i) {			// vis 30 generasjoner
		ca.print(219, ' ');					// vis med 219 maling, ' ' bakgrunn
		cout << " Gen: " << i << endl;		// legg på hvilken gen det er og \n
		ca.step();							// ta neste steg
	}
	
   </eksempelbruk2>
*/

#ifndef CAHEADER
#define CAHEADER

#include <iostream>
#include <vector>
#include <cmath>

class CA {
	
	// Standardverdi i tilfelle disse ikke blir spesifisert
	static const int DEFAULTRULESET = 30;			// standardverdi ruleset
	static const char DEFAULTBACKGROUND = '0';		// standardverdi bakgrunn
	static const char DEFAULTINK = '1';				// standardverdi ink
	
	// typedef av vec siden det brukes over gjennom hele koden
	typedef std::vector<bool>  vec;
	
	vec arr; 		// den nåværende tilstanden til systemet
	vec ruleset;	// regelsettet å bruke på systemet for hvert steg
	int gen;		// hvilken "generasjon" systemet er i
	
	
	// Interne utility funksjoner:
	void init(const vec initArr, const int rulesetInt);		// initialiserer
	vec rulesetIntToVec(const int rulesetInt);				// int til vec
	bool rules(const bool a, const bool b, const bool c); 	// Gir ny x verdi 
															// gitt x-1, x, x+1
	
	// offentlige funksjoner følger:
	public:
	
		// Constructors:
		CA(const vec initArr, const int rulesetInt);
		CA(const vec initArr);				
		CA(const int size, const int rulesetInt);	
		CA(const int size);					
		
		// Gettere og settere:
		vec getArr() {return arr;}			
		vec getRuleset() {return ruleset;}	
		int getGen() {return gen;}
		
		// Div. funksjoner:
		void print(const char background, const char ink);	// print med maling
		void printf(const char background, const char ink);	// -||- pluss \n
		void print();							// print med default maling
		void printf();							// -||- pluss \n
		void step();							// ta ett steg
		void step(const int n);					// ta n steg
};

//--------------------------INTERNE UTILITY FUNKSJONER--------------------------

// Initialiserer systemet med utgangsvec og spesifisert regelsett. Regelsett
//  gis som int, oversettes til vec. Alle constructors ender opp her
void CA::init(const vec initArr, const int rulesetInt) {
	arr = initArr;
	ruleset = rulesetIntToVec(rulesetInt);
	gen = 0;
}

// Oversett regelnummer til tilsvarende regelsett-vec. Tar en int og returnerer
//  en vec.
CA::vec CA::rulesetIntToVec(int rulesetInt) {
	bool tmp[8];						// tmp bool array til utregning
	
	// gjør rulesetInt om til et binærtall lagret i tmp
	int i = 0, r;						
	while (rulesetInt != 0) {
		r = rulesetInt % 2;
		tmp[i++] = (r != 0) ? 1 : 0;
		rulesetInt /= 2;
	}
	
	// resten av tmp med 0'er, slik at tmp er fyllt med 0 og 1
	for (int j = i; j < 8; ++j)
		tmp[j] = false;
	
	return vec(tmp, tmp+8); 	// returnerer en vec basert på tmp
}

// Returnerer ny verdi for en celle basert på (a, b, c) = (cellen til venstre,
//  cellen selv, og cellen til høyre) basert på regelsettet. Gjør dette ved å
//  oversette det binære tallet abc om til en posisjon i regelsettet ved: 
//  a*4 + b*2 + c. (tvinger verdiene til å være enten 1 eller 0 siden bool
//  ikke garanterer å være en av disse
bool CA::rules(const bool a, const bool b, const bool c) {	
	return ruleset[((a != 0) ? 1 : 0)*4 + ((b != 0) ? 1 : 0)*2 +
			((c != 0) ? 1 : 0)];								
}

//--------------------------------CONSTRUCTORS----------------------------------

// Lager et nytt CA basert på en utgangs-vec og et regelsettnummer.
CA::CA(const vec initArr, const int rulesetInt) {
	init(initArr, rulesetInt);
}

// Lager et nytt CA basert på et utgangs-vec og default regelsettnummer
CA::CA(const vec initArr) {
	init(initArr, DEFAULTRULESET);
}

// Lager et nytt CA basert på en størrelse og et regelsettnummer. Utangs-vec'en
//  blir en vec med størrelse size fyllt med 0'er med én 1 i midten.
CA::CA(const int size, const int rulesetInt) {
	vec initArr(size);				// tom vec fyllt med 0
	initArr[size/2] = true;			// plasser 1 i midten
	init(initArr, rulesetInt);
}

// Lager et nytt CA basert på en størrelse og default regelsettnummer. Utgangs-
//  vec'en blir en vec med størrelse size fyllt med 0'er med én 1 i midten
CA::CA(const int size) {
	vec initArr(size);				// tom vec fyllt med 0
	initArr[size/2] = true;         // plasser 1 i midten
	init(initArr, DEFAULTRULESET);
}

//---------------------------DIV. FUNKSJONER------------------------------------

// Printer ut CA med spesifisert bakgrunn og maling (uten linjeskift)
void CA::print(const char background, const char ink) {
	for (vec::iterator it = arr.begin(); it != arr.end(); ++it) {
		std::cout << ((*it != 0) ? ink : background);
	}	// itererer over cellene i vec og printer bakgrunn hvis 0, 1 hvis ikke
}

// Printer ut CA med default bakgrunn og maling med linjeskift
void CA::printf() {
	printf(DEFAULTINK, DEFAULTBACKGROUND);
}

// Printer CA med default bakgrunn og maling (uten linjeskift)
void CA::print() {
	print(DEFAULTINK, DEFAULTBACKGROUND);
}

// Printer ut CA med spesifisert bakgrunn og maling med linjeskift
void CA::printf(const char background, const char ink) {
	print(background, ink);
	std::cout << '\n';
}

// Ta et steg. Oppdater systemet med ruleset. Hver celle i systemet får en ny 
//  verdi basert på naboene til cellen. (NB! systemet "wrapper": naboene til 
//  i = 0 er max og 1)
void CA::step() {
	vec next(arr.size());	// vec for neste steg
	for (vec::size_type i = 0; i < arr.size(); ++i) {	// Regner ut neste verdi
		bool a = (i==0) ? arr[arr.size()-1] : arr[i-1];	// for cellen i basert
		bool b = arr[i];								// på naboene. 
		bool c = (i==arr.size()-1) ? arr[0] : arr[i+1];
		
		next[i] = rules(a, b, c);						// regn ut cellen i
	}
	
	arr = next;				// oppdater systemet til det neste systemet
	++gen;					// øk gen
}

// Ta n antall steg. Utility funksjon. Kjører step n ganger.
void CA::step(const int n) {
	for (int i = 0; i < n; ++i)
		step();
}

#endif