/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * map.h -- Map Utility Functions.                                            *
 ******************************************************************************/
 #pragma once

/* Local Header */
#include "calc.h"
#include "types.h"

/* Map Generation Macros */
#define ACTIVE_LOCATION    curlv[pc.maplv].map[r][c]

/******************************************************************************
 * FUNCTION:    set_loc
 * ARGUMENTS:   char  t
 *              LOC * s
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool set_loc( char t, LOC * s );


/******************************************************************************
 * FUNCTION:    fill
 * ARGUMENTS:   char     t
 *              LEVEL *  l
 *              COORD    d
 *              COORD    c
 * RETURNS:     int
 * WARNING:
 * NOTE://map, door row, door col, corner row, corner column
 ******************************************************************************/
bool fill( char t, LEVEL * l, COORD d, COORD c );
#define fill_wall(x,y,z) fill('w', x,y,z)
#define fill_floor(x,y,z) fill('.', x,y,z)

/******************************************************************************
 * FUNCTION:    init_lv
 * ARGUMENTS:   LEVEL *  l
 *              short    t
 * WARNING:
 * NOTE:
 ******************************************************************************/
void init_lv( LEVEL * l, short t );

/******************************************************************************
 * FUNCTION:    get_map_icon
 * ARGUMENTS:   LOC here
 * RETURNS:     char
 * WARNING:
 * NOTE:
 ******************************************************************************/
char get_map_icon(LOC here);


/******************************************************************************
 * FUNCTION:    buildgen
 * ARGUMENTS:   LEVEL *  outside
 *              LEVEL *  inside
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool buildgen( LEVEL * outside, LEVEL * inside );

/******************************************************************************
 * FUNCTION:
 * ARGUMENTS:
 * RETURNS:
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool towngen( LEVEL * l, unsigned short n );



/************************************EOF***************************************/
