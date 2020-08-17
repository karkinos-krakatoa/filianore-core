#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream>
#include "scalar.h"

namespace filianore
{

	template <typename T, typename MatrixType>
	class MatrixBase
	{
	public:
		size_t Size() const
		{
			return std::extent<decltype(MatrixType::data)>::value;
		}

		size_t Rows() const
		{
			return sizeof(MatrixType::data) / sizeof(MatrixType::data[0]);
		}

		size_t Cols() const
		{
			return sizeof(MatrixType::data[0]) / sizeof(int);
		}

		auto MakeUniform(T val)
		{
			for (size_t m = 0; m < Rows(); ++m)
			{
				for (size_t n = 0; n < Cols(); ++n)
				{
					MatrixT().data[m][n] = val;
				}
			}
			return MatrixT();
		}

		auto MakeIdentity(T val = 1)
		{
			for (size_t m = 0; m < Rows(); ++m)
			{
				for (size_t n = 0; n < Cols(); ++n)
				{
					if (m == n)
						MatrixT().data[m][n] = val;
					else
						MatrixT().data[m][n] = 0;
				}
			}
			return MatrixT();
		}

		T Trace()
		{
			T tr = 0;
			// Check if its a square matrix - return 0 if not
			if (Rows() != Cols())
				return 0;

			for (size_t i = 0; i < Rows(); ++i)
			{
				tr += MatrixT().data[i][i];
			}
			return tr;
		}

		using iterator = T *;
		using const_iterator = const T *;
		iterator begin()
		{
			return &MatrixT().data[0][0];
		}
		iterator end()
		{
			return &MatrixT().data[0][0] + Size();
		}
		const_iterator begin() const
		{
			return &MatrixT().data[0][0];
		}
		const_iterator end() const
		{
			return &MatrixT().data[0][0] + Size();
		}

		T &operator()(size_t m, size_t n)
		{
			return MatrixT().data[m][n];
		}
		const T &operator()(size_t m, size_t n) const
		{
			return MatrixT().data[m][n];
		}

	protected:
		MatrixType &MatrixT()
		{
			return static_cast<MatrixType &>(*this);
		}

		const MatrixType &MatrixT() const
		{
			return static_cast<const MatrixType &>(*this);
		}
	};

	// Base Matrix Class
	template <typename T, size_t M, size_t N>
	struct Matrix : MatrixBase<T, Matrix<T, M, N>>
	{
		T data[M][N];
	};

	// Matrix Operations
	template <typename T, size_t M, size_t N>
	inline auto operator+(const Matrix<T, M, N> &a, const Matrix<T, M, N> &b) -> Matrix<T, M, N> const
	{
		Matrix<T, M, N> result;
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				result.data[m][n] = a.data[m][n] + b.data[m][n];
			}
		}
		return result;
	}

	template <typename T, size_t M, size_t N>
	inline auto operator+=(Matrix<T, M, N> &a, const Matrix<T, M, N> &b)
	{
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				a.data[m][n] += b.data[m][n];
			}
		}
		return a;
	}

	template <typename T, size_t M, size_t N>
	inline auto operator-(const Matrix<T, M, N> &a, const Matrix<T, M, N> &b) -> Matrix<T, M, N> const
	{
		Matrix<T, M, N> result;
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				result.data[m][n] = a.data[m][n] - b.data[m][n];
			}
		}
		return result;
	}

	template <typename T, size_t M, size_t N>
	inline auto operator-=(Matrix<T, M, N> &a, const Matrix<T, M, N> &b)
	{
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				a.data[m][n] -= b.data[m][n];
			}
		}
		return a;
	}

	template <typename T, size_t M, size_t N>
	inline auto operator*(const Matrix<T, M, N> &a, T s) -> Matrix<T, M, N> const
	{
		Matrix<T, M, N> result;
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				result.data[m][n] = a.data[m][n] * s;
			}
		}
		return result;
	}

	template <typename T, size_t M, size_t N>
	inline auto operator*(const Matrix<T, M, N> &a, const Matrix<T, M, N> &b) -> Matrix<T, M, N> const
	{
		Matrix<T, M, N> result;
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				for (size_t i = 0; i < N; ++i)
				{
					result.data[m][n] += a.data[m][i] * b.data[i][n];
				}
			}
		}
		return result;
	}

	template <typename T, typename U, size_t M, size_t N>
	inline auto Pow(const Matrix<T, M, N> &a, U exp)
	{
		using V = decltype(a.data[0][0] * exp);
		Matrix<V, M, N> result = a;

		exp = abs(exp);
		for (size_t count = 1; count < exp; ++count)
		{
			result = result * a;
		}
		return result;
	}

	template <typename T, size_t M, size_t N>
	inline auto Transpose(const Matrix<T, M, N> &a)
	{
		Matrix<T, M, N> result;

		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				result.data[m][n] = a.data[n][m];
			}
		}
		return result;
	}

	template <typename T, size_t M, size_t N>
	inline bool operator==(const Matrix<T, M, N> &a, const Matrix<T, M, N> &b)
	{
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				if (a.data[m][n] != b.data[m][n])
					return false;
			}
		}
		return true;
	}

	template <typename T, size_t M, size_t N>
	inline bool operator!=(const Matrix<T, M, N> &a, const Matrix<T, M, N> &b)
	{
		for (size_t m = 0; m < M; ++m)
		{
			for (size_t n = 0; n < N; ++n)
			{
				if (a.data[m][n] != b.data[m][n])
					return true;
			}
		}
		return false;
	}

	// Inverse - Credit to PBRT for this one
	template <typename T, size_t M, size_t N>
	auto Inverse(const Matrix<float, M, N> &a)
	{
		int indxc[N], indxr[N];
		int ipiv[N];
		memset(ipiv, 0, sizeof(ipiv));
		float minv[N][N];
		memcpy(minv, a.data, N * N * sizeof(T));
		for (int i = 0; i < N; i++)
		{
			int irow = 0, icol = 0;
			float big = 0.f;
			for (int j = 0; j < N; j++)
			{
				if (ipiv[j] != 1)
				{
					for (int k = 0; k < N; k++)
					{
						if (ipiv[k] == 0)
						{
							if (std::abs(minv[j][k]) >= big)
							{
								big = float(std::abs(minv[j][k]));
								irow = j;
								icol = k;
							}
						}
						else if (ipiv[k] > 1)
							std::cout << "Singular matrix in MatrixInvert" << std::endl;
					}
				}
			}
			++ipiv[icol];
			if (irow != icol)
			{
				for (int k = 0; k < N; ++k)
					std::swap(minv[irow][k], minv[icol][k]);
			}
			indxr[i] = irow;
			indxc[i] = icol;
			if (minv[icol][icol] == 0)
			{
				std::cout << "Singular matrix in MatrixInvert" << std::endl;
			}
			float pivinv = 1.f / minv[icol][icol];
			minv[icol][icol] = 1.;
			for (int j = 0; j < N; j++)
				minv[icol][j] *= pivinv;
			for (int j = 0; j < N; j++)
			{
				if (j != icol)
				{
					float save = minv[j][icol];
					minv[j][icol] = 0;
					for (int k = 0; k < N; k++)
						minv[j][k] -= minv[icol][k] * save;
				}
			}
		}
		int _j = N - 1;
		for (int j = _j; j >= 0; j--)
		{
			if (indxr[j] != indxc[j])
			{
				for (int k = 0; k < N; k++)
					std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
			}
		}
		return Matrix<T, N, N>(minv);
	}

	// Matrix2
	template <typename T>
	struct Matrix<T, 2, 2> : MatrixBase<T, Matrix<T, 2, 2>>
	{
		T data[2][2];

		Matrix()
		{
			data[0][0] = data[0][1] = data[1][0] = data[1][1] = 0;
		}

		Matrix(T _m00, T _m01, T _m10, T _m11)
		{
			data[0][0] = _m00;
			data[0][1] = _m01;
			data[1][0] = _m10;
			data[1][1] = _m11;
		}

		Matrix(T mat[2][2])
		{
			memcpy(data, mat, (2 * 2) * sizeof(T));
		}

		T Determinant()
		{
			return (data[1][1] * data[0][0]) - (data[0][1] * data[1][0]);
		}
	};

	// Matrix3
	template <typename T>
	struct Matrix<T, 3, 3> : MatrixBase<T, Matrix<T, 3, 3>>
	{
		T data[3][3];

		Matrix()
		{
			data[0][0] = data[0][1] = data[0][2] = 0;
			data[1][0] = data[1][1] = data[1][2] = 0;
			data[2][0] = data[2][1] = data[2][2] = 0;
		}

		Matrix(T _m00, T _m01, T _m02, T _m10, T _m11, T _m12, T _m20, T _m21, T _m22)
		{
			data[0][0] = _m00;
			data[0][1] = _m01;
			data[0][2] = _m02;
			data[1][0] = _m10;
			data[1][1] = _m11;
			data[1][2] = _m12;
			data[2][0] = _m20;
			data[2][1] = _m21;
			data[2][2] = _m22;
		}

		Matrix(T mat[3][3])
		{
			memcpy(data, mat, (3 * 3) * sizeof(T));
		}

		T Determinant()
		{
			return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
				   data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
				   data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
		}
	};

	// Matrix4
	template <typename T>
	struct Matrix<T, 4, 4> : MatrixBase<T, Matrix<T, 4, 4>>
	{
		T data[4][4];

		Matrix()
		{
			data[0][0] = data[0][1] = data[0][2] = data[0][3] = 0;
			data[1][0] = data[1][1] = data[1][2] = data[1][3] = 0;
			data[2][0] = data[2][1] = data[2][2] = data[2][3] = 0;
			data[3][0] = data[3][1] = data[3][2] = data[3][3] = 0;
		}

		Matrix(T _m00, T _m01, T _m02, T _m03,
			   T _m10, T _m11, T _m12, T _m13,
			   T _m20, T _m21, T _m22, T _m23,
			   T _m30, T _m31, T _m32, T _m33)
		{
			data[0][0] = _m00;
			data[0][1] = _m01;
			data[0][2] = _m02;
			data[0][3] = _m03;
			data[1][0] = _m10;
			data[1][1] = _m11;
			data[1][2] = _m12;
			data[1][3] = _m13;
			data[2][0] = _m20;
			data[2][1] = _m21;
			data[2][2] = _m22;
			data[2][3] = _m23;
			data[3][0] = _m30;
			data[3][1] = _m31;
			data[3][2] = _m32;
			data[3][3] = _m33;
		}

		Matrix(T mat[4][4])
		{
			memcpy(data, mat, (4 * 4) * sizeof(T));
		}

		T Determinant()
		{
			Matrix<T, 3, 3> m1 = Matrix<T, 3, 3>(data[1][1], data[1][2], data[1][3], data[2][1], data[2][2], data[2][3], data[3][1], data[3][2], data[3][3]);
			Matrix<T, 3, 3> m2 = Matrix<T, 3, 3>(data[1][0], data[1][2], data[1][3], data[2][0], data[2][2], data[2][3], data[3][0], data[3][2], data[3][3]);
			Matrix<T, 3, 3> m3 = Matrix<T, 3, 3>(data[1][0], data[1][1], data[1][3], data[2][0], data[2][1], data[2][3], data[3][0], data[3][1], data[3][3]);
			Matrix<T, 3, 3> m4 = Matrix<T, 3, 3>(data[1][0], data[1][1], data[1][2], data[2][0], data[2][1], data[2][2], data[3][0], data[3][1], data[3][2]);

			return data[0][0] * m1.Determinant() - data[0][1] * m2.Determinant() + data[0][2] * m3.Determinant() - data[0][3] * m4.Determinant();
		}
	};

} // namespace filianore

#endif