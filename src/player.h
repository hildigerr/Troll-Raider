/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * player.h -- Player Character Utility Functions.                            *
 ******************************************************************************/
#pragma once

#include "types.h"


 /******************************************************************************
 * FUNCTION:    skill_check                                                   *
 * ARGUMENTS:   int     ability     -- The ability score being tested.        *
 *              int     adjust      -- Adjustment to the test.                *
 * RETURNS:     bool                -- Result of test                         *
 ******************************************************************************/
 bool skill_check( int ability, int adjust );


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
 bool init_mon( PLAYER * who, int t );


/******************************************************************************
 * FUNCTION:    unequip_me         -- Unequip an equipped item.               *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              int      slot      -- Which equipment slot will be removed?   *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
bool unequip_me( PLAYER * pc, int slot, bool verbose );
#define unequip(p) unequip_me(p,-1,true)


/************************************EOF***************************************/
