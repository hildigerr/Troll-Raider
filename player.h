/* $Id: player.h,v 1.2 2014/01/13 04:43:47 moonsdad Exp $ */
/******************************************************************************
 * player -- Player Character Utility Functions.                              *
 ******************************************************************************/
#pragma once

#include "types.h"
#include "calc.h"
#include "item.h"
#include "game.h"


/******************************************************************************
 * FUNCTION:    init_stat_data
 * ARGUMENTS:   STAT_DAT* d
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
inline int init_stat_data( STAT_DAT* d );


/******************************************************************************
 * FUNCTION:    init_player
 * ARGUMENTS:   PLAYER* p
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
int init_player(PLAYER* p);


/******************************************************************************
 * FUNCTION:    init_mon
 * ARGUMENTS:   PLAYER* who, int k, int innocent_qt, int hero_iqt
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool init_mon(PLAYER* who, int k, int innocent_qt, int hero_iqt );



/************************************EOF***************************************/
