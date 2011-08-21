
/*INDEX*   --   ordered list of functions contained in this file */
int init_itm(ITEM* itm, short lv_type);
int gear_up(ITEM* itm);
int init_stat_data( STAT_DAT* d );
int init_player(PLAYER* p, char* n);
int init_mon(PLAYER* m, short type);
int init_lv(LEVEL* l, short t);
/*****************************************************************/

int init_itm(ITEM* itm, short lv_type )
{	
	int i, m, s[MAX_ITEM_STATS];
	char c, b[2];
	FILE* datafile;
	long here;
	MYBOOL done;

/* READ IN DATA FILE */
	if( ( datafile = fopen( "item.dat", READ_ONLY ) ) == NULL )
		return( error("ERROR: Missing \"item.dat\" file", FAIL ) );

/* Initialize Necessary Variables */
	done.bbool = OFF;
	itm->worth = NOT_PLACED;
	m = NOT_PLACED;
//	if( m == 1 ) return( error( "BARF! BS Error!", m ) );
	for( i=0; i < MAX_ITEM_STATS; i++ ) s[i] = NOT_PLACED;

	itm->type = rng(MAX_ITM_TYPES) - 1;//Get Item Type == [0,MAX_ITM_TYPES)

	while( !feof(datafile) && (done.bbool == OFF))
	{
	/* Skip Over Comment Line */
		if( ( c = fgetc(datafile) ) == '#' ) while( fgetc(datafile) != '\n' ) /*error("BARF! Reading Comment", m)*/;
	/* Read In Maximum Items For This Type */
		else if( c == '$' )
		{
			error("BARF! Found Max Enties List", m );
			if( m == NOT_PLACED )
			{
			/* Find The Appropriate Entry */
				fseek( datafile, ( sizeof(char) * ( itm->type * 2 ) ), SEEK_CUR );//Will need rewrite if becomes more than 9//TODO:Make more robust
			/* Get Entry */
//				m = atoi( (char*)fgetc(datafile) );
				b[0] = fgetc(datafile);	b[1] = '\0'; m = atoi(b); error("BARF! Got m ",m);
			/* Veriy Entry's Legality */
				if( ( m < 0)||( m > MAX_ITM_PER_TYPE ) )
				{
					fclose(datafile);
					return( error("ERROR: Max Item Per Type Out of Range.  m = ", m) );
				}/* end m verify if */
			/* Process Entry */
				else m = rng(m) - 1;// m now ==(0,MAX_ITM_PER_TYPE]
			/* Go To The Next Line */ error("BARF! m is now ", m );
				while( fgetc(datafile) != '\n' );
			/* Record Position -- We Should Not Have to rewind before here */
				here = ftell(datafile);
				if( here < 0 ) //( fgetpos(datafile,here) != 0 )//
				{
					fclose(datafile);
					return(error("ERROR: Failed to record file position in \"itm.dat\" file!" , m));
				}/* end get position if */
			}/* end m not placed if */
			else{/* m != NOT_PLACED */
				fclose(datafile);
				return( error("ERROR: Double MAX Entries in \"itm.dat\" file!", m) );
			}/* end already got m else */		
		}/* end $ if */
	/* Read In Entries */
		else if( c == ':' )
		{
//			error("BARF! Found an Item entry", c);
			if( ( m < 0)||( m > MAX_ITM_PER_TYPE ) )
			{
				fclose(datafile);
				return( error("ERROR: Bad item Generated", m) );
			}/* end m verify if */
			else{/* m is ok */
			/* Get Item Type */
				b[0] = fgetc(datafile);	b[1] = '\0';//shouldn't need  to change b[1]
				c = atoi(b); error(" BARF: Got Item Type =", c );
			/* Verify That We Havn't Passed Our Item's Type Section */ //SHOULD BE UNNECESSARY
				if( c > itm->type )
				{
					error("BARF: itm->type ==",itm->type);
					if( fseek(datafile,here,SEEK_SET) != 0 ) //( fsetpos( datafile, here ) != 0 )
					{
						fclose(datafile);
						return(error("ERROR: Failed to rewind datafile",c));
					}/* end rewind if */
				}/* end gone too far if */
			/* Process The Section if of Correct Type */
				else if( c == itm->type )
				{
					error("BARF: Found itm type section", itm->type);
					error("BARF: m is curretly", m );
					/* Find Correct Listing In This Section */
					while( m > 0 )
						if( m-- == 1 )//Found It!
						{
							error("BARF! m is now ", m);
							fgetc(datafile);//bypass ':'
						/* Get Item Stats */
							for( i = 0; i < MAX_ITEM_STATS; i++ )
							{
								error("BARF: Getting item stat",i);
								mypause();
								while( ( c == fgetc(datafile) ) != ':' )
								{
									error("BARF: Converting to int", c);
									b[0] = c; b[1] = '\0'; c = atoi(b);
									error("BARF: IS now int", c);
									mypause();
									if( s[i] == NOT_PLACED ) s[i] = c;
									else s[i] = ( ( s[i] * 10 ) + c );
								}/* end !end of current number while */
							}/* end get item stats for */
						/* Get Item Worth */
							while( ( c == fgetc(datafile) ) != ':' )
							{
								b[0] = c; b[1] = '\0';
								if( itm->worth == NOT_PLACED ) itm->worth = atoi(b);
								else itm->worth = ( ( itm->worth * 10 ) + atoi(b) );
							}/* end get item worth while */
						/* Get Item Name */
							i = 0;
							while( ( c == fgetc(datafile) ) != ':' )
							{
								if( i > MAX_ITM_NAME_LEN )
								{
									fclose(datafile);
									return(error("ERROR: Item Name Too Long",i));
								}/* end name length check if */
								else/* havn't exceeded MAX_ITM_NAME_LEN */
									itm->name[i++] = c;
							}/* end get item name while */
							itm->name[i] = '\0';
							done.bbool = ONN;
							error("BARF: ITEM DONE!",i);
						}/* end m == 1 : Found Correct Listing In section if */
						else //Go to next Listing
							while( fgetc(datafile) != '\n' ) error("BARF: Skipping This Listing M",m);
				}/* end c == itm->type if */
				else /* Havn't Reached Our Section Yet */
					 while( fgetc(datafile) != '\n' ) /*error("BARF: Skipping section", c)*/;
			}/* end m is ok else */
		}/* end ':' else */
		else if( c == '\n' ) /* error("BARF: BLANK LINE" , c)*/;//continue;
		else
		{
			fclose(datafile);
			return( error("ERROR: Unexpected Input From \"Itm.dat\" file", c) );
		}/* end unexpected else */
		error("BARF! finished",m);
		//if(m > NOT_PLACED ) mypause();//TESTING
	}/* end file access while */

	for(i=0; i < MAX_ITEM_STATS; i++ )
	{
		itm->stats[i] = s[i];

	}/*end i<MAX_ITEM_STATS for*/

	itm->is_ = ONN;

	fclose(datafile);
	return 0;
}/* end init_itm func */



int gear_up(ITEM* itm)
{
	int i;

	itm->is_ = ONN;
	itm->type = 0;

	for(i=0; i < MAX_ITEM_STATS; i++ )
	{
		itm->stats[i] = 0;

	}/*end i<MAX_ITM_STATS for*/
	itm->worth = 0;
	
	return 0;/*WARNING: always succeeds*/
}/* end gear_up func */



int init_stat_data( STAT_DAT* d )
{
	d->food = 0;	
	d->turn = 0;
	d->family = 0;
	return 0;//WARNING Always succeeds 
}/* end init_stat_data func */


int init_player(PLAYER* p, char* n)
{
	int i = 0;

	p->name = n;
	if(p->name == NULL) return FAIL;

	for( i = 0; i < MAX_STATS; i++ ) p->stats[i] = rng(MAX_STAT_VAL);
	for( i = 0; i < MAX_HOLD; i++ )
		if( gear_up( &p->inventory[i] ) == FAIL ) return FAIL;

   for( i = 0; i < MAX_SLOTS; i++ ) p->equip[i] = NULL;

	p->explv = 0.0;
	p->money = 0;
	p->locr = NOT_PLACED;
	p->locc = NOT_PLACED;
	p->maplv = HVILLAGE;

	p->is_main = ONN;
	p->is_alive = ONN;
	p->is_human = OFF;
	p->is_unique = ONN; //doesnt matter

//	p->attdir = NO_ACTION;

//	if( p->data != NULL )
//		if( init_stat_data( p->data ) == FAIL ) return FAIL;
//	else return FAIL;

	return 0;
}/* end init_player func */



int init_mon(PLAYER* m, short lv_type)
{
	int stat_seed, i = 0;
	MYBOOL fail;
	fail.bbool = OFF;

	stat_seed = MAX_STAT_VAL;

	for( i = 0; i < MAX_STATS; i++ ) m->stats[i] = rng(stat_seed);
	m->name = NULL;//make buffer size based on data file
	for( i = 0; i < MAX_HOLD; i++ )
		if( ( gear_up( &m->inventory[i] )  ) == FAIL ) fail.bbool = ONN;
   for( i = 0; i < MAX_SLOTS; i++ )	m->equip[i] = NULL;
	m->explv = 0.0;
	m->money = 0;
	m->locr = NOT_PLACED; m->locc = NOT_PLACED;
	m->maplv = NOT_PLACED;
	m->is_main = OFF;
	m->is_alive = ONN;
	m->attdir = WAIT;
//	m->data = NULL;
	m->is_human = ONN;
	m->is_unique = OFF;

	if( fail.bbool == ONN) return FAIL;
	else return 0;
}/* end init_player func */


int init_lv(LEVEL* l, short t)
{
	int r, c, i, n, z;
	int hutspot[2][MAX_HUTS+1],//[x,y][Huts+Castle]Entrance Locations
	    oppscrn[2][MAX_HUTS+1];//[x,y][Huts+Castle]Opposite Corner Locations
	MYBOOL done;
	double length[2];

/* Keep Track of Current Level and Position in curlv array */
	l->type = t;

	l->is_new = ONN; /* Initially Unexplored */

/* Set All map flags Initially in Case Some Slip Through */
	for( r = 0; r < MAX_ROW; r++ )
		for( c = 0; c < MAX_COL; c++ )
		{
			if( ( ( r == 0 )||( c == 0 ) )||( ( r == (MAX_ROW-1) )||( c == (MAX_COL-1) ) ) )
			{/* Border is always wall */
				l->map[r][c].is_floor = OFF;
				l->map[r][c].is_occupied = OFF;
				l->map[r][c].is_visible = ONN;
				l->map[r][c].is_wall = ONN;
				l->map[r][c].is_door = OFF;
				l->map[r][c].is_ustair = OFF;
				l->map[r][c].is_dstair = OFF;
				l->map[r][c].is_trap = OFF;
			}/*end border ray check*/
			else{/* Interior is floors for now */
				l->map[r][c].is_floor = ONN;
				l->map[r][c].is_occupied = OFF;
				l->map[r][c].is_visible = ONN;
				l->map[r][c].is_wall = OFF;
				l->map[r][c].is_door = OFF;
				l->map[r][c].is_ustair = OFF;
				l->map[r][c].is_dstair = OFF;
				l->map[r][c].is_trap = OFF;
			}//end else4DEBUG
		}/* end map RC ffor */

/* Initialize Monsters */
	for( i = 0; i < MAX_LV_MON; i++ )
		if( init_mon(&l->mon[i],l->type) == FAIL )
			return( error("ERROR init_mon !success", i) );

/* Initialize Items */
	for( r = 0; r < MAX_LV_ITM; r++ )
		if( init_itm(&l->itm[i],l->type) == FAIL )
			return( error("ERROR init_itm !success", i) );

/* Generate Map Based on Type */
	n = rng(MAX_HUTS);	//n = MAX_HUTS;//TESTING

	switch( l->type )
	{
//		case TVILLAGE:
		case HVILLAGE: /* Create n Huts and 1 Castle */
			for( i = 0; i < (n+1); i++ )
			{
			/* Find Enterances to Buildings */
				done.bbool = OFF;
				while( done.bbool == OFF )
				{
					while( (r = rng(MAX_ROW) - 3) <2 );//error("r=", r );
				   while( (c = rng(MAX_COL) - 3) <2 );//error("c=", c );
					hutspot[0][i] = r;
					hutspot[1][i] = c;
					done.bbool = ONN;
					if( i == 0 ) continue;
					else for( z = 1; z <= i; z++ )
						if( dist( hutspot[0][z-1], hutspot[0][z],
									 hutspot[1][z-1], hutspot[1][z] ) < MIN_HUT_DIST )
//						if( dist( hutspot[0][z-i], hutspot[0][z],
//									 hutspot[1][z-i], hutspot[1][z] ) < MIN_HUT_DIST )						
							done.bbool = OFF;
				}/* end done.bbool while */	
			/* Set Building Enterance Flags */
				l->map[r][c].is_floor = OFF;
				l->map[r][c].is_occupied = OFF;
				l->map[r][c].is_visible = ONN;
				l->map[r][c].is_wall = OFF;
				l->map[r][c].is_door = ONN;
				l->map[r][c].is_ustair = OFF;
				l->map[r][c].is_dstair = OFF;
				l->map[r][c].is_trap = OFF;
			/* Expand Building Dimensions */
				/* Make Enterance Face Fathest Wall */
				length[NORTH_SOUTH] = biggest( DISTANCE_TO_NORTH_WALL, DISTANCE_TO_SOUTH_WALL );
				length[EAST_WEST]   = biggest( DISTANCE_TO_EAST_WALL,  DISTANCE_TO_WEST_WALL  );
				if( ( length[NORTH_SOUTH] / MAX_ROW ) > ( length[EAST_WEST] / MAX_COL ) )/* Face North||South */
					if( length[NORTH_SOUTH] == DISTANCE_TO_NORTH_WALL )/* Face North */
					{
						while( ( oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] + rng(MAX_HUT_WID) ) > ( MAX_COL - 2 ) );
						while( ( oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] + rng(MAX_HUT_HT) ) > ( MAX_ROW - 2 ) );
//						oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] + MAX_HUT_WID;
//						oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] + MAX_HUT_HT;
					}/* end north if */
					else{/* Face South */
						while( ( oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] + rng(MAX_HUT_WID) ) > ( MAX_COL - 2 ) );
						while( ( oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] - rng(MAX_HUT_HT) ) < 2 );
//						oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] + MAX_HUT_WID;
//						oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] - MAX_HUT_HT;
					}/* end South else */
				else/* Face East||West */
					if( length[EAST_WEST] == DISTANCE_TO_EAST_WALL )/* Face East */
					{
						while( ( oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] - rng(MAX_HUT_WID) ) < 2 );
						while( ( oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] - rng(MAX_HUT_HT) ) < 2 );
//						oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] - MAX_HUT_WID;
//						oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] - MAX_HUT_HT;
					}/* end East if */
					else{//face West
						while( ( oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] + rng(MAX_HUT_WID) ) > ( MAX_COL - 2 ) );
						while( ( oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] - rng(MAX_HUT_HT) ) < 2 );
//						oppscrn[EAST_WEST][i] = hutspot[EAST_WEST][i] + MAX_HUT_WID;
//						oppscrn[NORTH_SOUTH][i] = hutspot[NORTH_SOUTH][i] - MAX_HUT_HT;
					}/* end West else */
				/* Fill Building With Wall */
					if( fill_wall( l , hutspot[EAST_WEST][i], hutspot[NORTH_SOUTH][i], oppscrn[EAST_WEST][i], oppscrn[NORTH_SOUTH][i] ) == FAIL )
						return( error( "ERROR: Failed to Fill Building Walls", i ) );
				/* Place Doors *///TODO:if covered by fill func 
			}/* end i for */
			break;
		case IN_HHUTS:
			/* Divide map Into n Huts */
				
			/* Give Each Section 1 Exit */
			break;
		case CASL_GRD:
			/* Generate Castle Map Ground Level*/
			break;
		case CASL_UP0:	case CASL_UP1: case CASL_UP2:
			/* Genereate Castle Map Ground Level*/
			break;
		case CASL_DN0:	case CASL_DN1:	case CASL_DN2:
			/* Genereate Castle Map Ground Level*/
			break;
		default:
			return error("ERROR: Default Case LV", FAIL);
			/*ERROR*/
			break;
	}/* end type switch */

	return 0;
}/*end init_lv func */



