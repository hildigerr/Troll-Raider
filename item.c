/* $Id: item.c,v 1.6 2014/01/13 06:26:02 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * item.c -- item Utility Functions.                                          *
 ******************************************************************************/

#include "item.h"
#include "dat.h"
#include "calc.h"

#define ITEM_DATA_ELEMENT_QT 5

static DATA * data = NULL;

/******************************************************************************
 * FUNCTION:    getp_item   -- Get Particular Item                            *
 * ARGUMENTS:   ITEM* itm   -- The Item Being Generated                       *
 *              int t       -- The Item Type                                  *
 *              int m       -- The Item Index                                 *
 * RETURNS:     bool                                                          *
 * WARNING: Data line validity is not verified.                               *
 ******************************************************************************/
bool getp_item( ITEM* itm, int t, int m )
{
    int i, j, ix[ITEM_DATA_ELEMENT_QT];
    char ** s, * cptr;

    /* Load Item Data */
    if( !data && !(data = load_data( ITEM_DAT )) ) {
        Error( "Unable to load data", t );
        return false;
    } else s = data->lines;

    /* Verify t Param */
    if(( t < 0)||(t > MAX_ITEM_TYPES)) {
        Error( "Invalid t input parameter", t );
        return false;
    }/* End t in range If */

    /* Verify m Param */
    if( m < 0 ) m = rng( data->max[t] ) - 1;
    else if( m > data->max[t] ) {
        Error( "Invalid m input parameter", m );
        return false;
    }/* end m out of range if */

    /* Find Selection */
    for( i=0; i < t; i++ ) for( j=0; j < data->max[i]; j++ ) ++s;
    for( i=0; i < m; i++ ) ++s;

    /* Slice Line XXX Line validity Unchecked */
    cptr = *s;
    for( i=0, j=1; i < ITEM_DATA_ELEMENT_QT; i++ ) {
        while( cptr[j] != ':' ) ++j;
        ix[i] = j;
        cptr[j] = '\0';
    }/* End ITEM_DATA_ELEMENT_QT For */

    /* Verify Item Type */
    i = atoi( &cptr[1] );
    if( i == t ) itm->type = t;
    else {
        Error( "Wrong item type in datafile", i );
        return false;
    }/* End t If */

    if(( itm->type == REACH )||( itm->type == RANGE2 )) itm->is_2handed = true;
    else itm->is_2handed = false;

    /* Get Item Stats */
    for( i = 0; i < MAX_ITEM_STATS; i++ )
        itm->stats[i] = atoi( &(cptr[ix[i]+1]) );

    /* Get Item Worth */
    itm->worth = atoi( &(cptr[ix[i]+1]) );

    /* Get Item Name */
    ++i; stricpy( itm->name, &(cptr[ix[i]+1]), MAX_ITEM_NAME_LEN );

    /* Replace ':' Delimination Characters */
    for( i = 0; i < ITEM_DATA_ELEMENT_QT; i++ ) cptr[ix[i]] = ':';

    /* TODO Adjust Item Based on Lv and Whatever? */

    /* Set Remaining Item Flags */
    itm->is_ = true;
    itm->is_equipped = false;

    return true;
}/* end getp_item func */


/******************************************************************************
 * FUNCTION:    set_empty_item                                                *
 * ARGUMENTS:   ITEM *   itm                                                  *
 ******************************************************************************/
void set_empty_item( ITEM* itm )
{
    int i;

    stricpy( itm->name, "empty", MAX_ITEM_NAME_LEN);

    itm->is_ = false;
    itm->is_equipped = false;
    itm->is_2handed = false;
    itm->type = NOT_PLACED;

    for(i=0; i < MAX_ITEM_STATS; i++ ) itm->stats[i] = 0;

    itm->worth = 0;
}/*end set_empty_item func */


/******************************************************************************
 * FUNCTION:    slot_of         -- What is the equipment slot for the item?   *
 * ARGUMENTS:   ITEM * ptr      -- The item in question.                      *
 * RETURNS:     int             -- A slot or MAX_SLOTS if not equipable.      *
 * NOTE: Valid euipments slots are WEP, OFF, ARM, and HAT.                    *
 *       WEP items may be used in the OFF hand. OFF items are two handed.     *
 ******************************************************************************/
int slot_of( const ITEM * ptr )
{
    switch( ptr->type ) {
        case MELEE: case SHIELD: case RANGE1:   return WEP;
        case REACH: case RANGE2:                return OFF;
        case TYPARM:                            return ARM;
        case TYPHAT:                            return HAT;
        default:                                return MAX_SLOTS;
    }/* End type Switch */
}/* end slot_of func */


/******************************************************************************
 * FUNCTION:    is_equipable
 * ARGUMENTS:   ITEM* im
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool is_equipable( ITEM* im )//Will change as more types of items are added
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
void swap_item( ITEM* itmi, ITEM* itmj )
{
    ITEM tmp;
    int i;

    /* Swap Names */
    stricpy( tmp.name,   itmi->name, MAX_ITEM_NAME_LEN );
    stricpy( itmi->name, itmj->name, MAX_ITEM_NAME_LEN );
    stricpy( itmj->name, tmp.name,   MAX_ITEM_NAME_LEN );

    /* Swap Flags */
    tmp.is_   = itmi->is_;
    itmi->is_ = itmj->is_;
    itmj->is_ = tmp.is_;

    tmp.is_2handed   = itmi->is_2handed;
    itmi->is_2handed = itmj->is_2handed;
    itmj->is_2handed = tmp.is_2handed;

    tmp.is_equipped   = itmi->is_equipped;
    itmi->is_equipped = itmj->is_equipped;
    itmj->is_equipped = tmp.is_equipped;

    /* Swap Types */
    tmp.type   = itmi->type;
    itmi->type = itmj->type;
    itmj->type = tmp.type;

    /* Swap Stats */
    for( i = 0; i < MAX_ITEM_STATS; i++ )
    {
        tmp.stats[i]   = itmi->stats[i];
        itmi->stats[i] = itmj->stats[i];
        itmj->stats[i] = tmp.stats[i];
    }/* end MAX_ITEM_STATS for */

    /* Swap Worth */
    tmp.worth   = itmi->worth;
    itmi->worth = itmj->worth;
    itmj->worth = tmp.worth;

}/* end swap_item func */


/******************************************************************************
 * FUNCTION:    pick_up
 * ARGUMENTS:   ITEM* itm_ol
 *              ITEM* itm_nu
 * RETURNS:     bool
 * WARNING:
 * NOTE:        XXX UNUSED XXX
 ******************************************************************************/
void pick_up( ITEM* itm_ol, ITEM* itm_nu )
{
    int i;

    stricpy( itm_nu->name, itm_ol->name, MAX_ITEM_NAME_LEN );

    itm_nu->is_ = itm_ol->is_;
    itm_nu->is_2handed = itm_ol->is_2handed;

    itm_nu->is_equipped = false;//Cannot pickup and equip at once
    itm_nu->type = itm_ol->type;

    for(i=0; i < MAX_ITEM_STATS; i++ )
        itm_nu->stats[i] = itm_ol->stats[i];

    itm_nu->worth = itm_ol->worth;
}/* end pick_up func */



/************************************EOF***************************************/
