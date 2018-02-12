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
    bool done_with_sub = false;

    while( done_with_sub == false) {

        /* Get Input */
        if( ( cmd > INVENTORY )&&( cmd <= DESTROY_ITEM ) ); /* A Sub Command */
        else while( ( cmd = get_subi_cmd() ) == NO_ACTION );

        /* Process sub cmd */
        switch( cmd ) {

            case EQUIPMENT: done_with_sub = equip_item( pc ); break;

            case DROP_ITEM: done_with_sub = drop_item( pc, active_loc ); break;

            case REMOVE_ITEM: done_with_sub = unequip( pc ); break;

            case DESTROY_ITEM: done_with_sub = destroy_item( pc ); break;

            default: /* CANCELED */ return true;
        }/* End cmd Switch */

        cmd = NO_ACTION;/* Reset cmd for next loop */
    }/* end sub cmd while */

    return false; /* Performed an Action -- Don't need another command */
}/* end manage_inventory func */


/************************************EOF***************************************/
