/* $Id: game.h,v 1.3 2014/01/13 04:43:47 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * game.c -- Game Utility Functions.                                          *
 ******************************************************************************/
#pragma once

#define VERSION "4.1"

/* System Header */
#include <ctype.h>

/* Local Headers */
#include "types.h"
#include "cwin.h"
#include "calc.h"


/******************************************************************************
 * FUNCTION:    get_cmd, get_subi_cmd                                         *
 * RETURNS:     int         see definitions in types.h                        *
 ******************************************************************************/
inline int get_cmd( void );
inline int get_subi_cmd( void );


/******************************************************************************
 * FUNCTION:    get_slot                                                      *
 * ARGUMENTS:   char t      -- Type of query (see NOTE).                      *
 * RETURNS:     int         -- The chosen inventory slot.                     *
 * NOTE:            (t==i): asking for inventory slot                         *
 *                  (t==u): asking or either inventory or equipment slot      *
 *                  (t==e): asking for equipment slot                         *
 * if input is a_d and t==u then returns index dereferencer +10 in order to   *
 *  distinguish between i and e                                               *
 ******************************************************************************/
inline int get_slot(char t);


/******************************************************************************
 * FUNCTION:    get_hand                                                      *
 * RETURNS:     int         -- see definitions in types.h                     *
 ******************************************************************************/
inline int get_hand(void);


/******************************************************************************
 * FUNCTION:    story_line                                                    *
 * ARGUMENTS:   short   when    --  Storyline Time (eg: INTRO, END, etc...)   *
 *              WINDOW* where   --  Window to display the message to.         *
 * RETURNS:     int                                                           *
 ******************************************************************************/
/* Storyline Time Definitions */
#define INTRO     0
#define BEGINNING 1
#define MIDDLE    2
#define END       3
inline int story_line( short when, WINDOW* where );


/******************************************************************************
 * FUNCTION:skill_check
 * ARGUMENTS:int ability, int adjust
 * RETURNS:int
 * WARNING:
 * NOTE:
 ******************************************************************************/
inline int skill_check( int ability, int adjust );



/************************************EOF***************************************/
