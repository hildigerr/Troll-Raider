/* $Id: util.c,v 1.2 2013/05/05 09:10:27 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * util.c -- My Utility Functions.                                            *
 ******************************************************************************/

#include "proto.h"


/******************************************************************************
 * Function:    ERROR                                                         *
 * Arguments:   const char* who       String descriptor of failed function    *
 *              const char* barf      String message to dump to stderr        *
 *              int         status    Exit status return code                 *
 *              int         boo       boolean: if true display status int     *
 * RETURNS:     int                   The value of status input               *
 * Sends a string to stderr in the form:                                      *
 *                  "\nERROR: who: barf [status]\n\n"                         *
 * If boo is set to false then [status] will be ommited from the message.     *
 * If who is NULL then it will be ommited from the message as well. These     *
 * options can be combined as needed.                                         *
 * Suggested Usage:                                                           *
 *      exit(ERROR( who, "Your error message", status , 1||0 ));              *
 ******************************************************************************/
inline int _ERROR( const char* who, const char* barf, int status, int boo )
{
    if( who ) {
        if( boo )
            fprintf( stderr, "\nERROR: %s: %s [%d]\n\n", who, barf, status );
        else fprintf( stderr, "\nERROR: %s: %s\n\n", who, barf );
    } else {
        if( boo )
            fprintf( stderr, "\nERROR: %s [%d]\n\n", barf, status );
        else fprintf( stderr, "\nERROR: %s\n\n", barf );
    } return status;
}/* End ERROR Func */


/******************************************************************************
 * FUNCTION:    mypause                                                       *
 ******************************************************************************/
void mypause ( void ) 
{ 
//  printf ( "Press [Enter] to continue . . ." );
  fflush ( stdout );
  getchar();
}/* End mypause Func */



/************************************EOF***************************************/

