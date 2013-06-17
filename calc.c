/* $Id: calc.c,v 1.3 2013/05/06 05:04:01 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * calc.c -- My Math Functions.                                               *
 ******************************************************************************/

#include "calc.h"

/******************************************************************************
 * FUNCTION:    biggest                                                       *
 * ARGUMENTS:   double  a   -- The first value to compare.                    *
 *              double  b   -- The second value to compare.                   *
 * RETURNS:     double      -- The largest of a and b, or zero if the same.   *
 ******************************************************************************/
inline double biggest( double a, double b )
{
	return ( a == b )? 0.0 : ( a > b )? a : b;
}/* End biggest Func */


/******************************************************************************
 * FUNCTION:    dist                                                          *
 * ARGUMENTS:   int     ar  -- Point a Row                                    *
 *              int     br  -- Point b Row                                    *
 *              int     ac  -- Point a Col                                    *
 *              int     bc  -- Point b Col                                    *
 * RETURNS:     double      -- distance between point a and b                 *
 ******************************************************************************/
inline double dist( int ar, int br, int ac, int bc )
{
	return sqrt( squared( ar - br ) + squared( ac - bc ) );
}/* end dist func */


/******************************************************************************
 * FUNCTION:    rng     The Random Number God                                 *
 * ARGUMENTS:   int     max_die_face                                          *
 * RETURNS:     int     random int 1 to max_die_face                          *
 ******************************************************************************/
inline int rng( int max_die_face )
{
	return 1 + ( rand() % max_die_face );
}/* End rng Func */


/******************************************************************************
 * FUNCTION:    smallest                                                      *
 * ARGUMENTS:   int     a   -- The first value to compare.                    *
 *              int     b   -- The second value to compare.                   *
 * RETURNS:     int         -- a iff it is smaller than b.                    *
 ******************************************************************************/
inline int smallest( int a, int b )
{
	return ( a < b )? a : b;
}/* End smallest Func */



/************************************EOF***************************************/
