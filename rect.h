/*
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Original Author
kzinti @ pocketfrog.com

Copyright 2007
Marvin Sanchez
code.google.com/p/ashlar
*/

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
