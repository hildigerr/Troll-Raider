/* $Id: player.h,v 1.4 2014/01/13 08:50:50 moonsdad Exp $ */
/******************************************************************************
 * player -- Player Character Utility Functions.                              *
 ******************************************************************************/
#pragma once

/* Local Header */
#include "calc.h"
#include "types.h"

/******************************************************************************
 * FUNCTION:    init_stat_data
 * ARGUMENTS:   STAT_DAT* d
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
 bool init_stat_data( STAT_DAT* d );


/******************************************************************************
 * FUNCTION:    init_player
 * ARGUMENTS:   PLAYER* p
 * RETURNS:     int
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool init_player(PLAYER* p);


/******************************************************************************
 * FUNCTION:    init_mon
 * ARGUMENTS:   PLAYER* who, int k, int innocent_qt, int hero_iqt
 * RETURNS:     bool
 * WARNING:
 * NOTE:
 ******************************************************************************/
bool init_mon(PLAYER* who, int k, int innocent_qt, int hero_iqt );



/************************************EOF***************************************/
