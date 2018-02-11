/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * item .h -- item Utility Functions.                                         *
 ******************************************************************************/
#pragma once

#include "types.h"


/******************************************************************************
 * FUNCTION:    getp_item      -- Get Particular Item                         *
 * ARGUMENTS:   int    t       -- The Item Type                               *
 *              int    m       -- The Item Index                              *
 * RETURNS:     ITEM *         -- The Item Generated                          *
 * WARNING: XXX Data line validity is not verified.                           *
 ******************************************************************************/
ITEM * getp_item( int t, int m );


/******************************************************************************
 * FUNCTION:    slot_of         -- What is the equipment slot for the item?   *
 * ARGUMENTS:   ITEM * ptr      -- The item in question.                      *
 * RETURNS:     int             -- A slot or MAX_SLOTS if not equipable.      *
 * NOTE: Valid euipments slots are WEP, OFF, ARM, and HAT.                    *
 *       WEP items may be used in the OFF hand. OFF items are two handed.     *
 ******************************************************************************/
int slot_of( const ITEM * ptr );


/******************************************************************************
 * FUNCTION:    is_equipable        -- Is the item equipable?                 *
 * ARGUMENTS:   ITEM * itm          -- The item in question.                  *
 * RETURNS:     bool                                                          *
 ******************************************************************************/
bool is_equipable( ITEM * itm );



/************************************EOF***************************************/
