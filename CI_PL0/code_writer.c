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

#include "code_writer.h"
#include "code_gen.h"
#include "common.h"

static FILE* codeFile;

int openCodeFile(const char* name) {
	if(name == NULL) {
		fprintf(stderr, "openCodeFile - name == NULL\n");
		return FAIL;
	}
	
	codeFile = fopen(name, "wb");
	if(codeFile == NULL) {
		fprintf(stderr, "openCodeFile - Could not open file '%s'.\n", name);
		return FAIL;
	}
	
	return OK;
}

void closeCodeFile(void) {
	if(codeFile == NULL) {
		return;
	}
	
	fclose(codeFile);
}

int writeToCodeFile(codeElement_p e) {
	char t[2];
	
	if(e == NULL) {
		fprintf(stderr, "writeToCodeFile - e == NULL");
		return FAIL;
	}
	
	if(e->op == EndOfCode) {
		t[0] = (unsigned char)(e->number & 0xff);
		t[1] = (unsigned char)(e->number >> 8);
		if(fwrite(t, sizeof(char), 2, codeFile) != 2) {
			fprintf(stderr, "writeToCodeFile - writing 2 byte failed");
			return FAIL;
		}
	} else {
		t[0] = e->op;
		if(fwrite(t, sizeof(char), 1, codeFile) != 1) {
			fprintf(stderr, "writeToCodeFile - writing 1 byte failed");
			return FAIL;
		}
	}
	
	return OK;
}

int writeConstantToCodeFile(constant_p p) {
	char t[4];
	
	t[2] = (unsigned char)(0 & 0xff);
	t[3] = (unsigned char)(0 >> 8);
	t[0] = (unsigned char)(p->value & 0xff);
	t[1] = (unsigned char)(p->value >> 8);
	if(fwrite(t, sizeof(char), 4, codeFile) != 4) {
		fprintf(stderr, "writeToCodeFile - writing 4 byte failed");
		return FAIL;
	}
  
	return OK;
}
