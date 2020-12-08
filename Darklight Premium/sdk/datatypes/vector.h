#pragma once
// used: isfinite, fmodf, sqrtf
#include <cmath>

// @credits: https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/mathlib/vector.h

class Vector2D
{
public:
	Vector2D() = default;
	Vector2D(float x, float y)
	{
		this->x = x; this->y = y;
	}

	Vector2D operator+(const Vector2D& vecAdd) const
	{
		return Vector2D(
			this->x + vecAdd.x, 
			this->y + vecAdd.y);
	}
	Vector2D operator-(const Vector2D& vecSub) const
	{
		return Vector2D(
			this->x - vecSub.x,
			this->y - vecSub.y);
	}
	Vector2D operator/(float flDivide) const
	{
		return Vector2D(
			this->x / flDivide, 
			this->y / flDivide);
	}
	Vector2D operator-(float flSub) const
	{
		return Vector2D(
			this->x - flSub,
			this->y - flSub);
	}

	// equality
	bool operator==(const Vector2D& v) const;
	bool operator!=(const Vector2D& v) const;

	// arithmetic operations
	Vector2D& operator+=(const Vector2D& v)
	{
		x += v.x; y += v.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& v)
	{
		x -= v.x; y -= v.y;
		return *this;
	}

	Vector2D& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		return *this;
	}

	Vector2D& operator*=(const Vector2D& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}

	Vector2D& operator/=(const Vector2D& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}

	// this ought to be an opcode.
	Vector2D& operator+=(float fl)
	{
		x += fl;
		y += fl;
		return *this;
	}

	// this ought to be an opcode.
	Vector2D& operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		return *this;
	}
	Vector2D& operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		return *this;
	}
public:
	float x, y;
};

class Vector
{
public:
	Vector()
	{
		Init();
	}

	Vector(float x, float y, float z)
	{
		Init(x, y, z);
	}

	Vector(const float* arrVector)
	{
		Init(arrVector[0], arrVector[1], arrVector[2]);
	}

	Vector(const Vector& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z;
	}

	Vector(const Vector2D& vecBase2D)
	{
		this->x = vecBase2D.x; this->y = vecBase2D.y; this->z = 0.0f;
	}

	constexpr void Init(float x = 0.f, float y = 0.f, float z = 0.f)
	{
		this->x = x; this->y = y; this->z = z;
	}

	bool IsValid() const
	{
		return std::isfinite(this->x) && std::isfinite(this->y) && std::isfinite(this->z);
	}

	constexpr void Invalidate()
	{
		this->x = this->y = this->z = std::numeric_limits<float>::infinity();
	}

	float operator[](std::size_t nIndex) const
	{
		return ((float*)this)[nIndex];
	}

	float& operator[](std::size_t nIndex)
	{
		return ((float*)this)[nIndex];
	}

	bool operator==(const Vector& vecBase) const
	{
		if (this->x == vecBase.x && this->y == vecBase.y && this->z == vecBase.z)
			return true;

		return false;
	}

	bool operator!=(const Vector& vecBase) const
	{
		return !this->IsEqual(vecBase);
	}

	Vector& operator=(const Vector& vecBase)
	{
		this->x = vecBase.x; this->y = vecBase.y; this->z = vecBase.z;
		return *this;
	}

	Vector& operator=(const Vector2D& vecBase2D)
	{
		this->x = vecBase2D.x; this->y = vecBase2D.y; this->z = 0.0f;
		return *this;
	}

	Vector& operator+=(const Vector& vecBase)
	{
		this->x += vecBase.x; this->y += vecBase.y; this->z += vecBase.z;
		return *this;
	}

	Vector& operator-=(const Vector& vecBase)
	{
		this->x -= vecBase.x; this->y -= vecBase.y; this->z -= vecBase.z;
		return *this;
	}

	Vector& operator*=(const Vector& vecBase)
	{
		this->x *= vecBase.x; this->y *= vecBase.y; this->z *= vecBase.z;
		return *this;
	}

	Vector& operator/=(const Vector& vecBase)
	{
		this->x /= vecBase.x; this->y /= vecBase.y; this->z /= vecBase.z; return *this;
	}

	Vector& operator+=(float flAdd)
	{
		this->x += flAdd; this->y += flAdd; this->z += flAdd;
		return *this;
	}

	Vector& operator-=(float flSubtract)
	{
		this->x -= flSubtract; this->y -= flSubtract; this->z -= flSubtract;
		return *this;
	}

	Vector& operator*=(float flMultiply)
	{
		this->x *= flMultiply; this->y *= flMultiply; this->z *= flMultiply;
		return *this;
	}

	Vector& operator/=(float flDivide)
	{
		this->x /= flDivide; this->y /= flDivide; this->z /= flDivide;
		return *this;
	}

	Vector operator+(const Vector& vecAdd) const
	{
		return Vector(this->x + vecAdd.x, this->y + vecAdd.y, this->z + vecAdd.z);
	}

	Vector operator-(const Vector& vecSubtract) const
	{
		return Vector(this->x - vecSubtract.x, this->y - vecSubtract.y, this->z - vecSubtract.z);
	}

	Vector operator*(const Vector& vecMultiply) const
	{
		return Vector(this->x * vecMultiply.x, this->y * vecMultiply.y, this->z * vecMultiply.z);
	}

	Vector operator/(const Vector& vecDivide) const
	{
		return Vector(this->x / vecDivide.x, this->y / vecDivide.y, this->z / vecDivide.z);
	}

	Vector operator+(float flAdd) const
	{
		return Vector(this->x + flAdd, this->y + flAdd, this->z + flAdd);
	}

	Vector operator-(float flSubtract) const
	{
		return Vector(this->x - flSubtract, this->y - flSubtract, this->z - flSubtract);
	}

	Vector operator*(float flMultiply) const
	{
		return Vector(this->x * flMultiply, this->y * flMultiply, this->z * flMultiply);
	}

	Vector operator/(float flDivide) const
	{
		return Vector(this->x / flDivide, this->y / flDivide, this->z / flDivide);
	}

	bool IsEqual(const Vector& vecEqual) const
	{
		return (std::fabsf(this->x - vecEqual.x) < std::numeric_limits<float>::epsilon() &&
				std::fabsf(this->y - vecEqual.y) < std::numeric_limits<float>::epsilon() &&
				std::fabsf(this->z - vecEqual.z) < std::numeric_limits<float>::epsilon());
	}

	bool IsZero() const
	{
		return (std::fpclassify(this->x) == FP_ZERO &&
				std::fpclassify(this->y) == FP_ZERO &&
				std::fpclassify(this->z) == FP_ZERO);
	}

	Vector2D ToVector2D()
	{
		return Vector2D(this->x, this->y);
	}

	float Length() const
	{
		return std::sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	float LengthSqr() const
	{
		return DotProduct(*this);
	}

	float Length2D() const
	{
		return std::sqrtf(this->x * this->x + this->y * this->y);
	}

	float Length2DSqr() const
	{
		return (this->x * this->x + this->y * this->y);
	}

	float DistTo(const Vector& vecEnd) const
	{
		return (*this - vecEnd).Length();
	}

	float DistToSqr(const Vector& vecEnd) const
	{
		return (*this - vecEnd).LengthSqr();
	}

	void Normalize()
	{
		float flLength = this->Length();

		if (flLength != 0.0f)
			*this /= flLength;
		else
			this->x = this->y = this->z = 0.0f;
	}

	Vector Normalized() const
	{
		Vector vecOut = *this;
		vecOut.Normalize();
		return vecOut;
	}

	float NormalizeInPlace()
	{
		Vector vecOut = *this;

		float flLength = vecOut.Length();
		float flRadius = 1.0f / (flLength + std::numeric_limits<float>::epsilon());

		vecOut.x *= flRadius;
		vecOut.y *= flRadius;
		vecOut.z *= flRadius;

		return flLength;
	}

	float DotProduct(const Vector& vecDot) const
	{
		return (this->x * vecDot.x + this->y * vecDot.y + this->z * vecDot.z);
	}

	Vector CrossProduct(const Vector& vecCross) const
	{
		return Vector(this->y * vecCross.z - this->z * vecCross.y, this->z * vecCross.x - this->x * vecCross.z, this->x * vecCross.y - this->y * vecCross.x);
	}

	float* Base()
	{
		return (float*)this;
	}

	float const* Base() const
	{
		return (float const*)this;
	}

public:
	float x, y, z;
};

class Vector4D
{
public:
	Vector4D() = default;
	Vector4D(float x, float y, float z, float w)
	{
		this->x = x; this->y = y; this->z = z; this->w = w;
	}

public:
	float x, y, z, w;
};

class VectorAligned : public Vector
{
public:
	VectorAligned() = default;

	VectorAligned(const Vector& v)
	{
		this->x = v.x; this->y = v.y; this->z = v.z;
	}

public:
	float w;
};
