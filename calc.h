/* $Id: calc.h,v 1.3 2014/01/13 05:42:59 moonsdad Exp $ */
/******************************************************************************
 * calc -- My Math Functions.                                                 *
 ******************************************************************************/
#pragma once

/* System Headers */
#include <stdlib.h>
#include <math.h>

/* Local Header */
#include "types.h"

/* Macro Functions */
#define squared(x) (x * x)

/******************************************************************************
 * FUNCTION:    biggest                                                       *
 * ARGUMENTS:   double  a   -- The first value to compare.                    *
 *              double  b   -- The second value to compare.                   *
 * RETURNS:     double      -- The largest of a and b, or zero if the same.   *
 ******************************************************************************/
inline double biggest( double a, double b );


/******************************************************************************
 * FUNCTION:    dist                                                          *
 * ARGUMENTS:   COORD     a  -- First Point                                   *
 *              COORD     b  -- Second Point                                  *
 * RETURNS:     double      -- distance between point a and b                 *
 ******************************************************************************/
inline double dist( COORD a, COORD b );


/******************************************************************************
 * FUNCTION:    rng     The Random Number God                                 *
 * ARGUMENTS:   int     max_die_face                                          *
 * RETURNS:     int     random int 1 to max_die_face                          *
 ******************************************************************************/
inline int rng( int max_die_face );


/******************************************************************************
 * FUNCTION:    smallest                                                      *
 * ARGUMENTS:   int     a   -- The first value to compare.                    *
 *              int     b   -- The second value to compare.                   *
 * RETURNS:     int         -- a iff it is smaller than b.                    *
 ******************************************************************************/
inline int smallest( int a, int b );


/************************************EOF***************************************/
