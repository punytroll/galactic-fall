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

#include <expression.h>
#include <variable.h>

#include "common.h"
#include "variable_term.h"

using namespace Expressions::Operators;

Expressions::Variable::Variable(void)
{
	ON_DEBUG(std::cout << "Expressions::Variable::Variable()" << std::endl);
}

Expressions::Variable::Variable(Expressions::Expression && Expression) :
	_Term(std::move(Expression.ExtractTerm()))
{
	ON_DEBUG(std::cout << "Expressions::Variable::Variable(Expressions::Expression &&)" << std::endl);
	_Term->SetParent(this);
	InvalidateValue();
}

Expressions::Variable::~Variable(void)
{
	ON_DEBUG(std::cout << "Expressions::Variable::~Variable()" << std::endl);
	assert(_Term == nullptr || _Term->_Parent == this);
	while(_DependentVariableTerms.empty() == false)
	{
		auto DependentVariableTerm{_DependentVariableTerms.begin()};
		
		(*DependentVariableTerm)->InvalidateReference();
		_DependentVariableTerms.erase(DependentVariableTerm);
	}
}

float Expressions::Variable::GetValue(void)
{
	ON_DEBUG(std::cout << "Expressions::Variable::GetValue()" << std::endl);
	if(_Term != nullptr)
	{
		return _Term->GetValue();
	}
	else
	{
		return std::nanf("");
	}
}

void Expressions::Variable::InvalidateValue(void)
{
	ON_DEBUG(std::cout << "Expressions::Variable::InvalidateValue()" << std::endl);
	for(auto & DependentVariableTerm : _DependentVariableTerms)
	{
		ON_DEBUG(std::cout << "    Notifying one dependent variable term that we changed." << std::endl);
		DependentVariableTerm->InvalidateValue();
	}
}

void Expressions::Variable::SetExpression(Expressions::Expression && Expression)
{
	ON_DEBUG(std::cout << "Expressions::Variable::SetExpression(Expressions::Expression &&)" << std::endl);
	
	float OldValue{std::nanf("")};
	float NewValue{std::nanf("")};
	
	if(_Term != nullptr)
	{
		OldValue = _Term->GetValue();
		assert(_Term->_Parent == this);
		_Term->SetParent(nullptr);
	}
	_Term = Expression.ExtractTerm();
	if(_Term != nullptr)
	{
		_Term->SetParent(this);
		NewValue = _Term->GetValue();
	}
	if(OldValue != NewValue)
	{
		InvalidateValue();
	}
}

void Expressions::Variable::_AddDependentTerm(Expressions::VariableTerm * DependentVariableTerm)
{
	_DependentVariableTerms.insert(DependentVariableTerm);
}

void Expressions::Variable::_RemoveDependentTerm(Expressions::VariableTerm * DependentVariableTerm)
{
	_DependentVariableTerms.erase(DependentVariableTerm);
}

Expressions::Variable & Expressions::Variable::operator=(Expressions::Variable & Variable)
{
	ON_DEBUG(std::cout << "Expressions::Variable & Expressions::Variable::operator=(Expressions::Variable &)" << std::endl);
	SetExpression(Expressions::Expression(Variable));
	
	return *this;
}

Expressions::Variable & Expressions::Variable::operator=(Expressions::Expression && Expression)
{
	ON_DEBUG(std::cout << "Expressions::Variable & Expressions::Variable::operator=(Expressions::Expression &&)" << std::endl);
	SetExpression(std::move(Expression));
	
	return *this;
}
