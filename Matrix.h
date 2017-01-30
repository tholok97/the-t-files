/*
 * Matrix.h
 * Laget av: Thomas Løkkeborg
 * Beskrivelse: Generisk matrise som støtter mange operasjoner
 * Støtter:
	+=				-=				*= (skalar)			/= (skalar)		
	==				!=				+					-				
	* (matrise)		* (skalar)		/ (skalar)			<<
	^				transponering	inversering
 * Begrensninger: Inversering og multiplikasjon med skalar fungerer bare med 
	matriser av tall
 * <eksempelbruk1>

	Matrix<int, 3, 2> m1{
		{1, 2},
		{0, 1},
		{3, 9}
	};

	Matrix<int, 2, 4> m2{
		{1,0,6,2},
		{2,1,0,1}
	};

	std::cout << "m1 + m1: " << (m1 + m1) << std::endl;
	std::cout << "m1 * m2:\n";
	print_matrix(m1 * m2);

   </eksempelbruk1>
*/

#ifndef MATRIX_HEADER
#define MATRIX_HEADER

#include <initializer_list>		// initializer list
#include <stdexcept>			// out_of_range, overflow_error
#include <iostream>				// cout, endl
#include <cstddef>				// size_t
#include <string>				// to_string
#include <array>				// array

//---------------------------DECLARATIONS---------------------------------------

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

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(const Matrix<T, n, m>& ma, T scalar);

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(T scalar, const Matrix<T, n, m>& ma);

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator/(const Matrix<T, n, m>& ma, T scalar);

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator/(T scalar, const Matrix<T, n, m>& ma);

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

//---------------------------CLASS DEFINITION-----------------------------------

template<typename T, std::size_t n, std::size_t m>
class Matrix {
	typedef std::array<std::array<T, m>, n> Array_2d;
	friend bool operator== <T, n, m>(const Matrix& lhs, const Matrix& rhs);
	friend bool operator!= <T, n, m>(const Matrix& lhs, const Matrix& rhs);
	private:
		Array_2d mat;
	public:
		Matrix(): mat{} { }
		explicit Matrix(std::initializer_list<std::initializer_list<T>> outer);
		T& at(const std::size_t r, const std::size_t c);
		T at(const std::size_t r, const std::size_t c) const;
		Matrix<T, n, m>& operator+=(const Matrix& ma);
		Matrix<T, n, m>& operator-=(const Matrix& ma);
		Matrix<T, n, m>& operator*=(T scalar);
		Matrix<T, n, m>& operator/=(T scalar);
};

//------------------------------DEFINITIONS-------------------------------------

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

template<typename T, std::size_t n, std::size_t m>
T& Matrix<T, n, m>::at(const std::size_t r, const std::size_t c) {
	return mat.at(r).at(c);
}

template<typename T, std::size_t n, std::size_t m>
T Matrix<T, n, m>::at(const std::size_t r, const std::size_t c) const {
	return mat.at(r).at(c);
}

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

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m>& Matrix<T, n, m>::operator*=(T scalar) {
	for (std::size_t r = 0; r < n; ++r)
		for (std::size_t c = 0; c < m; ++c)
			mat.at(r).at(c) = mat.at(r).at(c) * scalar;
	return *this;
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m>& Matrix<T, n, m>::operator/=(T scalar) {
	for (std::size_t r = 0; r < n; ++r)
		for (std::size_t c = 0; c < m; ++c)
			mat.at(r).at(c) = mat.at(r).at(c) / scalar;
	return *this;
}

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

template<typename T, std::size_t n, std::size_t m, std::size_t x>
Matrix<T, n, m> operator*(const Matrix<T, n, x>& lhs,
		const Matrix<T, x, m>& rhs) {
	Matrix<T, n, m> ret;
	for (std::size_t r = 0; r < n; ++r) {
		for (std::size_t c = 0; c < m; ++c) {
			T value = 0;
			for (std::size_t i = 0; i < x; ++i)
				value += lhs.at(r, i) * rhs.at(i, c);
			ret.at(r, c) = value;
		}
	}
	return ret;	
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(const Matrix<T, n, m>& ma, T scalar) {
	Matrix<T, n, m> ret = ma;
	ret *= scalar;
	return ret;
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator*(T scalar, const Matrix<T, n, m>& ma) {
	Matrix<T, n, m> ret = ma;
	ret *= scalar;
	return ret;
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator/(const Matrix<T, n, m>& ma, T scalar) {
	Matrix<T, n, m> ret = ma;
	ret /= scalar;
	return ret;
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, n, m> operator/(T scalar, const Matrix<T, n, m>& ma) {
	Matrix<T, n, m> ret = ma;
	ret /= scalar;
	return ret;
}

template<typename T, std::size_t n>
Matrix<T, n, n> operator^(const Matrix<T, n, n>& ma, std::size_t power) {
	Matrix<T, n, n> ret(ma);
	if (power > 0) {
		for (size_t i = 0; i < power-1; ++i)
			ret = ret * ma;
	} else {
		return makeIdentity<T, n>();
	}
	return ret;
}

template<typename T, std::size_t n, std::size_t m>
Matrix<T, m, n> transposed(const Matrix<T, n, m>& ma) {
	Matrix<T, m, n> ret;
	for (std::size_t r = 0; r < m; ++r)
		for (std::size_t c = 0; c < n; ++c)
			ret.at(r, c) = ma.at(c, r);
	return ret;
}

template<typename T, std::size_t n>
Matrix<T, n, n> inverse(const Matrix<T, n, n>& ma) {
	Matrix<T, n, n*2> tmp;
	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
			tmp.at(i, j) = ma.at(i, j);
	for (size_t i = 0; i < n; ++i)
		tmp.at(i, i+n) = 1;
	for (size_t i = 0; i < n; ++i) {
		T t = tmp.at(i, i);
		if (t == 0) {
			throw std::overflow_error("MATRIX INVERSE ERROR: dividing by "
					"zero.. no solution?");
		}
		for (size_t j = 0; j < n*2; ++j)
			tmp.at(i, j) = tmp.at(i ,j) / t;
		for (size_t j = 0; j < n; ++j) {
			if (i != j) {
				t = tmp.at(j, i);
				for (size_t k = 0; k < n*2; ++k)
					tmp.at(j, k) = tmp.at(j, k) - t * tmp.at(i, k);
			}
		}
	}
	Matrix<T, n, n> ret;
	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < n; ++j)
			ret.at(i, j) = tmp.at(i, j+n);
	return ret;
}

template<typename T, std::size_t n>
Matrix<T, n, n> makeIdentity() {
	Matrix<T, n, n> ret;
	for (size_t i = 0; i < n; ++i)
		ret.at(i,i) = 1;
	return ret;
}

template<typename T, std::size_t n, std::size_t m>
std::ostream& print_matrix(const Matrix<T, n, m>& ma, std::ostream& os)  {	
	for (std::size_t r = 0; r < n; ++r) {
		for (std::size_t c = 0; c < m; ++c)
			os << ma.at(r, c) << '\t';
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
