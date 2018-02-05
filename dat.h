/******************************************************************************
 *  Troll Raider
 *      by Roberto Morrel HildigerR Vergaray
 * dat.h -- Load Data from Files
 ******************************************************************************/
#pragma once

#define QT_TYPES 0
#define QT_LINES 1
typedef struct {
    int qt[2];
    int * max;
    char ** lines;
} DATA;

DATA * load_data( const char * file );


/************************************EOF***************************************/