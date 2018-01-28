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
 * This is part of version 0.5.1 of expressions.
 **/

#include <cassert>
#include <cmath>
#include <iostream>

#include <variable.h>

#include "common.h"
#include "variable_term.h"

Expressions::VariableTerm::VariableTerm(Expressions::Variable * Variable) :
	_Variable(Variable)
{
	assert(Variable != nullptr);
	ON_DEBUG(std::cout << "Expressions::VariableTerm::VariableTerm(Expressions::Variable *)" << std::endl);
	_Variable->_AddDependentTerm(this);
}

Expressions::VariableTerm::~VariableTerm(void)
{
	ON_DEBUG(std::cout << "Expressions::VariableTerm::~VariableTerm()" << std::endl);
	if(_Variable != nullptr)
	{
		_Variable->_RemoveDependentTerm(this);
	}
}

float Expressions::VariableTerm::Calculate(void)
{
	ON_DEBUG(std::cout << "Expressions::VariableTerm::Calculate()" << std::endl);
	if(_Variable != nullptr)
	{
		return _Variable->GetValue();
	}
	else
	{
		return std::nanf("");
	}
}

void Expressions::VariableTerm::InvalidateReference(void)
{
	ON_DEBUG(std::cout << "Expressions::VariableTerm::InvalidateReference()" << std::endl);
	assert(_Variable != nullptr);
	_Variable = nullptr;
	InvalidateValue();
}
