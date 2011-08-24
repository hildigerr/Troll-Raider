
/*INDEX*   --   ordered list of functions contained in this file */
int get_cmd( void );
char get_map_icon(LOC here);

int init_display_right( WINDOW* d, PLAYER* p,  STAT_DAT* s);
/*****************************************************************/

int get_cmd( void )
{
	int input;
	input = getch();

	switch(input)
	{
		case '8': case KEY_UP: /*NORTH*/
			return NORTH;
		case '2': case KEY_DOWN:/*SOUTH*/
			return SOUTH;
		case '6': case KEY_RIGHT:/*EAST*/
			return EAST;
		case '4': case KEY_LEFT:/*WEST*/
			return WEST;
		case '9':/*NORTH_EAST*/
			return NORTH_EAST;
		case '7':/*NORTH_WEST*/
			return NORTH_WEST;
		case '3':/*SOUTH_EAST*/
			return SOUTH_EAST;
		case '1':/*SOUTH_WEST*/
			return SOUTH_WEST;
		case '5':/*WAIT*/
			return WAIT;
		case 'q': case 'Q':/*QUIT*/
			return QUIT;
		case 'I':/* DEBUGGING */
			return DEBUG_ITEM;
		default:
			return NO_ACTION;
	}/*end input switch*/
}/* end get_cmd func */


char get_map_icon(LOC here)
{
	if( here.is_visible != ONN ) return ' ';
	else if( here.is_wall == ONN ) return '#';
	else if( here.is_occupied == ONN ) return 'o';//tempfortest
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

