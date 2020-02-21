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

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "util.h"
#include "dict.h"
#include "stack.h"

void textPrint(char * text);
int isNum(char * foo);
void word_next(stack * stack, cmdbuffer * cmdbuf, dict * vocab);
void word_enter(stack * stack, cmdbuffer * cmdbuf, dict * vocab);
void word_exit(stack * stack, cmdbuffer * cmdbuf, dict * vocab);
void commandParse(char * line, cmdbuffer * cmdbuf, dict * vocab);
char * prompt(int ready);
// Searches dictionaries, runs a word if possible
void wordRun(cmdbuffer * cmdbuf, stack * stack, dict * vocab);

#endif
