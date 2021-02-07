#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include <assert.h>
#include "vec3_math.h"
#include "matrix.h"

namespace filianore
{

	class Transform
	{
	public:
		Matrix<float, 4, 4> mat, matInv;

		Transform() {}

		Transform(const float _mat[4][4])
		{
			mat = Matrix<float, 4, 4>(_mat[0][0], _mat[0][1], _mat[0][2], _mat[0][3],
									  _mat[1][0], _mat[1][1], _mat[1][2], _mat[1][3],
									  _mat[2][0], _mat[2][1], _mat[2][2], _mat[2][3],
									  _mat[3][0], _mat[3][1], _mat[3][2], _mat[3][3]);
			matInv = Inverse<float>(mat);
		}

		Transform(const Matrix<float, 4, 4> &_mat)
			: mat(_mat), matInv(Inverse<float>(_mat))
		{
		}

		Transform(const Matrix<float, 4, 4> &_mat, const Matrix<float, 4, 4> &_matInv)
			: mat(_mat), matInv(_matInv)
		{
		}

		Transform operator*(Transform t2)
		{
			Matrix<float, 4, 4> m1 = mat * t2.mat;
			Matrix<float, 4, 4> m2 = t2.matInv * matInv;
			return Transform(m1, m2);
		}

		bool operator==(const Transform &t) const
		{
			return t.mat == mat && t.matInv == matInv;
		}

		bool operator!=(const Transform &t) const
		{
			return t.mat != mat || t.matInv != matInv;
		}

		StaticArray<float, 3> PointTransform(const StaticArray<float, 3> &pt) const
		{
			float x = pt.x(), y = pt.y(), z = pt.z();

			float xp = mat.data[0][0] * x + mat.data[0][1] * y + mat.data[0][2] * z + mat.data[0][3];
			float yp = mat.data[1][0] * x + mat.data[1][1] * y + mat.data[1][2] * z + mat.data[1][3];
			float zp = mat.data[2][0] * x + mat.data[2][1] * y + mat.data[2][2] * z + mat.data[2][3];
			float wp = mat.data[3][0] * x + mat.data[3][1] * y + mat.data[3][2] * z + mat.data[3][3];

			if (wp == 1.0)
			{
				return StaticArray<float, 3>(xp, yp, zp);
			}
			else
			{
				assert(wp != 0);
				return StaticArray<float, 3>(xp, yp, zp) / wp;
			}
		}

		StaticArray<float, 3> VectorTransform(const StaticArray<float, 3> &v) const
		{
			float x = v.x(), y = v.y(), z = v.z();
			return StaticArray<float, 3>(mat.data[0][0] * x + mat.data[0][1] * y + mat.data[0][2] * z,
										 mat.data[1][0] * x + mat.data[1][1] * y + mat.data[1][2] * z,
										 mat.data[2][0] * x + mat.data[2][1] * y + mat.data[2][2] * z);
		}

		friend Transform Inverse(const Transform &t)
		{
			return Transform(t.matInv, t.mat);
		}

		bool SwapsHandedness() const
		{
			float det = mat.data[0][0] * (mat.data[1][1] * mat.data[2][2] - mat.data[1][2] * mat.data[2][1]) -
						mat.data[0][1] * (mat.data[1][0] * mat.data[2][2] - mat.data[1][2] * mat.data[2][0]) +
						mat.data[0][2] * (mat.data[1][0] * mat.data[2][1] - mat.data[1][1] * mat.data[2][0]);
			return det < 0;
		}
	};

	static Transform Translate(const StaticArray<float, 3> &delta)
	{
		Matrix<float, 4, 4> m(1.f, 0.f, 0.f, delta.x(),
							  0.f, 1.f, 0.f, delta.y(),
							  0.f, 0.f, 1.f, delta.z(),
							  0.f, 0.f, 0.f, 1.f);

		Matrix<float, 4, 4> mInv(1.f, 0.f, 0.f, -delta.x(),
								 0.f, 1.f, 0.f, -delta.y(),
								 0.f, 0.f, 1.f, -delta.z(),
								 0.f, 0.f, 0.f, 1.f);

		return Transform(m, mInv);
	}

	static Transform Scale(float x, float y, float z)
	{
		assert(x != 0 && y != 0 && z != 0);

		Matrix<float, 4, 4> m(x, 0.f, 0.f, 0.f,
							  0.f, y, 0.f, 0.f,
							  0.f, 0, z, 0.f,
							  0.f, 0.f, 0.f, 1.f);

		Matrix<float, 4, 4> mInv(1.f / x, 0.f, 0.f, 0.f,
								 0.f, 1.f / y, 0, 0,
								 0.f, 0, 1.f / z, 0,
								 0.f, 0.f, 0.f, 1.f);

		return Transform(m, mInv);
	}

	static Transform RotateX(float angle, bool inRadians = false)
	{
		float sin_t = std::sin(angle);
		float cos_t = std::cos(angle);

		if (!inRadians)
		{
			sin_t = std::sin(Radians<float>(angle));
			cos_t = std::cos(Radians<float>(angle));
		}

		Matrix<float, 4, 4> m(1.f, 0.f, 0.f, 0.f,
							  0.f, cos_t, -sin_t, 0.f,
							  0.f, sin_t, cos_t, 0.f,
							  0.f, 0.f, 0.f, 1.f);

		return Transform(m, Transpose(m));
	}

	static Transform RotateY(float angle, bool inRadians = false)
	{
		float sin_t = std::sin(angle);
		float cos_t = std::cos(angle);

		if (!inRadians)
		{
			sin_t = std::sin(Radians<float>(angle));
			cos_t = std::cos(Radians<float>(angle));
		}

		Matrix<float, 4, 4> m(cos_t, 0.f, sin_t, 0.f,
							  0.f, 1.f, 0.f, 0.f,
							  -sin_t, 0.f, cos_t, 0.f,
							  0.f, 0.f, 0.f, 1.f);

		return Transform(m, Transpose(m));
	}

	static Transform RotateZ(float angle, bool inRadians = false)
	{
		float sin_t = std::sin(angle);
		float cos_t = std::cos(angle);

		if (!inRadians)
		{
			sin_t = std::sin(Radians<float>(angle));
			cos_t = std::cos(Radians<float>(angle));
		}

		Matrix<float, 4, 4> m(cos_t, -sin_t, 0.f, 0.f,
							  sin_t, cos_t, 0.f, 0.f,
							  0.f, 0.f, 1.f, 0.f,
							  0.f, 0.f, 0.f, 1.f);

		return Transform(m, Transpose(m));
	}

} // namespace filianore

#endif