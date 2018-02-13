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
 * FUNCTION:    fill                Fill an area with a certain LOC type      *
 * ARGUMENTS:   LEVEL *  l          -- The level with an area to fill         *
 *              char     t          -- The LOC type                           *
 *              COORD    d,c        -- Two opposite corners defining the area *
 * RETURNS:     bool                Fails if area exeeds bounds of a map.     *
 * NOTE: XXX d and c use to stand for door and corner.                        *
 ******************************************************************************/
bool fill( LEVEL * l, char t, COORD d, COORD c );
#define fill_wall(x,y,z) fill(x,WALL,y,z)
#define fill_floor(x,y,z) fill(x,FLOOR,y,z)


/******************************************************************************
 * FUNCTION:    init_lv                    Initialize Level                   *
 * ARGUMENTS:   LEVEL *  l          -- The level being initialized            *
 *              short    t          -- the type of level                      *
 ******************************************************************************/
void init_lv( LEVEL * l, short t );


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
 * FUNCTION:    buildgen                -- Generate building interiors        *
 * ARGUMENTS:   LEVEL *  outside        -- The exterior level                 *
 *              LEVEL *  inside         -- The generated interior level       *
 ******************************************************************************/
void buildgen( LEVEL * outside, LEVEL * inside );


/******************************************************************************
 * FUNCTION:    towngen             -- Generate the town level.               *
 * ARGUMENTS:   LEVEL * l           -- The level.                             *
 *              unsigned short n    -- qt of buildings to generate.           *
 * RETURNS:     bool                -- Generation successfull                 *
 * Attempts to generate buildings MIN_HUT_DIST appart, however they may still *
 * overlap effectively generating less than n buildings. This makes them more *
 * interesting though, so it is permitted.                                    *
 ******************************************************************************/
bool towngen( LEVEL * l, unsigned short n );



/************************************EOF***************************************/
