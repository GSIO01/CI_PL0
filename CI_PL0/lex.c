/****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lex.h"

static FILE*		sourceFile;			/* Sourcefile				*/
morph_t				morph, morphInit;	/* Morphem				*/
static signed char	x;					/* Current input symbol 		*/
static int			z;					/* Current state of the automatn 	*/
static char			buffer[1024+1], *pBuffer;	/* Output buffer			*/
static int			end;				/* Flag					*/
int					line, col;			/* Current line and column		*/

/**
 * code word hash table 
 */
keywordTable_t keywordTable['Z'-'A'+1][8] =
{
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{"EGIN", zBGN},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{"ALL", zCLL},		{"ONST", zCST},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{"O",	zDO },	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{"ND",	zEND},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{"F",	zIF },	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{"DD",	zODD},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,     zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{"ROCEDURE", zPRC}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L, 	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{"HEN", zTHN},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{"AR",	zVAR},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{"HILE", zWHL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
	{{0L,	zNIL},	{0L,	zNIL},		{0L,	zNIL},		{0L,	 zNIL},		{0L, zNIL},		{0L, zNIL},		{0L, zNIL},		{0L,		 zNIL}},
};

/** 
 * symbol class vector 
 */
static char symbolClasses[128]=
/*     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F      */
/*---------------------------------------------------------*/
/* 0*/{7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/* 0*/
/*10*/ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,/*10*/
/*20*/ 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,/*20*/
/*30*/ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 0, 5, 4, 6, 0,/*30*/
/*40*/ 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2, 2, 8, 2, 8, 8,/*40*/
/*50*/ 8, 2, 8, 8, 8, 8, 8, 8, 2, 2, 2, 0, 0, 0, 0, 0,/*50*/
/*60*/ 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2, 2, 8, 2, 8, 8,/*60*/
/*70*/ 8, 2, 8, 8, 8, 8, 8, 8, 2, 2, 2, 0, 0, 0, 0, 0}/*70*/;

/**
 * automatn table 
 */
static char automatnTable[][9]=
/*        So      Zi      Bu      ':'      '='    '<' 	  '>'   Space	SymZ    */
/*------------------------------------------------------------------------------*/
/* 0 */{0+ifslb,1+ifsl ,2+ifgl ,3+ifsl ,0+ifslb,4+ifsl ,5+ifsl ,0+ifl , 9+ifgl ,
/* 1 */ 0+ifb  ,1+ifsl ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb , 0+ifb  ,
/* 2 */ 0+ifb  ,2+ifsl ,2+ifgl ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb , 2+ifgl ,
/* 3 */ 0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,6+ifsl ,0+ifb  ,0+ifb  ,0+ifb , 0+ifb  ,
/* 4 */ 0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,7+ifsl ,0+ifb  ,0+ifb  ,0+ifb , 0+ifb  ,
/* 5 */ 0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,8+ifsl ,0+ifb  ,0+ifb  ,0+ifb , 0+ifb  ,
/* 6 */ 0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb , 0+ifb  ,
/* 7 */ 0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb , 0+ifb  ,
/* 8 */ 0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb , 0+ifb  ,
/* 9 */ 0+ifb  ,2+ifgl ,2+ifgl ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb  ,0+ifb , 9+ifgl 
};

/**
 * Output function of the automatn.
 */
static void fl  (void);
static void fb  (void);
static void fgl (void);
static void fsl (void);
static void fslb(void);

typedef void (*fx)(void);
static fx vfx[]={fl,fb,fgl,fsl,fslb};

/**
 * Lexical analysis.
 */
morph_t* lex(void)
{
	int Zn;
	
	morph=morphInit;
	pBuffer=buffer;
	end=0;
	do
	{
		 /* calculate followup state */
		Zn=   automatnTable[z][symbolClasses[x&0x7f]]&0xF;
		/* execute function */
		vfx[((automatnTable[z][symbolClasses[x&0x7f]]))>>4]();
		/* switch automatn */
		z=Zn;
	} while (end==0 && x != -1);
	return &morph;
}

/**
 * Initialisation of lexical analysis.
 */
int initLex(char* fname) {
	char vName[128+1];
	
	strcpy(vName,fname);
	if (strstr(vName,".pl0")==NULL) strcat(vName,".pl0");
	
	sourceFile=fopen(vName,"r+t");
	if (sourceFile!=NULL) {x=fgetc(sourceFile); return OK;}
	return FAIL;
}

/**
 * Read.
 */
static void fl  (void)
{
	x=fgetc(sourceFile);
	if (x=='\n') line++, col=0;
	else col++;
}

/**
 * Write as capital letter.
 */
static void fgl (void)
{
	*pBuffer=(char)toupper(x);
	*(++pBuffer)=0;
	fl();
}

/**
 * Read and write.
 */
static void fsl (void)
{
	*pBuffer=(char)x;
	*(++pBuffer)=0;
	fl();
}
/**
 * Read, write, and terminate.
 */
static void fslb(void)
{
	fsl();fb();
}


static void fb(void) {
	switch (z) {
		case 0:
			morph.value.symbol = buffer[0];
			if(buffer[0] == -1) {
				morph.mc = mcEmpty;
			} else {
				morph.mc = mcSymb;
			}
			break;
		case 1:
			morph.value.numb = atol(buffer);
			morph.mc = mcNumb;
			break;
		case 2:
			morph.value.string = buffer;
			morph.mc = mcIdent;
			break;
		case 3: // :
			morph.value.symbol = (long)':';
			morph.mc = mcSymb;
			break;
		case 4: // <
			morph.value.symbol = (long)'<';
			morph.mc = mcSymb;
			break;
		case 5: // >
			morph.value.symbol = (long)'>';
			morph.mc = mcSymb;
			break;
		case 6:
			morph.value.symbol = (long)zErg;
			morph.mc = mcSymb;
			break;
		case 7:
			morph.value.symbol = (long)zle;
			morph.mc = mcSymb;
			break;
		case 8:
			morph.value.symbol = (long)zge;
			morph.mc = mcSymb;
			break;
		case 9:
			if(strlen(buffer) <= 9 &&
			   keywordTable[buffer[0]-'A'][strlen(buffer)-2].zs != zNIL &&
			   strstr(buffer, keywordTable[buffer[0]-'A'][strlen(buffer)-2].text) != NULL
			   ) {
				morph.value.symbol = keywordTable[buffer[0]-'A'][strlen(buffer)-2].zs;
				morph.mc = mcSymb;
			} else {
				morph.value.string = buffer;
				morph.mc = mcIdent;
			}
			break;
			
		default:
			break;
	}
	end = 1;
}

/**
 * Main function for standalone testing.
 */
#ifdef STANDALONE
int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s <input>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	initLex(argv[1]);
	while (X != EOF)
	{
		Lex();
		if(Morph.MC != mcEmpty)
			printf("Line%4d, Col%3d: ", line, col);
		switch(Morph.MC)
		{
			case mcSymb :
				if (Morph.Val.Symb==zErg)   	printf("Symbol,:=\n");   	else
				if (Morph.Val.Symb==zle )   	printf("Symbol,<=\n");    	else
				if (Morph.Val.Symb==zge )   	printf("Symbol,>=\n");    	else
				if (Morph.Val.Symb==zBGN)   	printf("Symbol,_BEGIN\n");	else
				if (Morph.Val.Symb==zCLL)   	printf("Symbol,_CALL\n"); 	else
				if (Morph.Val.Symb==zCST)   	printf("Symbol,_CONST\n");	else
				if (Morph.Val.Symb==zDO )   	printf("Symbol,_DO\n");   	else
				if (Morph.Val.Symb==zEND)   	printf("Symbol,_END\n");  	else
				if (Morph.Val.Symb==zIF )   	printf("Symbol,_IF\n");   	else
				if (Morph.Val.Symb==zODD)   	printf("Symbol,_ODD\n");  	else
				if (Morph.Val.Symb==zPRC)   	printf("Symbol,_PROCEDURE\n");  else
				if (Morph.Val.Symb==zTHN)   	printf("Symbol,_THEN\n"); 	else
				if (Morph.Val.Symb==zVAR)   	printf("Symbol,_VAR\n");  	else
				if (Morph.Val.Symb==zWHL)   	printf("Symbol,_WHILE\n"); 	else
				if (Morph.Val.Symb==(long)':')  printf("Symbol,:\n"); 		else
				if (Morph.Val.Symb==(long)'<')	printf("Symbol,<\n"); 		else
				if (Morph.Val.Symb==(long)'>')	printf("Symbol,>");
				if (isprint(Morph.Val.Symb))printf("Symbol,%c\n",(char)Morph.Val.Symb);
				break;
			case mcNumb :
				printf("Zahl  ,%ld\n",Morph.Val.Numb);
				break;
			case mcIdent:
				printf("Ident ,%s\n",(char*)Morph.Val.pStr);
				break;
			default:
				break;
		}
	}
	
	return 0;
}
#endif
