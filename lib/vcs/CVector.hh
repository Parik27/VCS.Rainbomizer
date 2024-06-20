#pragma once

class CVector
{
public:
    float x;
    float y;
    float z;
    float w;

    const CVector &
    operator+= (CVector const &right)
    {
        x += right.x;
        y += right.y;
        z += right.z;
        return *this;
    }

    const CVector &
    operator-= (CVector const &right)
    {
        x -= right.x;
        y -= right.y;
        z -= right.z;
        return *this;
    }

    const CVector &
    operator*= (float right)
    {
        x *= right;
        y *= right;
        z *= right;
        return *this;
    }

    const CVector &
    operator/= (float right)
    {
        x /= right;
        y /= right;
        z /= right;
        return *this;
    }

    CVector
    operator- () const
    {
        return CVector (-x, -y, -z);
    }

    const bool
    operator== (CVector const &right)
    {
        return x == right.x && y == right.y && z == right.z;
    }

    const bool
    operator!= (CVector const &right)
    {
        return x != right.x || y != right.y || z != right.z;
    }
};

inline CVector operator+(const CVector &left, const CVector &right)
{
	return CVector(left.x + right.x, left.y + right.y, left.z + right.z);
}

inline CVector operator-(const CVector &left, const CVector &right)
{
	return CVector(left.x - right.x, left.y - right.y, left.z - right.z);
}

inline CVector operator*(const CVector &left, float right)
{
	return CVector(left.x * right, left.y * right, left.z * right);
}

inline CVector operator*(float left, const CVector &right)
{
	return CVector(left * right.x, left * right.y, left * right.z);
}

inline CVector operator/(const CVector &left, float right)
{
	return CVector(left.x / right, left.y / right, left.z / right);
}
