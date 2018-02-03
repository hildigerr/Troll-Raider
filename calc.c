/* $Id: calc.c,v 1.5 2014/01/13 08:50:49 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * calc.c -- My Math Functions.                                               *
 ******************************************************************************/

#include "calc.h"
#include <math.h>

/******************************************************************************
 * FUNCTION:    biggest                                                       *
 * ARGUMENTS:   double  a   -- The first value to compare.                    *
 *              double  b   -- The second value to compare.                   *
 * RETURNS:     double      -- The largest of a and b, or zero if the same.   *
 ******************************************************************************/
double biggest( double a, double b )
{
    return ( a == b )? 0.0 : ( a > b )? a : b;
}/* End biggest Func */


/******************************************************************************
 * FUNCTION:    dist                                                          *
 * ARGUMENTS:   COORD     a  -- First Point                                   *
 *              COORD     b  -- Second Point                                  *
 * RETURNS:     double      -- distance between point a and b                 *
 ******************************************************************************/
double dist( COORD a, COORD b )
{
    return sqrt( squared( a.rowy - b.rowy ) + squared( a.colx - b.colx ) );
}/* End dist Func */


/******************************************************************************
 * FUNCTION:    rng     The Random Number God                                 *
 * ARGUMENTS:   int     max_die_face                                          *
 * RETURNS:     int     random int 1 to max_die_face                          *
 ******************************************************************************/
int rng( int max_die_face )
{
    return 1 + ( rand() % max_die_face );
}/* End rng Func */


/******************************************************************************
 * FUNCTION:    smallest                                                      *
 * ARGUMENTS:   int     a   -- The first value to compare.                    *
 *              int     b   -- The second value to compare.                   *
 * RETURNS:     int         -- a iff it is smaller than b.                    *
 ******************************************************************************/
int smallest( int a, int b )
{
    return ( a < b )? a : b;
}/* End smallest Func */



/************************************EOF***************************************/
