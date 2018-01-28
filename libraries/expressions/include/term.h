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

#ifndef EXPRESSIONS_TERM_H
#define EXPRESSIONS_TERM_H

#include <term_parent.h>

namespace Expressions
{
	class Expression;
	class Variable;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// As part of the "expression" library, the Term class is the abstract base class of all terms   //
	// in expressions.                                                                               //
	///////////////////////////////////////////////////////////////////////////////////////////////////
	class Term : public TermParent
	{
	public:
		friend class Expressions::Expression;
		friend class Expressions::Variable;
	public:
		Term(const Expressions::Term & Term) = delete;
		Term(Expressions::Term && Term) = delete;
		Term & operator=(const Expressions::Term & Term) = delete;
		Term & operator=(Expressions::Term && Term) = delete;
	public:
		virtual ~Term(void);
		float GetValue(void);
		virtual float Calculate(void) = 0;
		virtual void InvalidateValue(void) override;
		void SetParent(Expressions::TermParent * Parent);
	protected:
		Term(void);
	private:
		Expressions::TermParent * _Parent;
		float * _Value;
	}; 
}

#endif
