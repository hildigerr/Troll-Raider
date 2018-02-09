/******************************************************************************
 *                             Troll Raider                                   *
 * Created by:      Roberto Morrel HildigerR Vergaray                         *
 ******************************************************************************/

#define VERSION "0.4.1"

#include <time.h>
#include "game.h"
#include "player.h"
#include "item.h"
#include "map.h"

/* Defined Constants */
#define MAX_ITEM_WINDOWS 3
#define MAX_ADITIONAL_WINDOW_POINTERS_NEEDED 3

/***********************************************************************MAIN: */
int main( int argc, char* argv[] )
{
    int r, c, cmd;  /* row, column, command */
    unsigned int i; /* iterator */
    PLAYER pc;      /* player's character */
    PLAYER npc[MAX_NPC];   /* non player characters */
    STAT_DAT score;
    unsigned short hut_qt;
    LEVEL curlv[MAX_MAPS]; /* current levels */
    bool run, need_more_cmd, skip_a_turn;
//    FILE* indata;   /* saved game file access */
    WINDOW  /**display_btm,*/ *display_right,
            *cmd_list[MAX_ADITIONAL_WINDOW_POINTERS_NEEDED];

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
        exit( ERROR( NULL, "Failed to initialize player", FAIL ) );

    init_stat_data( &score );

    /* INITIALIZE MAPS */
    for( i = 0; i < MAX_MAPS; i++ ) init_lv( &curlv[i], i );

    /* INITIALIZE NPC ARRAY */
    for( i = 0; i < MAX_NPC; i++ )
        if( init_mon( &npc[i], rng(NPC_TYPE_QT) ) == false )
            return( ERROR( NULL, "Failed to initialize NPC", i ) );

    /* Generate Dungeon */
    hut_qt = 1 + rng( MAX_HUTS );
    if( towngen(&curlv[HVILLAGE], hut_qt) == false )
        exit( Error( "Failed to create town with n huts : n = ", hut_qt ) );
    if( buildgen(&curlv[HVILLAGE],&curlv[IN_HHUTS]) == false )
        exit( ERROR( NULL, "Failed to create huts :^(", FAIL ) );

    //XXX visible only after program termination:
    printf("\n\n\tTroll Raider v%s\tBy HILDIGERR\n\n", VERSION );

    /* INITIALIZE CURSES */
    initscr(); /* Start curses mode *///RETURNS WINDOW*
    atexit( (void(*)(void)) endwin );
    display_btm = subwin( stdscr, BTM_SUB_ROWS, BTM_SUB_COLS, MAX_ROW, 0 );
    if( display_btm == NULL ) exit( ERROR( NULL, "btm sub win", FAIL ) );
    display_right = subwin( stdscr, RT_SUB_ROWS, RT_SUB_COLS, 0, MAX_COL );
    if( display_right == NULL ) exit( ERROR( NULL, "right sub win", FAIL ) );
    raw();     /* Disable input buffering */
    noecho();  /* Disable input echoing */
    keypad(stdscr, TRUE); /* enable arrowkeys and such */

    /* RUN GAME */
    while( run == true ) {

        /* Set-up/Update RH Display */
        if( init_display_right( display_right, &pc, &score ) == false )
            exit( ERROR( NULL,
                "Failed to (re)initialize right hand display", score.turn ) );


        /* INITIALIZE CURRENT LEVEL IF NEEDED */
        if( curlv[pc.maplv].is_new == true ) {
            curlv[pc.maplv].is_new = false;

            draw_map( &curlv[pc.maplv] );

            if( score.turn == 0 ) {
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
        move(pc.locr,pc.locc);
        addch('@');

        /* UPDATE SCREEN */
        refresh();
        wrefresh(display_btm);
        wrefresh(display_right);

        if( skip_a_turn == true ) skip_a_turn = false;
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
                else if( ACTIVE_LOCATION.is_wall == true ){
                    say("You bumped into a wall!"); }//Currently Takes a Turn

                /* Attacking */
                else if( ACTIVE_LOCATION.is_occupied == true ) {
                    say("Attack!"); //TODO
                }/* end Attack if */

                /* Door Interaction */
                else if( ACTIVE_LOCATION.is_door == true ) {
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
                    else if(( pc.maplv == IN_HHUTS )
                         &&( ACTIVE_LOCATION.is_dstair == true )) {
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
                    if( ACTIVE_LOCATION.is_ustair == true )
                        say("You found some ascending stairs!");
                    else if( ACTIVE_LOCATION.is_dstair == true )
                        say("You found some descending stairs!");
                    else if( ACTIVE_LOCATION.is_trap == true ) {
                        say("You have stepped into a trap!"); //TODO
                    } else if( ACTIVE_LOCATION.litter.is_ == true ) {
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
                /* Create Data Display Windows */
                cmd_list[0] = newwin((MAX_ROW - BTM_SUB_ROWS ),(BTM_SUB_COLS - RT_SUB_COLS),0,0);
                if( cmd_list[0] == NULL ){ return ERROR(NULL, "No Cmd List Window!", 0); }
                cmd_list[1] = subwin(cmd_list[0],(MAX_ROW - BTM_SUB_ROWS ),((BTM_SUB_COLS - RT_SUB_COLS)/2),0,0);
                if( cmd_list[1] == NULL ){ return ERROR(NULL, "No Cmd List Window!", 1); }
                cmd_list[2] = subwin(cmd_list[0],(MAX_ROW - BTM_SUB_ROWS ),((BTM_SUB_COLS - RT_SUB_COLS)/2),0,((BTM_SUB_COLS - RT_SUB_COLS)/2));
                if( cmd_list[2] == NULL ){ return ERROR(NULL, "No Cmd List Window!", 2); }
                /* Display Data on New Window */
                /* Inventory *//* LHS */
                wprintw(cmd_list[1],"\n # Inventory [Hit,Dam] $");
                for(i=0; i < MAX_HOLD; i++)
                    if( pc.inventory[i].is_equipped == false )
                        wprintw(cmd_list[1],"\n %d. %s [%d,%d] %d",i,pc.inventory[i].name,pc.inventory[i].stats[0],pc.inventory[i].stats[1], pc.inventory[i].worth);
                /* Equipment *//* RHS */
                wprintw(cmd_list[2],"\n # Equipment [Hit,Dam] $");
                for(i=0; i < MAX_SLOTS; i++ )
                {
                    wprintw(cmd_list[2],"\n %c.", 97+i);
                    if( pc.equip[i] != NULL )
                        wprintw(cmd_list[2]," %s [%d,%d] %d",pc.equip[i]->name,pc.equip[i]->stats[0],pc.equip[i]->stats[1], pc.equip[i]->worth);
                    else if(i == WEP ) wprintw(cmd_list[2]," fist [0,0] 0", 97+i);//TODO: Make unarmed values based on abilities
                    else if(i == OFF ) wprintw(cmd_list[2]," fist [0,0] 0", 97+i);
                    else if(i == ARM ) wprintw(cmd_list[2]," bareskin [0,0] 0", 97+i);
                    else if(i == HAT ) wprintw(cmd_list[2]," hairs [0,0] 0", 97+i);
                    else ERROR( NULL, "Slot machine error!", i );
                }/* end MAX_SLOTS for */
                for( i = 1; i < MAX_ITEM_WINDOWS; i++ )
                {
                    box(cmd_list[i],'|','+');
                    overwrite(cmd_list[i],stdscr);
                    wrefresh(cmd_list[i]);
                }/* end MAX_ITEM_WINDOWS for */

                /* SUB CMD */
                need_more_cmd = manage_inventory( &pc, &ACTIVE_LOCATION, cmd );

                /* Replace Map */
                draw_map( &curlv[pc.maplv] );
                move(pc.locr,pc.locc); addch('@');

                /* Clean Up */
                for( i = MAX_ITEM_WINDOWS-1; i <= 0; i-- ) delwin(cmd_list[i]);
                touchwin(stdscr);
            }/* end INVENTORY cmd */

            else if( cmd == PICK_UP ) {
                need_more_cmd = true; /* Assume Nothing Happens */
                if( ACTIVE_LOCATION.litter.is_ == false )
                    say("There is nothing to pick up.");
                else if( pc.inventory[MAX_SLOTS-1].is_ != false )
                    say("You're inventory is full!");
                else {
                    for(i = 0; i < MAX_SLOTS; i++ ) {
                        if( pc.inventory[i].is_ == false ) {
                            swap_item(&ACTIVE_LOCATION.litter, &pc.inventory[i]);
                            say("Item picked up!");
                            need_more_cmd = false;
                            break;
                        }/* end got empty slot if */
                    }/* end MAX_SLOTS for */
                }/* end pickup else */
            }/* end PICK_UP if */

            else exit( ERROR( NULL, "cmd processing", cmd ) );
        } while( need_more_cmd == true );/* end need more cmd do */

        //TODO: Enemy actions

        score.turn += 1;
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

