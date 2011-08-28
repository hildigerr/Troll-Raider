
/*INDEX*   --   ordered list of functions contained in this file */
int slot_of( ITEM* ptr );
int is_equipable( ITEM* im );
void swap_item(ITEM* itmi, ITEM* itmj );
int pick_up(ITEM* itm_ol, ITEM* itm_nu );

/*****************************************************************/

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



int is_equipable( ITEM* im )//Will change as more types of items are added
{
	if(( im->type > 0 )&&(im->type < MAX_ITM_TYPES))
		return ONN;
	else
		return OFF;
}/* end is_equipable func */


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


int pick_up(ITEM* itm_ol, ITEM* itm_nu)
{
	int i;

	strcpy(itm_nu->name, itm_ol->name);

	itm_nu->is_ = itm_ol->is_;
	itm_nu->is_2handed = itm_ol->is_2handed;

	itm_nu->is_equipped = OFF;//Cannot pickup and equip at once
	itm_nu->type = itm_ol->type;

	for(i=0; i < MAX_ITEM_STATS; i++ )
	{
		itm_nu->stats[i] = itm_ol->stats[i];

	}/*end i<MAX_ITM_STATS for*/

	itm_nu->worth = itm_ol->worth;
	
	return 0;/*WARNING: always succeeds*/
}/* end pick_up func */
