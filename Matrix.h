/*
 * Matrix.h
 * Beskrivelse: Generisk matrise som støtter addisjon, substraksjon og 
	multiplikasjon.
 * <eksempelbruk1>

	Matrix<int, 3, 2> m1({{
		{1, 2},
		{0, 1},
		{3, 9}
	}});

	Matrix<int, 2, 4> m2({{
		{1,0,6,2},
		{2,1,0,1}
	}});

	std::cout << "m1 + m1: " << (m1 + m1) << std::endl;
	std::cout << "m1 * m2:\n";
	print_matrix(m1 * m2);

   </eksempelbruk1>
*/

#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include <iostream>		// cout, endl
#include <array>		// array

//---------------------------DECLARATIONS---------------------------------------

template<typename T, size_t n, size_t m>
class Matrix;

template<typename T, size_t n, size_t m>
std::ostream& operator<<(std::ostream& os, const Matrix<T, n, m>& ma);

template<typename T, size_t n, size_t m>
Matrix<T, n, m> operator+(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs);

template<typename T, size_t n, size_t m>
Matrix<T, n, m> operator-(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs);

template<typename T, size_t n, size_t m, size_t x>
Matrix<T, n, m> operator*(const Matrix<T, n, x>& lhs,
		const Matrix<T, x, m>& rhs);

template<typename t, size_t n, size_t m>
std::ostream& print_matrix(const Matrix<t, n, m>& ma, 
		std::ostream& os = std::cout);

//---------------------------CLASS DEFINITION-----------------------------------

template<typename T, size_t n, size_t m>
class Matrix {
	typedef std::array<std::array<T, m>, n> Array_2d;
	private:
		Array_2d mat;
	public:
		Matrix(): mat({}) { }
		Matrix(Array_2d a_2d): mat(a_2d) { }
		T& at(const size_t r, const size_t c);
		T at(const size_t r, const size_t c) const;
};

//------------------------------DEFINITIONS-------------------------------------

template<typename T, size_t n, size_t m>
T& Matrix<T, n, m>::at(const size_t r, const size_t c) {
	return mat.at(r).at(c);
}

template<typename T, size_t n, size_t m>
T Matrix<T, n, m>::at(const size_t r, const size_t c) const {
	return mat.at(r).at(c);
}

template<typename T, size_t n, size_t m>
std::ostream& operator<<(std::ostream& os, const Matrix<T, n, m>& ma) {
	std::cout << '{';
	for (size_t r = 0; r < n; ++r) {
		std::cout << '{';
		for (size_t c = 0; c < m; ++c) {
			std::cout << ma.at(r, c);
			if (c+1 < m)
				std::cout << ", ";
		}
		std::cout << '}';
		if (r+1 < n)
			std::cout << ", ";
	}
	std::cout << '}';
	return os;
}

template<typename T, size_t n, size_t m>
Matrix<T, n, m> operator+(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs) {
	Matrix<T, n, m> ret;
	for (size_t r = 0; r < n; ++r)
		for (size_t c = 0; c < m; ++c)
			ret.at(r, c) = lhs.at(r, c) + rhs.at(r, c);

	return ret;
}

template<typename T, size_t n, size_t m>
Matrix<T, n, m> operator-(const Matrix<T, n, m>& lhs, 
		const Matrix<T, n, m>& rhs) {
	Matrix<T, n, m> ret;
	for (size_t r = 0; r < n; ++r)
		for (size_t c = 0; c < m; ++c)
			ret.at(r, c) = lhs.at(r, c) - rhs.at(r, c);

	return ret;
}

template<typename T, size_t n, size_t m, size_t x>
Matrix<T, n, m> operator*(const Matrix<T, n, x>& lhs,
		const Matrix<T, x, m>& rhs) {
	Matrix<T, n, m> ret;
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < m; ++c) {
			T value = 0;
			for (size_t i = 0; i < x; ++i) {
				value += lhs.at(r, i) * rhs.at(i, c);
			}
			ret.at(r, c) = value;
		}
	}
	return ret;	
}

template<typename t, size_t n, size_t m>
std::ostream& print_matrix(const Matrix<t, n, m>& ma, std::ostream& os)  {	
	for (size_t r = 0; r < n; ++r) {
		for (size_t c = 0; c < m; ++c)
			std::cout << ma.at(r, c) << '\t';
		std::cout << std::endl;
	}
	return os;
}

#endif
