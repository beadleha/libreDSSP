/*	This file is part of libreDSSP.

	Copyright 2019 Alan Beadle

	libreDSSP is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libreDSSP is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with libreDSSP.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STACK_H
#define STACK_H

typedef struct variable variable;
typedef struct word word;
typedef struct coreword coreword;
typedef struct subdict subdict;
typedef struct dict dict;

typedef struct stack stack;

struct stack
{
	int capacity;
	int top;
	int * array;
};

stack * newStack();
int top(stack * stack);
int pop(stack * stack);
void push(stack * stack, int value);
void grow(stack * stack);

#endif
