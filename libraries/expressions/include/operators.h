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
 * This is part of version 0.6.0 of expressions.
 **/

#ifndef EXPRESSIONS_OPERATORS_H
#define EXPRESSIONS_OPERATORS_H

namespace Expressions
{
	class Expression;
	class Variable;
	
	namespace Operators
	{
		Expressions::Expression operator"" _c(long double Value);
		Expressions::Expression constant(double Value);
		Expressions::Expression operator-(Expressions::Expression && One);
		Expressions::Expression operator-(Expressions::Variable & Variable);
		Expressions::Expression operator+(Expressions::Expression && One, Expressions::Expression && Two);
		Expressions::Expression operator+(Expressions::Variable & One, Expressions::Expression && Two);
		Expressions::Expression operator+(Expressions::Variable & One, Expressions::Variable & Two);
		Expressions::Expression operator-(Expressions::Expression && One, Expressions::Expression && Two);
		Expressions::Expression operator-(Expressions::Variable & One, Expressions::Expression && Two);
		Expressions::Expression operator*(Expressions::Expression && One, Expressions::Expression && Two);
		Expressions::Expression operator/(Expressions::Expression && One, Expressions::Expression && Two);
	}
}

#endif
