#pragma once


#include <stdint.h>
#include <math.h>

#include <filianore/core/elemental.h>


namespace filianore
{
	// Vector Template
	template <typename T, typename VectorType> class VectorBase
	{
	public:
		size_t Size() const
		{
			return std::extent<decltype(VectorType::data)>::value;
		}


		void Zero()
		{
			std::fill(std::begin(VectorT().data), std::end(VectorT().data), T());
		}


		T LengthSquared() const
		{
			T result = 0.f;
			for (size_t i = 0; i < Size(); ++i)
			{
				result += (VectorT().data[i] * VectorT().data[i]);
			}
			return result;
		}


		T Length() const
		{
			return std::sqrt(LengthSquared());
		}

		T AbsLength() const
		{
			return std::abs(std::sqrt(LengthSquared()));
		}


		using iterator = T*;
		using const_iterator = const T*;
		iterator begin() { return &VectorT().data[0]; }
		iterator end() { return &VectorT().data[0] + Size(); }
		const_iterator begin() const { return &VectorT().data[0]; }
		const_iterator end() const { return &VectorT().data[0] + Size(); }

		T& operator [] (size_t i)
		{
			return VectorT().data[i];
		}

		const T& operator [] (size_t i) const
		{
			return VectorT().data[i];
		}

	protected:
		VectorType& VectorT()
		{
			return static_cast<VectorType&>(*this);
		}


		const VectorType& VectorT() const
		{
			return static_cast<const VectorType&>(*this);
		}

	};


	// Base Vector Class
	template <typename T, size_t N> struct Vector : VectorBase<T, Vector<T, N>>
	{
		union {
			T data[N];
			struct {
				T vals[N];
			};
		};
	};



	// Vector Operations
	template <typename T, size_t N>
	FILIANORE_INLINE Vector<T, N> Abs(const Vector<T, N>& v)
	{
		return Vector<T, N>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator+(const Vector<T, N>& a, const Vector<T, N>& b) -> Vector<T, N> const
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = a[i] + b[i];
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator+=(Vector<T, N>& a, const Vector<T, N>& b)
	{
		for (size_t i = 0; i < N; ++i)
		{
			a[i] += b[i];
		}
		return a;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator-(const Vector<T, N>& a, const Vector<T, N>& b) -> Vector<T, N> const
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = a[i] - b[i];
		}
		return result;
	}


	template <typename T, typename U, size_t N>
	FILIANORE_INLINE auto operator-=(Vector<T, N>& a, const Vector<T, N>& b)
	{
		for (size_t i = 0; i < N; ++i)
		{
			a[i] -= b[i];
		}
		return a;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator*(const Vector<T, N>& a, const Vector<T, N>& b) -> Vector<T, N> const
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = a[i] * b[i];
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator*=(Vector<T, N>& a, const Vector<T, N>& b)
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			a[i] *= b[i];
		}
		return a;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator*(const Vector<T, N>& a, T s) -> Vector<T, N> const
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = a[i] * s;
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator*=(Vector<T, N>& a, T s)
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			a[i] *= s;
		}
		return a;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator/(const Vector<T, N>& a, T s) -> Vector<T, N> const
	{
		assert(s != 0);
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			T s_inv = static_cast<T>(1) / s;
			result[i] = a[i] * s_inv;
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto operator/=(Vector<T, N>& a, T s)
	{
		assert(s != 0);
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			T s_inv = static_cast<T>(1) / s;
			a[i] *= s_inv;
		}
		return a;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto VecDiv(const Vector<T, N>& a, const Vector<T, N>& b) -> Vector<T, N> const
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			assert(b[i] != 0);
			T s_inv = static_cast<T>(1) / b[i];
			result[i] = a[i] * s_inv;
		}
		return result;
	}


	// Negation
	template <typename T, size_t N>
	FILIANORE_INLINE auto operator-(const Vector<T, N>& a)
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = -a[i];
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto Dot(const Vector<T, N>& a, const Vector<T, N>& b)
	{
		Vector<T, N> product = a * b;
		return std::accumulate(std::begin(product), std::end(product), static_cast<T>(0));
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto AbsDot(const Vector<T, N>& a, const Vector<T, N>& b)
	{
		return std::abs(Dot(a, b));
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto Normalize(const Vector<T, N>& a)
	{
		assert(a.Length() != 0);
		return a / a.Length();
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto MinComponent(const Vector<T, N>& a)
	{
		static T result = 0;
		for (size_t i = 1; i < N; ++i)
		{
			result = std::min(a[i - 1], a[i]);
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto MaxComponent(const Vector<T, N>& a)
	{
		static T result = 0;
		for (size_t i = 1; i < N; ++i)
		{
			result = std::max(a[i - 1], a[i]);
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto Min(const Vector<T, N>& a, const Vector<T, N>& b)
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = std::min(a[i], b[i]);
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto Max(const Vector<T, N>& a, const Vector<T, N>& b)
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = std::max(a[i], b[i]);
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto ProjectAUntoB(const Vector<T, N>& a, const Vector<T, N>& b)
	{
		assert(a.LengthSquared() != 0);
		return a * (Dot(a, b) / a.LengthSquared());
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto ProjectAUntoBScalar(const Vector<T, N>& a, const Vector<T, N>& b)
	{
		assert(a.Length() != 0);
		return Dot(a, b) / a.Length();
	}


	template <typename T, size_t N>
	FILIANORE_INLINE auto Reflect(const Vector<T, N>& i, const Vector<T, N>& n)
	{
		return i - n * ((Dot(i, n)) * static_cast<T>(2));
	}


	template <typename T, size_t N>
	FILIANORE_INLINE Vector<T, N> Faceforward(const Vector<T, N>& n, const Vector<T, N>& v)
	{
		float dot = Dot(n, v);
		if (dot != 0)
		{
			return dot > 0 ? n : -n;
		}
		Vector<T, N> offset = Vector<T, N>(Epsilon<T>, 0, 0);
		dot = Dot(n + offset, v);
		return dot > 0 ? n : -n;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE bool Refract(const Vector<T, N>& wi, const Vector<T, N>& n, const T& eta, Vector<T, N>* wt)
	{
		T cosI = Dot(wi, n);
		T sinI2 = std::max(static_cast<T>(0), static_cast<T>(1) - (cosI * cosI));
		T sinT2 = eta * eta * sinI2;

		if (sinT2 >= 1) /* Total Internal Reflection */
		{
			return false;
		}

		T cosT = std::sqrt(1 - sinT2);

		*wt = wi * -eta + n * (eta * cosI - cosT);

		return true;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE Vector<T, N> Permute(const Vector<T, N>& p, int dimens[N])
	{
		Vector<T, N> result;
		for (size_t i = 0; i < N; ++i)
		{
			result[i] = dimens[i];
		}
		return result;
	}


	template <typename T, size_t N>
	FILIANORE_INLINE Vector<T, N> Lerp(T t, const Vector<T, N>& v0, const Vector<T, N>& v1)
	{
		return (1 - t) * v0 + t * v1;
	}


	// Vector<T,3> - Specific Operations
	template <typename T>
	FILIANORE_INLINE void CoordinateSystem(const Vector<T, 3>& v1, Vector<T, 3>* v2, Vector<T, 3>* v3)
	{
		if (std::abs(v1.x) > std::abs(v1.y))
		{
			*v2 = -(Vector<T, 3>(-v1.z, 0, v1.x) / std::sqrt(v1.x * v1.x + v1.z * v1.z));
		}
		else
		{
			*v2 = -(Vector<T, 3>(0, v1.z, -v1.y) / std::sqrt(v1.x * v1.x + v1.z * v1.z));
		}

		*v3 = Cross(v1, *v2);
	}


	template <typename T>
	FILIANORE_INLINE auto Cross(const Vector<T, 3>& a, const Vector<T, 3>& b)
	{
		return Vector<T, 3>
			((a.y * b.z) - (a.z * b.y),
			(a.z * b.x) - (a.x * b.z),
				(a.x * b.y) - (a.y * b.x));
	}


	template <typename T>
	FILIANORE_INLINE auto Permute(const Vector<T, 3>& a, int x, int y, int z)
	{
		return Vector<T, 3>(a[x], a[y], a[z]);
	}


	template <typename T>
	FILIANORE_INLINE int MaxDimension(const Vector<T, 3>& v)
	{
		return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
	}


	template <typename T>
	FILIANORE_INLINE auto SphericalToVec3(T theta, T phi)
	{
		return Vector<T, 3>
			(sin(theta) * cos(phi),
				sin(theta) * sin(phi),
				cos(theta));
	}


	template <typename T>
	FILIANORE_INLINE auto Vec3ToSpherical(const Vector<T, 3>& a)
	{
		assert(a[0] != 0);
		assert(a[2] != 0);
		T theta = atan(a[1] / a[0]);
		T phi = atan(sqrt(a[0] * a[0] + a[1] * a[1]) / a[2]);
		return Vector<T, 2>(theta, phi);
	}


	// Vector2
	template <typename T>
	struct Vector<T, 2> : VectorBase<T, Vector<T, 2>>
	{
		union {
			T data[2];
			struct {
				T x, y;
			};
		};


		Vector()
			: x(0), y(0) { }

		Vector(T val) :
			x(val), y(val) { };

		Vector(T _x, T _y) :
			x(_x), y(_y) { };
	};


	// Vector3
	template <typename T>
	struct Vector<T, 3> : VectorBase<T, Vector<T, 3>>
	{
		union {
			T data[3];
			struct {
				T x, y, z;
			};
		};

		Vector()
			:x(0), y(0), z(0) { }

		Vector(T _x, T _y, T _z) :
			x(_x), y(_y), z(_z) { };


		bool operator==(const Vector<T, 3>& v) const {
			return x == v.x && y == v.y && z == v.z;
		}


		bool operator!=(const Vector<T, 3>& v) const {
			return x != v.x || y != v.y || z != v.z;
		}
	};


	// Vector4
	template <typename T>
	struct Vector<T, 4> : VectorBase<T, Vector<T, 4>>
	{

		union {
			T data[4];
			struct {
				T x, y, z, w;
			};
		};


		Vector()
			:x(0), y(0), z(0), w(0) { }


		Vector(T val) :
			x(val), y(val), z(val), w(val) { };


		Vector(T _x, T _y, T _z, T _w) :
			x(_x), y(_y), z(_z), w(_w) { };
	};
}
