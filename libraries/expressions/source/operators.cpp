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

#include <iostream>

#include <expression.h>
#include <operators.h>

#include "addition_term.h"
#include "common.h"
#include "constant_term.h"
#include "division_term.h"
#include "multiplication_term.h"
#include "negation_term.h"
#include "subtraction_term.h"
#include "variable_term.h"

Expressions::Expression Expressions::Operators::operator"" _c(long double Value)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator\"\" _c(long double)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::ConstantTerm >(Value));
}

Expressions::Expression Expressions::Operators::constant(double Value)
{
	ON_DEBUG(std::cout << "Expressions::Operators::constant(double)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::ConstantTerm >(Value));
}

Expressions::Expression Expressions::Operators::operator-(Expressions::Expression && One)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator-(Expressions::Expression &&)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::NegationTerm >(std::move(One.ExtractTerm())));
}

Expressions::Expression Expressions::Operators::operator-(Expressions::Variable & Variable)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator-(Expressions::Variable &)" << std::endl);
	
	return -Expressions::Expression(std::make_unique< Expressions::VariableTerm >(&Variable));
}

Expressions::Expression Expressions::Operators::operator+(Expressions::Expression && One, Expressions::Expression && Two)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator+(Expressions::Expression &&, Expressions::Expression &&)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::AdditionTerm >(std::move(One.ExtractTerm()), std::move(Two.ExtractTerm())));
}

Expressions::Expression Expressions::Operators::operator+(Expressions::Variable & One, Expressions::Expression && Two)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator+(Expressions::Variable &, Expressions::Expression &&)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::VariableTerm >(&One)) + std::move(Two.ExtractTerm());
}

Expressions::Expression Expressions::Operators::operator+(Expressions::Variable & One, Expressions::Variable & Two)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator+(Expressions::Variable &, Expressions::Variable &)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::VariableTerm >(&One)) + Expressions::Expression(std::make_unique< Expressions::VariableTerm >(&Two));
}

Expressions::Expression Expressions::Operators::operator-(Expressions::Expression && One, Expressions::Expression && Two)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator+(Expressions::Expression &&, Expressions::Expression &&)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::SubtractionTerm >(std::move(One.ExtractTerm()), std::move(Two.ExtractTerm())));
}

Expressions::Expression Expressions::Operators::operator-(Expressions::Variable & One, Expressions::Expression && Two)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator-(Expressions::Variable &, Expressions::Expression &&)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::VariableTerm >(&One)) - std::move(Two.ExtractTerm());
}

Expressions::Expression Expressions::Operators::operator*(Expressions::Expression && One, Expressions::Expression && Two)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator*(Expressions::Expression &&, Expressions::Expression &&)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::MultiplicationTerm >(std::move(One.ExtractTerm()), std::move(Two.ExtractTerm())));
}

Expressions::Expression Expressions::Operators::operator/(Expressions::Expression && One, Expressions::Expression && Two)
{
	ON_DEBUG(std::cout << "Expressions::Operators::operator/(Expressions::Expression &&, Expressions::Expression &&)" << std::endl);
	
	return Expressions::Expression(std::make_unique< Expressions::DivisionTerm >(std::move(One.ExtractTerm()), std::move(Two.ExtractTerm())));
}
