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


#include "parser.h"
#include "code_gen.h"
#include <stdarg.h>
#include <stdlib.h>
#include "common.h"

int code(tCode Code,...) {
	va_list ap;
	short sarg;

	list_add(codeList, createCodeElement(Code, 0), sizeof(codeElement_t));
	va_start(ap,Code);
	switch (Code)
	{
		/* ops with 3 parms */
		case entryProc:
			sarg=va_arg(ap,int);
			list_add(codeList, createCodeElement(EndOfCode, sarg), sizeof(codeElement_t));
		/* ops with 2 parms */
		case puValVrGlob:
		case puAdrVrGlob:
			sarg=va_arg(ap,int);
			list_add(codeList, createCodeElement(EndOfCode, sarg), sizeof(codeElement_t));
		/* ops with 1 parm */
		case puValVrMain:
		case puAdrVrMain:
		case puValVrLocl:
		case puAdrVrLocl:
		case puConst:
		case jmp :
		case jnot:
		case call:
			sarg=va_arg(ap,int);
			list_add(codeList, createCodeElement(EndOfCode, sarg), sizeof(codeElement_t));
		break;
		/* no parms */
		default : break;
	}
	va_end (ap);
	return OK;
}

codeElement_p createCodeElement(tCode code, long num) {
	codeElement_p n = (codeElement_p)malloc(sizeof(codeElement_t));
	
	if(n == NULL) {
		fprintf(stderr, "Could not allocate new codeElement\n");
		exit(EXIT_FAILURE);
	}
	
	n->op = code;
	n->number = num;
	n->index = currentCodeIndex;
	currentCodeIndex++;
	
	return n;
}

void destroyCodeElement(codeElement_p e) {
	if(e != NULL) {
		free(e);
	}
}

label_p createLabel(void) {
	label_p l = (label_p)malloc(sizeof(label_t));
	
	if(l == NULL) {
		fprintf(stderr, "Could not allocate new label");
		exit(EXIT_FAILURE);
	}
	
	return l;
}

void destroyLabel(label_p l) {
	if(l != NULL) {
		free(l);
	}
}

