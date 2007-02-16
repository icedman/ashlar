/*
Version: MPL 1.1/GPL 2.0/LGPL 2.1

The contents of this file are subject to the Mozilla Public License Version
1.1 (the "License"); you may not use this file except in compliance with
the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

Copyright 2007
Marvin Sanchez
code.google.com/p/ashlar
*/

#pragma once

#include "debug.h"

namespace Ash
{



	typedef bool (*SortFunc) (void*, void*);

	//! simple list node template. does not free nodes on destroy
	template<class _Node>
	class Node {
	public:
		Node()
		{
			prev = 0;
			next = 0;
		}
		~Node()
		{}

	public:
		_Node *prev;
		_Node *next;
		TRACEOBJ(nodeRef)
	};

	template<class _Node>
	class List {
	public:
		List()
		{
			head = 0;
			tail = 0;
			size = 0;
		}

		~List()
		{}

	public:

		//! Add an item
		bool Push(_Node* n)
		{
			if (!tail)
			{
				head = n;
				tail = n;
				size++;
				return true;
			}

			n->prev = tail;
			tail->next = n;
			tail = n;
			size++;
			return true;
		}

		//! Removes the last item
		bool Pop()
		{
			_Node *n = tail;
			if (!n) return false;

			tail = tail->prev;
			if (tail) tail->next = 0;
			size--;

			if (!size)
			{
				head=tail=0;
			}

			return true;
		}

		//! Clears the list
		void Clear()
		{
			while(Size())
				Pop();
		}

		//! Get current list size
		unsigned long Size()
		{
			return size;
		}

		//! Insert new item at index
		bool InsertAt(unsigned long index, _Node *n)
		{
			InsertBefore(n, GetAt(index));
		}

		//! Insert new item before a specified reference node
		bool InsertBefore(_Node *newn, _Node *refNode)
		{
			_Node *n = refNode;

			if (!n)
			{
				return Push(newn);
			}

			if (refNode == head)
			{
				head->prev = newn;
				newn->next = head;
				head = newn;
				size++;
				return true;
			}

			newn->next = n;
			newn->prev = n->prev;
			n->prev->next = newn;
			n->prev = newn;
			size++;

			return true;
		}

		//! Removes an item
		bool Remove(_Node *node)
		{
			if (!node)
			{
				return false;
			}

			if (node == tail)
			{
				return Pop();
			}

			_Node *np = node->prev;
			_Node *nn = node->next;

			if (node == head)
			{
				head = nn;
			} else {
				np->next = nn;
			}
			nn->prev = np;

			size--;
			if (!size)
			{ 
				head=tail = 0;
			}

			return true;
		}

		//! Removes an item at specified index
		bool RemoveAt(unsigned long index)
		{
			if (!size)
				return 0;
			if (index >= size-1)
				return Pop();

			return Remove(GetAt(index));
		}

		//! Retrieves an item
		_Node* operator[](unsigned long index)
		{
			return GetAt(index);
		}

		//! Retrieves an item
		_Node* GetAt(unsigned long index)
		{
			_Node *n = head;
			if (!n) return 0;

			unsigned long s=0;
			while(s!=index && n->next)
			{
				n = n->next;
				s++;
			}

			if (s!=index) return 0;
			return n;
		}

		//! Get first item
		inline _Node* GetFirst() { return head; }
		//! Get last item
		inline _Node* GetLast() { return tail; }

		//! Sort function
		void Sort(SortFunc func)
		{
			_Node *i = head;
			while(i)
			{
				_Node *j = i;
				while(j)
				{
					if (func(i, j))
					{
						// todo: swap nodes
					}
					j = j->next;
				}
				i = i->next;
			}
		}

	public:
		_Node *head;
		_Node *tail;
		unsigned long	size;
		TRACEOBJ(nodeRef)
	};

}