/******************************************************************************
 *                             rmhv_stdlib.c                                  *
 * Created by:      Roberto Morrel HildigerR Vergaray <moonsdad@gmail.com>    *
 ******************************************************************************/

#include "rmhv_stdlib.h"


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
}/* end ERROR func */


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
size_t stricpy( char* dest, const char* src, size_t n )
{
    size_t i; /* Itteration Counter */

    if( src && dest ) {
        for( i = 0; ( i < n )&&( src[i] != '\0' ); i++ ) dest[i] = src[i];
        if( i < n ) dest[i] = '\0'; 
        
        return i;
    } else return 0;
}/* End stricpy Func */


/******************************************************************************
 * FUNCTION:    mypause                                                       *
 ******************************************************************************/
inline void mypause( int display_prompt ) 
{
    if( display_prompt )
        printf( "Press [Enter] to continue . . ." );
    fflush( stdout );
//    fpurge( stdin );
    getchar();
}/* End mypause Func */



/************************************EOF***************************************/
