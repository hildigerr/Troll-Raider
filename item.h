/* $Id: item.h,v 1.2 2013/05/06 05:04:02 moonsdad Exp $ */
/******************************************************************************
 * item -- item Utility Functions.                                            *
 ******************************************************************************/
#pragma once

/* Local Header */
#include "types.h"


/******************************************************************************
 * FUNCTION:    getp_item
 * ARGUMENTS:   ITEM* itm   -- The Item Being Generated
 *              int t       -- The Item Type
 *              int m       -- 
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int getp_item(ITEM* itm, int t, int m );


/******************************************************************************
 * FUNCTION:    set_empty_item
 * ARGUMENTS:   ITEM*   itm
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int set_empty_item(ITEM* itm);


/******************************************************************************
 * FUNCTION:    slot_of
 * ARGUMENTS:   ITEM* ptr
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int slot_of( ITEM* ptr );


/******************************************************************************
 * FUNCTION:    is_equipable
 * ARGUMENTS:   ITEM* im
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int is_equipable( ITEM* im );

/******************************************************************************
 * FUNCTION:    swap_item
 * ARGUMENTS:   ITEM* itmi
 * WARNING:
 * NOTE:
 ******************************************************************************/
void swap_item(ITEM* itmi, ITEM* itmj );


/******************************************************************************
 * FUNCTION:    pick_up
 * ARGUMENTS:   ITEM* itm_ol
 *              ITEM* itm_nu
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int pick_up(ITEM* itm_ol, ITEM* itm_nu);



/************************************EOF***************************************/
