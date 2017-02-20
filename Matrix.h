/*
 * Matrix.h
 * Laget av: Thomas Løkkeborg
 * Beskrivelse: Generisk matrise som støtter mange operasjoner. Matrisen er 
	implementert oppå std::array, og viderefører mye til den. Som en konsekvens 
	oppfører matrisen seg på mange måter som en STL container
 * Støtter:
	+=				-=				*= (skalar)			/= (skalar)		
	==				!=				+					-				
	* (matrise)		* (skalar)		/ (skalar)			<<
	^				transponering	inversering			hente ut blokk
 * Begrensninger: 
	- Inversering fungerer bare med matriser av tall.
	- Blokker (Matrix::block) kan ikke referanseoverføres
	- Dimensjoner må være spesifisert ved compile-time. (templetert)
 * Eksempel:

	Matrix<double, 2, 4> m1{
		{1,		2,		3,		4},
		{5,		6,		7,		8}
	};

	Matrix<double, 4, 4> m2{
		{2,		2,		3,		2},
		{3,		4,		4,		2},
		{2,		3,		2,		3},
		{3,		4,		2,		2}
	};

	cout << "m1: " << m1 << "\n" << "m2: " << m2 << "\n\nm1 * m2:\n";
	print_matrix((m1 * m2)) << "\n\n";

	cout << "invers av m2:\n";
	print_matrix(inverse(m2)) << "\n\n";

	cout << "transponert av m2:\n";
	print_matrix(transposed(m2)) << "\n\n";
*/

#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include <initializer_list>		// initializer list
#include <stdexcept>			// out_of_range, overflow_error
#include <iostream>				// cout, endl
#include <cstddef>				// size_t
#include <iomanip>				// setw
#include <string>				// to_string
#include <array>				// array

//------------------------------- DEKLARASJONER --------------------------------

template<typename T, std::size_t n, std::size_t m>
class Matrix;

template<typename T, std::size_t n, std::size_t m>
std::ostream& operator<<(std::ostream& os, const Matrix<T, n, m>& ma);

template<typename T, std::size_t n, std::size_t m>
bool operator==(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs);

template<typename T, std::size_t n, std::size_t m>
bool operator!=(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs);

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator+(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs);

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator-(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs);

template<typename T, std::size_t n, std::size_t m, std::size_t x>
Matrix<T, n, m> operator*(const Matrix<T, n, x>& lhs,
		const Matrix<T, x, m>& rhs);

template<typename T, typename U, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(const Matrix<T, n, m>& ma, U scalar);

template<typename T, typename U, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(U scalar, const Matrix<T, n, m>& ma);

template<typename T, typename U, std::size_t n, std::size_t m>
Matrix<T, n, m> operator/(const Matrix<T, n, m>& ma, U scalar);

template<typename T, std::size_t n>
Matrix<T, n, n> operator^(const Matrix<T, n, n>& ma, std::size_t power);

template<typename T, std::size_t n, std::size_t m>
Matrix<T, m, n> transposed(const Matrix<T, n, m>& ma);

template<typename T, std::size_t n>
Matrix<T, n, n> inverse(const Matrix<T, n, n>& ma);

template<typename T, std::size_t n>
Matrix<T, n, n> makeIdentity();

template<typename T, std::size_t n, std::size_t m>
std::ostream& print_matrix(const Matrix<T, n, m>& ma, 
		std::ostream& os = std::cout);

//----------------------------- KLASSE-DEFINISJON ------------------------------

template<typename T, std::size_t n, std::size_t m>	// matrise av T, n x m stor
class Matrix {
	
	// disse operatorene er venner for å sende arbeid videre til std::array
	friend bool operator== <T, n, m>(const Matrix& lhs, const Matrix& rhs);
	friend bool operator!= <T, n, m>(const Matrix& lhs, const Matrix& rhs);

	private:
		
		// typedef for nestet std::array
		typedef std::array<std::array<T, m>, n> Array_2d;

		// mat er hvor matrisen lagres (se Array_2d)
		Array_2d mat;

		// statiske variabler for å lagre dimensjoner
		static constexpr std::size_t rows = n;
		static constexpr std::size_t cols = m;

	public:
		
		// lager matrise fyllt med 0
		Matrix(): mat{} { }

		// lager matrise med init-list (eks: { {1,2}, {3,4} })
		explicit Matrix(std::initializer_list<std::initializer_list<T>> outer);

		// returnerer element ved pos (r, c). Referanse / kopi versjoner
		T& at(const std::size_t r, const std::size_t c);
		T at(const std::size_t r, const std::size_t c) const;

		// kopier blokk fra (r,c) med størrelse x*y og returner som ny matrise
		template<std::size_t x, std::size_t y>
		Matrix<T, x, y> block(std::size_t r, std::size_t c);

		// getter for dimensjoner (non-type template parametre)
		static constexpr std::size_t get_rows() { return rows; }
		static constexpr std::size_t get_cols() { return cols; }

		// overloadede aritmetiske assignement-operatorer
		Matrix<T, n, m>& operator+=(const Matrix& ma);
		Matrix<T, n, m>& operator-=(const Matrix& ma);
		template<typename U>					// skalarer har egen template-
		Matrix<T, n, m>& operator*=(U scalar);	// parameter for å tillate
		template<typename U>					// multiplikasjon med alle typer
		Matrix<T, n, m>& operator/=(U scalar);	// som T kan multipliseres med
														
		// typedef for template-parameter og size_t
		typedef T Value_type;
		typedef std::size_t Size_type;
};

//-------------------------------- DEFINISJONER --------------------------------

// Constructor som fyller matrisa ut i fra en init-list. Oppfører seg som STL 
//	constructorer av samme form
template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m>::Matrix(
		std::initializer_list<std::initializer_list<T>> outer): mat{} {
	if (outer.size() > mat.size())
		throw std::out_of_range("MATRIX INIT ERROR: too many "
				"initializers for outer");
	size_t i = 0, j = 0;
	for (std::initializer_list<T> inner : outer) {
		if (inner.size() > mat[i].size())
			throw std::out_of_range("MATRIX INIT ERROR: too many "
					"initializers for inner at i = " + std::to_string(i));
		for (T value : inner) {
			mat[i][j] = value;
			++j;
		}
		j = 0;
		++i;
	}
}

// Gettere for å få element ved pos. (r, c). Arbeidet sendes videre til 
//	std::array::at. Oppfører seg som STL at() funksjoner av samme form

template<typename T, std::size_t n, std::size_t m>
T& Matrix<T, n, m>::at(const std::size_t r, const std::size_t c) {
	return mat.at(r).at(c);
}

template<typename T, std::size_t n, std::size_t m>
T Matrix<T, n, m>::at(const std::size_t r, const std::size_t c) const {
	return mat.at(r).at(c);
}

// Henter ut en blokk av matrisen og returnerer den som en ny matrise
template<typename T, std::size_t n, std::size_t m>
template<std::size_t x, std::size_t y>
Matrix<T, x, y> Matrix<T, n, m>::block(std::size_t r, std::size_t c) {
	Matrix<T, x, y> ret;
	for (size_t i = 0; i < x; ++i)
		for (size_t j = 0; j < y; ++j)
			ret.at(i, j) = at(i+r, j+c);
	return ret;
}

// Aritmetiske assignement-operatorer:

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m>& Matrix<T, n, m>::operator+=(const Matrix& ma) {
	for (std::size_t r = 0; r < n; ++r)
		for (std::size_t c = 0; c < m; ++c)
			mat.at(r).at(c) += ma.at(r, c);
	return *this;
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m>& Matrix<T, n, m>::operator-=(const Matrix& ma) {
	for (std::size_t r = 0; r < n; ++r)
		for (std::size_t c = 0; c < m; ++c)
			mat.at(r).at(c) -= ma.at(r, c);
	return *this;
}

// skalar har egen template-parameter for å la matrisen bli multiplisert med 
//	alle skalarer som støtter multiplikasjon med T
template<typename T, std::size_t n, std::size_t m>
template<typename U>
Matrix<T, n, m>& Matrix<T, n, m>::operator*=(U scalar) {
	for (std::size_t r = 0; r < n; ++r)
		for (std::size_t c = 0; c < m; ++c)
			mat.at(r).at(c) = mat.at(r).at(c) * scalar;
	return *this;
}

template<typename T, std::size_t n, std::size_t m>
template<typename U>		// se operator*= for begrunnelse
Matrix<T, n, m>& Matrix<T, n, m>::operator/=(U scalar) {
	for (std::size_t r = 0; r < n; ++r)
		for (std::size_t c = 0; c < m; ++c)
			mat.at(r).at(c) = mat.at(r).at(c) / scalar;
	return *this;
}

// Overloadet output operator som viser matrisen på denne formen: {{1,2},{3,4}}
template<typename T, std::size_t n, std::size_t m>
std::ostream& operator<<(std::ostream& os, const Matrix<T, n, m>& ma) {
	os << '{';
	for (std::size_t r = 0; r < n; ++r) {
		os << '{';
		for (std::size_t c = 0; c < m; ++c) {
			os << ma.at(r, c);
			if (c+1 < m)
				os << ", ";
		}
		os << '}';
		if (r+1 < n)
			os << ", ";
	}
	os << '}';
	return os;
}

// Sammenlignings-operatorer:

template<typename T, std::size_t n, std::size_t m>
bool operator==(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs) {
	return lhs.mat == rhs.mat;
}

template<typename T, std::size_t n, std::size_t m>
bool operator!=(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs) {
	return !(lhs.mat == rhs.mat);
}

// non-member aritmetiske operatorer: (merk at disse sender arbeidet videre til 
//	de tilsvarende member-versjonene

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator+(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs) {
	Matrix<T, n, m> ret = lhs;
	ret += rhs;
	return ret;
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator-(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs) {
	Matrix<T, n, m> ret = lhs;
	ret -= rhs;
	return ret;
}

// matrise * matrise multiplikasjon
template<typename T, std::size_t n, std::size_t m, std::size_t x>
Matrix<T, n, m> operator*(const Matrix<T, n, x>& lhs,
		const Matrix<T, x, m>& rhs) {
	Matrix<T, n, m> ret;
	for (std::size_t r = 0; r < n; ++r) {
		for (std::size_t c = 0; c < m; ++c) {
			T value = 0;								// ganger rader med 
			for (std::size_t i = 0; i < x; ++i)			// kolonner
				value += lhs.at(r, i) * rhs.at(i, c);
			ret.at(r, c) = value;
		}
	}
	return ret;	
}

template<typename T, typename U, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(const Matrix<T, n, m>& ma, U scalar) {
	Matrix<T, n, m> ret = ma;
	ret *= scalar;
	return ret;
}

template<typename T, typename U, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(U scalar, const Matrix<T, n, m>& ma) {
	Matrix<T, n, m> ret = ma;
	ret *= scalar;
	return ret;
}

template<typename T, typename U, std::size_t n, std::size_t m>
Matrix<T, n, m> operator/(const Matrix<T, n, m>& ma, U scalar) {
	Matrix<T, n, m> ret = ma;
	ret /= scalar;
	return ret;
}

template<typename T, std::size_t n>
Matrix<T, n, n> operator^(const Matrix<T, n, n>& ma, std::size_t power) {
	Matrix<T, n, n> ret(ma);
	if (power > 0) {
		for (std::size_t i = 0; i < power-1; ++i)
			ret = ret * ma;
	} else {
		return makeIdentity<T, n>();	// M^0 = I
	}
	return ret;
}

// Returnerer en transponert utgave av matrisa
template<typename T, std::size_t n, std::size_t m>
Matrix<T, m, n> transposed(const Matrix<T, n, m>& ma) {
	Matrix<T, m, n> ret;
	for (std::size_t r = 0; r < m; ++r)			// rader blir kolonner
		for (std::size_t c = 0; c < n; ++c)
			ret.at(r, c) = ma.at(c, r);
	return ret;
}

// (Prøver å) regne ut den inverse av matrisa ved Gauss-Jordan. Bare definert 
//	for kvadratiske matriser
template<typename T, std::size_t n>
Matrix<T, n, n> inverse(const Matrix<T, n, n>& ma) {
	Matrix<T, n, n*2> tmp;
	for (std::size_t i = 0; i < n; ++i)
		for (std::size_t j = 0; j < n; ++j)
			tmp.at(i, j) = ma.at(i, j);
	for (std::size_t i = 0; i < n; ++i)
		tmp.at(i, i+n) = 1;
	for (std::size_t i = 0; i < n; ++i) {
		T t = tmp.at(i, i);
		if (t == 0) {			// ingen løsning
			throw std::overflow_error("MATRIX INVERSE ERROR: dividing by "
					"zero.. no solution?");
		}
		for (std::size_t j = 0; j < n*2; ++j)
			tmp.at(i, j) = tmp.at(i ,j) / t;
		for (std::size_t j = 0; j < n; ++j) {
			if (i != j) {
				t = tmp.at(j, i);
				for (std::size_t k = 0; k < n*2; ++k)
					tmp.at(j, k) = tmp.at(j, k) - t * tmp.at(i, k);
			}
		}
	}
	Matrix<T, n, n> ret;
	for (std::size_t i = 0; i < n; ++i)
		for (std::size_t j = 0; j < n; ++j)
			ret.at(i, j) = tmp.at(i, j+n);
	return ret;
}

// Returnerer en identitetsmatrise av spesifiserte dimensjoner
template<typename T, std::size_t n>
Matrix<T, n, n> makeIdentity() {
	Matrix<T, n, n> ret;
	for (std::size_t i = 0; i < n; ++i)
		ret.at(i,i) = 1;
	return ret;
}

// Utility-funksjon som skriver matrisa på en menneskelig-leslig måte
template<typename T, std::size_t n, std::size_t m>
std::ostream& print_matrix(const Matrix<T, n, m>& ma, std::ostream& os)  {	
	for (std::size_t r = 0; r < n; ++r) {
		for (std::size_t c = 0; c < m; ++c)
			os << std::setw(12) << ma.at(r, c) << '\t';
		os << std::endl;
	}
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
