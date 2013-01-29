#ifndef _lex_h
#define _lex_h

#include <stdlib.h>
#include <stdio.h>

#include "common.h"

typedef enum fx_e {
	ifl   = 0x0 ,
	ifb   = 0x10,
	ifgl  = 0x20,
	ifsl  = 0x30,
	ifslb = 0x40
} fx_t;

/**
 * Morphem codes 
 */
typedef enum mc_e {
	mcEmpty,
	mcSymb,
	mcNumb,
	mcIdent,
	mcStrng
} mc_t;

/**
 * Multi char symbols.
 */
typedef enum zs_e
{
	zNIL,
	zErg = 128,
	zle,
	zge,
	zBGN,
	zCLL,
	zCST,
	zDO,
	zEND,
	zIF,
	zODD,
	zPRC,
	zTHN,
	zVAR,
	zWHL
} zs_t;

/**
 * Type for morphem
 */
typedef struct morph_s morph_t;
/**
 * Structure for Morphem
 */
struct morph_s
{
	mc_t  mc;	/**< Morphem code */
	int	  line;	/**< line         */
	int   col;	/**< column       */
	union val_u
	{
		long  numb;
		char* string;
		int   symbol;
	} value;
	int  len;   /**< morphem length */
};

/**
 * Type for keyword table.
 */
typedef struct keywordTable_s keywordTable_t;
/**
 * Structure for keyword hash table.
 */
struct keywordTable_s {
	char *text;
	zs_t  zs;
};

/**
 * Init lexical analysis.
 */
int initLex(char* fname);

/**
 * Lexical analysis.
 */
morph_t* lex(void);

extern morph_t morph, morphInit;
extern int line, col;

#endif /* _lex_h */
