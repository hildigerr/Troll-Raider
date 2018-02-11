/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * item.c -- item Utility Functions.                                          *
 ******************************************************************************/

#include "item.h"
#include "dat.h"

#define ITEM_DATA_ELEMENT_QT 5

static DATA * data = NULL;

/******************************************************************************
 * FUNCTION:    getp_item      -- Get Particular Item                         *
 * ARGUMENTS:   int    t       -- The Item Type                               *
 *              int    m       -- The Item Index                              *
 * RETURNS:     ITEM *         -- The Item Generated                          *
 * WARNING: XXX Data line validity is not verified.                           *
 ******************************************************************************/
ITEM * getp_item( int t, int m )
{
    int i, j, ix[ITEM_DATA_ELEMENT_QT];
    char ** s, * cptr;
    ITEM * itm = MALLOC( ITEM );

    if( !itm ) {
        Error( "Failed to allocate memory for ITEM.", sizeof(ITEM) );
        return NULL;
    }/* End !MALLOC If */

    /* Load Item Data */
    if( !data && !(data = load_data( ITEM_DAT )) ) {
        Error( "Unable to load data", t );
        return NULL;
    } else s = data->lines;

    /* Verify t Param */
    if(( t < 0)||(t > MAX_ITEM_TYPES)) {
        Error( "Invalid t input parameter", t );
        return NULL;
    }/* End t in range If */

    /* Verify m Param */
    if( m < 0 ) m = rng( data->max[t] );
    else if( m > data->max[t] ) {
        Error( "Invalid m input parameter", m );
        return NULL;
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
        return NULL;
    }/* End t If */

    if(( itm->type == REACH )||( itm->type == RANGE2 )) itm->is_2handed = true;
    else itm->is_2handed = false;

    /* Get Item Stats */
    for( i = 0; i < MAX_ITEM_STATS; i++ )
        itm->stats[i] = atoi( &(cptr[ix[i]+1]) );

    /* Get Item Worth */
    itm->worth = atoi( &(cptr[ix[i]+1]) );

    /* Get Item Name */
    ++i; stricpy( itm->name, &(cptr[ix[i]+1]), MAX_ITEM_NAME_LEN+1 );

    /* Replace ':' Delimination Characters */
    for( i = 0; i < ITEM_DATA_ELEMENT_QT; i++ ) cptr[ix[i]] = ':';

    /* TODO Adjust Item Based on Lv and Whatever? */

    return itm;
}/* end getp_item func */


/******************************************************************************
 * FUNCTION:    slot_of         -- What is the equipment slot for the item?   *
 * ARGUMENTS:   ITEM * ptr      -- The item in question.                      *
 * RETURNS:     int             -- A slot or MAX_SLOTS if not equipable.      *
 * NOTE: Valid euipments slots are WEP, OFF, ARM, and HAT.                    *
 *       WEP items may be used in the OFF hand. OFF items are two handed.     *
 ******************************************************************************/
int slot_of( const ITEM * ptr )
{
    assert( ptr != NULL );
    switch( ptr->type ) {
        case MELEE: case SHIELD: case RANGE1:   return WEP;
        case REACH: case RANGE2:                return OFF;
        case TYPARM:                            return ARM;
        case TYPHAT:                            return HAT;
        default:                                return MAX_SLOTS;
    }/* End type Switch */
}/* end slot_of func */


/******************************************************************************
 * FUNCTION:    is_equipable        -- Is the item equipable?                 *
 * ARGUMENTS:   ITEM * itm          -- The item in question.                  *
 * RETURNS:     bool                                                          *
 ******************************************************************************/
bool is_equipable( ITEM * itm )
{
    assert( itm != NULL );
    if( itm->type < MAX_ITEM_TYPES ) return true;
    else return false;
}/* end is_equipable func */



/************************************EOF***************************************/
