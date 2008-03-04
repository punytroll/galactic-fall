/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include <assert.h>

#include "../source/referencing.h"

class A
{
};

int main(int argc, char ** argv)
{
	{
		Reference< A > r;
		
		assert(r.IsValid() == false);
		assert(r.Get() == 0);
	}
	{
		Reference< A > r1;
		Reference< A > r2;
		
		assert(r1 == r2);
	}
	{
		A a;
		Reference< A > r(a);
		
		assert(r.IsValid() == true);
		assert(r.Get() == &a);
	}
	{
		A a;
		Reference< A > r;
		
		r = a;
		assert(r.IsValid() == true);
		assert(r.Get() == &a);
	}
	{
		A a;
		Reference< A > r(a);
		
		r.Clear();
		assert(r.IsValid() == false);
		assert(r.Get() == 0);
	}
	{
		A a;
		Reference< A > r(a);
		
		r.Invalidate();
		assert(r.IsValid() == false);
		assert(r.Get() == 0);
	}
	{
		A a;
		Reference< A > r1(a);
		Reference< A > r2;
		
		assert(r1 != r2);
	}
	{
		A a;
		Reference< A > r1(a);
		Reference< A > r2;
		
		r2 = r1;
		assert(r2.IsValid() == true);
		assert(r2.Get() == &a);
		assert(r2 == r1);
	}
	{
		A a;
		Reference< A > r1(a);
		Reference< A > r2(r1);
		
		assert(r2.IsValid() == true);
		assert(r2.Get() == &a);
		assert(r2 == r1);
	}
	{
		A a;
		Reference< A > r1(a);
		Reference< A > r2(r1);
		
		r1.Clear();
		assert(r2.IsValid() == true);
		assert(r2.Get() == &a);
		assert(r2 != r1);
	}
	{
		A a;
		Reference< A > r1(a);
		Reference< A > r2;
		
		r2 = r1;
		r1.Clear();
		assert(r2.IsValid() == true);
		assert(r2.Get() == &a);
		assert(r2 != r1);
	}
	{
		A a;
		Reference< A > r1(a);
		Reference< A > r2(r1);
		
		r1.Invalidate();
		assert(r2.IsValid() == false);
		assert(r2.Get() == 0);
		assert(r2 == r1);
	}
	{
		A a1;
		A a2;
		Reference< A > r1(a1);
		Reference< A > r2(a2);
		
		assert(r1 != r2);
		r1.Invalidate();
		r2.Invalidate();
		assert(r1 == r2);
	}
	
	return 0;
}
