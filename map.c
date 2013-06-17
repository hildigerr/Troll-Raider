/* $Id: map.c,v 1.3 2013/05/05 22:04:12 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * map.c -- Map Utility Functions.                                            *
 ******************************************************************************/

#include "map.h"

/******************************************************************************
 * FUNCTION:    set_loc
 * ARGUMENTS:   char t
 *              LOC* s
 * RETURNS:     int
 * WARNING:     
 * NOTE:        
 ******************************************************************************/
int set_loc( char t, LOC* s )//type spot
{
	switch(t) {
		case 'w': case 'W': case '#'://WALL
			s->is_floor = OFF;
			s->is_occupied = OFF;
			s->is_visible = ONN;
			s->is_wall = ONN;
			s->is_door = OFF;
			s->is_ustair = OFF;
			s->is_dstair = OFF;
			s->is_trap = OFF;
			break;
		case 'f': case 'F': case '.'://FLOOR
			s->is_floor = ONN;
			s->is_occupied = OFF;
			s->is_visible = ONN;
			s->is_wall = OFF;
			s->is_door = OFF;
			s->is_ustair = OFF;
			s->is_dstair = OFF;
			s->is_trap = OFF;
			break;
		case 'b': case 'B': case '+'://DOOR
			s->is_floor = OFF;
			s->is_occupied = OFF;
			s->is_visible = ONN;
			s->is_wall = OFF;
			s->is_door = ONN;
			s->is_ustair = OFF;
			s->is_dstair = OFF;
			s->is_trap = OFF;
			break;
		default:
			if( t != 0 )return t;
			else return FAIL;
	}/* End type Switch */
	return 0;
}/* End set_loc Func */


/******************************************************************************
 * FUNCTION:    fill
 * ARGUMENTS:   char    t
 *              LEVEL*  l
 *              int     dr
 *              int     dc
 *              int     cr
 *              int     cc
 * RETURNS:     int
 * WARNING:
 * NOTE://map, door row, door col, corner row, corner column
 ******************************************************************************/
int fill( char t, LEVEL* l, int dr, int dc, int cr, int cc )
{
	int i, j;
    //NOTE: Biggest returns zero if it's arguments are the same
	for( i = smallest(dr,cr); i < biggest(dr,cr); i++ )
        for( j = smallest(dc,cc); j < biggest(dc,cc); j++ ) {
            if( ( i > MAX_ROW )||( j > MAX_COL ) ) return FAIL;
            else set_loc(t,&l->map[i][j]);
        }/* End ij ffor */
	return 0;
}/* End fill Func */


/******************************************************************************
 * FUNCTION:    init_lv
 * ARGUMENTS:   LEVEL*  l
 *              short   t
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int init_lv( LEVEL* l, short t )
{
	int r, c;

    /* Keep Track of Current Level and Position in curlv array */
	l->type = t;

	l->is_new = ONN; /* Initially Unexplored */

    /* Set All map flags Initially in Case Some Slip Through */
	for( r = 0; r < MAX_ROW; r++ )
		for( c = 0; c < MAX_COL; c++ ) {
            /* Border is always wall */
			if( ( ( r == 0 )||( c == 0 ) )||( ( r == (MAX_ROW-1) )
                                          ||( c == (MAX_COL-1) ) ) ) {
				set_loc( 'w', &l->map[r][c] );
			}/* End border ray check */
			else set_loc('.',&l->map[r][c]); /* Interior is floors for now */
				
            /* Initialize Item Locations */
			if( set_empty_item(&l->map[r][c].litter) != 0 )
				return( ERROR( NULL, "Litter Bug!", l->map[r][c].litter.type) );
		}/* end map RC ffor */

	return 0;
}/*end init_lv func */


/******************************************************************************
 * FUNCTION:    get_map_icon
 * ARGUMENTS:   LOC here
 * RETURNS:     char
 * WARNING:
 * NOTE:
 ******************************************************************************/
char get_map_icon(LOC here)
{
	if( here.is_visible != ONN ) return ' ';
	else if( here.is_wall == ONN ) return '#';
	else if( here.is_occupied == ONN ) return 'o';//tempfortest
	else if( here.litter.is_ == ONN ) return '&';//tempfortest
	else if( here.is_floor == ONN ) return '.';
	else if( here.is_door == ONN ) return '+';
	else if( here.is_ustair == ONN ) return '>';
	else if( here.is_dstair == ONN ) return '<';
	else if( here.is_trap == ONN ) return 'x';//tempfortest
	else return '&';//DEBUG SYMBOL
}/* end get_map_icon func */


/******************************************************************************
 * FUNCTION:    buildgen
 * ARGUMENTS:   LEVEL*  outside
 *              LEVEL*  inside
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int buildgen( LEVEL* outside, LEVEL* inside )
{
	int r, c, i, j;
	bool found_first;
	bool done;

    /* copy/invert ouside map to inside map */
	for( r = 1; r < (MAX_ROW -1); r++ )
		for( c = 1; c < (MAX_COL -1); c++ )
			if( outside->map[r][c].is_wall == ONN )
                set_loc( '.' , &inside->map[r][c] );
			else if( outside->map[r][c].is_floor == ONN )
                set_loc( '#' , &inside->map[r][c] );
			else if( outside->map[r][c].is_door == ONN ) {
                //Both a door and exit to lv
				set_loc( '.' , &inside->map[r][c] );
                //tmp use trap to help find door location
				inside->map[r][c].is_trap = ONN; 
			}/* end outside_door if */

    /* Locate/Create exit doors */
	for( r = 1; r < (MAX_ROW -1); r++ )
		for( c = 1; c < (MAX_COL -1); c++ )
			if( inside->map[r][c].is_trap == ONN ) {
                //second found wall should usually be made exit door
				inside->map[r][c].is_trap = OFF;//stop tmp use
				found_first = OFF;
				done  = OFF;
				for( i = r-1; i <= r+1; i++ )
					if( done  == ONN ) break;
					else for( j = c-1; j <= c+1; j++ )
						if( inside->map[i][j].is_wall == ONN ) {
							if( found_first == OFF )
								found_first = ONN;
							else{
								set_loc( '+', &inside->map[i][j] );
								inside->map[i][j].is_dstair = ONN;
								done  = ONN;
								break;
							}/* end place exit door else */
						}/* end is_wall if */
			}/* end is_trap if */
/* TODO: Generate sub buildings attached to main buildings with tunnels */

	return 0;//warning always succeeds
}/* end buildgen */


/******************************************************************************
 * FUNCTION:
 * ARGUMENTS:
 * RETURNS:
 * WARNING:
 * NOTE:
 ******************************************************************************/
int towngen( LEVEL* l, unsigned short n )//n = score.room_qt
{
	unsigned short i,z;
	int dvert, dhorz;
	COORD hutspot[MAX_HUTS];
	RECT room[MAX_HUTS];
	bool done;

	if(( n < 1 )||( n > MAX_HUTS )) return FAIL;

	/* initialize rooms */
	for( i = 0; i < MAX_HUTS; i++ )
	{
		if( i <= n )
		{
			room[i].a.rowy = 2;
			room[i].a.colx = 2;
			room[i].b.rowy = MAX_ROW - 2;
			room[i].b.colx	= MAX_COL - 2;
		}else//unused
		{
			room[i].a.rowy = NOT_PLACED;
			room[i].a.colx = NOT_PLACED;
			room[i].b.rowy = NOT_PLACED;
			room[i].b.colx	= NOT_PLACED;
		}/*end unused else */
	}/* end MAX_HUTS for */

    /* Create n Huts and 1 Castle *///n+1 huts for now
	for( i = 0; i < (n+1); i++ )
	{
        /* Find centers of Buildings */
		done  = OFF;
		while( done  == OFF )
		{
			while( ( hutspot[i].rowy = rng(MAX_ROW) - ( 3 + MIN_HUT_HGT ) ) <2 );//ERROR("r=", hutspot[i].rowy );
			while( ( hutspot[i].colx = rng(MAX_COL) - ( 3 + MIN_HUT_WID ) ) <2 );//ERROR("c=", hutspot[i].colx );
			done  = ONN;
			if( i == 0 ) continue;
			else for( z = 1; z <= i; z++ )
				if( dist( hutspot[z-1].colx, hutspot[z].colx,hutspot[z-1].rowy, hutspot[z].rowy ) < MIN_HUT_DIST )				
					done  = OFF;
		}/* end done  while */	
        
        /* Expand Building Dimensions */
		done  = OFF;
		while( done  == OFF )
		{
            /* create room dimensions */
			do{
				dvert = rng(MAX_HUT_HGT);
				dhorz = rng(MAX_HUT_WID);

				room[i].a.rowy = hutspot[i].rowy - dvert;
				room[i].a.colx = hutspot[i].colx - dhorz;
				room[i].b.rowy = hutspot[i].rowy + dvert;
				room[i].b.colx = hutspot[i].colx + dhorz;
			}while( ( ( room[i].a.rowy < 2  )||( room[i].a.colx < 2 ) )||( ( room[i].b.rowy > ( MAX_ROW - 2 ) )||( room[i].b.colx > ( MAX_COL - 2 ) ) ) );
            
            /* check for conflicts with other rooms */
			if( l->map[room[i].a.rowy][room[i].a.colx].is_wall == ONN );//top-left
			else if(l->map[room[i].a.rowy][room[i].b.colx].is_wall == ONN );//top-right
			else if(l->map[room[i].b.rowy][room[i].a.colx].is_wall == ONN );//btm-left
			else if(l->map[room[i].b.rowy][room[i].b.colx].is_wall == ONN );//btm-right
			else done  = ONN;
		}/*end done  while */
        
        /* Fill Building With Wall */
		if( fill_wall( l , room[i].a.rowy, room[i].a.colx, room[i].b.rowy, room[i].b.colx ) == FAIL )
			return( ERROR( NULL, "Failed to Fill Building Walls", i) );
	
        /* Place Doors */ //inline with center//TODO:+-rng(dvert||dhorz -1)
		/* TODO: Make Enterance Face Fathest Wall? */
			switch( rng(4) )//NOW: Face Random Direction
			{
				case NORTH:hutspot[i].rowy = room[i].a.rowy; break;//NORTH
				case SOUTH:hutspot[i].rowy = room[i].b.rowy - 1; break;//SOUTH
				case EAST:hutspot[i].colx = room[i].b.colx - 1; break;//EAST
				case WEST:hutspot[i].colx = room[i].a.colx; break;//WEST
				default: exit(ERROR(NULL, "badswitch",FAIL));
			}/* end cardinal direction switch */
            
            /* Set Building Enterance Flags */
			set_loc( '+', &l->map[hutspot[i].rowy][hutspot[i].colx] );
	}/* end i for */

	/* Place NPCs */


	return 0;
}/* end towngen func */



/************************************EOF***************************************/
