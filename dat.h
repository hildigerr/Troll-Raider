/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * dat.h -- Load Data from Files                                              *
 ******************************************************************************/
#pragma once

#define QT_TYPES 0
#define QT_LINES 1
typedef struct {
    int qt[2];      /* The length of max and lines.                  */
    int * max;      /* qt[QT_LINES] == sum of each element of max.   */
    char ** lines;  /* One raw line for each entery in the datafile. */
} DATA;

/******************************************************************************
 * FUNCTION:    load_data               -- Load data from a file.             *
 * ARGUMENTS:   const char * file       -- The name of the data file.         *
 * RETURNS:     DATA *                  -- Newly allocated data from file.    *
 * WARNING: Data contents must be freed before freeing the DATA item itself.  *
 ******************************************************************************/
DATA * load_data( const char * file );


/************************************EOF***************************************/
