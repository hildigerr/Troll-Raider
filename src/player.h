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
 * FUNCTION:    equip_me           -- Unequip an equipped item.               *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              int      slot      -- Which item will be equiped?             *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
bool equip_me( PLAYER * who, int slot, bool verbose );
#define equip_item(x) equip_me(x,-1,true)


/******************************************************************************
 * FUNCTION:    unequip_me         -- Unequip an equipped item.               *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              int      slot      -- Which equipment slot will be removed?   *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
bool unequip_me( PLAYER * who, int slot, bool verbose );
#define unequip(p) unequip_me(p,-1,true)


/******************************************************************************
 * FUNCTION:    destroy_my_item    -- Destroy an item.                        *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              int      slot      -- Which slot contains the item?           *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
bool destroy_my_item( PLAYER * who, int slot, bool verbose );
#define destroy_item(p) destroy_my_item(p,-1,true)


/******************************************************************************
 * FUNCTION:    drop_my_item       -- Drop an item.                           *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              LOC    * where     -- Where will the item be dropped?         *
 *              int      slot      -- Which slot contains the item?           *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
bool drop_my_item( PLAYER * who, LOC * where, int slot, bool verbose );
#define drop_item(p,a) drop_my_item(p,a,-1,true)


/************************************EOF***************************************/
