/* $Id: calc.h,v 1.2 2013/05/06 05:04:01 moonsdad Exp $ */
/******************************************************************************
 * calc -- My Math Functions.                                                 *
 ******************************************************************************/
#pragma once

/* System Headers */
#include <stdlib.h>
#include <math.h>

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
 * ARGUMENTS:   int     ar  -- Point a Row                                    *
 *              int     br  -- Point b Row                                    *
 *              int     ac  -- Point a Col                                    *
 *              int     bc  -- Point b Col                                    *
 * RETURNS:     double      -- distance between point a and b                 *
 ******************************************************************************/
inline double dist( int ar, int br, int ac, int bc );


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
