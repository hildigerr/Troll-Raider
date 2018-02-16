/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * map.c -- Map Utility Functions.                                            *
 ******************************************************************************/

#include <math.h>
#include <curses.h>
#include "map.h"
#include "item.h"

#define MAX_HUTS     5
#define MAX_HUT_WID  6
#define MAX_HUT_HGT  4
#define MIN_HUT_WID  2
#define MIN_HUT_HGT  2
#define MIN_HUT_DIST 2.0
//TODO make these ^ defaults for input parameters //if!YAGNI?

//TODO: make map symbols configable
const char WALL   = '#';
const char FLOOR  = '.';
const char DOOR   = '+';
const char USTAIR = '>';
const char DSTAIR = '<';

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
 * FUNCTION:    randspot            -- Find a random spot                     *
 * RGUMENTS:   COORD * me           -- [IN/OUT] The Spot                      *
 * Generates a coord with enough space to build a room (usually).             *
 ******************************************************************************/
static void randspot( COORD * me )
{
    me->rowy = ( 1 + MIN_HUT_HGT ) + rng( MAX_ROW - ( 2 + MIN_HUT_HGT ) );
    me->colx = ( 1 + MIN_HUT_WID ) + rng( MAX_COL - ( 2 + MIN_HUT_WID ) );
}/* End randspot Func */


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
    if( ( room->a.rowy < 1  )||( room->a.colx < 1 ) ) return false;
    if( room->b.rowy > ( MAX_ROW - 2 ) ) return false;
    if( room->b.colx > ( MAX_COL - 2 ) ) return false;
    return true;
}/* End roomgen Func  */


/******************************************************************************
 * FUNCTION:    towngen             -- Generate the town level.               *
 * ARGUMENTS:   LEVEL * outside     -- [IN/OUT] The outside of the level.     *
 *              LEVEL * inside      -- [IN/OUT] The inside of the level.      *
 *              COORD * stair_spot  -- [IN/OUT] The enterance of the dungeon. *
 * RETURNS:     bool                -- Generation successfull                 *
 * Attempts to generate buildings MIN_HUT_DIST appart, however they may still *
 * overlap effectively generating less than n buildings. This makes them more *
 * interesting though, so it is permitted.                                    *
 ******************************************************************************/
static void towngen( LEVEL * outside, LEVEL * inside, COORD * stair_spot )
{
    unsigned short hut_qt = 1 + rng( MAX_HUTS );
    unsigned short i, j, z;
    COORD hutspot[MAX_HUTS];
    RECT room;

    /* Create hut_qt Huts */
    for( i = 0; i < hut_qt; i++ ) {

        do { /* Find centers of Buildings */
            randspot( &hutspot[i] );
            if( i > 0 ) for( z = 0; z < i; z++ )
                if( dist( hutspot[z], hutspot[i] ) < MIN_HUT_DIST ) continue;
        } while( !roomgen( &room, hutspot[i]) );

        /* Fill Buildings */
        fill_wall( outside, room );
        fill_floor( inside, room );

        /* Place Doors */ //inline with center//TODO:+-rng(dvert||dhorz -1)
        for( j = 0, z = rng(4) ; j < 4; j++ ) { /* Try Up to Each Direction NSEW *///TODO: Perhaps try only once
            if( ++z > WEST ) z = NORTH;
            switch( z ) {
                case NORTH:
                    hutspot[i].rowy = room.a.rowy;
                    assert( hutspot[i].rowy-1 > -1 );
                    if( outside->map[hutspot[i].rowy-1]
                                    [hutspot[i].colx].icon == WALL ) continue;
                    break;
                case SOUTH:
                    hutspot[i].rowy = room.b.rowy - 1;
                    if( outside->map[room.b.rowy]
                                    [hutspot[i].colx].icon == WALL ) continue;
                    break;
                case EAST:
                    hutspot[i].colx = room.b.colx - 1;
                    if( outside->map[hutspot[i].rowy]
                                    [room.b.colx].icon == WALL ) continue;
                    break;
                case WEST:
                    hutspot[i].colx = room.a.colx;
                    assert( hutspot[i].colx-1 > -1);
                    if( outside->map[hutspot[i].rowy]
                                    [hutspot[i].colx-1].icon == WALL ) continue;
                    break;
            }/* End cardinal direction Switch */

            /* Set Building Enterance Flags */
            set_loc( &outside->map[hutspot[i].rowy][hutspot[i].colx], DOOR );
            if( inside->map[hutspot[i].rowy-1]
                           [hutspot[i].colx].icon == WALL )      /* North */
                set_loc(&inside->map[hutspot[i].rowy-1][hutspot[i].colx], DOOR);
            else if( inside->map[hutspot[i].rowy+1]
                                [hutspot[i].colx].icon == WALL ) /* South */
                set_loc(&inside->map[hutspot[i].rowy+1][hutspot[i].colx], DOOR);
            else if( inside->map[hutspot[i].rowy]
                                [hutspot[i].colx+1].icon == WALL ) /* East */
                set_loc(&inside->map[hutspot[i].rowy][hutspot[i].colx+1], DOOR);
            else if( inside->map[hutspot[i].rowy]
                                [hutspot[i].colx-1].icon == WALL ) /* West */
                set_loc(&inside->map[hutspot[i].rowy][hutspot[i].colx-1], DOOR);
            break;
        }/* End NSEW For */
    }/* End hut_qt For */

    /* Place Decending Stair */
    randspot( stair_spot );
    if( outside->map[stair_spot->rowy][stair_spot->colx].icon == WALL )
        set_loc( &inside->map[stair_spot->rowy][stair_spot->colx], DSTAIR );
    else set_loc( &outside->map[stair_spot->rowy][stair_spot->colx], DSTAIR );

}/* End towngen Func */


/******************************************************************************
 * FUNCTION:    dungen                   Generate the Dungeon Maps            *
 * ARGUMENTS:   LEVEL    *     curlv       -- The Array of Levels             *
 *              PLAYER   *     npc         -- The Array of NPCs (initialized) *
 ******************************************************************************/
bool dungen( LEVEL * curlv, PLAYER * npc )
{
    int i;
    COORD prev_stair_spot;
    COORD top_left = { 0, 0 };
    COORD btm_right = { MAX_ROW, MAX_COL };
    RECT room = { top_left, btm_right }; /* Full Map */

    /* Initialize Maps */
    for( i = 0; i < MAX_MAPS; i++ ) {
        /* Keep Track of Current Level and Position in array */
        curlv[i].type = i;
        curlv[i].is_new = true; /* Initially Unexplored */
        if( i == HVILLAGE ) fill_floor( &curlv[i], room );
        else  fill_wall( &curlv[i], room );
    }/* Init Maps For */

    /* Generate Town */
    towngen( &curlv[HVILLAGE], &curlv[IN_HHUTS], &prev_stair_spot );

    /*** Generate Dungeons & Castle ***/

    /* Dungeon Starts in a Room */
    if( !roomgen( &room, prev_stair_spot ) ) { /* Stairs Too near Edge of Map */
        set_loc( &( curlv[CASL_DN0].map[prev_stair_spot.rowy]
                                       [prev_stair_spot.colx] ), FLOOR );
        //TODO: Perhaps generate special map with outer cooridor, else
        //      Generate hall and room, else what?
    } else fill_floor( &curlv[CASL_DN0], room );

    /* Link Stairs */
//     i = MAX_MAPS-1;                 /* From Deepest level */
//     do{ randspot( &prev_stair_spot ); }
//     while( !roomgen(&room, prev_stair_spot) );
//     fill_floor( &curlv[i], room );
//     set_loc(&(curlv[i].map[prev_stair_spot.rowy][prev_stair_spot.colx]),USTAIR);
//     for( i--; i > CASL_GRD; i-- ) {   /* Up to GRD */
// //         while( !roomgen( &room, prev_stair_spot ) );
// //         fill_floor( &curlv[i], room );
// //         set_loc( &(curlv[i].map[prev_stair_spot.rowy][prev_stair_spot.colx]), DSTAIR );
// //         do{ randspot( &prev_stair_spot ); } while( !roomgen(&room, prev_stair_spot) ); // && No DSTAIR within room
// //         fill_floor( &curlv[i], room );
// //         set_loc( &(curlv[i].map[prev_stair_spot.rowy][prev_stair_spot.colx]), USTAIR );
//     }/* End i to CASTL_GRD For */
    // TODO: Fill all with floor
    // TODO: set DSTAIR
    // TODO: Create Tower and exit
    // TODO: create USTAIR in tower
    // TODO: for each continue upward to top


    /* TODO: Generate More Rooms and Paths */

    /* TODO: Place NPCs, traps, ... */

    return true;
}/* End dungen Func */



/************************************EOF***************************************/
