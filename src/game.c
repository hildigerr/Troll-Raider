/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * game.c -- Game Utility Functions.                                          *
 ******************************************************************************/

#include "game.h"
#include "item.h"
#include "player.h"


/******************************************************************************
 * FUNCTION:    get_cmd     -- Get command input.                             *
 * RETURNS:     int         -- The command input (see definitions in types.h) *
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
 * FUNCTION:    get_subi_cmd        -- Get inventory sub command.             *
 * RETURNS:     int                 -- The command received.                  *
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
 * FUNCTION:    get_u_slot      -- Get either inventory or equipment slot     *
 * ARGUMENTS:   char * prompt   -- The input prompt.                          *
 * RETURNS:     int             -- The chosen slot.                           *
 * NOTE: Treats input 'a' through 'd' as hexidecimal to distinguish between   *
 *       inventory and equipment.                                             *
 ******************************************************************************/
 static int get_u_slot( char * prompt )
{
    int input;
    do{ say(prompt); input = getch();
        switch ( input ) {
            case KEY_ESC: case ' ': case 'q': case 'Q':     return CANCEL;
            case 'a': case 'A': return 10;
            case 'b': case 'B': return 11;
            case 'c': case 'C': return 12;
            case 'd': case 'D': return 13;
            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
            case '8': case '9': case '0': return input - '0';
            default: input = NOT_PLACED;
        }/* End input Switch */
    } while( input == NOT_PLACED );
    return input;
}/* End get_u_slot Func */


/******************************************************************************
 * FUNCTION:    manage_inventory                                              *
 * ARGUMENTS    PLAYER *  pc          -- The Player's Character               *
 *              LOC    *  active_loc  -- The Player's Location (For Dropping) *
 *              int       cmd         -- The inventory command                *
 * RETURNS:     bool         -- Does the player still need to take this turn? *
 * The commands are:                                                          *
 *    INVENTORY -- View it and maybe do one of the others as a sub command:   *
 *    EQUIPMENT    -- Equip an item in inventory for use.                     *
 *    DROP_ITEM    -- Drop an item from inventory or equipment.               *
 *    REMOVE_ITEM  -- Unequip an item, keeping it ininventory.                *
 *    DESTROY_ITEM -- Destroy an item so that it no longer exists.            *
 ******************************************************************************/
 bool manage_inventory( PLAYER* pc, LOC* active_loc, int cmd )
{
    unsigned int i; /* iterator */
    bool done_with_sub = false;

    while( done_with_sub == false) {

        /* Get Input */
        if( ( cmd > INVENTORY )&&( cmd <= DESTROY_ITEM ) ); /* A Sub Command */
        else while( ( cmd = get_subi_cmd() ) == NO_ACTION );

        /* Process sub cmd */
        switch( cmd ) {

            case REMOVE_ITEM: done_with_sub = unequip( pc ); break;

            case EQUIPMENT: done_with_sub = equip_item( pc ); break;

            case DESTROY_ITEM: {
                /* Select Slot */
                cmd = get_u_slot("Destroy which item? ");

                if( cmd == CANCEL ) {
                    say("Canceled. Nothing was destroyed.");
                    continue;
                }/* End Cancel If */

                if( cmd < MAX_HOLD )/* User Selects Item */ {
                    /* Check for item existence @ slot location */
                    if( pc->inventory[cmd].is_ == false )
                        say("You have no item in that slot.");
                    else if( pc->inventory[cmd].is_equipped ) {
                        for( i = 0; i < MAX_SLOTS; i++ ) {
                            if( pc->equip[i] == &(pc->inventory[cmd]) ) {
                                cmd = i+10; break;
                            }/* End Find Equip Slot If */
                        }/* End MAX_SLOTS For */
                    } else { /* There is a real and unequpped item there */
                        /* confirm destruction -- last chance */
                        say("Last chance: "
                            "Are you sure you want to destroy that item? "); //TODO Use Item name
                        if( toupper(getch()) == 'Y' ) {
                            /* destroy item */
                            set_empty_item(&(pc->inventory[cmd]));
                            say("Item Destroyed!");//TODO Use Item name
                            done_with_sub = true;
                        } else /* assume no */
                            say("Canceled. Nothing was destroyed.");
                    }/* end real item else */
                }/* end item selection if */

                if( cmd < (MAX_SLOTS+10) )/* User Selects Equip */ {
                    cmd -= 10;
                    /* Check for item existence @ slot location */
                    if( pc->equip[cmd] == NULL )
                        say("You have no item equipped in that slot.");
                    else /* item exists */ {
                        /* confirm destruction -- last chance */
                        say("Last chance: "
                            "Are you sure you want to destroy that item? ");//TODO Use Item name
                        if( toupper(getch()) == 'Y' ) {
                            /* destroy item */
                            set_empty_item(pc->equip[cmd]);
                            pc->equip[cmd] = NULL;
                            say("Item Destroyed!");//TODO Use Item name
                            done_with_sub = true;
                        } else /* assume no */
                            say("Canceled. Nothing was destroyed.");
                    }/* end item exists else */
                }/* end equip select if */
            }/* end DESTROY_ITEM case */ break;

            case DROP_ITEM: {
                /* Select Slot */
                cmd = get_u_slot("Drop which item? ");

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
                            say("Item Dropped!");//TODO Use Item name
                        }/* end empty floor location */
                        else say("You can't drop that here.");//TODO implement heap of junk container?//TODO Use Item name
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
                            say("Item dropped!");//TODO Use Item name
                            if( slot_of( &(active_loc->litter) ) != ARM ) return true;//equipped items that are non Armor are free to drop
                        }/* end empty floor location */
                        else say("You can't drop that here.");//TODO implement heap of junk container?//TODO Use Item name
                        done_with_sub = true;
                    }/* end item exists else */
                }/* end equip select if */
                else/* Handle Impossible Errors */
                    Barf( "You broke it!", FAIL );
            }/* end DROP_ITEM sub cmd */ break;

            default: /* CANCELED */ return true;
        }/* End cmd Switch */

        cmd = NO_ACTION;/* Reset cmd for next loop */
    }/* end sub cmd while */

    return false; /* Performed an Action -- Don't need another command */
}/* end manage_inventory func */


/************************************EOF***************************************/
