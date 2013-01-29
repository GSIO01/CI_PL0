/*
 Copyright (c) 2013 Walter Julius Hennecke
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include "namelist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void destroyIdent(ident_p b) {
	if(b != NULL) {
		switch (b->type) {
			case btype_Bez:
				destroyIdent(b->object);
				break;
			case btype_Const:
				destroyConst(b->object);
				break;
			case btype_Prc:
				destroyProc(b->object);
				break;
			case btype_Var:
				destroyVar(b->object);
				
			default:
				break;
		}
		free(b);
	}
}

ident_p createIdent(char* ident) {	
	ident_p n =(ident_p)malloc(sizeof(ident_t));
	if(n == NULL) {
		fprintf(stderr, "Could not allocate new tBez\n");
		exit(EXIT_FAILURE);
	}
	
	n->type = btype_Bez;
	n->name = strdup(ident);
	n->procIndex = procedureIndex;
		
	return n;
}

void destroyConst(constant_p c) {
	if(c != NULL) free(c);
}

constant_p createConst(long val) {
	constant_p n;
	constant_p o;
	
	if(constantList == NULL) return NULL;
	
	o = searchConst(val);
	
	if(o != NULL) {
		return o;
	} else {
		n = (constant_p)malloc(sizeof(constant_t));
		if(n == NULL) {
			fprintf(stderr, "Could not allocate new tConst\n");
			exit(EXIT_FAILURE);
		}
		n->type = btype_Const;
		n->value = val;
		n->index = constantIndex;
		constantIndex++;
		list_add(constantList, n, sizeof(constant_t));
	}
	
	return n;
}

constant_p searchConst(long val) {
	constant_p tmp = NULL;
	list_iter_p iter;
	
	if(constantList == NULL) return NULL;
	
	iter = list_iterator(constantList, FRONT);
	if(iter == NULL) return NULL;
	
	for(tmp = list_next(iter); tmp != NULL; tmp = list_next(iter)) {
		if (tmp->value == val) {
			return tmp;
		}
	}
	
	return NULL;
}

void destroyVar(variable_p v) {
	if(v != NULL) free(v);
}

variable_p createVar(void) {
	variable_p v = (variable_p)malloc(sizeof(variable_t));
	
	if(v == NULL) {
		fprintf(stderr, "Could not allocate new tVar");
		exit(EXIT_FAILURE);
	}
	v->type = btype_Var;
	v->displacement = currentProcedure->spzzVar;
	currentProcedure->spzzVar += 4;
	
	return v;
}

void destroyProc(procedure_p p) {
	if(p != NULL) {
		if(p->identList != NULL) return;
		
		free(p);
	}
}

procedure_p createProc(procedure_p parent) {
	procedure_p n = (procedure_p)malloc(sizeof(procedure_t));
	
	if(n == NULL) {
		fprintf(stderr, "Could not allocate new tProc\n");
		exit(EXIT_FAILURE);
	}
	
	n->type = btype_Prc;
	n->parent = parent;
	n->identList = create_list();
	n->spzzVar = 0;
	n->index = ++procedureIndex;
	
	return n;
}

ident_p searchIdent(procedure_p proc, char* ident) {
	ident_p tmp = NULL;
	procedure_p p = proc;
	list_iter_p iter = NULL;
	
	iter = list_iterator(p->identList, FRONT);
	for(tmp = (ident_p)list_next(iter); tmp != NULL; tmp = (ident_p)list_next(iter)) {
		if(!strcmp(tmp->name, ident)) {
			destroy_iterator(iter);
			return tmp;
		}
	}
	
	return NULL;
}

ident_p searchIdentGlobal(char* ident) {
	ident_p tmp = NULL;
	procedure_p c;
	
	for (c = currentProcedure; c != NULL; c = c->parent) {
		tmp = searchIdent(c, ident);
		if(tmp != NULL) {
			return tmp;
		}
	}
	
	return tmp;
}
