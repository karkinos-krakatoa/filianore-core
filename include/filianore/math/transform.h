#pragma once


#include "vec3_math.h"
#include "matrix.h"


namespace filianore
{

    template <typename T> class Transform
	{
	public:
		Matrix<T, 4, 4> mat, matInv;

		Transform() { }

		Transform(const float _mat[4][4])
		{
			mat = Matrix<T, 4, 4>(_mat[0][0], _mat[0][1], _mat[0][2], _mat[0][3],
				_mat[1][0], _mat[1][1], _mat[1][2], _mat[1][3],
				_mat[2][0], _mat[2][1], _mat[2][2], _mat[2][3],
				_mat[3][0], _mat[3][1], _mat[3][2], _mat[3][3]);
			matInv = Inverse(mat);
		}

		Transform(const Matrix<T, 4, 4>& _mat)
			: mat(_mat), matInv(Inverse(_mat))
		{ }

		Transform(const Matrix<T, 4, 4>& _mat, const Matrix<T, 4, 4>& _matInv)
			: mat(_mat), matInv(_matInv)
		{ }

		Transform operator*(Transform t2)
		{
			Matrix<T, 4, 4> m1 = mat * t2.mat;
			Matrix<T, 4, 4> m2 = t2.matInv * matInv;
			return Transform(m1, m2);
		}

		bool operator==(const Transform& t) const
		{
			return t.mat == mat && t.matInv == matInv;
		}

		bool operator!=(const Transform& t) const
		{
			return t.mat != mat || t.matInv != matInv;
		}

		StaticArray<T, 3> PointTransform(const StaticArray<T, 3>& pt) const
		{
			T x = pt.x, y = pt.y, z = pt.z;

			T xp = mat.data[0][0] * x + mat.data[0][1] * y + mat.data[0][2] * z + mat.data[0][3];
			T yp = mat.data[1][0] * x + mat.data[1][1] * y + mat.data[1][2] * z + mat.data[1][3];
			T zp = mat.data[2][0] * x + mat.data[2][1] * y + mat.data[2][2] * z + mat.data[2][3];
			T wp = mat.data[3][0] * x + mat.data[3][1] * y + mat.data[3][2] * z + mat.data[3][3];

			if (wp == 1.0)
			{
				return StaticArray<T, 3>(xp, yp, zp);
			}
			else
			{
				assert(wp != 0);
				return StaticArray<T, 3>(xp, yp, zp) / wp;
			}
		}

		StaticArray<T, 3> VectorTransform(const StaticArray<T, 3>& v) const
		{
			T x = v.x, y = v.y, z = v.z;
			return StaticArray<T, 3>(mat.data[0][0] * x + mat.data[0][1] * y + mat.data[0][2] * z,
					    mat.data[1][0] * x + mat.data[1][1] * y + mat.data[1][2] * z,
					    mat.data[2][0] * x + mat.data[2][1] * y + mat.data[2][2] * z);
		}

		friend Transform Inverse(const Transform& t)
		{
			return Transform(t.matInv, t.mat);
		}

		bool SwapsHandedness() const {
			T det = mat.data[0][0] * (mat.data[1][1] * mat.data[2][2] - mat.data[1][2] * mat.data[2][1]) -
				    mat.data[0][1] * (mat.data[1][0] * mat.data[2][2] - mat.data[1][2] * mat.data[2][0]) +
				    mat.data[0][2] * (mat.data[1][0] * mat.data[2][1] - mat.data[1][1] * mat.data[2][0]);
			return det < 0;
		}

	};


	template <typename T> Transform<T> Translate(const StaticArray<T, 3>& delta)
	{
		Matrix<T, 4, 4> m(1, 0, 0, delta.x,
			0, 1, 0, delta.y,
			0, 0, 1, delta.z,
			0, 0, 0, 1);

		Matrix<T, 4, 4> mInv(1, 0, 0, -delta.x,
			0, 1, 0, -delta.y,
			0, 0, 1, -delta.z,
			0, 0, 0, 1);

		return Transform<T>(m, mInv);
	}


	template <typename T> Transform<T> Scale(T x, T y, T z)
	{
		assert(x != 0 && y != 0 && z != 0);

		Matrix<T, 4, 4> m(x, 0, 0, 0,
						  0, y, 0, 0,
						  0, 0, z, 0,
						  0, 0, 0, 1);

		Matrix<T, 4, 4> mInv(1 / x, 0, 0, 0,
							 0, 1 / y, 0, 0,
							 0, 0, 1 / z, 0,
						     0, 0, 0, 1);

		return Transform<T>(m, mInv);
	}


	template <typename T> Transform<T> RotateX(T angle)
	{
		T sin_t = std::sin(Radians<T>(angle));
		T cos_t = std::cos(Radians<T>(angle));

		Matrix<T, 4, 4> m(1, 0, 0, 0,
			0, cos_t, -sin_t, 0,
			0, -sin_t, cos_t, 0,
			0, 0, 0, 1);

		return Transform<T>(m, Transpose(m));
	}


	template <typename T> Transform<T> RotateY(T angle)
	{
		T sin_t = std::sin(Radians<T>(angle));
		T cos_t = std::cos(Radians<T>(angle));

		Matrix<T, 4, 4> m(cos_t, 0, sin_t, 0,
			0, 1, 0, 0,
			-sin_t, 0, cos_t, 0,
			0, 0, 0, 1);

		return Transform<T>(m, Transpose(m));
	}


	template <typename T> Transform<T> RotateZ(T angle)
	{
		T sin_t = std::sin(Radians<T>(angle));
		T cos_t = std::cos(Radians<T>(angle));

		Matrix<T, 4, 4> m(cos_t, -sin_t, 0, 0,
			sin_t, cos_t, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		return Transform<T>(m, Transpose(m));
	}

}