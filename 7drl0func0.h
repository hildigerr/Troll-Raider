
/*INDEX*   --   ordered list of functions contained in this file */
int error( char* msg, int ref );
double biggest( double a, double b );
int squared( int x );
double dist( int ar, int br, int ac, int bc );
void wsay( WINDOW* where, char* string );
int rng(int max_die_face);
int smallest( int a, int b );
int fill_wall( LEVEL* l, int dr, int dc, int cr, int cc );
void myflush( FILE *in );
void mypause( void );

/*****************************************************************/


int error( char* msg, int ref )
{
	fprintf( stderr, "\n %s [ %d ]\n", msg, ref );
	return FAIL;
}/* end error func */



double biggest( double a, double b )
{
	if( a == b ) return 0.0;
	else if( a > b ) return a;
	else return b;
}/* end biggest func */



int squared( int x )
{
	return (x * x);
}/* end squared func */



double dist( int ar, int br, int ac, int bc )
{
	return sqrt( squared( ar - br ) + squared( ac - bc ) );
}/* end dist func */



void wsay( WINDOW* where, char* string)
{
	int i = 0;

	if( winsertln(where) == ERR )
		exit( error("ERROR: wsay blank line insertion", FAIL ) );
	else while(string[i] != '\0')
	{
		wmove(where,0,i);
		if( winsch(where,string[i++]) == ERR )
			exit( error("ERROR: wsay message insert display", FAIL) );
	}/*end string !terminated while*/

	wrefresh(where);
}/* end wsay func */



int rng( int max_die_face )
{
	int num = 1;

	num += rand() % max_die_face;		/* random int 1 to max_die_face */

//	num = max_die_face;	//testing

	return num;
}/* end rng func */



int smallest( int a, int b )
{
	if( a < b ) return a;
	else return b;
}/* end smallest func */



int fill_wall( LEVEL* l, int dr, int dc, int cr, int cc )//map, door row, door col, corner row, corner column
{
	int i, j;
		for( i = smallest(dr,cr); i < biggest(dr,cr); i++ )
			for( j = smallest(dc,cc); j < biggest(dc,cc); j++ )
			{//NOTE: Biggest returns zero if it's arguments are the same
				if( ( i > MAX_ROW )||( j > MAX_COL ) ) return FAIL;
				else{
					l->map[i][j].is_floor = OFF;
					l->map[i][j].is_occupied = OFF;
					l->map[i][j].is_visible = ONN;
					l->map[i][j].is_wall = ONN;
					l->map[i][j].is_door = OFF;
					l->map[i][j].is_ustair = OFF;
					l->map[i][j].is_dstair = OFF;
					l->map[i][j].is_trap = OFF;
				}/*end else*/
			}/* end ij ffor */
	return 0;
}/* end fill_wall func */



void myflush ( FILE *in )
{
  int ch;

  do
    ch = fgetc ( in ); 
  while ( ch != EOF && ch != '\n' ); 

  clearerr ( in );
}/* end myflush func */



void mypause ( void ) 
{ 
//  printf ( "Press [Enter] to continue . . ." );
  fflush ( stdout );
  getchar();
} /* end mypause func */



