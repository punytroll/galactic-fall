/**
 * expressions
 * Copyright (C) 2017-2018  Hagen Möbius
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

/**
 * This is part of version 0.6.1 of expressions.
 **/

#include <cassert>
#include <iostream>

#include <term.h>

#include "common.h"

Expressions::Term::Term(void) :
	_Parent(nullptr),
	_Value(nullptr)
{
	ON_DEBUG(std::cout << "Expressions::Term::Term()" << std::endl);
}

Expressions::Term::~Term(void)
{
	delete _Value;
	_Value = nullptr;
	ON_DEBUG(std::cout << "Expressions::Term::~Term()" << std::endl);
}

float Expressions::Term::GetValue(void)
{
	ON_DEBUG(std::cout << "Expressions::Term::GetValue()" << std::endl);
	if(_Value == nullptr)
	{
		_Value = new float(Calculate());
	}
	assert(_Value != nullptr);
	
	return *_Value;
}

void Expressions::Term::InvalidateValue(void)
{
	ON_DEBUG(std::cout << "Expressions::Term::InvalidateValue()" << std::endl);
	delete _Value;
	_Value = nullptr;
	if(_Parent != nullptr)
	{
		_Parent->InvalidateValue();
	}
}

void Expressions::Term::SetParent(Expressions::TermParent * Parent)
{
	ON_DEBUG(std::cout << "Expressions::Term::SetParent(Expressions::TermParent *)" << std::endl);
	assert(_Parent == nullptr || Parent == nullptr);
	_Parent = Parent;
}
