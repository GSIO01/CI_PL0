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


#include "code_gen.h"
#include "code_fact.h"
#include "parser.h"
#include "code.h"
#include "parser_error.h"

int fa1(void) {
	constant_p c = searchConst(morph.value.numb);
	
	DEBUG_PRINT("fa1");
	
	if(c == NULL) {
		c = createConst(morph.value.numb);
	}
	
	
	if(code(puConst, c->index) == FAIL) {
		error(ERR_CODEGEN, NULL);
		return FAIL;
	}
	
	return OK;
}

int fa2(void) {
	ident_p b = searchIdentGlobal(morph.value.string);
	
	DEBUG_PRINT("fa2");
	
	if (b == NULL) {
		error(ERR_UKN_IDENT, NULL);
		return FAIL;
	}

	if(b->type == btype_Const) {
		constant_p c = b->object;
		return code(puConst, c->index);
	}
	
	if(b->type == btype_Var) {
		variable_p v;
		
		if(b->procIndex == 0) {
			v = b->object;
			
			return code(puValVrMain, v->displacement);
		}
		
		if(b->procIndex == currentProcedure->index) {
			v = b->object;
			
			return code(puValVrLocl, v->displacement);
		}
		
		v = b->object;
		return code(puValVrGlob, v->displacement, b->procIndex);
	}
	
	error(ERR_EXP_CONSTVAR, NULL);
	return FAIL;
}
