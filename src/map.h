/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * map.h -- Map Utility Functions.                                            *
 ******************************************************************************/
 #pragma once

#include "types.h"

/* Map Generation Macros */
#define ACTIVE_LOCATION    curlv[pc.maplv].map[r][c]

extern const char WALL;
extern const char FLOOR;
extern const char DOOR;
extern const char USTAIR;
extern const char DSTAIR;

/******************************************************************************
 * FUNCTION:    set_loc             Initialize a location's type              *
 * ARGUMENTS:   LOC * spot          -- The spot to initialize                 *
 *              char  type          -- The type for this spot                 *
 ******************************************************************************/
void set_loc( LOC * spot, char type  );


/******************************************************************************
 * FUNCTION:    get_map_icon         Get the map icon to display for a LOC    *
 * ARGUMENTS:   LOC     here      -- The location in question.                *
 * RETURNS:     char              -- The symobl to display for this location. *
 ******************************************************************************/
char get_map_icon( LOC here );


/******************************************************************************
 * FUNCTION:    draw_map                                                      *
 * ARGUMENTS:   LEVEL * curlv   -- The Level to draw.                         *
 ******************************************************************************/
void draw_map( LEVEL * curlv );


/******************************************************************************
 * FUNCTION:    dungen                   Generate the Dungeon Maps            *
 * ARGUMENTS:   LEVEL    *     curlv       -- The Array of Levels             *
 *              PLAYER   *     npc         -- The Array of NPCs (initialized) *
 ******************************************************************************/
bool dungen( LEVEL * curlv, PLAYER * npc );



/************************************EOF***************************************/
