
/*INDEX*   --   ordered list of functions contained in this file */
int get_cmd( void );
int get_subi_cmd( void );
int get_uitem(void);
int get_user_num( int maximum );
int get_slot( void );
int get_hand(void);
char get_map_icon(LOC here);

int init_display_right( WINDOW* d, PLAYER* p,  STAT_DAT* s);
/*****************************************************************/

int get_cmd( void )
{
	int input;
	input = getch();

	switch(input)//TODO SAVE
	{
		case '8': case KEY_UP: 		/*NORTH*/	return NORTH;
		case '2': case KEY_DOWN:	/*SOUTH*/	return SOUTH;
		case '6': case KEY_RIGHT:	/*EAST*/		return EAST;
		case '4': case KEY_LEFT:	/*WEST*/		return WEST;
		case '9':					/*NORTH_EAST*/	return NORTH_EAST;
		case '7':					/*NORTH_WEST*/	return NORTH_WEST;
		case '3':					/*SOUTH_EAST*/	return SOUTH_EAST;
		case '1':					/*SOUTH_WEST*/	return SOUTH_WEST;
		case '5':						/*WAIT*/		return WAIT;
		case 'q': 
		case 'Q': case KEY_ESC:		/*QUIT*/		return QUIT;
		case 'i': 					/*INVENTORY*/  return INVENTORY;
		case 'e': case 'w':		/*EQUIPMENT*/  return EQUIPMENT;
		case 'u': case 'T':	 /*UNEQUIP ITEM*/ return REMOVE_ITM;
		case 'd':				 	/*DROP ITEM*/	return DROP_ITEM;
		case 'K':				 /*DESTROY ITEM*/ return DESTROY_ITEM;
		case 'I':					/*DEBUGGING*/  return DEBUG_ITEM;
		default:	return NO_ACTION;
	}/*end input switch*/
}/* end get_cmd func */


int get_subi_cmd( void )
{
	int input;
	input = getch();

	switch(input)
	{
		case 'f': case 'F': case ' ':				/* exit sub cmd prompt */
		case 'q': /*case 'i':*/ case KEY_ESC: 
			return SUB_FIN;
		case 'D': case 'd':							/* drop an item */
			return DROP_ITEM;
		case 'E': case 'e': case 'r': case 'w': case 'W':/* Equip/Ready an Item */
			return EQUIPMENT;
		case 'u': case 'U': case 't': case 'T': case 'R': /* Remove an Item */
			return REMOVE_ITM;
		case 'K': case 'k': case 'x': case 'X':
			return DESTROY_ITEM;
		default:
			return NO_ACTION;
	}/*end input switch*/
}/* end get_cmd func */

int get_uitem(void)
{
	int input;
	input = getch();

	switch (input)
	{
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': case 'A': return 10; /* index dereferencer +10 to     */
		case 'b': case 'B': return 11; /*  distinguish between i and e  */
		case 'c': case 'C': return 12;
		case 'd': case 'D': return 13;
		case ' ': case KEY_ESC: case 'q': case 'Q':	return CANCEL;
		default: return NOT_PLACED;
	}/* end input switch */
}/* end get_uitem func */

int get_user_num( int maximum )
{
	int u;
	char input[2];
	input[0] = getch();
	input[1] = '\0';

	
	if( isdigit(input[0]) )	u = atoi(input);
	else if( input[0] = KEY_ESC ) return CANCEL;
	else return NOT_PLACED;
	
	if( u < 0 )
		return NOT_PLACED;
	else if( u > maximum )
		return NOT_PLACED;
	else
		return u;
}/* end get_user_num func */


int get_slot( void )
{
	int input;
	input = getch();

	switch( input )
	{
		case 'a': case 'A': case '1': return 0;
		case 'b': case 'B': case '2': return 1;
		case 'c': case 'C': case '3': return 2;
		case 'd': case 'D': case '4': return 3;
		default: return NOT_PLACED;
	}/* end input switch */
}/* end get_slot func */


int get_hand(void)
{
	int input;
	input = getch();

	switch (input)
	{
		case '1': case 'r': case 'R': case 'a': case 'A': return WEP;
		case '2': case 'l': case 'L': case 'b': case 'B': return OFF;
		case '0': case 'q': case 'Q': case ' ': case KEY_ESC: return CANCEL;
		default: return NOT_PLACED;
	}/* end input switch */

}/* end get_hand func */



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






int init_display_right( WINDOW* d, PLAYER* p, STAT_DAT* s )
{
	int i;
	char* dflav[DFMAX] ={ "Vault", /* for each dungeon lv an ambiguous 5 letter name */
								 "Jails",
								 "Depth"  };
	wmove(d,1,1);
	wprintw(d," %s  LV %.2f   $%d\n", p->name, p->explv, p->money);
	for( i = 0; i < MAX_STATS; i++ )
	{
		wprintw(d,"\n");
		switch(i)
		{
			case STR:	wprintw(d,"  STR ");		break;
			case AGI:	wprintw(d,"  AGI ");		break;
			case MEN:	wprintw(d,"  MEN ");		break;
			case CON:	wprintw(d,"  CON ");		break;
			case CHA:	wprintw(d,"  CHA ");		break;
			case LUC:	wprintw(d,"  LUC ");		break;
			default:
				return FAIL;
		}/*end i switch */
		wprintw(d,"%d", p->stats[i]);
	}/* end MAX_STATS i for */
	wprintw(d,"\n\n  Food: %d\n  Family Saved: %d ", s->food, s->family );

/* Initialize i for Flavor */
	i = rng(DFMAX) - 1;

	switch(p->maplv)
	{
//		case TVILLAGE:	wprintw(d,"\n\n Troll Village ");	break;
		case HVILLAGE:	wprintw(d,"\n\n Human Village ");	break;
		case IN_HHUTS:	wprintw(d,"\n\n In a Hut      ");	break;
		case CASL_GRD:	wprintw(d,"\n\n Castle Ground ");	break;
		case CASL_UP0:	wprintw(d,"\n\n Tower 1st LV  ");	break;
		case CASL_UP1:	wprintw(d,"\n\n Tower 2nd LV  ");	break;
		case CASL_UP2:	wprintw(d,"\n\n Tower 3rd LV  ");	break;
		case CASL_DN0:	case CASL_DN1:	case CASL_DN2:
			wprintw(d,"\n\n Dungeon %s ", dflav[i] ); break;
		default:
			wprintw(d,"\n\n ERROR ");	break;
	}/*end switch*/
	wprintw(d,"\n  Turn: %d\n",s->turn);
	box(d,'|','-');//
	return 0;
}/* end init_display_right func*/

