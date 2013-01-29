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


#include "code_cond.h"
#include "code_gen.h"
#include "code.h"
#include "parser_error.h"

int co1(void) {
	DEBUG_PRINT("co1");
	
	if(code(odd) == FAIL) {
		error(ERR_CODEGEN, NULL);
		return FAIL;
	}
	
	return OK;
}

int co2(void) {
	DEBUG_PRINT("co2");
	
	condOp = (long)morph.value.symbol;
	
	return OK;
}

int co3(void) {
	int result = OK;
  
	DEBUG_PRINT("co3");
	
	switch (condOp) {
		case zle:
			result =  code(cmpLE);
			break;
		case zge:
			result =  code(cmpGE);
			break;
		case '#':
			result =  code(cmpNE);
			break;
		case '=':
			result =  code(cmpEQ);
			break;
		case '<':
			result =  code(cmpLT);
			break;
		case '>':
			result =  code(cmpGT);
			break;
		default:
			result =  FAIL;
	}
	
	if(result == FAIL) {
		error(ERR_CODEGEN, NULL);
		return FAIL;
	}
	
	return OK;
}