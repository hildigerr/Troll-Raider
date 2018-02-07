/* $Id: item.h,v 1.3 2014/01/13 06:26:02 moonsdad Exp $ */
/******************************************************************************
 * item -- item Utility Functions.                                            *
 ******************************************************************************/
#pragma once

/* Local Header */
#include "types.h"


/******************************************************************************
 * FUNCTION:    getp_item   -- Get Particular Item                            *
 * ARGUMENTS:   ITEM* itm   -- The Item Being Generated                       *
 *              int t       -- The Item Type                                  *
 *              int m       -- The Item Index                                 *
 * RETURNS:     bool                                                          *
 * WARNING: Data line validity is not verified.                               *
 ******************************************************************************/
bool getp_item( ITEM* itm, int t, int m );


/******************************************************************************
 * FUNCTION:    set_empty_item                                                *
 * ARGUMENTS:   ITEM *   itm                                                  *
 ******************************************************************************/
void set_empty_item( ITEM * itm );


/******************************************************************************
 * FUNCTION:    slot_of
 * ARGUMENTS:   ITEM* ptr
 * RETURNS:     int
 ******************************************************************************/
int slot_of( const ITEM* ptr );


/******************************************************************************
 * FUNCTION:    is_equipable
 * ARGUMENTS:   ITEM* im
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool is_equipable( ITEM* im );

/******************************************************************************
 * FUNCTION:    swap_item
 * ARGUMENTS:   ITEM* itmi
 * WARNING:
 * NOTE:
 ******************************************************************************/
void swap_item( ITEM* itmi, ITEM* itmj );


/******************************************************************************
 * FUNCTION:    pick_up
 * ARGUMENTS:   ITEM* itm_ol
 *              ITEM* itm_nu
 * WARNING:
 * NOTE:        XXX UNUSED XXX
 ******************************************************************************/
void pick_up( ITEM* itm_ol, ITEM* itm_nu );



/************************************EOF***************************************/
