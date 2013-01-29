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


#include "code_stmt.h"
#include "code_gen.h"
#include "code.h"
#include "parser_error.h"

int st1(void) {
	ident_p i = searchIdentGlobal(morph.value.string);
	variable_p v = NULL;
	
	DEBUG_PRINT("st1");
	
	if(i == NULL) {
		error(ERR_UKN_IDENT, NULL);
		return FAIL;
	} else {
		if(i->type != btype_Var) {
			error(ERR_CONSTPRC_ASSIGN, NULL);
			return FAIL;
		}
		
		v = (variable_p)i->object;
		
		if(i->procIndex == 0) {
			return code(puAdrVrMain, v->displacement);
		}
		
		if(i->procIndex == currentProcedure->index) {
			return code(puAdrVrLocl, v->displacement);
		}

		if(code(puAdrVrGlob, v->displacement, i->procIndex) == FAIL) {
			error(ERR_CODEGEN, NULL);
			return FAIL;
		}
		
		return OK;
	}
	
	error(ERR_UNKNOWN, NULL);
	return FAIL;
}

int st2(void) {
	DEBUG_PRINT("st2");
	
	if(code(storeVal) == FAIL) {
		error(ERR_CODEGEN, NULL);
		return FAIL;
	}
	
	return OK;
}

int st3(void) {
	label_p l = createLabel();
	list_iter_p iter;
	
	DEBUG_PRINT("st3");
	
	if(code(jnot, 0) == FAIL) {
		error(ERR_CODEGEN, NULL);
		return FAIL;
	}
	
	iter = list_iterator(codeList, BACK);
	l->element = list_prev(iter);
	l->index = l->element->index;
	list_add(labelStack, l, sizeof(label_t));

	destroy_iterator(iter);
	return OK;
}

int st4(void) {
	label_p l = list_pop(labelStack);
	codeElement_p ce;
	list_iter_p iter = list_iterator(codeList, BACK);
	
	DEBUG_PRINT("st4");
	
	for(ce = list_prev(iter); ce != NULL; ce = list_prev(iter)) {
		if(ce->index == l->index) {
			destroyLabel(l);
			return OK;
		}
		if(ce->op != EndOfCode) {
			l->element->number++;
		} else {
			l->element->number += 2;
		}
	}
	
	error(ERR_UNKNOWN, NULL);
	return FAIL;
}

int st9(void) {
	ident_p i = searchIdentGlobal(morph.value.string);
	variable_p v = NULL;
	
	DEBUG_PRINT("st9");
	
	if(i == NULL) {
		error(ERR_UKN_IDENT, NULL);
		return FAIL;
	} else {
		if(i->type != btype_Var) {
			error(ERR_EXP_IDENT, NULL);
			return FAIL;
		}
		
		v = (variable_p)i->object;

		if(i->procIndex == 0) {
			if(code(puAdrVrMain, v->displacement) == FAIL) {
				error(ERR_CODEGEN, NULL);
				return FAIL;
			}
			if(code(getVal) == FAIL) {
				error(ERR_CODEGEN, NULL);
				return FAIL;
			}
			
			return OK;
		}
		
		if(i->procIndex == currentProcedure->index) {
			if(code(puAdrVrLocl, v->displacement) ==  FAIL) {
				error(ERR_CODEGEN, NULL);
				return FAIL;
			}
			if(code(getVal) == FAIL) {
			      error(ERR_CODEGEN, NULL);
			      return FAIL;
			}
			
			return OK;
		}

		if(code(puAdrVrGlob, v->displacement, i->procIndex) == FAIL) { 
			error(ERR_CODEGEN, NULL);
			return FAIL;
		}
		if(code(getVal) == FAIL) {
			error(ERR_CODEGEN, NULL);
			return FAIL;
		}
		
		return OK;
	}
	
	error(ERR_UNKNOWN, NULL);
	return FAIL;
}

int st10(void) {
	DEBUG_PRINT("st10");
	
	if(code(putVal) == FAIL) {
	      error(ERR_CODEGEN, NULL);
	      return FAIL;
	}
	
	return OK;
}

int st5(void) {
	label_p l = createLabel();
	
	DEBUG_PRINT("st5");
	
	l->index = currentCodeIndex;
	list_add(labelStack, l, sizeof(label_t));
	
	return OK;
}

int st6(void) {
	label_p l = createLabel();
	list_iter_p iter;
	
	DEBUG_PRINT("st6");
	
	if(code(jnot, 0) == FAIL) {
		error(ERR_CODEGEN, NULL);
		return FAIL;
	}
	iter = list_iterator(codeList, BACK);
	l->element = list_prev(iter);
	l->index = l->element->index;
	list_add(labelStack, l, sizeof(label_t));
	
	return OK;
}

int st7(void) {
	label_p l = list_pop(labelStack);
	list_iter_p iter = list_iterator(codeList, BACK);
	codeElement_p ce;
	int i = 0;
	
	DEBUG_PRINT("st7");
	
	for(ce = list_prev(iter); ce != NULL; ce = list_prev(iter)) {
		if(ce->index == l->index) {
			l->element->number += 3;
			destroyLabel(l);
			break;
		}
		if(ce->op != EndOfCode) {
			l->element->number++;
		} else {
			l->element->number += 2;
		}
	}
	
	l = list_pop(labelStack);
	destroy_iterator(iter);
	iter = list_iterator(codeList, BACK);
	
	for(ce = list_prev(iter); ce != NULL; ce = list_prev(iter)) {
		if(ce->index == l->index) {
			if(ce->op != EndOfCode) {
			  i--;
			} else {
			  i -= 2;
			}
			i -= 3;
			destroyLabel(l);
			break;
		}
		if(ce->op != EndOfCode) {
			i--;
		} else {
			i -= 2;
		}
	}
	
	if(code(jmp, i) == FAIL) {
		error(ERR_CODEGEN, NULL);
		return FAIL;
	}
	
	return OK;
}

int st8(void) {
	ident_p i = searchIdentGlobal(morph.value.string);
	procedure_p p = NULL;
	
	DEBUG_PRINT("st8");
	
	if(i == NULL) {
		error(ERR_UKN_IDENT, NULL);
		return FAIL;
	} else {
		if(i->type != btype_Prc) {
			error(ERR_EXP_PRC, NULL);
			return FAIL;
		}
		
		p = (procedure_p)i->object;
		if(code(call, p->index) == FAIL) {
			error(ERR_CODEGEN, NULL);
			return FAIL;
		}
		
		return OK;
	}
	
	error(ERR_UNKNOWN, NULL);
	return FAIL;
}


