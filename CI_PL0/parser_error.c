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


#include "parser_error.h"

void error(perror_t error, ident_p other) {
	switch (error) {
		case ERR_CONST_REDEF:
			printf("%d:%d - redefinition of CONST '%s'\n", line, col, morph.value.string);
			printf("%d:%d - previous definition was here\n", other->line, other->column);
			break;
		case ERR_VAR_REDEF:
			printf("%d:%d - redefiniton of VAR '%s'\n", line, col, morph.value.string);
			printf("%d:%d - previous definition was here\n", other->line, other->column);
			break;
		case ERR_PRC_REDEF:
			printf("%d:%d - redefinition of PROCEDURE '%s'\n", line, col, morph.value.string);
			printf("%d:%d - previous definition was here\n", other->line, other->column);
			break;
		case ERR_UKN_IDENT:
			printf("%d:%d - unknown ident '%s'\n", line, col, morph.value.string);
			break;
		case ERR_EXP_CONSTVAR:
			printf("%d:%d - got PROCEDURE '%s' expectet VAR or CONST\n", line, col, morph.value.string);
			break;
		case ERR_CONSTPRC_ASSIGN:
			printf("%d:%d - assignment to CONST or PROCURE '%s'\n", line, col, morph.value.string);
			break;
		case ERR_EXP_IDENT:
			printf("%d:%d - expected ident\n", line, col);
			break;
		case ERR_EXP_PRC:
			printf("%d:%d - expected procedure\n", line, col);
			break;
		case ERR_CODEGEN:
			printf("%d:%d - code generation failed\n", line, col);
			break;
		case ERR_UNKNOWN:
		default:
			printf("%d:%d - Unknown error\n", line, col);
			break;
	}
}