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

#ifndef CI_PL0_namelist_h
#define CI_PL0_namelist_h

#include "list.h"

/**
 Diffrent tpyes for for bez_t.
 */
typedef enum btype_e {
	btype_Bez,
	btype_Prc,
	btype_Const,
	btype_Var,
	bytpe_Labl
} btype_t;

/**
 * Ident type.
 */
typedef struct ident_s ident_t;
/**
 * Structure describing an ident.
 */
struct ident_s {
	btype_t	type;		/**< Type of the ident. */
	short	procIndex;	/**< Index of procedure this ident belongs to. */
	void*	object;		/**< The acutal object. */
	char*	name;		/**< The name of the ident. */
	int		line;		/**< Lines this ident was found on. */
	int		column;		/**< Column this ident was found. */
};
/**
 * Ident pointer.
 */
typedef ident_t * ident_p;

/**
 * Variable type.
 */
typedef struct variable_s variable_t;
/**
 * Structure describing a variable.
 */
struct variable_s
{
	btype_t type;			/**< Type */
	int		displacement;	/**< Relative displacement */
};
/**
 * Variable pointer.
 */
typedef variable_t * variable_p;

/**
 * Constant type.
 */
typedef struct constant_s constant_t;
/**
 * Structure describing a constant.
 */
struct constant_s
{
	btype_t		type;	/**< Type. */
	long		value;	/**< Value. */
	int			index;  /**< Constant index. */
};
/**
 * Constant pointer.
 */
typedef constant_t * constant_p;

/**
 * Procedure type.
 */
typedef struct procedure_s procedure_t;
/**
 * Procedure pointer.
 */
typedef procedure_t * procedure_p;
/**
 * Structure describing a procedure.
 */
struct procedure_s
{
	btype_t			type;			/**< Type */
	short			index;			/**< Procedure index. */
	procedure_p		parent;			/**< Parant procedure. */
	list_p			identList;		/**< Ident list. */
	int				spzzVar;		/**< Relative variable displacement var. */
};

extern procedure_p currentProcedure;
extern int procedureIndex;
extern int constantIndex;
extern list_p constantList;

/**
 * Destroy an ident.
 */
void destroyIdent(ident_p b);

/**
 * Destroy a constant.
 */
void destroyConst(constant_p c);

/**
 * Destroy a variable.
 */
void destroyVar(variable_p v);

/**
 * Destroy a procedure.
 */
void destroyProc(procedure_p p);

/**
 * Create a new ident.
 */
ident_p	createIdent(char* ident);

/**
 * Create a new constant.
 */
constant_p createConst(long val);

/**
 * Create a new variable.
 */
variable_p createVar(void);

/**
 * Create a new procedure.
 */
procedure_p createProc(procedure_p parent);

/**
 * Search for a constant.
 */
constant_p searchConst(long val);

/**
 * Search for an ident.
 */
ident_p	searchIdent(procedure_p Proc,char* ident);

/**
 * Search globally for an ident.
 */
ident_p	searchIdentGlobal(char* ident);

#endif
