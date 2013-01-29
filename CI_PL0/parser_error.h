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


#ifndef CI_PL0_parser_error_h
#define CI_PL0_parser_error_h

#include "parser.h"

/**
 * Error codes.
 */
typedef enum perror_e {
	ERR_CONST_REDEF,
	ERR_VAR_REDEF,
	ERR_PRC_REDEF,
	ERR_UKN_IDENT,
	ERR_EXP_CONSTVAR,
	ERR_EXP_IDENT,
	ERR_EXP_PRC,
	ERR_CONSTPRC_ASSIGN,
	ERR_CODEGEN,
	ERR_UNKNOWN
} perror_t;

/**
 * Print error message for a given error code.
 */
void error(perror_t error, ident_p other);

#endif
