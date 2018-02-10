/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * map.c -- Map Utility Functions.                                            *
 ******************************************************************************/

#include <math.h>
#include <curses.h>
#include "map.h"
#include "item.h"

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
 * FUNCTION:    set_loc
 * ARGUMENTS:   char  t
 *              LOC * s
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool set_loc( char t, LOC * s )//type spot
{
    switch(t) {
        case 'w': case 'W': case '#'://WALL
            s->is_floor = false;
            s->is_occupied = false;
            s->is_visible = true;
            s->is_wall = true;
            s->is_door = false;
            s->is_ustair = false;
            s->is_dstair = false;
            s->is_trap = false;
            break;
        case 'f': case 'F': case '.'://FLOOR
            s->is_floor = true;
            s->is_occupied = false;
            s->is_visible = true;
            s->is_wall = false;
            s->is_door = false;
            s->is_ustair = false;
            s->is_dstair = false;
            s->is_trap = false;
            break;
        case 'b': case 'B': case '+'://DOOR
            s->is_floor = false;
            s->is_occupied = false;
            s->is_visible = true;
            s->is_wall = false;
            s->is_door = true;
            s->is_ustair = false;
            s->is_dstair = false;
            s->is_trap = false;
            break;
        default:
            if( t != 0 )return t;
            else return false;
    }/* End type Switch */
    return true;
}/* End set_loc Func */


/******************************************************************************
 * FUNCTION:    fill
 * ARGUMENTS:   char     t
 *              LEVEL *  l
 *              COORD    d
 *              COORD    c
 * RETURNS:     bool
 * WARNING:
 * NOTE://map, door row, door col, corner row, corner column
 ******************************************************************************/
bool fill( char t, LEVEL * l, COORD d, COORD c )
{
    int i, j, s = smallest(d.colx,c.colx),
        b[2] = { biggest(d.rowy,c.rowy), biggest(d.colx,c.colx) };
    /* NOTE: Biggest returns zero if it's arguments are the same. */

    for( i = smallest(d.rowy,c.rowy); i < b[0]; i++ )
        for( j = s; j < b[1]; j++ ) {
            if( ( i > MAX_ROW )||( j > MAX_COL ) ) return false;
            else set_loc(t,&l->map[i][j]);
        }/* End ij ffor */
    return true;
}/* End fill Func */


/******************************************************************************
 * FUNCTION:    init_lv
 * ARGUMENTS:   LEVEL *  l
 *              short    t
 * WARNING:
 * NOTE:
 ******************************************************************************/
void init_lv( LEVEL * l, short t )
{
    int r, c;

    /* Keep Track of Current Level and Position in curlv array */
    l->type = t;

    l->is_new = true; /* Initially Unexplored */

    /* Set All map flags Initially in Case Some Slip Through */
    for( r = 0; r < MAX_ROW; r++ )
        for( c = 0; c < MAX_COL; c++ ) {
            /* Border is always wall */
            if( ( ( r == 0 )||( c == 0 ) )||( ( r == (MAX_ROW-1) )
                                          ||(   c == (MAX_COL-1) ) ) ) {
                set_loc( 'w', &l->map[r][c] );
            }/* End border ray check */
            else set_loc( '.', &l->map[r][c] ); /* Interior is floors for now */

            /* Initialize Item Locations */
            set_empty_item( &l->map[r][c].litter );

        }/* end map RC ffor */
}/*end init_lv func */


/******************************************************************************
 * FUNCTION:    get_map_icon
 * ARGUMENTS:   LOC here
 * RETURNS:     char
 * WARNING:
 * NOTE:
 ******************************************************************************/
char get_map_icon( LOC here )
{
    if( here.is_visible != true )       return ' ';
    else if( here.is_wall == true )     return '#';
    else if( here.is_occupied == true ) return 'o';//tempfortest
    else if( here.litter.is_ == true )  return '&';//tempfortest
    else if( here.is_floor == true )    return '.';
    else if( here.is_door == true )     return '+';
    else if( here.is_ustair == true )   return '>';
    else if( here.is_dstair == true )   return '<';
    else if( here.is_trap == true )     return 'x';//tempfortest
    else                                return '&';//DEBUG SYMBOL
}/* end get_map_icon func */


/******************************************************************************
 * FUNCTION:    draw_map                                                      *
 * ARGUMENTS:   LEVEL* curlv   -- The Level to draw.                          *
 ******************************************************************************/
void draw_map( LEVEL * curlv )
{
    int r, c;
    for( r = 0; r < MAX_ROW; r++ ) for( c = 0; c < MAX_COL; c++ ) {
        move(r,c);
        addch( get_map_icon( curlv->map[r][c] ) );
    }/* end [r][c] screen initialization */
}


/******************************************************************************
 * FUNCTION:    buildgen
 * ARGUMENTS:   LEVEL *  outside
 *              LEVEL *  inside
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool buildgen( LEVEL * outside, LEVEL * inside )
{
    int r, c, i, j;
    bool found_first;
    bool done;

    /* copy/invert ouside map to inside map */
    for( r = 1; r < (MAX_ROW -1); r++ )
        for( c = 1; c < (MAX_COL -1); c++ )
            if( outside->map[r][c].is_wall == true )
                set_loc( '.' , &inside->map[r][c] );
            else if( outside->map[r][c].is_floor == true )
                set_loc( '#' , &inside->map[r][c] );
            else if( outside->map[r][c].is_door == true ) {
                //Both a door and exit to lv
                set_loc( '.' , &inside->map[r][c] );
                //tmp use trap to help find door location
                inside->map[r][c].is_trap = true;
            }/* end outside_door if */

    /* Locate/Create exit doors */
    for( r = 1; r < (MAX_ROW -1); r++ )
        for( c = 1; c < (MAX_COL -1); c++ )
            if( inside->map[r][c].is_trap == true ) {
                //second found wall should usually be made exit door
                inside->map[r][c].is_trap = false;//stop tmp use
                found_first = false;
                done  = false;
                for( i = r-1; i <= r+1; i++ )
                    if( done  == true ) break;
                    else for( j = c-1; j <= c+1; j++ )
                        if( inside->map[i][j].is_wall == true ) {
                            if( found_first == false )
                                found_first = true;
                            else{
                                set_loc( '+', &inside->map[i][j] );
                                inside->map[i][j].is_dstair = true;
                                done  = true;
                                break;
                            }/* end place exit door else */
                        }/* end is_wall if */
            }/* end is_trap if */

    /* TODO: Generate sub buildings attached to main buildings with tunnels */

    return true;//warning always succeeds
}/* end buildgen */


/******************************************************************************
 * FUNCTION:    towngen             -- Generate the town level.               *
 * ARGUMENTS:   LEVEL * l           -- The level.                             *
 *              unsigned short n    -- qt of buildings to generate.           *
 * RETURNS:     bool                -- Generation successfull                 *
 * Attempts to generate buildings MIN_HUT_DIST appart, however they may still *
 * overlap effectively generating less than n buildings. This makes them more *
 * interesting though, so it is permitted.                                    *
 ******************************************************************************/
bool towngen( LEVEL * l, unsigned short n )
{
    unsigned short i, j, z;
    int dvert, dhorz;
    COORD hutspot[MAX_HUTS];
    RECT room[MAX_HUTS];

    assert( n > 0 );
    assert( n <= MAX_HUTS );

    /* Create n Huts */
    for( i = 0; i < n; i++ ) {

        while( true ) {
            /* Find centers of Buildings */
            hutspot[i].rowy =  ( 2 + MIN_HUT_HGT ) + rng( MAX_ROW - ( 2 + MIN_HUT_HGT ) );
            hutspot[i].colx =  ( 2 + MIN_HUT_WID ) + rng( MAX_COL - ( 2 + MIN_HUT_WID ) );
            if( i > 0 ) for( z = 0; z < i; z++ )
                if( dist( hutspot[z], hutspot[i] ) < MIN_HUT_DIST ) continue;

            /* Expand Building Dimensions */
            dvert = MIN_HUT_HGT + rng(MAX_HUT_HGT);
            dhorz = MIN_HUT_WID + rng(MAX_HUT_WID);

            room[i].a.rowy = hutspot[i].rowy - dvert;
            room[i].a.colx = hutspot[i].colx - dhorz;
            room[i].b.rowy = hutspot[i].rowy + dvert;
            room[i].b.colx = hutspot[i].colx + dhorz;

            /* Don't Bump the Outer Wall */
            if( ( room[i].a.rowy < 2  )||( room[i].a.colx < 2 ) ) continue;
            if( room[i].b.rowy > ( MAX_ROW - 2 ) ) continue;
            if( room[i].b.colx > ( MAX_COL - 2 ) ) continue;

            break;
        }/*end !done while */

        /* Fill Building With Wall */
        if( fill_wall( l , room[i].a, room[i].b ) == false )
            return( Error( "Failed to Fill Building Walls", i ) );

        /* Place Doors */ //inline with center//TODO:+-rng(dvert||dhorz -1)
        for( j = 0, z = rng(4) ; j < 4; j++ ) { /* Try Up to Each Direction NSEW *///TODO: Perhaps try only once
            if( ++z > WEST ) z = NORTH;
            switch( z ) {
                case NORTH:
                    hutspot[i].rowy = room[i].a.rowy;
                    if( l->map[hutspot[i].rowy-1][hutspot[i].colx].is_wall )
                        continue;
                    break;
                case SOUTH:
                    hutspot[i].rowy = room[i].b.rowy - 1;
                    if( l->map[room[i].b.rowy][hutspot[i].colx].is_wall )
                        continue;
                    break;
                case EAST:
                    hutspot[i].colx = room[i].b.colx - 1;
                    if( l->map[hutspot[i].rowy-1][room[i].b.colx].is_wall )
                        continue;
                    break;
                case WEST:
                    hutspot[i].colx = room[i].a.colx;
                    if( l->map[hutspot[i].rowy][hutspot[i].colx-1].is_wall )
                        continue;
                    break;
            }/* End cardinal direction Switch */

            /* Set Building Enterance Flags */
            set_loc( '+', &l->map[hutspot[i].rowy][hutspot[i].colx] );//TODO: make map symbols configable
            break;
        }/* End NSEW For */
    }/* End n For */

    /* TODO: Place NPCs */


    return true;
}/* End towngen Func */



/************************************EOF***************************************/
