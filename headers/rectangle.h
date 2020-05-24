#pragma once

#include <cmath>

template<typename T>
class Rectangle
{
public:
	Rectangle(T x = 0, T y = 0, T w = 0, T h = 0) :
		x(x),
		y(y),
		w(w),
		h(h)
	{}

	bool contains(T pointX, T pointY) const
	{
		return (pointX > x && pointX < getX2() && pointY > y && pointY < getY2());
	}

	bool isColliding(const Rectangle& other)
	{
		return (getX() < other.getX2() &&
			getX2() > other.getX() &&
			getY() < other.getY2() &&
			getY2() > other.getY());
	}

	// returns xmovement and ymovement required to prevent collision
	void getCollisionDirection(const Rectangle& other, T& xDirection, T& yDirection)
	{
		T leftGap = other.getX2() - getX();
		T rightGap = other.getX() - getX2();
		T topGap = other.getY2() - getY();
		T bottomGap = other.getY() - getY2();

		T xDir = (std::abs(leftGap) < std::abs(rightGap)) ? leftGap : rightGap;
		T yDir = (std::abs(topGap) < std::abs(bottomGap)) ? topGap : bottomGap;

		if (std::abs(xDir) < std::abs(yDir))
			yDir = 0;
		else if (std::abs(xDir) > std::abs(yDir))
			xDir = 0;

		xDirection = xDir;
		yDirection = yDir;
	}

	void shift(T shiftX, T shiftY)
	{
		x += shiftX;
		y += shiftY;
	}

	T getX() const
	{
		return x;
	}
	T getY() const
	{
		return y;
	}
	T getW() const
	{
		return w;
	}
	T getH() const
	{
		return h;
	}
	void setX(T n)
	{
		x = n;
	}
	void setY(T n)
	{
		y = n;
	}
	void setW(T n)
	{
		w = n;
	}
	void setH(T n)
	{
		h = n;
	}

	T getX2() const
	{
		return x + w;
	}
	T getY2() const
	{
		return y + h;
	}
private:
	T x;
	T y;
	T w;
	T h;
};