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


#include "code_block.h"
#include "code_gen.h"
#include "code.h"
#include "parser.h"
#include "parser_error.h"

int bl1(void) {
	ident_p b = searchIdent(currentProcedure, morph.value.string);
	
	DEBUG_PRINT("bl1");
	
	if(b != NULL) {
		error(ERR_CONST_REDEF, b);
		return FAIL;
	} else {
		b = createIdent(morph.value.string
						);
		b->type = btype_Const;
		b->line = line;
		b->column = col;
		list_add(currentProcedure->identList, b, sizeof(ident_t));
	}
	
	return OK;
}

int bl2(void) {
	constant_p o;
	list_iter_p iter = list_iterator(constantList, FRONT);
	
	DEBUG_PRINT("bl2");
	
	ident_p b;
	constant_p c = createConst(morph.value.numb);
	b = (ident_p)list_last(currentProcedure->identList);
	b->object = c;
	
	for(o = (constant_p)list_next(iter); o != NULL; o = (constant_p)list_next(iter)) {
		if (o->value == morph.value.numb) {
			c->index = o->index;
		}
	}
	
	return OK;
}

int bl3(void) {
	ident_p b = searchIdent(currentProcedure, morph.value.string);
	variable_p v;
	
	DEBUG_PRINT("bl3");
	
	if(b != NULL) {
		error(ERR_VAR_REDEF, b);
		return FAIL;
	} else {
		b = createIdent(morph.value.string);
		b->type = btype_Var;
		v = createVar();
		b->object = v;
		list_add(currentProcedure->identList, b, sizeof(ident_t));
	}
	
	return OK;
}

int bl4(void) {
	ident_p b = searchIdent(currentProcedure, morph.value.string);
	procedure_p p;
	ident_p pb;
	
	DEBUG_PRINT("bl4");
	
	if(b != NULL) {
		error(ERR_PRC_REDEF, b);
		return FAIL;
	} else {
		p = createProc(currentProcedure);
		pb = createIdent(morph.value.string);
		pb->type = btype_Prc;
		pb->object = p;
		list_add(currentProcedure->identList, pb, sizeof(ident_t));
		currentProcedure = p;
	}
	
	return OK;
}

int bl5(void) {
	label_p l = (label_p)list_pop(labelStack);
	list_iter_p iter;
	codeElement_p e;
	
	DEBUG_PRINT("bl5");
	
	if(code(retProc) == FAIL) {
	  error(ERR_CODEGEN, NULL);
	  return FAIL;
	}
	
	iter = list_iterator(codeList, BACK);
	for(e = (codeElement_p)list_prev(iter); e != NULL; e = (codeElement_p)list_prev(iter)) {
		if(e->index == l->index) {
			l->element->number += 3;
			destroyLabel(l);
			break;
		}
		if(e->op != EndOfCode) {
			l->element->number++;
		} else {
			l->element->number += 2;
		}
	}
	
	destroy_list(currentProcedure->identList);
	currentProcedure = currentProcedure->parent;
	
	return OK;
}

int bl6(void) {
	label_p l = createLabel();
	list_iter_p iter;
	
	if(code(entryProc, 0, currentProcedure->index, currentProcedure->spzzVar) == FAIL) return FAIL;
	iter = list_iterator(codeList, BACK);
	list_prev(iter); list_prev(iter);
	l->element = (codeElement_p)list_prev(iter);
	l->index = l->element->index;
	list_add(labelStack, l, sizeof(label_t));
	
	return OK;
}


