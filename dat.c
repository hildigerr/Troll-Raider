/******************************************************************************
 *  Troll Raider
 *      by Roberto Morrel HildigerR Vergaray
 * dat.c -- Load Data from Files
 ******************************************************************************/

#include "rmhv_stdlib.h"
#include "dat.h"

//#define UNIT_TESTING

#define BUFFER_SIZE 80


DATA * load_data( const char * file )
{
    FILE * datafile = fopen( file, READ_ONLY );
    DATA * data = MALLOC( DATA );
    char  buf[BUFFER_SIZE], * line;
    char * cptr = &buf[1];
    int n = 0, entry = 0;

    if( !datafile || !data ) return NULL;

    data->lines = NULL; data->max = NULL;
    data->qt[QT_TYPES] = 1; data->qt[QT_LINES] = 0;


    while( !feof(datafile) ) {
        int i = 0; n++;
        while( ( (buf[i++] = fgetc(datafile)) != '\n' )&&( !feof(datafile) ) )
        if( i == BUFFER_SIZE ) {
            ERROR( "load_data", "Datafile line greater than BUFFER_SIZE", n );
            fclose(datafile);
            return NULL;
        }/* End Read Line into Buffer While If */

        switch( buf[0] ) {

            /* Skip Blank Lines and Comments */
            case '\n': case '#': case EOF: break;

            /* Read In Maximum Items For This Type */
            case '$': {
                if( data->lines )
                    ERROR( "Warning","Ignoring extra $-line in data file.", n );
                else {
                    buf[--i] = '\0'; /* Replace Newline */
                    //BARF( buf, i );
                    while( --i >= 0 ) if( buf[i] == ':' ) {
                        data->qt[QT_TYPES]++;
                        buf[i] = '\0';
                    }/* End : If While */
                    //BARF( "qt_types", data->qt[QT_TYPES] );
                    if( !(data->max = CALLOC(data->qt[QT_TYPES], int) ) ) {
                        ERROR( "load_data","Cannot allocate data", n );
                        fclose(datafile);
                        free( data );
                        return NULL;
                    }/* End If */
                    for( i = 0; i < data->qt[QT_TYPES]; i++ ) {
                        data->max[i] = atoi(cptr);
                        //BARF( cptr, data->max[i] );
                        data->qt[QT_LINES] += data->max[i];
                        while( *cptr != '\0' ) cptr++; cptr++;
                    }/* End data->qt[QT_TYPES] For */
                    //BARF( "qt_lines", data->qt[QT_LINES] );
                    if( !(data->lines = CALLOC( data->qt[QT_LINES], char* )) ) {
                        ERROR( "load_data","Cannot allocate data", n );
                        fclose(datafile);
                        free( data->max );
                        free( data );
                        return NULL;
                    }/* End !CALLOC If */
                }/* End !data->lines else */
            } break;

            /* Read In Entries */
            case ':': {
                if( !data->lines ) {
                    ERROR( "load_data", "Data entry before $Max spec.", n );
                    fclose(datafile);
                    free( data );
                    return NULL;
                }/* End !data->lines If */
                if( entry >= data->qt[QT_LINES] ) {
                    ERROR("load_data", "Found more enteries than expected.", n);
                    //XXX Memory not freed -- but this is a fatal error anyway.
                    fclose(datafile);
                    free( data );
                    return NULL;
                }/* End too many If */
                buf[--i] = '\0'; /* Replace Newline */
                if( !(line = CALLOC( i, char )) ) {
                    ERROR( "load_data","Cannot allocate new line", n );
                    //XXX May leak memory -- but this is a fatal error anyway.
                    fclose(datafile);
                    free( data );
                    return NULL;
                } /* End !CALLOC if */
                stricpy( line, buf, i );
                data->lines[entry++] = line;
                //BARF( line, entry );
            } break;

            default: /* Catch Any Unexpected Input and Report it */
                ERROR( "load_data", "Unexpected Input From data file", buf[0] );
                fclose(datafile);
                free( data );
                return NULL;
        }/* */
    }/* end !feof while */

    fclose(datafile);
    return data;
}/* End load_data Func */

#ifdef UNIT_TESTING
int main ()
{
    int i,j;
    char * title[] = {  "money", "melee", "reach", "range",
                        "2hrange", "shield", "hat", "body"}, ** cptr;
    DATA * data = load_data( "dat/item.dat" );
    cptr = data->lines;

    for( i=0; i < data->qt[QT_TYPES]; i++ ) {
        printf("%s\n", title[i]);
        for( j=0; j < data->max[i]; j++ ) {
            printf( "%s\n", *cptr );
            ++cptr;
        }/* end max[i] for */
        printf("\n");
    }/* end QT_TYPES for */
    return 0;
}
#endif
