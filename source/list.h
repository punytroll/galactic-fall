/**
 * Copyright (C) 2009  Hagen Möbius
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
 * This is version 1.0.1 of the templated simple list.
 **/

#ifndef LIST_H
#define LIST_H

template < typename Type >
struct Node
{
	Node(const Type & Value) :
		_Value(Value)
	{
	}
	
	Node< Type > * const GetNext(void)
	{
		return _Next;
	}
	
	Node< Type > * const GetPrevious(void)
	{
		return _Previous;
	}
	
	const Type & GetValue(void) const 
	{
		return _Value;
	}
	
	void SetValue(const Type & Value)
	{
		_Value = Value;
	}
	
	Node< Type > * _Next;
	Node< Type > * _Previous;
	Type _Value;
};

template < typename Type >
class List
{
public:
	List(void) :
		_Head(0),
		_Tail(0)
	{
	}
	
	~List(void)
	{
		while(_Head != 0)
		{
			Remove(_Head);
		}
	}
	
	Node< Type > * GetHead(void)
	{
		return _Head;
	}
	
	Node< Type > * GetTail(void)
	{
		return _Tail;
	}
	
	Node< Type > * Append(const Type & Value)
	{
		Node< Type > * NewNode(new Node< Type >(Value));
		
		NewNode->_Next = 0;
		if(_Head == 0)
		{
			NewNode->_Previous = 0;
			_Head = NewNode;
		}
		else
		{
			NewNode->_Previous = _Tail;
			_Tail->_Next = NewNode;
		}
		_Tail = NewNode;
		
		return NewNode;
	}
	
	void Remove(Node< Type > * Node)
	{
		if(Node == _Head)
		{
			_Head = Node->GetNext();
		}
		else
		{
			Node->_Previous->_Next = Node->_Next;
		}
		if(Node == _Tail)
		{
			_Tail = Node->GetPrevious();
		}
		else
		{
			Node->_Next->_Previous = Node->_Previous;
		}
		
		delete Node;
	}
private:
	Node< Type > * _Head;
	Node< Type > * _Tail;
};

#endif
