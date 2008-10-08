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

#include "../source/callbacks/callbacks.h"

bool g_Action;

void Test0(void)
{
}

void Test0Action(void)
{
	assert(g_Action == false);
	g_Action = true;
}

void Test1Action(bool & Action)
{
	assert(Action == false);
	Action = true;
}

bool Test0True(void)
{
	return true;
}

bool Test0False(void)
{
	return false;
}

bool Test1Negate(bool Argument)
{
	return !Argument;
}

class Test
{
public:
	void Test0(void)
	{
	}
	
	void Test0Action(void)
	{
		assert(g_Action == false);
		g_Action = true;
	}
	
	void Test1Action(bool Action)
	{
		g_Action = Action;
	}
};

int main(int argc, char ** argv)
{
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback0< void > TestCallback;
		
		assert(TestCallback.IsValid() == false);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback0< void > TestCallback(Callback(Test0));
		
		assert(TestCallback.IsValid() == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback0< void > TestCallback;
		
		TestCallback = Callback(Test0);
		assert(TestCallback.IsValid() == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback0< void > TestCallback(Callback(Test0Action));
		
		g_Action = false;
		TestCallback();
		assert(g_Action == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback0< void > TestCallback;
		
		TestCallback = Callback(Test0Action);
		g_Action = false;
		TestCallback();
		assert(g_Action == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback0< bool > TestCallback(Callback(Test0True));
		
		assert(TestCallback() == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback0< bool > TestCallback(Callback(Test0False));
		
		assert(TestCallback() == false);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Test Tester;
		Callback0< void > TestCallback(Callback(&Tester, &Test::Test0));
		
		assert(TestCallback.IsValid() == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Test Tester;
		Callback0< void > TestCallback;
		
		TestCallback = Callback(&Tester, &Test::Test0);
		assert(TestCallback.IsValid() == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Test Tester;
		Callback0< void > TestCallback(Callback(&Tester, &Test::Test0Action));
		
		g_Action = false;
		TestCallback();
		assert(g_Action == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Test Tester;
		Callback0< void > TestCallback;
		
		TestCallback = Callback(&Tester, &Test::Test0Action);
		g_Action = false;
		TestCallback();
		assert(g_Action == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback1< bool, bool > TestCallback;
		
		assert(TestCallback.IsValid() == false);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback1< bool, bool > TestCallback(Callback(Test1Negate));
		
		assert(TestCallback.IsValid() == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback1< bool, bool > TestCallback;
		
		TestCallback = Callback(Test1Negate);
		assert(TestCallback.IsValid() == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback1< bool, bool > TestCallback(Callback(Test1Negate));
		
		assert(TestCallback(true) == false);
		assert(TestCallback(false) == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback1< bool, bool > TestCallback;
		
		TestCallback = Callback(Test1Negate);
		assert(TestCallback(true) == false);
		assert(TestCallback(false) == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Callback1< void, bool & > TestCallback(Callback(Test1Action));
		bool Action(false);
		
		TestCallback(Action);
		assert(Action == true);
	}
	
	std::cout << "-----------------------------" << std::endl;
	
	{
		Test Tester;
		Callback0< void > TestCallback(Bind1(Callback(&Tester, &Test::Test1Action), true));
		
		g_Action = false;
		TestCallback();
		assert(g_Action == true);
	}
	
	return 0;
}
