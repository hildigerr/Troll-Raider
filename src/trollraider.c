/******************************************************************************
 *                             Troll Raider                                   *
 * Created by:      Roberto Morrel HildigerR Vergaray                         *
 ******************************************************************************/

#define VERSION "0.4.1"

#include <time.h>
#include "cwin.h"
#include "player.h"
#include "item.h"
#include "map.h"


/******************************************************************************
 * FUNCTION:    get_cmd     -- Get command input.                             *
 * RETURNS:     int         -- The command input (see definitions in types.h) *
 ******************************************************************************/
static int get_cmd( void ) // TODO Make command controls configable.
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
static bool manage_inventory( PLAYER * pc, LOC * active_loc, int cmd )
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


/***********************************************************************MAIN: */
int main( int argc, char* argv[] )
{
    int r, c, cmd;  /* row, column, command */
    unsigned int i; /* iterator */
    PLAYER pc;      /* player's character */
    PLAYER npc[MAX_NPC];   /* non player characters */
    unsigned long turn = 0;
    LEVEL curlv[MAX_MAPS]; /* current levels */
    bool run, need_more_cmd, skip_a_turn;
//    FILE* indata;   /* saved game file access */
    WINDOW  * display_right,
            * sub_win_root, * sub_win_lhs, * sub_win_rhs;

    srand(time(NULL));        /* use clock value as starting seed */

    run = true;
    skip_a_turn = false;

    /* READ IN DATA FILE(S) For Saved Game */
//    if( ( indata = fopen( dat_file_name, READ_ONLY ) ) == NULL )
//        exit( ERROR(NULL, "Missing \"item.dat\" file", FAIL ) );

    /* Get Player Name */
    printf("\nWhat is the name for your character? ");
    if( !fgets( pc.name, MAX_NAME_LEN+1, stdin ) )
        exit( ERROR( "fgets", "failed to get string", MAX_NAME_LEN ) );

    /* INITIALIZE PLAYER */
    if( init_player( &pc ) == false )
        exit( Error( "Failed to initialize player", FAIL ) );

    /* INITIALIZE NPC ARRAY */
    for( i = 0; i < MAX_NPC; i++ )
        if( !init_mon( &npc[i], rng(NPC_TYPE_QT) ) )
            exit( Error( "Failed to initialize NPC", i ) );

    /* INITIALIZE MAPS */
    if( !dungen( curlv, npc ) )
        exit( Error( "Failed to generate dungeon levels.", FAIL ) );

    //XXX visible only after program termination:
    printf("\n\n\tTroll Raider v%s\tBy HILDIGERR\n\n", VERSION );

    /* INITIALIZE CURSES */
    initscr(); /* Start curses mode *///RETURNS WINDOW*
    atexit( (void(*)(void)) endwin );
    display_btm = subwin( stdscr, BTM_SUB_ROWS, BTM_SUB_COLS, MAX_ROW, 0 );
    if( display_btm == NULL ) exit( ERROR( NULL, "btm sub win", FAIL ) );
    display_right = subwin( stdscr, RT_SUB_ROWS, RT_SUB_COLS, 0, MAX_COL );
    if( display_right == NULL ) exit( ERROR( NULL, "right sub win", FAIL ) );
    /* Create Data Display sub-Windows */
    sub_win_root = newwin((MAX_ROW - BTM_SUB_ROWS ),(BTM_SUB_COLS - RT_SUB_COLS),0,0);
    if( sub_win_root == NULL ) return ERROR(NULL, "No Cmd List Window!", 0);
    sub_win_lhs = subwin(sub_win_root,(MAX_ROW - BTM_SUB_ROWS ),((BTM_SUB_COLS - RT_SUB_COLS)/2),0,0);
    if( sub_win_lhs == NULL ) return ERROR(NULL, "No Cmd List Window!", 1);
    sub_win_rhs = subwin(sub_win_root,(MAX_ROW - BTM_SUB_ROWS ),((BTM_SUB_COLS - RT_SUB_COLS)/2),0,((BTM_SUB_COLS - RT_SUB_COLS)/2));
    if( sub_win_rhs == NULL ) return ERROR(NULL, "No Cmd List Window!", 2);
    raw();     /* Disable input buffering */
    noecho();  /* Disable input echoing */
    keypad(stdscr, TRUE); /* enable arrowkeys and such */

    /* RUN GAME */
    while( run ) {

        /* Set-up/Update RH Display */
        if( init_display_right( display_right, &pc, turn ) == false )
            exit( Error( "Failed to (re)initialize right hand display",turn ) );

        /* INITIALIZE CURRENT LEVEL IF NEEDED */
        if( curlv[pc.maplv].is_new == true ) {
            curlv[pc.maplv].is_new = false;

            draw_map( &curlv[pc.maplv] );

            if( turn == 0 ) {
                refresh();

                /* Get Race and Class */
                say("You are a man-eating troll!");

                /* Get Attack Direction */
                say("You approach the human village."
                    " From which direction would you like to attack? ");
                switch( getch() ) {
                    case 'N': case 'n': case '8': /* NORTH */
                        pc.locc = 1 + rng( MAX_COL - 2 );
                        pc.locr = 1;
                        break;
                    case 'S': case 's': case '2': /* SOUTH */
                        pc.locc = 1 + rng( MAX_COL - 2 );
                        pc.locr = ( MAX_ROW - 2 );
                        break;
                    case 'E': case 'e': case '6': /* EAST */
                        pc.locr = 1 + rng( MAX_ROW - 2 );
                        pc.locc = ( MAX_COL - 2 );
                        break;
                    case 'W': case 'w': case '4': /* WEST */
                        pc.locr = 1 + rng( MAX_ROW - 2 );
                        pc.locc = 1;
                        break;
                    case '9': /* NORTH_EAST */
                        pc.locc = ( MAX_COL - 2 );
                        pc.locr = 1;
                        break;
                    case '1': /* SOUTH_WEST */
                        pc.locr = ( MAX_ROW - 2 );
                        pc.locc = 1;
                        break;
                    case '3': /* SOUTH_EAST */
                        pc.locc = ( MAX_COL - 2 );
                        pc.locr = ( MAX_ROW - 2 );
                        break;
                    case 'Q': case 'q': exit(0);
                    default: /* NORTH_WEST; */
                        pc.locc = 1;
                        pc.locr = 1;
                }/* end attdir switch */
            }/* end initial turn if */
        }/* end is_new if */

        /* UPDATE PLAYER LOCATION */
        mvaddch( pc.locr, pc.locc, '@' );

        /* UPDATE SCREEN */
        refresh();
        wrefresh(display_btm);
        wrefresh(display_right);

        if( skip_a_turn == true ) skip_a_turn = false; //TODO: use pc.is_awake?
        else /* TAKE TURN */ do {

            /* GET CMD INPUT */
            while( ( cmd = get_cmd() ) == NO_ACTION );
            need_more_cmd = false;
            r = pc.locr; c = pc.locc;

            /* PROCESS CMD */
            if( cmd == WAIT ) say("You wait...");
            else if( cmd == QUIT ) {
                say("Are you sure you want to quit? ");
                if( toupper(getch()) == 'Y' ) run = false;
//                else say("Oh ok, then we wont kick you out of here.");
                continue;
            }/* end QUIT */

            /* MOVING */
            else if( ( cmd > NO_ACTION )&&( cmd < WAIT )) {
                /* Find Target Location */
                switch( cmd ) {
                    case SOUTH_WEST: { r += 1; c -= 1; } break;
                    case SOUTH     : { r += 1;         } break;
                    case SOUTH_EAST: { r += 1; c += 1; } break;
                    case WEST      : {         c -= 1; } break;
                    case EAST      : {         c += 1; } break;
                    case NORTH_WEST: { r -= 1; c -= 1; } break;
                    case NORTH     : { r -= 1;         } break;
                    case NORTH_EAST: { r -= 1; c += 1; } break;
                    default:
                        exit( ERROR(NULL, "Bad Logic MOVING cmd filter", cmd));
                }/* end cmd switch */

                /* MOVE assess legality */
                if( ( r > MAX_ROW )||( r < 0 ) )
                    exit( ERROR( NULL, "Move Out of Vertical Bounds", r ) );
                else if( ( c > MAX_COL )||( c < 0 ) )
                    exit( ERROR( NULL, "Move Out of Horizontal Bounds", c ) );
                else if( ACTIVE_LOCATION.icon == WALL )
                    say("You bumped into a wall!"); //Currently Takes a Turn

                /* Attacking */
                else if( ACTIVE_LOCATION.mon != NULL ) {
                    say("Attack!"); //TODO
                }/* end Attack if */

                /* Door Interaction */
                else if( ACTIVE_LOCATION.icon == DOOR ) {
                    if( pc.maplv == HVILLAGE ) {
                        say("Do you want to enter the building? ");
                        if( toupper( getch() ) == 'N' ) need_more_cmd = true;
                        else { /* Enter Human Huts Level */
                            pc.maplv = IN_HHUTS;
                            curlv[HVILLAGE].is_new = true;
                            pc.locr = r;
                            pc.locc = c;
                        }/* End !N Else */
                    }/* end HVILLAGE if */
                    else if( pc.maplv == IN_HHUTS ) { //XXX Doors lead back to village
                            say("Do you want to exit the building? ");
                            if( toupper( getch() ) == 'N' ) need_more_cmd = true;
                            else { /* Exit Human Huts Level */
                                pc.maplv = HVILLAGE;
                                curlv[IN_HHUTS].is_new = true;
                            }/* End Else */
                    }/* end HVILLAGE if */
                    else if( skill_check(pc.stats[STR],0) == false)
                        say("The door is stuck!");
                    else;
                        //TODO open door, move char, etc...

                }/* end door if */ else /* Move Normally */ {
                    if( ACTIVE_LOCATION.icon == USTAIR )
                        say("You found some ascending stairs!");
                    else if( ACTIVE_LOCATION.icon == DSTAIR )
                        say("You found some descending stairs!");
                    else if( ACTIVE_LOCATION.is_trap == true ) {
                        say("You have stepped into a trap!"); //TODO
                    } else if( ACTIVE_LOCATION.litter != NULL ) {
                        say("You have found an item!"); //TODO
                    }/* end something there if*/
                    move(pc.locr,pc.locc);
                    addch(get_map_icon(curlv[pc.maplv].map[pc.locr][pc.locc]));//MAYBE DEFINE TO PREVIOUS_LOCATION
                    pc.locr = r;
                    pc.locc = c;
                }/*end normal move else*/
            }/* end cmd in MOVING range if */

            /* Inventory and Equipment Managment */
            else if( ( cmd >= INVENTORY )&&( cmd <= DESTROY_ITEM ) ) {

                /* Display Inventory in LHS Subwindow */
                wprintw(sub_win_lhs,"\n # Inventory   [Hit,Dam] $");
                for(i=0; i < MAX_HOLD; i++)
                    if( pc.inventory[i] != NULL )
                        wprintw(sub_win_lhs,"\n %d. %-10s [%3d,%3d] %3d", i,
                            pc.inventory[i]->name,
                            pc.inventory[i]->stats[0],
                            pc.inventory[i]->stats[1],
                            pc.inventory[i]->worth );
                box(sub_win_lhs,'|','+');

                /* Display Equipment in RHS Subwindow */
                wprintw(sub_win_rhs,"\n # Equipment   [Hit,Dam] $");
                for(i=0; i < MAX_SLOTS; i++ ) {
                    wprintw(sub_win_rhs,"\n %c.", 97+i);
                    if( pc.equip[i] != NULL )
                        wprintw(sub_win_rhs," %-10s [%3d,%3d] %3d",
                            pc.equip[i]->name,
                            pc.equip[i]->stats[0],
                            pc.equip[i]->stats[1],
                            pc.equip[i]->worth );
                    else if( i == WEP )
                        wprintw( sub_win_rhs," fist       [  0,  0]   0" );//TODO: Make unarmed values based on abilities
                    else if( i == OFF )
                        wprintw( sub_win_rhs," fist       [  0,  0]   0" );
                    else if( i == ARM )
                        wprintw( sub_win_rhs," bareskin   [  0,  0]   0" );
                    else if( i == HAT )
                        wprintw( sub_win_rhs," hairs      [  0,  0]   0" );
                    else ERROR( NULL, "Slot machine error!", i );
                }/* end MAX_SLOTS for */
                box(sub_win_rhs,'|','+');

                /* Display The Subwindows */
                overwrite(sub_win_root,stdscr); wrefresh(sub_win_root);

                /* SUB CMD */
                need_more_cmd = manage_inventory( &pc, &ACTIVE_LOCATION, cmd );

                /* Replace Map */
                draw_map( &curlv[pc.maplv] ); mvaddch( pc.locr, pc.locc, '@' );

                /* Clean Up */
                werase( sub_win_root );
                wmove( sub_win_lhs, 0, 0 ); wmove( sub_win_rhs, 0, 0 );
                touchwin( stdscr );
            }/* end INVENTORY cmd */

            else if( cmd == PICK_UP ) {
                need_more_cmd = true; /* Assume Nothing Happens */
                if( !ACTIVE_LOCATION.litter )
                    say("There is nothing to pick up.");
                else {
                    for(i = 0; i < MAX_HOLD; i++ ) {
                        if( !pc.inventory[i] ) {
                            pc.inventory[i] = ACTIVE_LOCATION.litter;
                            ACTIVE_LOCATION.litter = NULL;
                            vsay("You picked up the %s!",pc.inventory[i]->name);
                            need_more_cmd = false;
                            break;
                        }/* end got empty slot if */
                    }/* end MAX_SLOTS for */
                    if( i == MAX_SLOTS ) say("You're inventory is full!");
                }/* end pickup else */
            }/* end PICK_UP if */

            else exit( ERROR( NULL, "cmd processing", cmd ) );
        } while( need_more_cmd == true );/* end need more cmd do */

        //TODO: Enemy actions

        turn++;
    }/* end run while */

    /* TODO: Display And Save High Score Report */

    /* TERMINATE PROGRAM */
    return 0;
}/* end main func */


///////////////////////////////////////////////////////////////////////////TODO:
// wsay( where, "You have saved all your family "
//              "and slaughered the humans." );
// //TODO: output qt of family saved
// mypause(0);//more();
// wsay( where, "You smell another human settlement nearby, "
//              "do you want to approach it? " );
// //TODO: output direction and? you must attack from that direction
// if( toupper(getch()) == 'Y' )

