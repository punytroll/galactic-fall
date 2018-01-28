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

#ifndef EXPRESSIONS_ADDITION_TERM_H
#define EXPRESSIONS_ADDITION_TERM_H

#include <memory>

#include "term.h"

namespace Expressions
{
	class AdditionTerm : public Term
	{
	public:
		AdditionTerm(void) = delete;
		AdditionTerm(const Expressions::AdditionTerm & Term) = delete;
		AdditionTerm(Expressions::AdditionTerm && Term) = delete;
		AdditionTerm & operator=(const Expressions::AdditionTerm & Term) = delete;
		AdditionTerm & operator=(Expressions::AdditionTerm && Term) = delete;
	public:
		AdditionTerm(std::unique_ptr< Expressions::Term > && One, std::unique_ptr< Expressions::Term > && Two);
		virtual ~AdditionTerm(void);
		virtual float Calculate(void) override;
	private:
		std::unique_ptr< Expressions::Term > _One;
		std::unique_ptr< Expressions::Term > _Two;
	};
}

#endif
