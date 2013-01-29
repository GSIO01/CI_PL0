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


#ifndef CI_PL0_code_gen_h
#define CI_PL0_code_gen_h

#include "code.h"
#include "namelist.h"

/**
 * Add the specified operation to the code list.
 */
int code(tCode Code, ...);

/**
 * Code element type.
 */
typedef struct codeElement_s codeElement_t;
/**
 * Code element pointer.
 */
typedef codeElement_t * codeElement_p;
/**
 * Structure describing a code element.
 */
struct codeElement_s {
	long index;	/**< Index of the code element. */
	tCode op;	/**< op code of the code element. */
	long number;	/**< Number value of the code element. */
};

/**
 * Create a new code element 
 */
codeElement_p createCodeElement(tCode op, long num);
/**
 * Destroy a code element. 
 */
void destroyCodeElement(codeElement_p e);

/**
 * Label type.
 */
typedef struct label_s label_t;
/**
 * Label pointer.
 */
typedef label_t * label_p;
/**
 * Structure describing a label.
 */
struct label_s {
	long index;		/**< Index of the code element */
	btype_t type;		/**< Type of the code element */
	codeElement_p element;	/**< The code element. */
};

/**
 * Create a label.
 */
label_p createLabel(void);
/**
 * Destroy a label.
 */
void destroyLabel(label_p l);

long currentCodeIndex;

#endif
