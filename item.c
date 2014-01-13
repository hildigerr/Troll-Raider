/* $Id: item.c,v 1.5 2014/01/13 04:43:47 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * item.c -- item Utility Functions.                                          *
 ******************************************************************************/

#include "item.h"

/******************************************************************************
 * FUNCTION:    getp_item
 * ARGUMENTS:   ITEM* itm   -- The Item Being Generated
 *              int t       -- The Item Type
 *              int m       --
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int getp_item(ITEM* itm, int t, int m )
{
	int i, s[MAX_ITEM_STATS];
	char c, b[2];
	FILE* datafile;
	long here;
	bool done;

    /* READ IN DATA FILE */
	if( ( datafile = fopen( ITEM_DAT, READ_ONLY ) ) == NULL )
		return( ERROR( NULL, "Missing \"item.dat\" file", FAIL ) );

    /* Initialize Necessary Variables */
	done = false;
	itm->worth = NOT_PLACED;
	for( i = 0; i < MAX_ITEM_STATS; i++ ) s[i] = NOT_PLACED;

	if(( t < 0)||(t > MAX_ITEM_TYPES))
        return( ERROR( NULL, "bad t input to getp_item func", t ) );
	else itm->type = t;

	if(( itm->type == REACH )||( itm->type == RANGE2 )) itm->is_2handed = true;
	else itm->is_2handed = false;

	while(done == false) {
		while( !feof(datafile) ) {

            /* Skip Over Comment Lines */
			if( ( c = fgetc(datafile) ) == '#' )
                while( fgetc(datafile) != '\n' )
                    /*ERROR("BARF! Reading Comment", m )*/;

            /* Read In Maximum Items For This Type */
			else if( c == '$' )
			{
                /* Go To The Next Line */
				while( fgetc(datafile) != '\n' );
                /* Record Position -- Should Not Have to rewind before here */
				here = ftell(datafile);
				if( here < 0 ) //( fgetpos(datafile,here) != 0 )//
				{
					fclose(datafile);
					return( ERROR( NULL, "Failed to record file position "
                                 "in \"item.dat\" file!" , m ) );
				}/* end get position if *//*now:end position verification if */
			}/* end line begins with '$' if */

            /* Read In Entries */
			else if( c == ':' ) {
				//ERROR("BARF! Found an Item entry", c);
                /* Check if we should be done *///Report output
				if(done == true)//DAY6BUG Infinite Loop Checker
				{
					#ifdef DEBUG
					if( itm->is_ == true )
                        ERROR(NULL,"item is_ set true", MAX_ITEM_STATS);
					ERROR(NULL,"Item type", itm->type );
					ERROR(NULL,"Item Worth",itm->worth );
					for( i = 0 ; i < MAX_ITEM_STATS ; i++ ) {
						ERROR(NULL,"Stat number",i);
						ERROR(NULL,"stat quality",itm->stats[i]);
					}/* end MAX_ITEM_STATS for */
					ERROR(NULL,itm->name,MAX_ITEM_NAME_LEN);
					mypause();//DO NOT SUPPRESS
					#endif
				}/* end done == true : DAY6BUG Infinite Loop Checker */

                /* Process Data Table */
				else /* done == false */
				{
                    /* Verify Readiness */
					if( ( m < 0)||( m > MAX_ITEM_PER_TYPE ) ) {
						fclose(datafile);
						return( ERROR( NULL, "Bad item Generated", m ) );
					}/* end m out of range if */
					else/* m is in range */ {
                        /* Get Item Type */
						b[0] = fgetc(datafile);	b[1] = '\0';//shouldn't need  to change b[1]
						c = atoi(b); //ERROR(" BARF: Got Item Type =", c );
                        /* Verify That We Havn't Passed Our Item's Type Section */ //SHOULD BE UNNECESSARY
						if( c > itm->type ) {
							ERROR( NULL, "BARF: GOT PASED OUR ITEM"
                                            " : itm->type ==",itm->type); //mypause();
							if( fseek(datafile,here,SEEK_SET) != 0 ) //( fsetpos( datafile, here ) != 0 )
							{
								fclose(datafile);
								return( ERROR( NULL, "Failed to rewind datafile",c ) );
							}/* end rewind failed if */
						}/* end need rewind if */

                        /* Process The Section if of Correct Type */
						else if( c == itm->type ) {
							//ERROR("BARF: Found correct item type section", itm->type); //mypause();
                            /* Find Correct Listing In This Section */
							while( m >= 0 )//Searching...
							{
								if( m-- == 0 )//FOUND IT!
								{
									fgetc(datafile);//bypass ':'
                                    /* Get Item Stats */
									for( i = 0; i < MAX_ITEM_STATS; i++ )
									{
										//ERROR("BARF: Getting item stat",i);	//mypause();
										while( ( ( c = fgetc(datafile) ) != ':' )&&( !feof(datafile) ) )
										{
											if(( c < 48 )||(c > 57)) { ERROR( NULL, "BAD INPUT", c ); mypause(0); }
											//ERROR("BARF: Converting to int", c);
											b[0] = c; b[1] = '\0'; c = atoi(b);//ERROR("BARF: IS now int", c);
											if( s[i] == NOT_PLACED ) s[i] = c;
											else s[i] = ( ( s[i] * 10 ) + c );
											//ERROR("BARF: Got item stat", s[i]);
										}/* end !end of current number or file while */
									}/* end get item stats for */
                                    /* Get Item Worth */
									//ERROR("BARF: Getting Item Worth", c );
									while( ( ( c = fgetc(datafile) ) != ':' )&&( !feof(datafile) ) )
									{
										if(( c <48 )||(c > 57)) { ERROR(NULL,"BAD INPUT", c); mypause(0); }
										//ERROR("BARF: Converting to int", c);
										b[0] = c; b[1] = '\0';
										if( itm->worth == NOT_PLACED ) itm->worth = atoi(b);
										else itm->worth = ( ( itm->worth * 10 ) + atoi(b) );
										//ERROR("BARF: Got item worth", itm->worth ); //mypause();
									}/* end get item worth while */
                                    /* Get Item Name */
									i = 0;
									while( ( ( c = fgetc(datafile) ) != ':' )&&( !feof(datafile) ) )
									{
										//ERROR("BARF: Getting Item Name",c); //mypause();
										if( i > MAX_ITEM_NAME_LEN )
										{
											fclose(datafile);
											return(ERROR(NULL, "Item Name Too Long",i));//TODO: Make truncate name instead
										}/* end name length check if */
										else if( ( c < 65 )||( c > 122 ))//ASCII
										{
											fclose(datafile);
											return(ERROR(NULL,"Bad Item Name Input",c));
										}/* end ASCII rang check if */
										else/* havn't exceeded MAX_ITEM_NAME_LEN and is a character */
											itm->name[i++] = c;
									}/* end get item name while */
									itm->name[i] = '\0';
                                    /* Set other Item Flags */
									done = true;
									//ERROR("BARF: ITEM DONE!",i);//mypause();
								}/* end m ==-- Found Correct Listing In section if */
								else //Go to next Listing
								{
									while( fgetc(datafile) != '\n' ) /* ERROR("BARF: Skipping This Listing M=",m) */;
									fgetc(datafile);//bypass ':'
									if( (c = fgetc(datafile) ) != itm->type ) /*ERROR("DAY6Bug--day12revisit",FAIL)*/;//Have to skip even if is wrong//shouldn't be wrong
									//fgetc(datafile);
								}/* end skip line in correct section else */
							}/* end m>= 0 while : Done Searching */
						}/* end c == itm->type if : Finished Processing Correct Section */
						else /* Havn't Reached Our Section Yet */
							while( fgetc(datafile) != '\n' ) /*ERROR("BARF: Skipping section", c)*/;
					}/* end m within range else */
				}/* end done == false else : Processing Data Table Complete */
			}/* end line begins with ':' if */
            /* Skip Blank Lines */
			else if( c == '\n' ) /* ERROR("BARF: BLANK LINE" , c)*/;//continue;
			else /* Catch Any Unexpected Input and Report it */
			{
				fclose(datafile);
				return( ERROR( NULL, "Unexpected Input From \"Item.dat\" file", c) );
			}/* end unexpected input else */
			//ERROR("BARF: SHOULD EXIT NOW IF PAUSED",c);
			//if( done == true ) mypause();//TESTING
			if( done == true ) break;//Force Completion//
		}/* end !feof while */
		//ERROR("BARF: FOUND END OF FILE OR BROKE OUT OF LOOP",c);
		if( done != true )
		{
			fclose(datafile);
			return(ERROR(NULL,"Item data not found. Possible broken datafile.",FAIL));
		}/* end !done if */
	}/* end !done while */

	//ERROR("BARF: Successfully initialized item!", itm->type );//mypause();

    /* Adjust Item Based on Lv and Whatever *///no adjustment for now
	for( i = 0; i < MAX_ITEM_STATS; i++ )
	{
		itm->stats[i] = s[i];

	}/*end i<MAX_ITEM_STATS for*/

    /* Set Item Flags */
	itm->is_ = true;
	itm->is_equipped = false;

	fclose(datafile);
	return 0;
}/* end getp_item func */


/******************************************************************************
 * FUNCTION:    set_empty_item
 * ARGUMENTS:   ITEM*   itm
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int set_empty_item(ITEM* itm)
{
	int i;

	if( strncpy(itm->name, "empty", MAX_ITEM_NAME_LEN) == NULL )
        return FAIL;

	itm->is_ = false;
	itm->is_equipped = false;
	itm->is_2handed = false;
	itm->type = NOT_PLACED;

	for(i=0; i < MAX_ITEM_STATS; i++ )
	{
		itm->stats[i] = 0;

	}/*end i<MAX_ITEM_STATS for*/

	itm->worth = 0;
	return itm->worth;//for fun
}/*end set_empty_item func */


/******************************************************************************
 * FUNCTION:    slot_of
 * ARGUMENTS:   ITEM* ptr
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int slot_of( ITEM* ptr )
{
	switch( ptr->type )
	{
		case MELEE: return WEP;//melee
		case REACH: return OFF;//reach
		case RANGE1: return WEP;//range//was off
		case RANGE2: return OFF;
		case SHIELD: return WEP;//shields//was off
		case TYPHAT: return HAT;//hats
		case TYPARM: return ARM;//body
		default: return WEP;//TODO figure out safe way to handle exceptions//was BAD_SLOT
	}
}/* end slot_of func */


/******************************************************************************
 * FUNCTION:    is_equipable
 * ARGUMENTS:   ITEM* im
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int is_equipable( ITEM* im )//Will change as more types of items are added
{
	if(( im->type > 0 )&&(im->type < MAX_ITEM_TYPES))
		return true;
	else
		return false;
}/* end is_equipable func */


/******************************************************************************
 * FUNCTION:    swap_item
 * ARGUMENTS:   ITEM* itmi
 * WARNING:
 * NOTE:
 ******************************************************************************/
void swap_item(ITEM* itmi, ITEM* itmj )
{
	ITEM tmp;
	int i;

    /* Swap Names */
	strcpy( tmp.name , itmi->name );
   strcpy( itmi->name, itmj->name);
	strcpy( itmj->name, tmp.name );

//	tmp.name = itmi->name;
//	itmi->name = itmj->name;
//	itmj->name = tmp.name;

    /* Swap Flags */
	tmp.is_ = itmi->is_;
	itmi->is_ = itmj->is_;
	itmj->is_ = tmp.is_;

	tmp.is_2handed = itmi->is_2handed;
	itmi->is_2handed = itmj->is_2handed;
	itmj->is_2handed= tmp.is_2handed;

	tmp.is_equipped = itmi->is_equipped;
	itmi->is_equipped = itmj->is_equipped;
	itmj->is_equipped = tmp.is_equipped;

    /* Swap Types */
	tmp.type = itmi->type;
	itmi->type = itmj->type;
	itmj->type = tmp.type;


    /* Swap Stats */
	for( i = 0; i < MAX_ITEM_STATS; i++ )
	{
		tmp.stats[i] = itmi->stats[i];
		itmi->stats[i] = itmj->stats[i];
		itmj->stats[i] = tmp.stats[i];
	}/* end MAX_ITEM_STATS for */

    /* Swap Worth */
	tmp.worth = itmi->worth;
	itmi->worth = itmj->worth;
	itmj->worth = tmp.worth;

}/* end swap_item func */


/******************************************************************************
 * FUNCTION:    pick_up
 * ARGUMENTS:   ITEM* itm_ol
 *              ITEM* itm_nu
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int pick_up(ITEM* itm_ol, ITEM* itm_nu)
{
	int i;

	strcpy(itm_nu->name, itm_ol->name);

	itm_nu->is_ = itm_ol->is_;
	itm_nu->is_2handed = itm_ol->is_2handed;

	itm_nu->is_equipped = false;//Cannot pickup and equip at once
	itm_nu->type = itm_ol->type;

	for(i=0; i < MAX_ITEM_STATS; i++ )
	{
		itm_nu->stats[i] = itm_ol->stats[i];

	}/*end i<MAX_ITEM_STATS for*/

	itm_nu->worth = itm_ol->worth;

	return 0;/*WARNING: always succeeds*/
}/* end pick_up func */



/************************************EOF***************************************/
