/*
 * SvgWriter.h
 * v1.0
 * Beskrivelse: SvgWriter lar deg tegne tegninger eksportere dem til svg filer.
*/

#ifndef SVGWRITERHEADER
#define SVGWRITERHEADER

#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <string>

class SvgWriter {
	
	private:
		std::string outputString;				// det som skal dumpes i filen
		std::string path;						// path til filen
		
		// verdier for style
		std::string strokeColor,
				strokeWidth, 
				lineCap, 
				opacity,
				fill;
		
		int width, height;	// vidden og høyden på tegneflaten
		
		// Interne utility funksjoner
		std::string styles();		// returnerer en string med styleinfo
	
	public:
		// Constructor:
		SvgWriter(std::string path, int width, int height);
		
		// Gettere og settere:
		std::string getOutputString() {return outputString;}
		void setStrokeColor(int r, int g, int b);
		void setStrokeWidth(int w);
		void setLineCap(std::string cap) {lineCap = cap;}
		void setOpacity(float opacity);
		void setFill(int r, int g, int b);
		
		// Div. funksjoner
		bool line(float x1, float y1, float x2, float y2);	// tegner linje
		bool rect(float x, float y, float w, float h);		// tegner firkant
		bool circle(float x, float y, float r);				// tegner sirkel
		void write();										// dumper til fil
};

//----------------------INTERNE UTILITY FUNKSJONER------------------------------

// Returnerer en ferdigformatert string med style info. Kan puttes direkte inn
//	i style feltet i foreks en linje i svg.
std::string SvgWriter::styles() {
	
	std::ostringstream oss;
	oss << "stroke:" << strokeColor << ";stroke-width:" << 
				strokeWidth << ";stroke-linecap:" << lineCap << 
				";opacity:" << opacity << ";fill:" << fill;
	return oss.str();
}

//-----------------------------CONSTRUCTORS-------------------------------------

// Lager en ny SvgWriter basert på path til fil, vidde og høyde
SvgWriter::SvgWriter(std::string path, int width, int height): path(path), 
		outputString(""), strokeColor("rgb(0,0,0)"), strokeWidth("2"), 
		lineCap("round"), opacity("1"), fill("rgb(0,0,0)"), 
		width(width), height(height) {
			
}

//---------------------------SETTERE OG GETTERE---------------------------------

// Tar en farge på formen (int, int, int) og setter 'strokeColor' lik denne.
void SvgWriter::setStrokeColor(int r, int g, int b) {
	
	std::ostringstream oss;
	oss << "rgb(" << r << "," << g << "," << b << ")";
	strokeColor = oss.str();
}

// Tar en vidde på formen int og setter 'strokeWidth' lik denne
void SvgWriter::setStrokeWidth(int w) {
	
	std::ostringstream oss;
	oss << w;
	strokeWidth = oss.str();
}

// Tar en opacity på formen float og setter 'opacity' lik denne
void SvgWriter::setOpacity(float opacity) {
	
	std::ostringstream oss;
	oss << opacity;
	this->opacity = oss.str();
}

void SvgWriter::setFill(int r, int g, int b) {
	
	std::ostringstream oss;
	oss << "rgb(" << r << "," << g << "," << b << ")";
	fill = oss.str();
}

//-----------------------------DIV FUNKSJONER-----------------------------------

// Legger til en linje i outputString gitt (x1,y1) og (x2,y2). Henter stilinfo
//  fra 'styles()'. Linjer i svg er på formen: <line x1="0" y1="0" 
//		x2="200" y2="200" style="stroke:rgb(255,0,0);stroke-width:2"/>
//	Returnerer true hvis den klarte å tegne, false hvis ikke (ikke implementert
//	enda)
bool SvgWriter::line(float x1, float y1, float x2, float y2) {
	
	std::ostringstream oss;
	oss << "\t<line x1='" << x1 << "' y1='" << y1 << "' x2='" << x2 << 
			"' y2='" << y2 << "' style='" << styles	() << "'/>\n";
	outputString += oss.str();
	
	return true;		// OBS OBS MÅ KODES
}

// Legger til et rektangel i outputString gitt (x,y), vidde og høyde. Henter
// 	stilinfo fra 'styles()'. Rektangler i svg er på formen: <line x="1" y="2"
//		width="200" height="20" style="fill:rgb(255,0,0)"\>"
//	Returnerer true hvis den klarte å tegne, false hvis ikke (ikke implementert
//	enda)
bool SvgWriter::rect(float x, float y, float w, float h) {
	
	std::ostringstream oss;
	oss << "<rect x='" << x << "' y='" << y << "' width='" << w << 
			"' height='" << h << "' style='" << styles() << "'/>\n";
	outputString += oss.str();
	
	return true;		// OBS OBS MÅ KODES
}

// Legger til en sirkel i outputString gitt (x,y), vidde og høyde. Henter
// 	stilinfo fra 'styles()'. Sirkler i svg er på formen: <circle cx="100"
//		cy="200" r="50" style="fill:rgb(255,0,0)"\>
//	Returnerer true hvis den klarte å tegne, false hvis ikke (ikke implementert
//	enda)
bool SvgWriter::circle(float x, float y, float r) {
	
	std::ostringstream oss;
	oss << "<circle cx='" << x << "' cy='" << y << "' r='" << r << "' style='" 
			<< styles() << "'/>\n";
	outputString += oss.str();
	
	return true;		// OBS OBS MÅ KODES	
}

// Dumper 'outputString' i en svg fil ved 'path'. Dumper en fast header + vidde
//  og bredde + innmaten ('outputString')
void SvgWriter::write() {
	
	std::ofstream file;
	
	file.open(path.c_str());
	file << "<?xml version='1.0' encoding='ASCII' standalone='yes'?>\n" << 
			"<svg xmlns='http://www.w3.org/2000/svg' " << 
			"xmlns:xlink='http://www.w3.org/1999/xlink' version='1.0' " << 
			"width='" << width << "' height='" << height << "'>\n" << 
			outputString << "</svg>\n";
	file.close();
}

#endif