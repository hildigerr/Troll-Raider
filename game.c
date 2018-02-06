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
 * FUNCTION:    get_slot                                                      *
 * ARGUMENTS:   char t      -- Type of query (see NOTE).                      *
 * RETURNS:     int         -- The chosen inventory slot.                     *
 * NOTE:            (t==i): asking for inventory slot                         *
 *                  (t==u): asking or either inventory or equipment slot      *
 *                  (t==e): asking for equipment slot                         *
 * if input is a_d and t==u then returns index dereferencer +10 in order to   *
 *  distinguish between i and e                                               *
 ******************************************************************************/
 int get_slot( char t )
{
    switch (getch()) {
        case KEY_ESC: case ' ': case 'q': case 'Q':     return CANCEL;
        case 'a': case 'A':
            if( t == 'e' ) return 0; else if( t == 'u' )return 10;
        case 'b': case 'B':
            if( t == 'e' ) return 1; else if( t == 'u' )return 11;
        case 'c': case 'C':
            if( t == 'e' ) return 2; else if( t == 'u' )return 12;
        case 'd': case 'D':
            if( t == 'e' ) return 3; else if( t == 'u' )return 13;
            else return NOT_PLACED;
        case '1': if(( t == 'i' )||( t == 'u' ))return 1; else return 0;
        case '2': if(( t == 'i' )||( t == 'u' ))return 2; else return 1;
        case '3': if(( t == 'i' )||( t == 'u' ))return 3; else return 2;
        case '4': if(( t == 'i' )||( t == 'u' ))return 4; else return 3;
        case '5': if(( t == 'i' )||( t == 'u' ))return 5;
        case '6': if(( t == 'i' )||( t == 'u' ))return 6;
        case '7': if(( t == 'i' )||( t == 'u' ))return 7;
        case '8': if(( t == 'i' )||( t == 'u' ))return 8;
        case '9': if(( t == 'i' )||( t == 'u' ))return 9;
        case '0': if(( t == 'i' )||( t == 'u' ))return 0;
        default: return NOT_PLACED;
    }/* End input Switch */
}/* End get_slot Func */


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
 * RETURNS:     bool                 -- Result of test                        *
 ******************************************************************************/
 bool skill_check( int ability, int adjust )//TODO: Make more interesting
{
    return ( ( rng(MAX_STAT_VAL) + adjust ) < ability )? true : false;
}/* end skill_check func */


bool manage_inventory( PLAYER* pc, LOC* active_loc, int cmd )
{
    ITEM* itmptr;   /* items */
    unsigned int i; /* iterator */
    bool done_with_sub = false;
    while( done_with_sub == false) {
        /* Get Input */
        if( ( cmd > INVENTORY )&&( cmd < PICK_UP ) ); /* All i sub cmds accessed directly in this range */
        else while( ( cmd = get_subi_cmd() ) == NO_ACTION );
        /* Process sub cmd */
        if( cmd == REMOVE_ITEM )
        {
            /* count how many items we have equipped */
            cmd = 0; /* initialize for temporary iterative use */
            for( i = 0; i < MAX_SLOTS; i++ ) if( pc->equip[i] != NULL ) { cmd += 1; itmptr = pc->equip[i]; }

            if( cmd == 0 ) { say("You have nothing equipped."); }
            else if( cmd == 1 ) /* assume unequipping that one */
            {
                /* confirm */
                say("Do you really want to remove that item? ");
                if( toupper( getch() ) == 'Y' )
                {
                    for( i = 0; i < MAX_SLOTS; i++ ) if( itmptr == pc->equip[i] ) pc->equip[i] = NULL;//TODO TEST
                    itmptr->is_equipped = false;
                    say("Item removed");
                    done_with_sub = true;
                }/* end affirm remove item if */
                else say("Canceled! Item still equipped.");
            }/* end only one item to unequip if */
            else {
                /* Select Slot */
                do{ say("Remove which item? "); }while( ( cmd = get_slot('e') ) == NOT_PLACED );
                if( cmd == CANCEL )
                    say("Canceled: Item still equipped.");
                else if( pc->equip[cmd] == NULL ) say("You have no item equipped for that slot");
                else{
                    pc->equip[cmd]->is_equipped = false;
                    pc->equip[cmd] = NULL;
                    say("Item removed");
                    done_with_sub = true;
                }/* end remove item else */
            }/* end more than one item to remove else */
            cmd = NO_ACTION;/* Reset cmd for general use *///Not Necessary
        }/* end REMOVE_ITM if */
        else if( cmd == EQUIPMENT )/* Equip Item */
        {
            /* Which Item? */
            do{ say("Equip which item? "); } while(( cmd = get_slot('i')) == NOT_PLACED );
            if( cmd == CANCEL ) { say("Equip Item Canceled."); continue; }
            else itmptr = &(pc->inventory[cmd]);
            /* Verify Equipability */
            if( is_equipable( itmptr ) == true )
            {
                /* Verify and Unequipp if needed */
                if( itmptr->is_equipped == true ) { say("That item is already equipped."); }
                /* Check item type and slot if needed */
                else switch( slot_of(itmptr) )
                {
                    case HAT: /* must equip to HAT slot */
                    case ARM: /* must equip to ARM slot *///TODO:NEEDS TESTING
                        if( pc->equip[slot_of(itmptr)] != NULL )
                        {
                            say("Are you sure you want to replace the item you currently have equipped? ");//TODO Takes 2 turns//make skip turn flag
                            if( toupper(getch()) == 'Y' )
                            {
                                pc->equip[slot_of(itmptr)]->is_equipped = false;//Unequip old item
                                itmptr->is_equipped = true;
                                pc->equip[slot_of(itmptr)] = itmptr;
                                say("Equipment swapped!");
                                done_with_sub = true;
                            }/* end yes if */
                            else /* assume no */ say("Canceled. You did not replace your equipped item.");
                        }/* end slot is ! empty if */
                        else /* slot is empty */
                        {
                            itmptr->is_equipped = true;
                            pc->equip[slot_of(itmptr)] = itmptr;
                            say("Item equipped!");
                            done_with_sub = true;
                        }/* end empty slot else */
                        break;
                    case WEP: /* can be in WEP or OFF */
                    /* if unarmed arm without hesitation in 1st slot */
                        if( pc->equip[WEP] == NULL )
                        {
                            itmptr->is_equipped = true;
                            pc->equip[WEP] = itmptr;
                            say("Item equipped!");
                            done_with_sub = true;
                        }/* end unarmed if */
                    /* else if !have secondary equipment arm in offhand without hesitation */
                        else if( pc->equip[OFF] == NULL )
                        {
                            itmptr->is_equipped = true;
                            pc->equip[OFF] = itmptr;
                            say("Item equipped!");
                            done_with_sub = true;
                        }/* end off-hand unarmed if */
                    /* else ask if want to replace 1st or 2nd */
                        else{
                            do{say("Replace which item?");}while( ( cmd = get_hand() ) == NOT_PLACED);
                            if( cmd == WEP )
                            {
                                pc->equip[WEP]->is_equipped = false;
                                itmptr->is_equipped = true;
                                pc->equip[WEP] = itmptr;
                                say("Equipment swapped!");
                                done_with_sub = true;
                            }/* end replace WEP hand if */
                            else if( cmd == OFF )
                            {
                                pc->equip[OFF]->is_equipped = false;
                                itmptr->is_equipped = true;
                                pc->equip[OFF] = itmptr;
                                say("Equipment swapped!");
                                done_with_sub = true;
                            }/* end replace OFF else */
                            /* else canceled *//////////////////////////////expecting more?//TODO
                        }/* end armed else */
                        break;
                    case OFF: /* These are all 2handed weapons *///TODO
                        if( itmptr->is_2handed == false ) ERROR(NULL,"Strange Item!", itmptr->type );
                        //if 2handed confirm unequip if necessary else equip
                        //else not 2handed and...there are no 2handed weapons to return off
                        break;
                    //case MAX_SLOTS://which one to use????
                    default: ERROR(NULL, "Hit switch default!", slot_of(itmptr)); /* ERROR */
                }/* end slot switch */
            }/* end is_equipable if */
            else say("You cannot equip that item.");
        }/* end EQUIPMENT sub cmd */
        else if( cmd == DESTROY_ITEM )
        {
        /* Select Slot */
            do{say("Destroy which item? ");}while( (cmd = get_slot('u')) == NOT_PLACED );

            if( cmd == CANCEL )
            {
                say("Canceled. Nothing was destroyed.");
            }/* end CANCEL if */
            else if( cmd < MAX_HOLD )/* User Selects Item */
            {
            /* Check for item existence @ slot location */
                if( pc->inventory[cmd].is_ == false )
                {
                    say("You have no item in that slot.");
                }/* end no item if */
                else/* There is a real item there */
                {
                /* confirm destruction -- last chance */
                    say("Last chance: Are you sure you want to destroy that item? ");
                    if( toupper(getch()) == 'Y' )
                    {
                    /* destroy item */
                        set_empty_item(&(pc->inventory[cmd]));
                        say("Item Destroyed!");
                        done_with_sub = true;
                    }else /* assume no */
                    {
                        say("Canceled. Nothing was destroyed.");
                    }/* end assume no else */
                }/* end real item else */
            }/* end item selection if */
            else if( cmd < (MAX_SLOTS+10) )/* User Selects Equip */
            {
                cmd -= 10;
            /* Check for item existence @ slot location */
                if( pc->equip[cmd] == NULL )
                {
                    say("You have no item equipped in that slot.");
                }else /* item exists */
                {
                /* confirm destruction -- last chance */
                    say("Last chance: Are you sure you want to destroy that item? ");
                    if( toupper(getch()) == 'Y' )
                    {
                    /* destroy item */
                        set_empty_item(pc->equip[cmd]);
                        pc->equip[cmd] = NULL;
                        say("Item Destroyed!");
                        done_with_sub = true;
                    }else /* assume no */
                    {
                        say("Canceled. Nothing was destroyed.");
                    }/* end assume no else */
                }/* end item exists else */
            }/* end equip select if */
            else/* Handle Impossible Errors */
                ERROR(NULL,"You broke it!", FAIL);
        }/* end DESTROY_ITEM if */
        else if( cmd == DROP_ITEM )
        {
        /* Select Slot */
            do{say("Drop which item? ");}while( (cmd = get_slot('u')) == NOT_PLACED );

            if( cmd == CANCEL )
            {
                say("Canceled. Nothing was dropped.");
            }/* end CANCEL if */
            else if( cmd < MAX_HOLD )/* User Selects Item */
            {
            /* Check for item existence @ slot location */
                if( pc->inventory[cmd].is_ == false )
                {
                    say("You have no item in that slot.");
                }/* end no item if */
                else/* There is a real item there */
                {
                /* Drop item */
                    // copy item to floor//remove from player//    set_empty_item();
                    if( active_loc->litter.is_ == false )
                    {
                        swap_item( &(active_loc->litter), &(pc->inventory[cmd]) );
                        say("Item Dropped!");
                    }/* end empty floor location */
                    else say("You can't drop that here.");//TODO
                    done_with_sub = true;
                }/* end real item else */
            }/* end item selection if */
            else if( cmd < (MAX_SLOTS+10) )/* User Selects Equip */
            {
                cmd -= 10;
            /* Check for item existence @ slot location */
                if( pc->equip[cmd] == NULL )
                {
                    say("You have no item equipped in that slot.");
                }else /* item exists */
                {
                /* Drop item */
                    // copy item to floor// remove from player //    set_empty_item(pc->equip[cmd]);
                    if( active_loc->litter.is_ == false )
                    {
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
        }/* end DROP_ITEM sub cmd */
        else /* CANCELED */ return true;
    }/* end sub cmd while */
    return false; /* Performed an Action -- Don't need another command */
}/* end manage_inventory func */


/************************************EOF***************************************/
