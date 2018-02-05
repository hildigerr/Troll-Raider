
#include "rmhv_stdlib.h"

#define BUFFER_SIZE 80
#define UNIT_TESTING

char ** load_data( char * file )
{
    FILE* datafile = fopen( file, READ_ONLY );
    char  buf[BUFFER_SIZE], * line, ** data = NULL;
    char * cptr = &buf[1];
    int n = 0, qt_lines = 1, qt_types = 1, entry = 0;

    if( !datafile ) return NULL;

    while( !feof(datafile) ) {
        int i = 0; n++;
        while( ( (buf[i++] = fgetc(datafile)) != '\n' )&&( !feof(datafile) ) )
        if( i == BUFFER_SIZE ) {
            ERROR("load_data", "Datafile line greater than BUFFER_SIZE", n);
            fclose(datafile);
            return NULL;
        }/* End Read Line into Buffer While If */

        switch( buf[0] ) {

            /* Skip Blank Lines and Comments */
            case '\n': case '#': case EOF: break;

            /* Read In Maximum Items For This Type */
            case '$': {
                if( data )
                    ERROR("Warning","Ignoring extra $-line in data file.", n);
                else {
                    buf[--i] = '\0'; /* Replace Newline */
                    //BARF( buf, i );
                    if( !(line = CALLOC( i, char )) ) {
                        ERROR("load_data","Cannot allocate new line", n);
                        fclose(datafile);
                        return NULL;
                    } else stricpy( line, buf, i );
                    while( --i >= 0 ) if( buf[i] == ':' ) {
                        qt_types++;
                        buf[i] = '\0';
                    }/* End : If While */
                    //BARF("qt_types", qt_types);
                    for( i = 0; i < qt_types; i++ ) {
                        BARF( cptr, atoi(cptr) );
                        qt_lines += atoi( cptr );
                        while( *cptr != '\0' ) cptr++; cptr++;
                    }/* End qt_types For */
                    //BARF("qt_lines", qt_lines);
                    if( !(data = CALLOC( qt_lines, char* )) ) {
                        ERROR("load_data","Cannot allocate data", n);
                        fclose(datafile);
                        free( line );
                        return NULL;
                    } else data[entry++] = line;
                }/* End !data else */
            } break;

            /* Read In Entries */
            case ':': {
                if( !data ) {
                    ERROR("load_data", "Data entry before $Max spec.", n);
                    fclose(datafile);
                    return NULL;
                }/* End !data If */
                if( entry >= qt_lines ) {
                    ERROR("load_data", "Found more enteries than expected.", n);
                    //XXX Memory not freed -- but this is a fatal error anyway.
                    fclose(datafile);
                    return NULL;
                }/* End too many If */
                buf[--i] = '\0'; /* Replace Newline */
                if( !(line = CALLOC( i, char )) ) {
                    ERROR("load_data","Cannot allocate new line", n);
                    //XXX May leak memory -- but this is a fatal error anyway.
                    fclose(datafile);
                    return NULL;
                } /* End !CALLOC if */
                stricpy( line, buf, i );
                data[entry++] = line;
                //BARF( line, entry );
            } break;

            default: /* Catch Any Unexpected Input and Report it */
                ERROR( "load_data", "Unexpected Input From data file", buf[0] );
                fclose(datafile);
                return NULL;
        }/* */
    }/* end !feof while */

    fclose(datafile);
    return data;
}/* End load_data Func */

#ifdef UNIT_TESTING
int main ()
{
    char ** data = load_data( "dat/item.dat" );
    return 0;
}
#endif
