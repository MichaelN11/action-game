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

	int getX()
	{
		return x;
	}
	int getY()
	{
		return y;
	}
	int getW()
	{
		return w;
	}
	int getH()
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
private:
	int x;
	int y;
	int w;
	int h;
};