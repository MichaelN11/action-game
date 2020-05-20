#pragma once

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