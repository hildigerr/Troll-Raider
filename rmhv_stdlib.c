/******************************************************************************
 *                             rmhv_stdlib.c                                  *
 * Created by:      Roberto Morrel HildigerR Vergaray                         *
 ******************************************************************************/

#include "rmhv_stdlib.h"


/******************************************************************************
 * Function:    _ERROR                                                        *
 * Arguments:   const char* who       String descriptor of failed function    *
 *              const char* barf      String message to dump to stderr        *
 *              int         status    Exit status return code                 *
 *              bool        debug     if true display as non-error            *
 * RETURNS:     int                   The value of status input               *
 * Sends a string to stderr in the form:                                      *
 *                  "\nERROR: who: barf [status]\n\n"                         *
 * If status is 0 then [status] will be ommited from the message.             *
 * If who is NULL then it will be ommited from the message as well.           *
 * If debug is true then the form will become: "\nwho: barf [status]\n\n"     *
 * These options can be combined as needed.                                   *
 * Some common combinations are provided as macros.                           *
 * Suggested Usage:                                                           *
 *      exit( ERROR( who, "Your error message", status ) );                   *
 ******************************************************************************/
int _ERROR( const char * who, const char * barf, int status, bool debug )
{
    fprintf( stderr, "\n%s",  (debug)? "":"ERROR: ");
    if( who )  fprintf( stderr, "%s: ", who );
    if( barf ) fprintf( stderr, "%s ", barf );
    (status)?  fprintf( stderr, "[%d]\n\n", status ): fprintf( stderr, "\n\n" );
    return status;
}/* end _ERROR func */


/******************************************************************************
 * FUNCTION:        stricpy                                                   *
 * ARGUMENTS:       char*       dest      Destination string                  *
 *                  const char* src       Source string                       *
 *                  size_t      n         max qt of chars that fit in dest    *
 * RETURNS:         size_t                i == qt of copied characters        *
 * Pretty much identical to the simple implementation of strncpy() example    *
 * shown in man page. I changed the return value, added more robust parameter *
 * checking, and only assign one null string terminator.                      *
 * WARNING: If dest is completely filled it will not be null terminated.      *
 ******************************************************************************/
size_t stricpy( char * dest, const char * src, size_t n )
{
    size_t i; /* Itteration Counter */

    assert( src && dest );

    for( i = 0; ( i < n )&&( src[i] != '\0' ); i++ ) dest[i] = src[i];
    if( i < n ) dest[i] = '\0';

    return i;

}/* End stricpy Func */


/******************************************************************************
 * FUNCTION:    mypause                                                       *
 ******************************************************************************/
void mypause( bool display_prompt )
{
    if( display_prompt ) printf( "Press [Enter] to continue . . ." );
    fflush( stdout );
//    fpurge( stdin );
    getchar();
}/* End mypause Func */



/************************************EOF***************************************/
