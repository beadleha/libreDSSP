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
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "stack.h"
#include "elem.h"

stack * newStack() {
	stack * new_stack = malloc(sizeof(stack));
	new_stack->array = malloc(10*sizeof(int));
	new_stack->capacity = 10;
	new_stack->top = -1; // -1 indicates empty stack
	return new_stack;
}

// If stack is empty, do not use!
int top(stack * workStack) {
	assert(workStack->top > -1);
	return (workStack->array[workStack->top]);
}

// If stack is empty, do not use!
int pop(stack * stack) {
	assert(stack->top > -1);
	return (stack->array[(stack->top)--]);
}

void push(stack * stack, int value) {
	(stack->top)++;
	stack->array[stack->top] = value;
	if((stack->capacity) == ((stack->top)+1)) grow(stack);
	return;
}

void grow(stack * stack){
	stack->capacity = 2 * (stack->capacity);
	stack->array = realloc(stack->array, (stack->capacity)*sizeof(int));
}

