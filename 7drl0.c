#include "7drl0def.h"
#include "7drl0func0.h"
#include "7drl0func1.h"
#include "7drl0func2.h"
#include "7drl0func3.h"


int main( int argc, char* argv[] )
{
	int r, c, cmd;
	short i;
	PLAYER pc; /*player's character*/
	STAT_DAT score;
	LEVEL curlv[MAX_MAPS]; /*current levels*/
	MYBOOL run;
//	char* dat_file_name = ;
//	FILE* indata;
	char* nam = "Skallagrim\n";
//	char nam[RT_SUB_COLS - 2];
	WINDOW *display_btm, *display_right;

	srand(time(NULL));	    /* use clock value as starting seed */

	run.bbool = ONN;

/* READ IN DATA FILE(S) */
//	if( ( indata = fopen( dat_file_name, READ_ONLY ) ) == NULL )
//		exit( error("ERROR: Missing \"item.dat\" file", FAIL ) );
/* PROCESS DATA FILES */

/* INITIALIZE PLAYER */
//	printf("\nWhat is the name for your character? ");
//	if( fgets(nam, (RT_SUB_COLS - 2), stdin ) == NULL )
//		exit( error("ERROR: fgets failed to get string", (RT_SUB_COLS - 2) ) );
////	myflush(stdin);
	if( init_player( &pc, nam ) == FAIL )
		exit( error("ERROR: Failed to initialize player", FAIL ) );
	if( init_stat_data( &score ) == FAIL )
		exit( error("ERROR: Failed to initialize scores", FAIL ) );

	pc.attdir =	story_line( BEGINNING, NULL );//BEGINNING Must be before curses initialization for now

/* INITIALIZE CURSES */
   initscr(); /* Start curses mode *///RETURNS WINDOW*
	display_btm = subwin(stdscr,BTM_SUB_ROWS,BTM_SUB_COLS,MAX_ROW,0);
	if( display_btm == NULL ) exit( error("ERROR: btm sub win", FAIL ) );
	display_right = subwin(stdscr,RT_SUB_ROWS,RT_SUB_COLS,0,MAX_COL);
	if( display_right == NULL ) exit( error("ERROR:right sub win", FAIL ) );
   raw();     /* Disable input buffering */
   noecho();  /* Disable input echoing */
	keypad(stdscr, TRUE); /* enable arrowkeys and such */

/* INITIALIZE MAPS */
	for( i = 0; i < MAX_MAPS; i++ ) if( init_lv( &curlv[i], i ) == FAIL )		exit( error("ERROR: Failed to initialize level", i ) );
//	{
//		endwin();/* End curses mode */
//		exit( error("ERROR: Failed to initialize level", i ) );
//	}/* end init maps for */

/* RUN GAME */
	while( run.bbool == ONN )
	{
		score.turn += 1;

	/* Set-up/Update RH Display */
		if( init_display_right( display_right, &pc, &score ) == FAIL )
		{
			endwin();/* End curses mode */
			exit( error("ERROR: Failed to initialize right hand display", FAIL ) );
		}/* end setup RH Display fail if */

	/* INITIALIZE CURRENT LEVEL IF NEEDED */
		if(curlv[pc.maplv].is_new == ONN)
		{
			for( r = 0; r < MAX_ROW; r++ )
				for( c = 0; c < MAX_COL; c++ )
				{
					move(r,c);
					addch( get_map_icon( ACTIVE_LOCATION ) );
				}/* end [r][c] screen initialization */

			if( score.turn == 1 )
			{
				refresh();

			/* Get Attack Direction */
				say("You approach the human village. From which direction would you like to attack? ");
				cmd = getch();
				if     (( toupper(cmd) == 'N' )||(cmd == '8')) pc.attdir = NORTH;
				else if(( toupper(cmd) == 'S' )||(cmd == '2')) pc.attdir = SOUTH;
				else if(( toupper(cmd) == 'E' )||(cmd == '6')) pc.attdir = EAST;
				else if(( toupper(cmd) == 'W' )||(cmd == '4')) pc.attdir = WEST;
				else if( cmd == '9' ) pc.attdir = NORTH_EAST;
				else if( cmd == '1' ) pc.attdir = SOUTH_WEST;
				else if( cmd == '3' ) pc.attdir = SOUTH_EAST;
				else if( toupper(cmd) == 'Q' ){ endwin(); return 0; }
				else pc.attdir = NORTH_WEST;

			/* Get Players Starting Position */
				switch( pc.attdir )
				{
					case NORTH: while( ( pc.locc = rng( MAX_COL - 2 ) ) < 1 ); pc.locr = 1;               break;
					case SOUTH: while( ( pc.locc = rng( MAX_COL - 2 ) ) < 1 ); pc.locr = ( MAX_ROW - 2 ); break;
					case EAST:  while( ( pc.locr = rng( MAX_ROW - 2 ) ) < 1 ); pc.locc = ( MAX_COL - 2 ); break;
					case WEST:  while( ( pc.locr = rng( MAX_ROW - 2 ) ) < 1 ); pc.locc = 1;               break;
					case NORTH_EAST:	pc.locc = ( MAX_COL - 2 ); pc.locr = 1;               break;
					case SOUTH_EAST:	pc.locc = ( MAX_COL - 2 ); pc.locr = ( MAX_ROW - 2 ); break;
					case SOUTH_WEST:  pc.locr = ( MAX_ROW - 2 );	pc.locc = 1;               break;
					case NORTH_WEST:  default:	   pc.locc = 1;   pc.locr = 1;
				}/* end pc.attdir switch */
			}/* end initial turn if */
//			else
			curlv[pc.maplv].is_new = OFF;
		}/* end is_new if */

	/* UPDATE PLAYER LOCATION */
		move(pc.locr,pc.locc);
		addch('@');

	/* UPDATE SCREEN */
		wrefresh(display_btm);
		refresh();
		wrefresh(display_right);

	/* GET CMD INPUT */
		while( ( cmd = get_cmd() ) == NO_ACTION );

	/* PROCESS CMD */
		r = pc.locr; c = pc.locc;
		if( cmd == NO_ACTION )
		{
			//NEED TO GET ANOTHER CMD
			say("NEED TO GET ANOTHER CMD");//SHOULD NOT DISPLAY NOW
		}/* end cmd is NO_ACTION if */
		else if( ( cmd > NO_ACTION )&&( cmd < WAIT ))/* MOVING */
		{
		/* Find Target Location */
			if( cmd == SOUTH_WEST )		 { r += 1; c -= 1; }/* end SW if */
			else if( cmd == SOUTH )		 { r += 1;      	 }/* end S if */
			else if( cmd == SOUTH_EAST ){ r += 1; c += 1; }/* end SE if */
			else if( cmd == WEST )		 {         c -= 1; }/* end W if */
			else if( cmd == EAST )		 { 		  c += 1; }/* end E if */
			else if( cmd == NORTH_WEST ){ r -= 1; c -= 1; }/* end NW if */
			else if( cmd == NORTH )		 { r -= 1;         }/* end SW if */
			else if( cmd == NORTH_EAST ){ r -= 1; c += 1; }/* end N if */
			else exit( error( "ERROR: Bad Logic MOVING cmd filter", cmd ) );

		/* MOVE assess legality */
			if( ( r > MAX_ROW )||( r < 0 ) )
				exit( error("ERROR: Move Out of Vertical Bounds", r ) );
			else if( ( c > MAX_COL )||( c < 0 ) )
				exit( error("ERROR: Move Out of Horizontal Bounds", c ) );
			else if( ACTIVE_LOCATION.is_wall == ONN ) {say("You bumped into a wall!");}//NEED TO GET ANOTHER CMD
		/* MOVE possibilities */
			else if( ACTIVE_LOCATION.is_occupied == ONN ) {say("Attack!");}
			else if( ACTIVE_LOCATION.is_door == ONN )
			{
				if( pc.maplv == HVILLAGE )
				{
					say("Do you want to enter the building? ");
					if( toupper( getch() ) == 'N' );//NEED TO GET ANOTHER CMD
					else{ /* Assume YES */
						if( pc.maplv == HVILLAGE ) pc.maplv = IN_HHUTS;
						else if( pc.maplv == CASL_GRD ) pc.maplv = CASL_DN0;
						else if( pc.maplv == CASL_DN0 ) pc.maplv = CASL_DN1;
						else if( pc.maplv == CASL_DN1 ) pc.maplv = CASL_DN2;
						else;///
					}/* end assumed confirmed yes else */
				}/* end HVILLAGE if */
				else if( skill_check(pc.stats[STR],0) == FAIL)
					say("The door is stuck!");
				else;
					//open door, move char, etc...
			}/* end door if */
			else if( ACTIVE_LOCATION.is_ustair == ONN ) {say("You found some ascending stairs!");}
			else if( ACTIVE_LOCATION.is_dstair == ONN ) {say("You found some descending stairs!");}
			else if( ACTIVE_LOCATION.is_trap == ONN ) {say("You have stepped into a trap!");}
			else /* Move Normally */
			{ 
				move(pc.locr,pc.locc);
				addch(get_map_icon(ACTIVE_LOCATION));
				pc.locr = r;
				pc.locc = c;
			}/*end normal move else*/
		}/* end cmd in MOVING range if */
		else if( cmd == WAIT ){/*DO NOTHING*/say("You wait...");}
		else if( cmd == QUIT )
		{
			say("Are you sure you want to quit? ");
			if( toupper(getch()) == 'Y' ) run.bbool = OFF;
//			else say("Oh ok, then we wont kick you out of here.");
		}/* end QUIT */
		else exit( error("ERROR: cmd processing", cmd ) );
//check for death
	}/* end run while */

/* Display And Save High Score Report */

/* TERMINATE PROGRAM */
	endwin();/* End curses mode */
//	fclose(indata);
	return 0;
}/* end main func */

