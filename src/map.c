/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * map.c -- Map Utility Functions.                                            *
 ******************************************************************************/

#include <math.h>
#include <curses.h>
#include "map.h"
#include "item.h"

//TODO: make map symbols configable
const char WALL   = '#';
const char FLOOR  = '.';
const char DOOR   = '+';
const char USTAIR = '>';
const char DSTAIR = '<';

COORD prev_stair_spot;

/* Macro Functions */
#define squared(x) (x * x)


/******************************************************************************
 * FUNCTION:    biggest                                                       *
 * ARGUMENTS:   double  a   -- The first value to compare.                    *
 *              double  b   -- The second value to compare.                   *
 * RETURNS:     double      -- The largest of a and b, or zero if the same.   *
 ******************************************************************************/
#define biggest( a, b ) (( a == b )? 0.0 : ( a > b )? a : b)


/******************************************************************************
 * FUNCTION:    smallest                                                      *
 * ARGUMENTS:   int     a   -- The first value to compare.                    *
 *              int     b   -- The second value to compare.                   *
 * RETURNS:     int         -- a iff it is smaller than b.                    *
 ******************************************************************************/
#define smallest( a, b ) (( a < b )? a : b)


/******************************************************************************
 * FUNCTION:    dist                                                          *
 * ARGUMENTS:   COORD     a  -- First Point                                   *
 *              COORD     b  -- Second Point                                  *
 * RETURNS:     double      -- distance between point a and b                 *
 ******************************************************************************/
static double dist( COORD a, COORD b )
{
    return sqrt( squared( a.rowy - b.rowy ) + squared( a.colx - b.colx ) );
}/* End dist Func */


/******************************************************************************
 * FUNCTION:    set_loc             Initialize a location's type              *
 * ARGUMENTS:   LOC * spot          -- The spot to initialize                 *
 *              char  type          -- The type for this spot                 *
 ******************************************************************************/
void set_loc( LOC * spot, char type  )
{
    spot->litter = NULL;
    spot->mon = NULL;
    spot->icon = type;
    spot->is_trap = false;
}/* End set_loc Func */


/******************************************************************************
 * FUNCTION:    fill                Fill an area with a certain LOC type      *
 * ARGUMENTS:   LEVEL *  l          -- The level with an area to fill         *
 *              char     t          -- The LOC type                           *
 *              RECT    c           -- The rectangular area to fill.          *
 ******************************************************************************/
#define fill_wall(x,y) fill(x,WALL,y)
#define fill_floor(x,y) fill(x,FLOOR,y)
static void fill( LEVEL * l, char t, RECT x )
{
    int i, j, s = smallest( x.a.colx, x.b.colx ),
        b[2] = { biggest( x.a.rowy, x.b.rowy ), biggest( x.a.colx, x.b.colx ) };
    /* NOTE: Biggest returns zero if it's arguments are the same. */

    for( i = smallest( x.a.rowy, x.b.rowy ); i < b[0]; i++ ) {
        for( j = s; j < b[1]; j++ ) {
            assert( i < MAX_ROW );
            assert( j < MAX_COL );
            set_loc( &l->map[i][j], t );
        }/* End j For */
    }/* End i For */
}/* End fill Func */


/******************************************************************************
 * FUNCTION:    get_map_icon         Get the map icon to display for a LOC    *
 * ARGUMENTS:   LOC     here      -- The location in question.                *
 * RETURNS:     char              -- The symobl to display for this location. *
 ******************************************************************************/
char get_map_icon( LOC here )
{

    if( here.litter != NULL ) return '&';//tempfortest
    if( here.mon != NULL )    return 'o';//tempfortest
                              return here.icon;
}/* end get_map_icon func */


/******************************************************************************
 * FUNCTION:    draw_map                                                      *
 * ARGUMENTS:   LEVEL * curlv   -- The Level to draw.                         *
 ******************************************************************************/
void draw_map( LEVEL * curlv )
{
    int r, c;
    for( r = 0; r < MAX_ROW; r++ ) for( c = 0; c < MAX_COL; c++ )
        mvaddch( r,c, get_map_icon( curlv->map[r][c] ) );
}/* End draw_map Func */


/******************************************************************************
 * FUNCTION:    roomgen                 Genearate a Room                      *
 * ARGUMENTS:   RECT * room            -- The room to be generated.           *
 *              COORD  center          -- Center the room here.               *
 * RETURNS:     bool                    True if room is within bounds.        *
 ******************************************************************************/
static bool roomgen( RECT * room, COORD center )
{
    /* Expand Room Dimensions */
    int dvert = MIN_HUT_HGT + rng(MAX_HUT_HGT),
        dhorz = MIN_HUT_WID + rng(MAX_HUT_WID);

    room->a.rowy = center.rowy - dvert;
    room->a.colx = center.colx - dhorz;
    room->b.rowy = center.rowy + dvert;
    room->b.colx = center.colx + dhorz;

    /* Don't Bump the Outer Wall */
    if( ( room->a.rowy < 2  )||( room->a.colx < 2 ) ) return false;
    if( room->b.rowy > ( MAX_ROW - 2 ) ) return false;
    if( room->b.colx > ( MAX_COL - 2 ) ) return false;
    return true;
}/* End roomgen Func  */


/******************************************************************************
 * FUNCTION:    towngen             -- Generate the town level.               *
 * ARGUMENTS:   LEVEL * outside     -- The level.                             *
 * RETURNS:     bool                -- Generation successfull                 *
 * Attempts to generate buildings MIN_HUT_DIST appart, however they may still *
 * overlap effectively generating less than n buildings. This makes them more *
 * interesting though, so it is permitted.                                    *
 ******************************************************************************/
static void towngen( LEVEL * outside, LEVEL * inside )
{
    unsigned short hut_qt = 1 + rng( MAX_HUTS );
    unsigned short i, j, z;
    COORD hutspot[MAX_HUTS];
    RECT room[MAX_HUTS];

    /* Create hut_qt Huts */
    for( i = 0; i < hut_qt; i++ ) {

        do { /* Find centers of Buildings */
            hutspot[i].rowy =  ( 2 + MIN_HUT_HGT ) + rng( MAX_ROW - ( 2 + MIN_HUT_HGT ) );
            hutspot[i].colx =  ( 2 + MIN_HUT_WID ) + rng( MAX_COL - ( 2 + MIN_HUT_WID ) );
            if( i > 0 ) for( z = 0; z < i; z++ )
                if( dist( hutspot[z], hutspot[i] ) < MIN_HUT_DIST ) continue;
        } while( !roomgen( &room[i], hutspot[i]) );

        /* Fill Buildings */
        fill_wall( outside, room[i] );
        fill_floor( inside, room[i] );

        /* Place Doors */ //inline with center//TODO:+-rng(dvert||dhorz -1)
        for( j = 0, z = rng(4) ; j < 4; j++ ) { /* Try Up to Each Direction NSEW *///TODO: Perhaps try only once
            if( ++z > WEST ) z = NORTH;
            switch( z ) {
                case NORTH:
                    hutspot[i].rowy = room[i].a.rowy;
                    if( outside->map[hutspot[i].rowy-1][hutspot[i].colx].icon == WALL )
                        continue;
                    break;
                case SOUTH:
                    hutspot[i].rowy = room[i].b.rowy - 1;
                    if( outside->map[room[i].b.rowy][hutspot[i].colx].icon == WALL )
                        continue;
                    break;
                case EAST:
                    hutspot[i].colx = room[i].b.colx - 1;
                    if( outside->map[hutspot[i].rowy-1][room[i].b.colx].icon == WALL )
                        continue;
                    break;
                case WEST:
                    hutspot[i].colx = room[i].a.colx;
                    if( outside->map[hutspot[i].rowy][hutspot[i].colx-1].icon == WALL )
                        continue;
                    break;
            }/* End cardinal direction Switch */

            /* Set Building Enterance Flags */
            set_loc( &outside->map[hutspot[i].rowy][hutspot[i].colx], DOOR );
            if( inside->map[hutspot[i].rowy-1][hutspot[i].colx].icon == WALL )      /* North */
                set_loc( &inside->map[hutspot[i].rowy-1][hutspot[i].colx], DOOR );
            else if( inside->map[hutspot[i].rowy+1][hutspot[i].colx].icon == WALL ) /* South */
                set_loc( &inside->map[hutspot[i].rowy+1][hutspot[i].colx], DOOR );
            else if( inside->map[hutspot[i].rowy][hutspot[i].colx+1].icon == WALL ) /* East */
                set_loc( &inside->map[hutspot[i].rowy][hutspot[i].colx+1], DOOR );
            else if( inside->map[hutspot[i].rowy][hutspot[i].colx-1].icon == WALL ) /* West */
                set_loc( &inside->map[hutspot[i].rowy][hutspot[i].colx-1], DOOR );
            break;
        }/* End NSEW For */
    }/* End hut_qt For */

    /* Place Decending Stair */
    i = rng( MAX_ROW ); j = rng( MAX_COL );
    if( outside->map[i][j].icon == WALL ) set_loc( &inside->map[i][j], DSTAIR );
    else set_loc( &outside->map[i][j], DSTAIR );
    prev_stair_spot.rowy = i; prev_stair_spot.colx = j;

}/* End towngen Func */


/******************************************************************************
 * FUNCTION:    dungen                   Generate the Dungeon Maps            *
 * ARGUMENTS:   LEVEL    *     curlv       -- The Array of Levels             *
 *              PLAYER   *     npc         -- The Array of NPCs (initialized) *
 ******************************************************************************/
bool dungen( LEVEL * curlv, PLAYER * npc )
{
    int i;
    static const COORD TOP_LEFT = { 0, 0 };
    static const COORD BTM_RIGHT = { MAX_ROW, MAX_COL };
    RECT room = { TOP_LEFT, BTM_RIGHT }; /* Full Map */

    /* Initialize Maps */
    for( i = 0; i < MAX_MAPS; i++ ) {
        /* Keep Track of Current Level and Position in array */
        curlv[i].type = i;
        curlv[i].is_new = true; /* Initially Unexplored */
        if( i == HVILLAGE ) fill_floor( &curlv[i], room );
        else  fill_wall( &curlv[i], room );
    }/* Init Maps For */

    /* Generate Town */
    towngen( &curlv[HVILLAGE], &curlv[IN_HHUTS] );

    /*** Generate Dungeons & Castle ***/

    /* Dungeon Starts in a Room */
    while( !roomgen( &room, prev_stair_spot) );
    fill_floor( &curlv[CASL_DN0], room );

    /* TODO: Link Stairs */

    /* TODO: Generate More Rooms and Paths */

    /* TODO: Place NPCs, traps, ... */

    return true;
}/* End dungen Func */



/************************************EOF***************************************/
