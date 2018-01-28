/**
 * algebra
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
 * This is part of version 0.5.0 of expressions.
 **/

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

#include <expression.h>
#include <term.h>

#include "common.h"
#include "variable_term.h"

Expressions::Expression::Expression(void)
{
	ON_DEBUG(std::cout << "Expressions::Expression::Expression()" << std::endl);
}

Expressions::Expression::Expression(Expression && Expression)
{
	ON_DEBUG(std::cout << "Expressions::Expression::Expression(Expressions::Expression &&)" << std::endl);
	if(_Term != nullptr)
	{
		assert(_Term->_Parent == this);
		_Term->SetParent(nullptr);
	}
	_Term = Expression.ExtractTerm();
	if(_Term != nullptr)
	{
		_Term->SetParent(this);
	}
}

Expressions::Expression::Expression(Expressions::Variable & Variable) :
	_Term(std::make_unique< Expressions::VariableTerm >(&Variable))
{
	ON_DEBUG(std::cout << "Expressions::Expression::Expression(Expressions::Variable &)" << std::endl);
	_Term->SetParent(this);
}

Expressions::Expression::Expression(std::unique_ptr< Expressions::Term > && Term) :
	_Term(std::move(Term))
{
	ON_DEBUG(std::cout << "Expressions::Expression::Expression(std::unique_ptr< Expressions::Term > &&)" << std::endl);
	_Term->SetParent(this);
}

Expressions::Expression::~Expression(void)
{
	ON_DEBUG(std::cout << "Expressions::Expression::~Expression()" << std::endl);
	assert(_Term == nullptr || _Term->_Parent == this);
}

std::unique_ptr< Expressions::Term > Expressions::Expression::ExtractTerm(void)
{
	ON_DEBUG(std::cout << "std::unique_ptr< Expressions::Term > Expressions::Expression::ExtractTerm()" << std::endl);
	_Term->SetParent(nullptr);
	
	return std::move(_Term);
}

float Expressions::Expression::GetValue(void)
{
	ON_DEBUG(std::cout << "float Expressions::Expression::GetValue()" << std::endl);
	if(_Term)
	{
		return _Term->GetValue();
	}
	else
	{
		return NAN;
	}
}

void Expressions::Expression::InvalidateValue(void)
{
	ON_DEBUG(std::cout << "Expressions::Expression::InvalidateValue()" << std::endl);
}

Expressions::Expression & Expressions::Expression::operator=(Expressions::Expression && Expression)
{
	ON_DEBUG(std::cout << "Expressions::Expression::operator=(Expressions::Expression &&)" << std::endl);
	if(_Term != nullptr)
	{
		assert(_Term->_Parent == this);
		_Term->SetParent(nullptr);
	}
	_Term = Expression.ExtractTerm();
	if(_Term != nullptr)
	{
		_Term->SetParent(this);
	}
	
	return *this;
}
