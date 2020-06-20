#pragma once

#include <cmath>

const float PI = (float)(2 * acos(0.0));

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

	float getCollisionAngle(const Rectangle& other)
	{
		float angle = 0;
		T xDir, yDir;
		getCollisionDirection(other, xDir, yDir);
		// diagonal collision
		if (xDir != 0 && yDir != 0)
		{
			angle = PI / 4;
			if (xDir < 0)
			{
				angle += PI / 2;
			}
			if (yDir < 0)
			{
				angle *= -1;
			}
		}
		// vertical collision
		else if (yDir != 0)
		{
			// compare left sides and right sides, find the max distance this rectangle extends past other
			T leftGap = other.getX() - getX();
			T rightGap = getX2() - other.getX2();
			T maxGap = (leftGap > rightGap) ? leftGap : rightGap;
			// if this rectangle is inside both edges of other, collision angle is 90 degrees
			if (maxGap <= 0)
			{
				angle = PI / 2;
			}
			// get the ratio of the max gap between edges and convert it to an angle
			else
			{
				float gapWidthRatio = maxGap / getW();
				angle = (1 - gapWidthRatio) * (PI / 2);
				// angle is to the left
				if (leftGap > rightGap)
				{
					angle = PI - angle;
				}
			}
			// angle is downward
			if (yDir > 0)
			{
				angle *= -1;
			}
		}
		// horizontal collision
		else if (xDir != 0)
		{
			T downGap = getY2() - other.getY2();
			T upGap = other.getY() - getY();
			T maxGap = (downGap > upGap) ? downGap : upGap;
			if (maxGap <= 0)
			{
				angle = 0;
			}
			else
			{
				float gapHeightRatio = maxGap / getH();
				angle = gapHeightRatio * (PI / 2);
				if (downGap > upGap)
				{
					angle *= -1;
				}
			}
			if (xDir < 0)
			{
				// use negative angles for downward angles
				if (angle > 0)
				{
					angle = PI - angle;
				}
				else
				{
					angle = -PI - angle;
				}
			}
		}
		
		return angle;
	}

	void scalePositionAndSize(float scale)
	{
		x = (T)(x * scale);
		y = (T)(y * scale);
		w = (T)(w * scale);
		h = (T)(h * scale);
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