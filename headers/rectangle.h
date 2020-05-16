#pragma once

class Rectangle

{
public:
	Rectangle(int x = 0, int y = 0, int w = 0, int h = 0) :
		x(x),
		y(y),
		w(w),
		h(h)
	{}

	bool contains(int pointX, int pointY) const
	{
		return (pointX > x && pointX < getX2() && pointY > y && pointY < getY2());
	}

	int getX() const
	{
		return x;
	}
	int getY() const
	{
		return y;
	}
	int getW() const
	{
		return w;
	}
	int getH() const
	{
		return h;
	}
	void setX(int n)
	{
		x = n;
	}
	void setY(int n)
	{
		y = n;
	}
	void setW(int n)
	{
		w = n;
	}
	void setH(int n)
	{
		h = n;
	}

	int getX2() const
	{
		return x + w;
	}
	int getY2() const
	{
		return y + h;
	}
private:
	int x;
	int y;
	int w;
	int h;
};