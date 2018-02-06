/* $Id: game.h,v 1.5 2014/01/13 08:50:49 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * game.c -- Game Utility Functions.                                          *
 ******************************************************************************/
#pragma once

/* Local Header */
#include "cwin.h"

/******************************************************************************
 * FUNCTION:    get_cmd, get_subi_cmd                                         *
 * RETURNS:     int         see definitions in types.h                        *
 ******************************************************************************/
 int get_cmd( void );


/******************************************************************************
 * FUNCTION:    get_hand                                                      *
 * RETURNS:     int         -- see definitions in types.h                     *
 ******************************************************************************/
 int get_hand(void);


/******************************************************************************
 * FUNCTION:skill_check
 * ARGUMENTS:int ability, int adjust
 * RETURNS:bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
 bool skill_check( int ability, int adjust );


/******************************************************************************
 * FUNCTION:    manage_inventory                                              *
 * ARGUMENTS    PLAYER *  pc          -- The Player's Character               *
 *              LOC *     active_loc  -- The Player's Location (For Dropping) *
 *              int       cmd         -- The inventory command                *
 * RETURNS:     bool         -- Does the player still need to take this turn? *
 * The commands are:                                                          *
 *    INVENTORY -- View it and maybe do one of the others as a sub command:   *
 *    EQUIPMENT -- Equip an item in inventory for use.                        *
 *    DROP_ITEM -- Drop an item from inventory or equipment.                  *
 *    REMOVE_ITEM -- Unequip an item, keeping it ininventory.                 *
 *    DESTROY_ITEM -- Destroy an item so that it no longer exists.            *
 ******************************************************************************/
 bool manage_inventory( PLAYER* pc, LOC* active_loc, int cmd );


/************************************EOF***************************************/
