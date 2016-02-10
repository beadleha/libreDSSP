/*	This file is part of libreDSSP.

	Copyright 2016 Alan Beadle

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

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "util.h"
#include "dict.h"
#include "elem.h"
#include "stack.h"

// This can be reduced but it hits every case
int isnum(char * foo){
	int i = 0;
	char temp = foo[0];

	if(foo == NULL) return 0;
	if(strlen(foo) == 0) return 0;

	// If first is minus, scan from beginning to see if all are digits
	if((temp=='-') && (strlen(foo) > 1)){
		for (i=1; i < (strlen(foo)); i++){
			if(!isdigit(foo[i])) return 0;
		}
	}else for (i=0; i < (strlen(foo)); i++){
		if(!isdigit(foo[i])) return 0;
	}
	return 1;
}

// Takes a linked command sequence and attempts to run it as DSSP code
void run(stack * stack, elem * seqHead, dict * vocab){
	elem * seqPrev; // We use this to free each element after we are done reading it
	elem * tempSeq;

	tempSeq = seqHead;
	do{
		assert(tempSeq != NULL);
		if(isnum(tempSeq->chars)){ // Numerical constant
			push(stack, atoi(tempSeq->chars));
		}else if (!strcmp(tempSeq->chars, ":")){ // Function declaration
			defWord(seqHead,vocab);
			// TODO We should free the entire sequence first;
			tempSeq = NULL;
		}else if (tempSeq->chars[0] == '['){ // Comment
			// Do nothing
		}else{ // Not a number or a function declaration
			wordRun(tempSeq, stack, vocab);
		}
		if(tempSeq != NULL){ // This will be NULL if we did a function declaration
			if(tempSeq->next == NULL) return; // Fixes DO loop crashes
			seqPrev = tempSeq;
			tempSeq = tempSeq->next;
			free(seqPrev); // We should be done with this element
		}
	}while(tempSeq != NULL);
	return;
}

// Takes command line and splits it by spaces, returns sequence
elem * parseInput(char * line){
	char ch;
	int i = 0;
	int j = 0;
	elem * seqHead;
	elem * seqcurr;
	elem * seqtail;

	seqHead = malloc(sizeof(elem));
	seqtail = seqHead;
	seqtail->next = NULL;

	// TODO Not safe!
	while((line[j] != '\0') && (j < 79)){
		ch = line[j++];
		if (ch == '[') {
			seqtail->chars[i++] = '[';
			while(((ch = line[j++]) != ']')){
				if(i>8) break; // TODO This limits a word or comment to 8 chars due to fixed size in struct
				seqtail->chars[i++] = ch;
			}
			seqtail->chars[i++] = ']';

		} else if(ch != ' ') {
			if(i>8) break; // TODO This limits a word or comment to 8 chars due to fixed size in struct
			seqtail->chars[i++] = ch;

		} else if ((ch == ' ') && (i != 0)) { // Handles adjacent spaces
			seqtail->chars[i] = '\0';
			seqcurr = seqtail;
			seqtail = malloc(sizeof(elem));
			seqcurr->next = seqtail; // old tail used to point to NULL, now to new elem
			seqtail->next = NULL;
			i=0;
		}
	}
	seqtail->chars[i] =  '\0';
	return seqHead;
}

char * prompt(){
	char * line = readline ("* ");
	add_history(line);
	//Check for EOF.
	if (!line){
		printf("\n");
		return "BYE";
	}
	return line;
}

// Searches dictionaries, runs a word if possible
void wordRun(elem * sequence, stack * stack, dict * vocab){
	char * elemName = sequence->chars;
	coreword * tempCore;
	word * tempWord;

	if(elemName[0] == '\0') return;

	// Search core dict first
	tempCore = vocab->core;
	do{
		if(!strcmp(tempCore->name, elemName)){
			// Run built-in function. sequence provided so that conditionals can conditionally delete adjacent words.
			tempCore->func(stack, sequence, vocab);
			return;
		}
		tempCore = tempCore->next;
	}while(tempCore != NULL);

	if(vocab->sub->wordlist != NULL){
		// Search subdicts (for now just one)
		tempWord = vocab->sub->wordlist;
		do{
			if(!strcmp(tempWord->name, elemName)){
				// run programmed word
				run(stack, parseInput(tempWord->definition), vocab);
				return;
			}
			tempWord = tempWord->next;
		}while(tempWord != NULL);
	}

	fprintf(stderr,"ERROR: %s unrecognized\n",elemName);
	return;
}


