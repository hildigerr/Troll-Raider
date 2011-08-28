/*INDEX*   --   ordered list of functions contained in this file */
int story_line( short when, WINDOW* where );
int skill_check( int ability, int adjust );
/*****************************************************************/

int story_line( short when, WINDOW* where )
{
	int opt = 0;

	switch( when )
	{
		case INTRO:
			printf("\n\n\tTroll Raider v%s\tBy HILDIGERR\n\n", version);
			opt = NO_ACTION;			
			break;
		case BEGINNING:
			break;
		case MIDDLE:
			if( where == NULL ) break;
			wsay( where, "You have saved all your family and slaughered the humans." );//output qt of family saved
			mypause();//more();
			wsay(where, "You smell another human settlement nearby, do you want to approach it? " );//output direction and? you must attack from that direction
			if( toupper(getch()) == 'Y' ) opt = 0;
			else opt = 0;
			break;
		case END:
			if( where == NULL ) break;
			break;
		default:
			opt = 0;
	}/*end when switch*/

	return opt;
}/* end story_line func*/



int skill_check( int ability, int adjust )//TODO: Make more interesting
{
	int roll;

	roll = rng(MAX_STAT_VAL) + adjust;

	if( roll < ability ) return 0;
	else return FAIL;

}/* end skill_check func */
