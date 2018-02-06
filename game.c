/* $Id: game.c,v 1.6 2014/01/13 08:50:49 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * game.c -- Game Utility Functions.                                          *
 ******************************************************************************/

/* Local Headers */
#include "game.h"
#include "item.h"


/******************************************************************************
 * FUNCTION:    get_cmd                                                       *
 * RETURNS:     int         see definitions in types.h                        *
 ******************************************************************************/
 int get_cmd( void )
{
    switch( getch() ) {
        case '8': case KEY_UP:          return NORTH;
        case '2': case KEY_DOWN:        return SOUTH;
        case '6': case KEY_RIGHT:       return EAST;
        case '4': case KEY_LEFT:        return WEST;
        case '9':                       return NORTH_EAST;
        case '7':                       return NORTH_WEST;
        case '3':                       return SOUTH_EAST;
        case '1':                       return SOUTH_WEST;
        case '5':                       return WAIT;
        case 'q':
        case 'Q': case KEY_ESC:         return QUIT;
        case 'g': case ',':             return PICK_UP;
        case 'i': case 'I':             return INVENTORY;
        case 'e': case 'w':             return EQUIPMENT;
        case 'u': case 'T':             return REMOVE_ITEM;
        case 'd':                       return DROP_ITEM;
        case 'K':                       return DESTROY_ITEM;
        //TODO SAVE_GAME
        default:    return NO_ACTION;
    }/* End input Switch */
}/* End get_cmd Func */


/******************************************************************************
 * FUNCTION:    get_subi_cmd                                                  *
 * RETURNS:     int         see definitions in types.h                        *
 ******************************************************************************/
static int get_subi_cmd( void )
{
    switch( getch() ) {
        case 'f': case 'F': case KEY_ESC:   /* exit sub cmd prompt */
        case 'q': case 'Q': case ' ':          return SUB_FIN;

        case 'D': case 'd':                 /* drop an item */
                                               return DROP_ITEM;

        case 'E': case 'e': case 'r':       /* Equip/Ready an Item */
        case 'w': case 'W':                    return EQUIPMENT;

        case 'u': case 'U': case 'R':       /* Remove an Item */
        case 't': case 'T':                    return REMOVE_ITEM;

        case 'K': case 'k':                 /* Destroy an Item */
        case 'x': case 'X':                    return DESTROY_ITEM;

        default:                               return NO_ACTION;
    }/* End input Switch */
}/* End get_cmd Func */


/******************************************************************************
 * FUNCTION:    get_i_slot  -- Get Inventory Slot from user.                  *
 * RETURNS:     int         -- The chosen inventory slot.                     *
 ******************************************************************************/
 static int get_i_slot( void )
{
    int input = getch();
    switch ( input ) {
        case KEY_ESC: case ' ': case 'q': case 'Q': return CANCEL;
        case '1': case '2': case '3': case '4': case '5': case '6': case '7':
        case '8': case '9': case '0': return input - '0';
        default: return NOT_PLACED;
    }/* End input Switch */
}/* End get_i_slot Func */


/******************************************************************************
 * FUNCTION:    get_u_slot    -- Get either inventory or equipment slot       *
 * RETURNS:     int           -- The chosen slot.                             *
 * NOTE: Treats input 'a' through 'd' as hexidecimal to distinguish between   *
 *       inventory and equipment.                                             *
 ******************************************************************************/
 static int get_u_slot( void )
{
    int input = getch();
    switch ( input ) {
        case KEY_ESC: case ' ': case 'q': case 'Q':     return CANCEL;
        case 'a': case 'A': return 10;
        case 'b': case 'B': return 11;
        case 'c': case 'C': return 12;
        case 'd': case 'D': return 13;
        case '1': case '2': case '3': case '4': case '5': case '6': case '7':
        case '8': case '9': case '0': return input - '0';
        default: return NOT_PLACED;
    }/* End input Switch */
}/* End get_u_slot Func */


/******************************************************************************
 * FUNCTION:    get_e_slot  -- Get equipment slot from user.                  *
 * RETURNS:     int         -- The chosen equipment slot.                     *
 ******************************************************************************/
 static int get_e_slot( void )
{
    switch (getch()) {
        case KEY_ESC: case ' ': case 'q': case 'Q':     return CANCEL;
        case 'a': case 'A': case '1': return 0;
        case 'b': case 'B': case '2': return 1;
        case 'c': case 'C': case '3': return 2;
        case 'd': case 'D': case '4': return 3;
        default: return NOT_PLACED;
    }/* End input Switch */
}/* End get_e_slot Func */


/******************************************************************************
 * FUNCTION:    get_hand                                                      *
 * RETURNS:     int         -- see definitions in types.h                     *
 ******************************************************************************/
 int get_hand( void )
{
    switch( getch() ) {
        case '1': case 'r': case 'R': case 'a': case 'A': return WEP;
        case '2': case 'l': case 'L': case 'b': case 'B': return OFF;
        case '0': case 'q': case 'Q': case ' ': case KEY_ESC: return CANCEL;
        default: return NOT_PLACED;
    }/* End input Switch */
}/* End get_hand Func */


/******************************************************************************
 * FUNCTION:    skill_check                                                   *
 * ARGUMENTS:   int     ability     -- The ability score being tested.        *
 *              int     adjust      -- Adjustment to the test.                *
 * RETURNS:     bool                -- Result of test                         *
 ******************************************************************************/
 bool skill_check( int ability, int adjust )//TODO: Make more interesting
{
    return ( ( rng(MAX_STAT_VAL) + adjust ) < ability )? true : false;
}/* end skill_check func */


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
 #define BUFFER_SIZE 80
 bool manage_inventory( PLAYER* pc, LOC* active_loc, int cmd )
{
    ITEM * itmptr;
    unsigned int i; /* iterator */
    bool done_with_sub = false;
    char buf[BUFFER_SIZE];

    while( done_with_sub == false) {

        /* Get Input */
        if( ( cmd > INVENTORY )&&( cmd <= DESTROY_ITEM ) ); /* A Sub Command */
        else while( ( cmd = get_subi_cmd() ) == NO_ACTION );

        /* Process sub cmd */
        switch( cmd ) {

            case REMOVE_ITEM: {
                int j, cnt = 0;
                /* count how many items we have equipped */
                for( i = 0; i < MAX_SLOTS; i++ ) if( pc->equip[i] != NULL ) {
                    cnt += 1;
                    /* In case there is only 1 Save reference to it */
                    itmptr = pc->equip[i]; j = i;
                }/* End equip For If */

                if( cnt == 0 ) say("You have nothing equipped.");

                else if( cnt == 1 ) { /* assume unequipping that one */
                    snprintf( buf, BUFFER_SIZE,
                        "Do you really want to remove your %s?", itmptr->name );
                    say( buf );
                    if( toupper( getch() ) == 'Y' ) {
                        pc->equip[j] = NULL;
                        itmptr->is_equipped = false;
                        snprintf( buf,BUFFER_SIZE, "%s removed", itmptr->name );
                        say( buf );
                        done_with_sub = true;
                    } else { /* Assume Negative Response */
                        snprintf( buf, BUFFER_SIZE,
                            "Canceled! %s still equipped.", itmptr->name );
                        say( buf );
                    }/* End Canceled Else */
                }/* end only one item to unequip if */

                else { /* Select Slot */
                    do{ say("Remove which item? "); }
                    while( ( cnt = get_e_slot() ) == NOT_PLACED );

                    if( cnt == CANCEL ) say("Canceled: Items still equipped.");
                    else if( pc->equip[cnt] == NULL )
                        say("You have no item equipped for that slot");
                    else{
                        snprintf( buf,BUFFER_SIZE,
                            "%s removed", pc->equip[cnt]->name );
                        say( buf );
                        pc->equip[cnt]->is_equipped = false;
                        pc->equip[cnt] = NULL;
                        done_with_sub = true;
                    }/* end remove item else */
                }/* end more than one item to remove else */
            }/* end REMOVE_ITM case */ break;

            case EQUIPMENT: /* Equip Item */ {
                /* Which Item? */
                do{ say("Equip which item? "); }
                while(( cmd = get_i_slot()) == NOT_PLACED );

                if( cmd == CANCEL ) { say("Equip Item Canceled."); continue; }
                else itmptr = &(pc->inventory[cmd]);

                /* Verify Equipability */
                if( !is_equipable(itmptr) )
                    say("You cannot equip that item.");
                else if( itmptr->is_equipped )
                    say("That item is already equipped.");

                /* Check item type and slot if needed */
                else switch( slot_of(itmptr) ) {

                    case HAT: /* must equip to HAT slot */
                    case ARM: /* must equip to ARM slot *///TODO:NEEDS TESTING, Use item Name
                        if( pc->equip[slot_of(itmptr)] != NULL ) {
                            say("Are you sure you want to replace the item you currently have equipped? ");//TODO Takes 2 turns//make skip turn flag
                            if( toupper(getch()) == 'Y' ) {
                                /* Unequip old item */
                                pc->equip[slot_of(itmptr)]->is_equipped = false;
                                say("Equipment swapped!");
                            } else /* assume no */ {
                                say("Canceled. You did not replace your equipped item.");
                                cmd = NO_ACTION;/* Reset cmd for next loop */
                                continue;
                            }/* End Y/N If-Else */
                        } else /* slot is empty */ say("Item equipped!");
                        itmptr->is_equipped = true;
                        pc->equip[slot_of(itmptr)] = itmptr;
                        done_with_sub = true;
                        break;

                    case WEP: { /* can be in WEP or OFF */
                        /* Select Weapon Slot */
                        int slot = NOT_PLACED;
                        /* if unarmed arm without hesitation in 1st slot */
                        if( pc->equip[WEP] == NULL ) slot = WEP;
                        /* else arm in offhand without hesitation if possible */
                        else if( pc->equip[OFF] == NULL ) slot = OFF;
                        if( slot != NOT_PLACED ) say("Item equipped!");
                        else { /* else ask if want to replace 1st or 2nd */
                            do{say("Replace which item?");}
                            while( ( slot = get_hand() ) == NOT_PLACED );
                            if( ( slot == WEP )||( slot == OFF ) ) {
                                pc->equip[slot]->is_equipped = false;
                                say("Equipment swapped!");
                            }/* En Swapped If */
                        }/* End ask for slot Else */

                        /* Equip Selection */
                        if( ( slot == WEP )||( slot == OFF ) ) {
                            itmptr->is_equipped = true;
                            pc->equip[slot] = itmptr;
                            done_with_sub = true;
                        } else say("Canceled: nothing equipped.");
                    } /* End WEP Case */ break;

                    case OFF: /* These are all 2handed weapons *///TODO
                        if( itmptr->is_2handed == false ) ERROR( NULL,"Strange Item!", itmptr->type );
                        //if 2handed confirm unequip if necessary else equip
                        //else not 2handed and...there are no 2handed weapons to return off
                        break;
                    //case MAX_SLOTS://which one to use????

                    default: ERROR(NULL, "Hit switch default!", slot_of(itmptr)); /* ERROR */
                }/* end slot switch */
            }/* end EQUIPMENT sub cmd */ break;

            case DESTROY_ITEM: {
                /* Select Slot */
                do{ say("Destroy which item? "); }
                while( (cmd = get_u_slot()) == NOT_PLACED );

                if( cmd == CANCEL ) say("Canceled. Nothing was destroyed.");

                else if( cmd < MAX_HOLD )/* User Selects Item */ {
                    /* Check for item existence @ slot location */
                    if( pc->inventory[cmd].is_ == false )
                        say("You have no item in that slot.");
                    else /* There is a real item there */ {
                        /* confirm destruction -- last chance */
                        say("Last chance: Are you sure you want to destroy that item? ");
                        if( toupper(getch()) == 'Y' ) {
                            /* destroy item */
                            set_empty_item(&(pc->inventory[cmd]));
                            say("Item Destroyed!");
                            done_with_sub = true;
                        } else /* assume no */
                            say("Canceled. Nothing was destroyed.");
                    }/* end real item else */
                }/* end item selection if */

                else if( cmd < (MAX_SLOTS+10) )/* User Selects Equip */ {
                    cmd -= 10;
                    /* Check for item existence @ slot location */
                    if( pc->equip[cmd] == NULL )
                        say("You have no item equipped in that slot.");
                    else /* item exists */ {
                        /* confirm destruction -- last chance */
                        say("Last chance: Are you sure you want to destroy that item? ");
                        if( toupper(getch()) == 'Y' ) {
                            /* destroy item */
                            set_empty_item(pc->equip[cmd]);
                            pc->equip[cmd] = NULL;
                            say("Item Destroyed!");
                            done_with_sub = true;
                        } else /* assume no */
                            say("Canceled. Nothing was destroyed.");
                    }/* end item exists else */
                }/* end equip select if */
                else/* Handle Impossible Errors */
                    ERROR(NULL,"You broke it!", FAIL);
            }/* end DESTROY_ITEM case */ break;

            case DROP_ITEM: {
                /* Select Slot */
                do{ say("Drop which item? "); }
                while( (cmd = get_u_slot()) == NOT_PLACED );

                if( cmd == CANCEL ) say("Canceled. Nothing was dropped.");

                else if( cmd < MAX_HOLD )/* User Selects Item */ {
                    /* Check for item existence @ slot location */
                    if( pc->inventory[cmd].is_ == false )
                        say("You have no item in that slot.");

                    else/* There is a real item there */ {
                        /* Drop item */
                        // copy item to floor//remove from player//    set_empty_item();
                        if( active_loc->litter.is_ == false ) {
                            swap_item( &(active_loc->litter), &(pc->inventory[cmd]) );
                            say("Item Dropped!");
                        }/* end empty floor location */
                        else say("You can't drop that here.");//TODO
                        done_with_sub = true;
                    }/* end real item else */
                }/* end item selection if */

                else if( cmd < (MAX_SLOTS+10) )/* User Selects Equip */ {
                    cmd -= 10;
                    /* Check for item existence @ slot location */
                    if( pc->equip[cmd] == NULL )
                        say("You have no item equipped in that slot.");
                    else /* item exists */ {
                        /* Drop item */
                        // copy item to floor// remove from player //    set_empty_item(pc->equip[cmd]);
                        if( active_loc->litter.is_ == false ) {
                            pc->equip[cmd]->is_equipped = false;
                            swap_item( &(active_loc->litter), pc->equip[cmd] );
                            pc->equip[cmd] = NULL;
                            say("Item dropped!");
                            if( slot_of( &(active_loc->litter) ) != ARM ) return true;//equipped items that are non Armor are free to drop
                        }/* end empty floor location */
                        else say("You can't drop that here.");//TODO
                        done_with_sub = true;
                    }/* end item exists else */
                }/* end equip select if */
                else/* Handle Impossible Errors */
                    ERROR(NULL,"You broke it!", FAIL);
            }/* end DROP_ITEM sub cmd */ break;

            default: /* CANCELED */ return true;
        }/* End cmd Switch */

        cmd = NO_ACTION;/* Reset cmd for next loop */
    }/* end sub cmd while */

    return false; /* Performed an Action -- Don't need another command */
}/* end manage_inventory func */


/************************************EOF***************************************/
