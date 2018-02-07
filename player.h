/* $Id: player.h,v 1.4 2014/01/13 08:50:50 moonsdad Exp $ */
/******************************************************************************
 * player -- Player Character Utility Functions.                              *
 ******************************************************************************/
#pragma once

/* Local Header */
#include "calc.h"
#include "types.h"


/******************************************************************************
 * FUNCTION:    init_stat_data                                                *
 * ARGUMENTS:   STAT_DAT * d                                                  *
 ******************************************************************************/
 void init_stat_data( STAT_DAT * d );


/******************************************************************************
 * FUNCTION:    init_player                                                   *
 * ARGUMENTS:   PLAYER * p                                                    *
 * RETURNS:     bool                                                          *
 * NOTE:        Expects player's name to already be set.                      *
 ******************************************************************************/
 bool init_player( PLAYER * p);


/******************************************************************************
 * FUNCTION:    init_mon        -- Initialize Monsters (NPCS)                 *
 * ARGUMENTS:   PLAYER *  who   -- The thing being initialized.               *
 *              int        t    -- The type of monster to generate.           *
 * RETURNS:     bool            -- True on success.                           *
 *  NPC types are: NPC_TTROLL, HUMAN_INCT, HUMAN_COMN, and HUMAN_HERO         *
 * NOTE: NPC_TTROLL[0] is the Player.                                         *
 * TODO: Perhaps do rng(data->qt[QT_LINES]) and remove lines for uniques.     *
 ******************************************************************************/
 bool init_mon( PLAYER* who, int t );



/************************************EOF***************************************/
