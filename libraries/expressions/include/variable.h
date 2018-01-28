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

#ifndef EXPRESSIONS_VARIABLE_H
#define EXPRESSIONS_VARIABLE_H

#include <memory>
#include <unordered_set>

#include <term_parent.h>

namespace Expressions
{
	class Expression;
	class Term;
	class VariableTerm;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	// As part of the "expression" library, a Variable contains an expression, that can be changed.  //
	// Other expressions can reference this variable and its value with a VariableTerm. These terms  //
	// are called dependent, because any change of the variable's value (by changing its expression) //
	// also changes the value of the VariableTerm. A list of back references to the dependent term   //
	// is kept, so that invalidations can be propagated appropriately.                               //
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	class Variable : public TermParent
	{
	public:
		friend class Expressions::VariableTerm;
	public:
		Variable(const Expressions::Variable & Variable) = delete;
		Variable(Expressions::Variable && Variable) = delete;
		Expressions::Variable & operator=(const Expressions::Variable & Variable) = delete;
		Expressions::Variable & operator=(Expressions::Variable && Variable) = delete;
	public:
		Variable(void);
		Variable(Expressions::Expression && Expression);
		~Variable(void);
		float GetValue(void);
		virtual void InvalidateValue(void) override;
		void SetExpression(Expressions::Expression && Expression);
		Expressions::Variable & operator=(Expressions::Variable & Variable);
		Expressions::Variable & operator=(Expressions::Expression && Expression);
	private:
		void _AddDependentTerm(Expressions::VariableTerm * DependentVariableTerm);
		void _RemoveDependentTerm(Expressions::VariableTerm * DependentVariableTerm);
		std::unordered_multiset< Expressions::VariableTerm * > _DependentVariableTerms;
		std::unique_ptr< Expressions::Term > _Term;
	};
}

#endif
