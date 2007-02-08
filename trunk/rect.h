#pragma once

#include "common.h"

// POINT & RECT
#include <windows.h>

namespace Ash {

	class Point : public POINT
	{
	public:

		Point()
		{}

		Point( long ax, long ay )
		{
			x = ax;
			y = ay;
		}

		void Set( long ax, long ay )
		{
			x = ax;
			y = ay;
		}

		void Translate( long dx, long dy )
		{
			x += dx;
			y += dy;
		}

		void Scale( long s)
		{
			x *= s;
			y *= s;
		}

		inline Point& operator+=( const Point& p )
		{
			x += p.x;
			y += p.y;
			return *this;
		}

		inline Point& operator-=( const Point& p )
		{
			x -= p.x;
			y -= p.y;
			return *this;
		}
	};

	inline Point operator+( const Point& a, const Point& b )
	{
		Point p(a);
		p += b;
		return p;
	}

	inline Point operator-( const Point& a, const Point& b )
	{
		Point p(a);
		p -= b;
		return p;
	}

	class Rect : public RECT
	{
	public:

		Rect()
		{}

		Rect( long l, long t, long r, long b)
		{
			left = l;
			top = t;
			right = r;
			bottom = b;
		}

		Rect( const Point& p, long w, long h )
		{
			left = p.x;
			top = p.y;
			right = p.x + w;
			bottom = p.y + h;
		}

		long Width() const
		{
			return right - left;
		}

		long Height() const
		{
			return bottom - top;
		}

		bool IsEmpty() const
		{
			return (right <= left) || (bottom <= top);
		}

		bool Contains( const Point& Point ) const
		{
			return Point.x >= left && Point.x < right && Point.y >= top && Point.y < bottom;
		}

		void Move( long x, long y )
		{
			Translate( x-left, y-top );
		}

		void Move( const Point& p )
		{
			Translate( p.x-left, p.y-top );
		}

		void Set( long l, long t, long r, long b )
		{
			left = l;
			top = t;
			right = r;
			bottom = b;
		}

		void Translate( long dx, long dy )
		{
			left += dx;
			right += dx;
			top += dy;
			bottom += dy;
		}

		void Translate( const Point& p )
		{
			left += p.x;
			right += p.x;
			top += p.y;
			bottom += p.y;
		}
	};

};
