/*
 * Screen.h
 * Laget av: Thomas L�kkeborg
 * Beskrivelse: Implementasjon av en slags tegneflate i kommandolinjen. Lar deg
	tegne dotter, linjer og firkanter.
 * Der input er valgfritt gjelder disse defaultverdiene:
	- bakgrunn: 	176
	- maling: 		178
 * <eksempelbruk>
 
	Screen scr(100, 50);			// ny 100x50 screen med default maling osv
	scr.dot(1,1);					// dot ved (1,1)
	scr.drawRect(2,5, 30, 40);		// rektangel ved (2,5) til (30,40)
	scr.drawLine(25,25, 99,49);		// linje fra (25,25) til (99,49)
	scr.print();					// print scr til kommandolinjen
 
   </eksemeplbruk>
 * (Fikset i v1.1) BUGS: linjen tegnes ikke riktig n�r x1 < x2 XOR y1 < y2
 * Potensielle forbedringer:
	- overloadede []- og <<-operatorer
*/

#ifndef SCREENHEADER
#define SCREENHEADER

#include <iostream>
#include <string>
#include <cmath>

const float PI = 3.14;	// Tiln�rmingsverdi for pi

class Screen {
	
	private:
		// Standardverdi for bakgrunn og maling
		static const char DEFAULTBACKGROUND = 176;
		static const char DEFAULTINK = 178;
		
		int width, height;				// Vidden og H�yden p� tegneflaten
		char backgroundChar, ink;		// Bakgrunn og maling
		std::string content;			// string til � holde tegneflaten
		
		// Interne utility funksjoner
		bool within(const int x, const int y);	// Sjekk om (x, y) er innenfor
		bool swap(int &a, int &b);				// swapper intene a og b
		bool swap(float &a, float &b);			// swapper floatene a og b
	
	public:
		// Constructors:
		Screen(const int width, const int height, 
				const char backgroundChar, const char ink);
		Screen(const int width, const int height);
		
		// Gettere og settere:
		int getWidth() { return width; }
		int getHeight() { return height; }
		void setInk(const char ink) {this->ink = ink;}
		void setBackgroundchar(const char backgroundChar) {
			this->backgroundChar = backgroundChar;
		}
		char getInk() { return ink; }
		char getBackgroundChar() { return backgroundChar; }
		
		// Div. funksjoner
		bool dot(const int x, const int y);				// Tegner dot ved (x,y)
		bool drawLine(int x1, int y1, int x2, int y2);	// Tegner linje
		bool drawRect(int x, int y,
				const int width, const int height); 	// Tegner firkant
		void clear();									// Nullstiller flaten
		void print();									// Printer tegneflaten
};

//----------------------------- PRIVATE FUNKSJONER -----------------------------

// Sjekker om (x, y) er innenfor tegneflaten og returnerer en passende bool
//  verdi
bool Screen::within(const int x, const 	int y) {
	
	if (x > width-1 || x < 0 || y > height-1 || y < 0)
		return false;
	else
		return true;
}

// Swap intene a og b
bool Screen::swap(int &a, int &b) {
	
	int save = a;
	a = b;
	b = save;
}

// Swapper floatene a og b
bool Screen::swap(float &a, float &b) {
	
	float save = a;
	a = b;
	b = save;
}

//-------------------------------- CONSTRUCTORS --------------------------------

// Constructor for Screen. Lager en ny Screen basert p� vidde, h�yde, bakgrunn
//  og maling. Fyller tegneflaten med bakgrunnsmaling
Screen::Screen(const int width, const int height, const char backgroundChar, 
		const char ink) {
			
	// Initialiserer variabler
	this->width = width;
	this->height = height;
	this->backgroundChar = backgroundChar;
	this->ink = ink;
	
	// Fyller content med backgroundChar
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			content += backgroundChar;
		}
	}
}

// Alternativ constructor for Screen. Lager en ny Screen basert p� vidde og
//  h�yde. Bakgrunn og maling blir bestem av standardverdiene lagret i Screen.
Screen::Screen(const int width, const int height) {
	
	// Initialiserer variabler
	this->width = width;
	this->height = height;
	this->backgroundChar = DEFAULTBACKGROUND;
	this->ink = DEFAULTINK;
	
	// Fyller content med backgroundChar
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			content += DEFAULTBACKGROUND;
		}
	}
}

//----------------------------- DIVERSE FUNKSJONER -----------------------------

// Tegner en dot ved (x, y). Sjekker f�rst om (x, y) er utenfor tegneflaten,
//  finner deretter posisjonen i content (pos = x + y*width) og tegner dotten
//  p� tegneflaten. Returner true hvis dotten ble plassert, false hvis ikke
bool Screen::dot(const int x, const int y) {
	
	// Sjekker om (x, y) er innenfor tegneflaten. -> (Returnerer false)
	if (!within(x, y))
		return false;
	
	// Oversetter fra (x, y) til pos i content (pos = x + y*width)
	int pos = x + y*width;
	
	/*
	cout << "Placing dot at (" << x << ", " << y << 	// Debugtekst
			"). Pos in content is: " << pos << '\n';
	*/
	
	content[pos] = ink;		// Plasserer dotten
	
	return true;
}

// Tegner linje fra (x1, y1) til (x2, y2). Kj�rer ikke hvis et punkt er utenfor
// tegneflaten. Returnerer true hvis tegningen ble utf�rt, false hvis ikke.
bool Screen::drawLine(int x1, int y1, int x2, int y2) {
	
	// Kontroller at punktene er innenfor tegneflaten
	if (!within(x1, y1) || !within(x2, y2))
		return false;
	
	// Hvis den pr�ver � tegne linja 100% baklengs kan vi svappe start og slutt
	if (x1 > x2 && y1 > y2) {
		swap(x1, x2);
		swap(y1, y2);
	}

	float deltaX = x2 - x1;		// Endring i x retning
	float deltaY = y2 - y1;		// Endring i y retning
	
	// Hvis deltaX eller deltaY er 0 tegner den bare en enkel rett linje og
	// returnerer deretter funksjonenen
	if (deltaX == 0) {
		for (int i = y1; i < y2; ++i)
			dot(x1, i);
		return true;
	} else if (deltaY == 0) {
		for (int i = x1; i < x2; ++i)
			dot(i, y1);
		return true;
	}
	
	// FORKLARING P� MATTEN:
	//  Ser for meg en linje mellom (x1, y1) og (x2, y2). Linjen har lengde
	//	'hyp' og er 'rad' radianer fra x-linja. Tar steg nedover linja p�
	//	st�rrelse 1 og regner for hvert steg ut hvilken celle jeg lander p�.
	//	Gj�r dette ved � sette 'i' = 0.5 (fordi jeg vil tegne linja fra midten 
	//	av startcellen), og �ke med 1 for hvert steg. Formelen for cellen (x,y)
	//	jeg lander p� ved 'i' (- 0.5) steg blir da:
	//	$$x = i * cos(rad)$$
	//	$$y = i * sin(rad)$$
	//	Fortsetter � ta steg helt til 'i' blir st�rre enn 'hyp'.
	//	NB! Hvis 'deltaX' < 0 s� skal vi tegne baklengs i x retning, ganger da
	//  x endringen med -1, samme gjelder 'deltaY' < 0
	
	float rad = std::atan2(std::abs(deltaY), std::abs(deltaX));
	float hyp = std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));
	
	for (float i = 0.5; i < hyp; ++i) {
		int kx = (deltaX < 0) ? -i * std::cos(rad) : i * std::cos(rad);
		int ky = (deltaY < 0) ? -i * std::sin(rad) : i * std::sin(rad);
		dot(x1 + kx, y1 + ky);
	}		
	
	// tegner start- og sluttposisjon for sikkerhetsskyld
	dot(x1, y1);
	dot(x2, y2);
	
	return true;
}

// Tegn et rektangel med pos (x, y), vidde width og h�yde height. Funksjonen
//  kj�rer bare om rektangelet er innenfor tegneflaten
bool Screen::drawRect(int x, int y, const int width, 
		const int height) {
	
	// Kontroller at punktene er innenfor tegneflaten
	if (!within(x, y) || !within(x+width, y+height))
		return false;
	
	// Tegn rektangelet ved hjelp av drawLine
	drawLine(x,y, width,y);
	drawLine(x,y, x,height);
	drawLine(width,y, width,height);
	drawLine(x,height, width,height);
	
	return true;
}

// Nullstiller tegneflaten. Fyller den med bakgrunn
void Screen::clear() {
	
	content = "";
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			content += backgroundChar;
		}
	}
}

// Print ut tegneflaten
void Screen::print() {
	
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			std::cout << content[width*i + j];
		}
		std::cout << '\n';
	}
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
