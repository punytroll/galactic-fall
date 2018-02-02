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

#ifndef EXPRESSIONS_EXPRESSION_H
#define EXPRESSIONS_EXPRESSION_H

#include <memory>

#include <operators.h>
#include <term_parent.h>

namespace Expressions
{
	class Term;
	class Variable;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// As part of the expression library, the Expression class is a shallow wrapper class around any //
	// term. It is mainly used to hide the implementation detail of the unique_ptr for terms.        //
	///////////////////////////////////////////////////////////////////////////////////////////////////
	class Expression : public TermParent
	{
	public:
		friend class Expressions::Variable;
		friend Expressions::Expression Expressions::Operators::operator"" _c(long double Value);
		friend Expressions::Expression Expressions::Operators::constant(double Value);
		friend Expressions::Expression Expressions::Operators::operator-(Expressions::Expression && One);
		friend Expressions::Expression Expressions::Operators::operator-(Expressions::Variable & Variable);
		friend Expressions::Expression Expressions::Operators::operator+(Expressions::Expression && One, Expressions::Expression && Two);
		friend Expressions::Expression Expressions::Operators::operator+(Expressions::Variable & One, Expressions::Expression && Two);
		friend Expressions::Expression Expressions::Operators::operator+(Expressions::Variable & One, Expressions::Variable & Two);
		friend Expressions::Expression Expressions::Operators::operator-(Expressions::Expression && One, Expressions::Expression && Two);
		friend Expressions::Expression Expressions::Operators::operator-(Expressions::Variable & One, Expressions::Expression && Two);
		friend Expressions::Expression Expressions::Operators::operator*(Expressions::Expression && One, Expressions::Expression && Two);
		friend Expressions::Expression Expressions::Operators::operator/(Expressions::Expression && One, Expressions::Expression && Two);
	public:
		Expression(const Expressions::Expression & Expression) = delete;
		Expressions::Expression & operator=(const Expressions::Expression & Expression) = delete;
	public:
		Expression(void);
		Expression(Expressions::Variable & Variable);
		Expression(Expressions::Expression && Expression);
		~Expression(void);
		std::unique_ptr< Expressions::Term > ExtractTerm(void);
		float GetValue(void);
		virtual void InvalidateValue(void);
		Expressions::Expression & operator=(Expressions::Expression && Expression);
	private:
		Expression(std::unique_ptr< Expressions::Term > && Term);
		std::unique_ptr< Expressions::Term > _Term;
	};
}

#endif
