/* $Id: game.c,v 1.6 2014/01/13 08:50:49 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * game.c -- Game Utility Functions.                                          *
 ******************************************************************************/

/* Local Headers */
#include "game.h"
#include "trollraider.h"

/******************************************************************************
 * FUNCTION:    get_cmd                                                       *
 * RETURNS:     int         see definitions in types.h                        *
 ******************************************************************************/
 int get_cmd( void )
{
    switch( getch() ) {
        case '8': case KEY_UP:         /*NORTH*/       return NORTH;
        case '2': case KEY_DOWN:    /*SOUTH*/       return SOUTH;
        case '6': case KEY_RIGHT:    /*EAST*/        return EAST;
        case '4': case KEY_LEFT:    /*WEST*/        return WEST;
        case '9':                    /*NORTH_EAST*/    return NORTH_EAST;
        case '7':                    /*NORTH_WEST*/    return NORTH_WEST;
        case '3':                    /*SOUTH_EAST*/    return SOUTH_EAST;
        case '1':                    /*SOUTH_WEST*/    return SOUTH_WEST;
        case '5':                    /*WAIT*/        return WAIT;
        case 'q':
        case 'Q': case KEY_ESC:        /*QUIT*/        return QUIT;
        case 'g': case ',':         /*PICK UP*/     return PICK_UP;
        case 'i':                     /*INVENTORY*/   return INVENTORY;
        case 'e': case 'w':         /*EQUIPMENT*/   return EQUIPMENT;
        case 'u': case 'T':         /*UNEQUIP ITEM*/return REMOVE_ITEM;
        case 'd':                     /*DROP ITEM*/    return DROP_ITEM;
        case 'K':                   /*DESTROY ITEM*/return DESTROY_ITEM;
        case 'I':                    /*DEBUGGING*/   return DEBUG_ITEM;
        default:    return NO_ACTION; //TODO SAVE_GAME
    }/* End input Switch */
}/* End get_cmd Func */


/******************************************************************************
 * FUNCTION:    get_subi_cmd                                                  *
 * RETURNS:     int         see definitions in types.h                        *
 ******************************************************************************/
 int get_subi_cmd( void )
{
    switch( getch() ) {
        case 'f': case 'F': case KEY_ESC:   /* exit sub cmd prompt */
        case 'q': case 'Q': case ' ':          return SUB_FIN;

        case 'D': case 'd':                    /* drop an item */
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
        case KEY_ESC: case ' ': case 'q': case 'Q':    return CANCEL;
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
 * FUNCTION:    story_line                                                    *
 * ARGUMENTS:   short   when    --  Storyline Time (eg: INTRO, END, etc...)   *
 *              WINDOW* where   --  Window to display the message to.         *
 * RETURNS:     int                                                           *
 ******************************************************************************/
 int story_line( short when, WINDOW* where )
{
    int opt = 0;

    switch( when ) {
        case INTRO:
            printf("\n\n\tTroll Raider v%s\tBy HILDIGERR\n\n", VERSION );
            opt = NO_ACTION;
            break;
        case BEGINNING:
            break;
        case MIDDLE:
            if( where == NULL ) break;
            wsay( where, "You have saved all your family "
                         "and slaughered the humans." );
            //TODO: output qt of family saved
            mypause(0);//more();
            wsay( where, "You smell another human settlement nearby, "
                         "do you want to approach it? " );
            //TODO: output direction and? you must attack from that direction
            if( toupper(getch()) == 'Y' ) opt = 0;
            else opt = 0;
            break;
        case END:
            if( where == NULL ) break;
            break;
        default:
            opt = 0;
    }/* End when Switch */

    return opt;
}/* End story_line Func */


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



/************************************EOF***************************************/
