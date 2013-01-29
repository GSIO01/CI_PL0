#ifndef _bogen_h
#define _bogen_h

#include "common.h"

typedef enum bDesc_e {
    BgNl= 0,   /* NIL	    */
    BgSy= 1,   /* Symbol    */
    BgMo= 2,   /* Morphem   */
    BgGr= 4,   /* Graph	    */
    BgEn= 8,   /* Graph end */
} bDesc_t;

/* 
 
 * A graph is described by a vector of bows.
 * Every bow contains the index of the following bow
 * and the index of a possible alternate bow.			
 */

/**
 * Bow type.
 */
typedef struct bow_s bow_t;
/**
 * Bow pointer.
 */
typedef bow_t * bow_p;
/**
 * Structure describing the bow.
 */
struct bow_s
{
  bDesc_t desc;			/**< Short description of the bow.	*/
  union data_u			/**< Detailed description.		*/
  {
    unsigned long init;		/**< For init				*/
    int           symbol;	/**< symbol				*/
    mc_t           morphem;	/**< Morphem code			*/
    int           graph;	/**< Graph				*/
  } data;
  int (*action)(void);		/**< Action/NULL			*/
  int next;			/**< Index of following bow / -1	*/
  int alt;			/**< Index alternate bow / -1		*/
};

#endif /* _bogen_h */
