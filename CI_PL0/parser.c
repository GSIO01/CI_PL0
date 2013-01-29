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


#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "common.h"
#include "parser.h"
#include "graph.h"
#include "code.h"
#include "list.h"
#include "parser_error.h"
#include "code_block.h"
#include "code_stmt.h"
#include "code_term.h"
#include "code_fact.h"
#include "code_expr.h"
#include "code_term.h"
#include "code_cond.h"
#include "code_writer.h"

bow_t gExpression[], gFactor[], gTerm[], gCondition[], gStatement[], gBlock[], gProgramm[],
		gConstDeclList[], gVarDeclList[], gProcDecl[], gConstDecl[], gVarDecl[],
		gAssignmentStatement[], gConditionalStatement[], gLoopStatement[], gCompoundStatment[],
		gProcedureCall[], gInputStatement[], gOutputStatment[], gProcDeclList[];

bow_t *graphs[] = {	gProgramm, gBlock, gStatement, gCondition, gExpression, gFactor, gTerm,
					gConstDeclList, gVarDeclList, gProcDecl, gConstDecl, gVarDecl,
					gAssignmentStatement, gConditionalStatement, gLoopStatement,
					gCompoundStatment, gProcedureCall, gInputStatement, gOutputStatment,
					gProcDeclList };

char *gNames[] = {	"Program", "Block", "Statement", "Condition", "Expression", "Factor", "Term",
					"Const Declaration List", "Var Declaration List", "Procedure Declaration",
					"Const Declaration", "Var Declaration", "Assignment Statement",
					"Conditional Statement", "Loop Statement", "Compound Statement",
					"Procedure Call", "Input Statement", "Output Statement",
					"Procedure Declaration List" };

typedef enum g_e {	G_PROGRAMM, G_BLOCK, G_STATEMENT, G_CONDITION, G_EXPRESSION, G_FACTOR,
					G_TERM, G_CONSTDECLLIST, G_VARDECLLIST, G_PROCDECL, G_CONSTDECL, G_VARDECL,
					G_ASSIGNMENTSTMT, G_CONDITIONALSTMT, G_LOOPSTMT, G_COMPOUNDSTMT, G_PROCEDURECALL,
					G_INPUTSTMT, G_OUTPUTSTMT, G_PROCDECLLIST
} g_t;

bow_t gProgramm[] = 
{
/*0*/	{ BgGr,	{(ul)G_BLOCK	},	NULL,  1,  -1 },
/*1*/	{ BgSy,	{(ul)'.'		},	NULL,  2,  -1 },
/*2*/	{ BgEn,	{(ul)0			},	NULL, -1,  -1 }
};

bow_t gBlock[] = {
/*0*/	{ BgGr,	{(ul)G_CONSTDECLLIST},	NULL,  2,   1 },
/*1*/	{ BgNl,	{(ul)0				},	NULL,  2,  -1 },
/*2*/	{ BgGr,	{(ul)G_VARDECLLIST	},	NULL,  4,   3 },
/*3*/	{ BgNl,	{(ul)0				},	NULL,  4,  -1 },
/*4*/	{ BgGr,	{(ul)G_PROCDECLLIST	},	NULL,  5,  -1 },
/*5*/	{ BgNl,	{(ul)0				},	 bl6,  6,  -1 },
/*6*/	{ BgGr,	{(ul)G_STATEMENT	},	 bl5,  7,  -1 },
/*7*/	{ BgEn,	{(ul)0				},	NULL, -1,  -1 }
};

bow_t gConstDeclList[] = 
{
 /*0*/	{ BgSy,	{(ul)zCST		},	NULL,  1,  -1 },
 /*1*/	{ BgGr,	{(ul)G_CONSTDECL},	NULL,  3,  -1 },
 /*2*/	{ BgSy,	{(ul)','		},	NULL,  1,  -1 },
 /*3*/	{ BgSy, {(ul)';'		},	NULL,  4,   2 },
 /*4*/	{ BgEn, {(ul)0			},	NULL, -1,  -1 }
};

bow_t gConstDecl[] = 
{
/*0*/	{ BgMo,	{(ul)mcIdent	},	 bl1,  1,  -1 },
/*1*/	{ BgSy,	{(ul)'='		},	NULL,  2,  -1 },
/*2*/	{ BgMo,	{(ul)mcNumb		},	 bl2,  3,  -1 },
/*3*/	{ BgEn, {(ul)0			},	NULL, -1,  -1 }
};

bow_t gVarDeclList[] =
{
/*0*/	{ BgSy,	{(ul)zVAR	},	NULL,  1,  -1 },
/*1*/	{ BgGr,	{(ul)G_VARDECL	},	NULL,  3,  -1 },
/*2*/	{ BgSy,	{(ul)','	},	NULL,  1,  -1 },
/*3*/	{ BgSy,	{(ul)';'	},	NULL,  4,   2 },
/*4*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gVarDecl[] = 
{
/*0*/	{ BgMo,	{(ul)mcIdent	},	 bl3,  1,  -1 },
/*1*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gProcDeclList[] =
{
/*0*/	{ BgGr,	{(ul)G_PROCDECL	},	NULL,  0,   1 },
/*1*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 },
};

bow_t gProcDecl[] =
{
/*0*/	{ BgSy,	{(ul)zPRC	},	NULL,  1,  -1 },
/*1*/	{ BgMo,	{(ul)mcIdent	},	 bl4,  2,  -1 },
/*2*/	{ BgSy,	{(ul)';'	},	NULL,  3,  -1 },
/*3*/	{ BgGr,	{(ul)G_BLOCK	},	NULL,  4,  -1 },
/*4*/	{ BgSy,	{(ul)';'	},	NULL,  5,  -1 },
/*5*/	{ BgEn, {(ul)0		},	NULL, -1,  -1 }
};

bow_t gStatement[] =
{
/*0*/	{ BgGr,	{(ul)G_ASSIGNMENTSTMT	},	NULL,  8,   1 },
/*1*/	{ BgGr,	{(ul)G_CONDITIONALSTMT	},	NULL,  8,   2 },
/*2*/	{ BgGr,	{(ul)G_LOOPSTMT		},	NULL,  8,   3 },
/*3*/	{ BgGr,	{(ul)G_COMPOUNDSTMT	},	NULL,  8,   4 },
/*4*/	{ BgGr,	{(ul)G_PROCEDURECALL	},	NULL,  8,   5 },
/*5*/	{ BgGr,	{(ul)G_INPUTSTMT	},	NULL,  8,   6 },
/*6*/	{ BgGr,	{(ul)G_OUTPUTSTMT	},	NULL,  8,   7 },
/*7*/	{ BgNl,	{(ul)0			},	NULL,  8,  -1 },
/*8*/	{ BgEn,	{(ul)0			},	NULL, -1,  -1 }
};

bow_t gCondition[] = 
{
/*0*/	{ BgSy,	{(ul)zODD		},	NULL,  1,   2 },
/*1*/	{ BgGr,	{(ul)G_EXPRESSION	},	 co1, 10,  -1 },
/*2*/	{ BgGr,	{(ul)G_EXPRESSION	},	NULL,  3,  -1 },
/*3*/	{ BgSy,	{(ul)'='		},	 co2,  9,   4 },
/*4*/	{ BgSy,	{(ul)'#'		},	 co2,  9,   5 },
/*5*/	{ BgSy,	{(ul)'<'		},	 co2,  9,   6 },
/*6*/	{ BgSy,	{(ul)zge		},	 co2,  9,   7 },
/*7*/	{ BgSy,	{(ul)'>'		},	 co2,  9,   8 },
/*8*/	{ BgSy,	{(ul)zle		},	 co2,  9,  -1 },
/*9*/	{ BgGr, {(ul)G_EXPRESSION	},	 co3, 10,  -1 },
/*10*/	{ BgEn,	{(ul)0			},	NULL, -1,  -1 }
};

bow_t gExpression[] =
{ // TODO check graph
/*0*/	{ BgSy,	{(ul)'-'		},	NULL,  1,  2 },
/*1*/	{ BgGr,	{(ul)G_TERM		},	 ex1,  4, -1 },
/*2*/	{ BgNl,	{(ul)0			},	NULL,  3, -1 },
/*3*/	{ BgGr,	{(ul)G_TERM		},	NULL,  4, -1 },
/*4*/	{ BgSy,	{(ul)'+'		},	NULL,  5,  6 },
/*5*/	{ BgGr,	{(ul)G_TERM		},	 ex2,  4,  8 },
/*6*/	{ BgSy,	{(ul)'-'		},	NULL,  7,  8 },
/*7*/	{ BgGr,	{(ul)G_TERM		},	 ex3,  4,  8 },
/*8*/	{ BgEn,	{(ul)0			},	NULL, -1, -1 }
};

bow_t gFactor[] =
{
/*0*/	{ BgMo,	{(ul)mcNumb		}, 	 fa1,  5,   1 },
/*1*/	{ BgMo,	{(ul)mcIdent		},	 fa2,  5,   2 },
/*2*/	{ BgSy,	{(ul)'('		},	NULL,  3,  -1 },
/*3*/	{ BgGr,	{(ul)G_EXPRESSION	},	NULL,  4,  -1 },
/*4*/	{ BgSy,	{(ul)')'		},	NULL,  5,  -1 },
/*5*/	{ BgEn,	{(ul)0			},	NULL, -1,  -1 }
};

bow_t gTerm[] = 
{
/*0*/	{ BgGr,	{(ul)G_FACTOR	},	NULL,  1,  -1 },
/*1*/	{ BgSy,	{(ul)'*'	},	NULL,  2,   3 },
/*2*/	{ BgGr,	{(ul)G_FACTOR	},	 tr1,  1,  -1 },
/*3*/	{ BgSy,	{(ul)'/'	},	NULL,  4,   5 },
/*4*/	{ BgGr,	{(ul)G_FACTOR	},	 tr2,  1,  -1 },
/*5*/	{ BgNl,	{(ul)0		},	NULL,  6,  -1 },
/*6*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gAssignmentStatement[] = 
{
/*0*/	{ BgMo,	{(ul)mcIdent		},	 st1,  1,  -1 },
/*1*/	{ BgSy,	{(ul)zErg		},	NULL,  2,  -1 },
/*2*/	{ BgGr,	{(ul)G_EXPRESSION	},	 st2,  3,  -1 },
/*3*/	{ BgEn,	{(ul)0			},	NULL, -1,  -1 }
};

bow_t gConditionalStatement[] =
{
/*0*/	{ BgSy,	{(ul)zIF	},	NULL,  1,  -1 },
/*1*/	{ BgGr,	{(ul)G_CONDITION},	 st3,  2,  -1 },
/*2*/	{ BgSy,	{(ul)zTHN	},	NULL,  3,  -1 },
/*3*/	{ BgGr,	{(ul)G_STATEMENT},	 st4,  4,  -1 },
/*4*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gLoopStatement[] = 
{
/*0*/	{ BgSy,	{(ul)zWHL	},	 st5,  1,  -1 },
/*1*/	{ BgGr,	{(ul)G_CONDITION},	 st6,  2,  -1 },
/*2*/	{ BgSy,	{(ul)zDO	},	NULL,  3,  -1 },
/*3*/	{ BgGr,	{(ul)G_STATEMENT},	 st7,  4,  -1 },
/*4*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gCompoundStatment[] = 
{  
/*0*/	{ BgSy,	{(ul)zBGN	},	NULL,  1,  -1 },
/*1*/	{ BgGr,	{(ul)G_STATEMENT},	NULL,  2,  -1 },
/*2*/	{ BgSy,	{(ul)';'	},	NULL,  1,   3 },
/*3*/	{ BgSy,	{(ul)zEND	},	NULL,  4,  -1 },
/*4*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gProcedureCall[] =
{
/*0*/	{ BgSy,	{(ul)zCLL	},	NULL,  1,  -1 },
/*1*/	{ BgMo,	{(ul)mcIdent	},	 st8,  2,  -1 },
/*2*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gInputStatement[] =
{
/*0*/	{ BgSy,	{(ul)'?'	},	NULL,  1,  -1 },
/*1*/	{ BgMo,	{(ul)mcIdent	},	 st9,  2,  -1 },
/*2*/	{ BgEn,	{(ul)0		},	NULL, -1,  -1 }
};

bow_t gOutputStatment[] =
{
/*0*/	{ BgSy,	{(ul)'!'		},	NULL,  1,  -1 },
/*1*/	{ BgGr,	{(ul)G_EXPRESSION	},	st10,  2,  -1 },
/*2*/	{ BgEn,	{(ul)0			},	NULL, -1,  -1 }
};

void initPars(void) {
	constantIndex = 0;
	procedureIndex = 0;
	currentProcedure = &mainProgramm;
	mainProgramm.index = procedureIndex;
	mainProgramm.type = btype_Prc;
	mainProgramm.identList = create_list();
	if(mainProgramm.identList == NULL) {
		fprintf(stderr, "Could not create new list");
		exit(EXIT_FAILURE);
	}
	mainProgramm.parent = NULL;
	mainProgramm.spzzVar = 0;
	constantList = create_list();
	if(constantList == NULL) {
		fprintf(stderr, "Could not create new list");
		exit(EXIT_FAILURE);
	}
	codeList = create_list();
	if(codeList == NULL) {
		fprintf(stderr, "Could not create new list");
		exit(EXIT_FAILURE);
	}
	labelStack = create_list();
	if(labelStack == NULL) {
		fprintf(stderr, "Could not create new list");
		exit(EXIT_FAILURE);
	}

}

int pars(int pGraph) {
  bow_t* pBog = graphs[pGraph];
  int succ = FAIL, TokenEaten = 0;

  if (morph.mc == mcEmpty) lex();
  
  while(1) {
    switch(pBog->desc & (BgNl+BgSy+BgMo+BgGr+BgEn))
    {
	case BgNl: 
		succ = OK;
		break;
	case BgSy: 
		succ = (morph.value.symbol == pBog->data.symbol);
		break;
	case BgMo: 
		succ = (morph.mc == (mc_t)pBog->data.morphem);
		break;
	case BgGr:
		succ = pars(pBog->data.graph);
		break;
	case BgEn:
		return 1;   /* Ende erreichet - Erfolg */
    }
	  if(succ == ERROR) {
	    return ERROR;
	  }
	  if(succ == OK && pBog->action != NULL) {
		  succ = pBog->action();
	  }
	  if(succ == FAIL) { /* Alternativbogen probieren */
		  if (pBog->alt != -1) {
			  pBog=graphs[pGraph]+pBog->alt;
		  } else if(TokenEaten) {
			  return ERROR;
		  } else {
			  return FAIL;
		  }
	  } else { /* Morphem formal akzeptiert */
		  if (pBog->desc & BgSy || pBog->desc & BgMo) {
			  lex(); TokenEaten++;
		  }
		  pBog=graphs[pGraph]+pBog->next;
	  }
	} /* while */
}

int main(int argc, char *argv[]) {
	list_iter_p iter;
	codeElement_p codeElement;
	codeElement_t helper;
	constant_p c;
	char* outname;
	
	if (argc>1) { 
		if (initLex(argv[1])) { /* Open in lexer o.k.?    */
			initPars();
			switch (pars(G_PROGRAMM)) {
				case OK:
					printf("\n OK\n");
					
					outname = strdup(argv[1]);
					outname[strlen(outname)-3] = 'c';
					
					if(openCodeFile(outname) == FAIL) {
						exit(EXIT_FAILURE);
					}
					
					helper.op = EndOfCode;
					helper.number = procedureIndex + 1;
					writeToCodeFile(&helper);
					helper.number = 0;
					writeToCodeFile(&helper);
					
					iter = list_iterator(codeList, FRONT);
					for(codeElement = list_next(iter); codeElement != NULL; codeElement = list_next(iter)) {
						writeToCodeFile(codeElement);
					}
					destroy_iterator(iter);
					
					iter = list_iterator(constantList, FRONT);
					for(c = list_next(iter); c != NULL; c = list_next(iter)) {
						writeConstantToCodeFile(c);
					}
					
					closeCodeFile();
					break;
				case FAIL:
					printf("\n FAIL\n");
				case ERROR:
					printf("\n ERROR\n");
				default:
					break;
			}
		}
	} else {
		printf("Usage: %s <quelle>\n", argv[0]);
	}
  return EXIT_SUCCESS;
}

