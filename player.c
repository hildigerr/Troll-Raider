/* $Id: player.c,v 1.6 2014/01/13 08:50:50 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * player.c -- Player Character Utility Functions.                            *
 ******************************************************************************/

/* Local Headers */
#include "player.h"
#include "item.h"

/******************************************************************************
 * FUNCTION:    init_stat_data
 * ARGUMENTS:   STAT_DAT* d
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
inline bool init_stat_data( STAT_DAT* d )
{
	d->food = 0;
	d->turn = 0;
	d->family = 0;
	d->hut_qt = 0;
	return true;//WARNING Always succeeds
}/* end init_stat_data func */


/******************************************************************************
 * FUNCTION:    init_player
 * ARGUMENTS:   PLAYER* p
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool init_player(PLAYER* p)
{
	int i = 0;

	if(p->name == NULL) return false;

	for( i = 0; i < MAX_STATS; i++ ) p->stats[i] = rng(MAX_STAT_VAL);
	for( i = 0; i < MAX_HOLD; i++ )
		if( set_empty_item( &p->inventory[i] ) != true ) return false;

    for( i = 0; i < MAX_SLOTS; i++ ) p->equip[i] = NULL;

	p->hp[0] = p->stats[CON];
	p->hp[1] = p->hp[0];
	p->explv = 0.0;
	p->money = 0;
	p->locr = NOT_PLACED;
	p->locc = NOT_PLACED;
	p->maplv = HVILLAGE;

	p->is_main = true;
	p->is_alive = true;
	p->is_human = false;
	p->is_awake = true;

//	p->attdir = NO_ACTION;

	return true;
}/* end init_player func */


/******************************************************************************
 * FUNCTION:    init_mon
 * ARGUMENTS:   PLAYER* who, int k, int innocent_qt, int hero_iqt
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool init_mon(PLAYER* who, int k, int innocent_qt, int hero_iqt )// who -> mon[k]
{
	int t, s, q, m, n, stat_seed, i = 0;
    bool fail = false;
	FILE* datafile;
	char c, b[2];

    /* Open Data File If Needed */
	if( k > MAX_FAM ) {

        /* READ IN DATA FILE */
		if( !( datafile = fopen( MON_DAT, READ_ONLY ) ) )
			return( (bool)ERROR( NULL, " Missing \"human.dat\" file", (int)false ) );

        /* Get to Max Types Location in datafile */
		do{ c = fgetc(datafile); } while( c != '$' );
		if( ( k + MAX_FAM ) < innocent_qt ) t = 0;//innocent
		else if( ( k + MAX_FAM + innocent_qt ) < hero_iqt )//hero
		{
			t = 2;
			do{ c = fgetc(datafile); }while( c != ':' );
			do{ c = fgetc(datafile); }while( c != ':' );
		}/* end hero if */
		else//common
			do{ c = fgetc(datafile); t = 1; }while( c != ':' );
        /* Get Max Types and conintue */
		b[0] = fgetc(datafile);	b[1] = '\0';
		s = atoi(b);
		s = rng(s);
		do{ c = fgetc(datafile); }while( ( !feof(datafile) )&&( c != '\n' ) );
	}/* end mon not fam if */

    /* Perform Initiliazations */
	if( k < MAX_FAM )//Family Member
	{
		who->is_human = false;
		stat_seed = ( MAX_STAT_VAL /2 ) +1;
		who->money = 0;
		who->explv = 0.0;
	}/* end fam if */
	else if( ( k + MAX_FAM ) < innocent_qt )//Innocent Bystander
	{
		who->is_human = true;
		stat_seed = MAX_STAT_VAL /2;
		who->money = 0;
		who->explv = 0.05;
	}/* end innocent if */
	else if( ( k + MAX_FAM + innocent_qt ) < hero_iqt )//Hero
	{
		who->is_human = true;
		stat_seed = MAX_STAT_VAL + ( MAX_STAT_VAL /2 );
		who->money = rng(10);//TODO Make meaningful
		who->explv = 0.15;//TODO Make meaningful
	}/* end hero if */
	else//Commoner
	{
		who->is_human = true;
		stat_seed = MAX_STAT_VAL;
		who->money = rng(4);//TODO Make meaningful
		who->explv = 0.1;
	}/* end commoner else */

	for( i = 0; i < MAX_STATS; i++ ) who->stats[i] = rng(stat_seed);
	who->locr = NOT_PLACED; who->locc = NOT_PLACED;
	who->maplv = NOT_PLACED;
	who->is_main = false;
	who->is_alive = true;
	who->attdir = WAIT;
	who->is_awake = false;
	who->hp[0] = who->stats[CON];
	who->hp[1] = who->hp[0];
	for( i = 0; i < MAX_HOLD; i++ )
		if( ( set_empty_item( &who->inventory[i] )  ) != true ) fail = true;
   for( i = 0; i < MAX_SLOTS; i++ )	who->equip[i] = NULL;

	who->name[0] = ' '; who->name[1] = '\0';//TODO testing tmp change etc

	if( k < MAX_FAM )//Early Finish for Family//They do not have equipment
	{
		if( fail == true ) return false;
		else return true;
	}/* end early finish if *///for family

	while( !feof(datafile) )
	{
		c = fgetc(datafile);
	/* Skip Over Comment Lines */
		if( ( c ) == '#' ) while( fgetc(datafile) != '\n' ) ;//if( feof(datafile) ) return false/*error("BARF! Reading Comment", m )*/;
		else if( c == '\n' );
		else if( c == ':' )//Read in Entries
		{
			b[0] = fgetc(datafile);	b[1] = '\0'; c = atoi(b);
			if( c == t )
			{
				if( --s == 0 )//Found Correct Entry
				{
					fgetc(datafile); //Bypass ':'
					b[0] = fgetc(datafile);	b[1] = '\0';
					q = atoi(b); q = rng(q);
					fgetc(datafile); //Bypass ':'
					for( i = 0; i < q; i++ )
					{
						b[0] = fgetc(datafile);	b[1] = '\0'; m = atoi(b);
						fgetc(datafile); //Bypass ':'
						b[0] = fgetc(datafile);	b[1] = '\0'; n = atoi(b);
						fgetc(datafile); //Bypass ':'
						if( getp_item( &who->inventory[0], m, n ) == false ) fail = true;
					}/* end q for */
					do{ c = fgetc(datafile); }while( ( !feof(datafile) )&&( c != '@' ) );
				/* Get Name */
					i = 0;
					while( ( ( c = fgetc(datafile) ) != ':' )&&( !feof(datafile) ) )
					{
						if( i > MAX_NAME_LEN )
						{
							fclose(datafile);
							return(ERROR(NULL," Item Name Too Long",i));//TODO: Make truncate name instead
						}/* end name length check if */
						else if( ( c < 65 )||( c > 122 ))//ASCII
						{
							fclose(datafile);
							return(ERROR(NULL,"Bad Item Name Input",c));
						}/* end ASCII rang check if */
						else/* havn't exceeded MAX_ITEM_NAME_LEN and is a character */
							who->name[i++] = c;
					}/* end get item name while */
					who->name[i] = '\0';
				}/* end found correct entry if */
				else while( fgetc(datafile) != '\n' );//Get to next line
			}/* end (c==t) if */
			else while( fgetc(datafile) != '\n' );//Get to next line
		}/* end line begins with ':' if */
	}/* end feof while */

	fclose(datafile);
	return fail;
}/* end init_mon func */



/************************************EOF***************************************/
