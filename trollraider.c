/* $Id: trollraider.c,v 1.5 2014/01/13 08:50:50 moonsdad Exp $ */

/******************************************************************************
 *                             Troll Raider                                   *
 * Created by:      Roberto Morrel HildigerR Vergaray <moonsdad@gmail.com>    *
 ******************************************************************************/

/* System Headers */
#include <time.h>

/* Local Headers */
#include "trollraider.h"
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
    LEVEL curlv[MAX_MAPS]; /* current levels */
    bool run, need_more_cmd, done_with_sub, skip_a_turn;
//    FILE* indata;   /* saved game file access */
    WINDOW  *display_btm, *display_right,
            *cmd_list[MAX_ADITIONAL_WINDOW_POINTERS_NEEDED];
    ITEM* itmptr;   /* items */

    srand(time(NULL));        /* use clock value as starting seed */

    run = true;
    skip_a_turn = false;

    /* READ IN DATA FILE(S) For Saved Game */
//    if( ( indata = fopen( dat_file_name, READ_ONLY ) ) == NULL )
//        exit( ERROR(NULL, "Missing \"item.dat\" file", FAIL ) );

    /* Get Player Name */
    printf("\nWhat is the name for your character? ");
    if( !fgets( pc.name, sizeof(pc.name), stdin ) )
        exit( ERROR( "fgets", "failed to get string", sizeof(pc.name) ) );

    /* INITIALIZE PLAYER */
    if( init_player( &pc ) == false )
        exit( ERROR( NULL, "Failed to initialize player", FAIL ) );
    if( init_stat_data( &score ) == false )
        exit( ERROR( NULL, "Failed to initialize scores", FAIL ) );

    /* GIVE PC A CLUB */
    if( getp_item( &pc.inventory[0], 1, 1 ) == false )
        return( ERROR(NULL, "Failed to get starting equipment",2) );

    score.hut_qt = rng(MAX_HUTS);// score.hut_qt = MAX_HUTS;//TESTING//

    /* INITIALIZE MAPS */
    for( i = 0; i < MAX_MAPS; i++ )
        if( init_lv( &curlv[i], i ) == false )
            exit( ERROR( NULL, "Failed to initialize level", i ) );

    /* INITIALIZE NPC ARRAY */
    r = score.hut_qt * rng(2);//innocent qt
    c = MAX_MAPS + ( rng(MAX_MAPS) / 2 );//initial hero qt
    for( i = 0; i < MAX_NPC; i++ )
        if( init_mon( &npc[i], i, r, c ) == false )
            return( ERROR( NULL, "init_mon !success", i ) );

    /* Generate Dungeon */
    if( towngen(&curlv[HVILLAGE], score.hut_qt) == false )
        exit( _ERROR( NULL, "Failed to create town with n huts : n = ",
                      score.hut_qt, 1 ) );
    else if( buildgen(&curlv[HVILLAGE],&curlv[IN_HHUTS]) == false )
        exit( ERROR( NULL, "Failed to create huts :^(", FAIL ) );

    //INTRO Must be before curses initialization for now
    //Actually displays only after program termination
    pc.attdir =    story_line( INTRO, NULL );

    /* INITIALIZE CURSES */
    initscr(); /* Start curses mode *///RETURNS WINDOW*
    atexit( endwin );
    display_btm = subwin(stdscr,BTM_SUB_ROWS,BTM_SUB_COLS,MAX_ROW,0);
    if( display_btm == NULL ) exit( ERROR( NULL, "btm sub win", FAIL ) );
    display_right = subwin(stdscr,RT_SUB_ROWS,RT_SUB_COLS,0,MAX_COL);
    if( display_right == NULL ) exit( ERROR( NULL, "right sub win", FAIL ) );
    raw();     /* Disable input buffering */
    noecho();  /* Disable input echoing */
    keypad(stdscr, TRUE); /* enable arrowkeys and such */

    /* RUN GAME */
    while( run == true ) {

        /* Set-up/Update RH Display */
        if( init_display_right( display_right, &pc, &score ) == false ) {
            endwin();/* End curses mode */
            exit(ERROR(NULL,"Failed to initialize right hand display",FAIL));
        }/* end setup RH Display fail if */

        /* INITIALIZE CURRENT LEVEL IF NEEDED */
        if(curlv[pc.maplv].is_new == true) {
            for( r = 0; r < MAX_ROW; r++ ) for( c = 0; c < MAX_COL; c++ ) {
                move(r,c);
                addch( get_map_icon( ACTIVE_LOCATION ) );
            }/* end [r][c] screen initialization */

            if( score.turn == 0 ) {
                refresh();

                /* Get Race and Class */
                say("You are a man-eating troll!");

                /* Get Attack Direction */
                say("You approach the human village."
                    " From which direction would you like to attack? ");
                cmd = getch();
                if     (( toupper(cmd) == 'N' )||(cmd == '8')) pc.attdir = NORTH;
                else if(( toupper(cmd) == 'S' )||(cmd == '2')) pc.attdir = SOUTH;
                else if(( toupper(cmd) == 'E' )||(cmd == '6')) pc.attdir = EAST;
                else if(( toupper(cmd) == 'W' )||(cmd == '4')) pc.attdir = WEST;
                else if( cmd == '9' ) pc.attdir = NORTH_EAST;
                else if( cmd == '1' ) pc.attdir = SOUTH_WEST;
                else if( cmd == '3' ) pc.attdir = SOUTH_EAST;
                else if( toupper(cmd) == 'Q' ){ endwin(); exit(0); }
                else pc.attdir = NORTH_WEST;

                /* Get Players Starting Position */
                switch( pc.attdir ) {
                    case NORTH: while( ( pc.locc = rng( MAX_COL - 2 ) ) < 1 ); pc.locr = 1;               break;
                    case SOUTH: while( ( pc.locc = rng( MAX_COL - 2 ) ) < 1 ); pc.locr = ( MAX_ROW - 2 ); break;
                    case EAST:  while( ( pc.locr = rng( MAX_ROW - 2 ) ) < 1 ); pc.locc = ( MAX_COL - 2 ); break;
                    case WEST:  while( ( pc.locr = rng( MAX_ROW - 2 ) ) < 1 ); pc.locc = 1;               break;
                    case NORTH_EAST:     pc.locc = ( MAX_COL - 2 ); pc.locr = 1;                          break;
                    case SOUTH_EAST:     pc.locc = ( MAX_COL - 2 ); pc.locr = ( MAX_ROW - 2 );            break;
                    case SOUTH_WEST:     pc.locr = ( MAX_ROW - 2 );    pc.locc = 1;                          break;
                    case NORTH_WEST:  default:   pc.locc = 1;       pc.locr = 1;
                }/* end pc.attdir switch */
            }/* end initial turn if */
//            else
            curlv[pc.maplv].is_new = false;
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

            /* PROCESS CMD */
            r = pc.locr; c = pc.locc;
            if( ( cmd > NO_ACTION )&&( cmd < WAIT ))/* MOVING */ {
                /* Find Target Location */
                if( cmd == SOUTH_WEST )         { r += 1; c -= 1; }/* end SW if */
                else if( cmd == SOUTH )         { r += 1;         }/* end S  if */
                else if( cmd == SOUTH_EAST )    { r += 1; c += 1; }/* end SE if */
                else if( cmd == WEST )          {         c -= 1; }/* end W  if */
                else if( cmd == EAST )          {         c += 1; }/* end E  if */
                else if( cmd == NORTH_WEST )    { r -= 1; c -= 1; }/* end NW if */
                else if( cmd == NORTH )         { r -= 1;         }/* end SW if */
                else if( cmd == NORTH_EAST )    { r -= 1; c += 1; }/* end N  if */
                else exit( ERROR( NULL, "Bad Logic MOVING cmd filter", cmd ) );

                /* MOVE assess legality */
                if( ( r > MAX_ROW )||( r < 0 ) )
                    exit( ERROR(NULL, "Move Out of Vertical Bounds", r ) );
                else if( ( c > MAX_COL )||( c < 0 ) )
                    exit( ERROR(NULL, "Move Out of Horizontal Bounds", c ) );
                else if( ACTIVE_LOCATION.is_wall == true ){
                    say("You bumped into a wall!"); }//Currently Takes a Turn
                /* MOVE possibilities */
                else if( ACTIVE_LOCATION.is_occupied == true ) {say("Attack!");}
                else if( ACTIVE_LOCATION.is_door == true ) {
                    if( pc.maplv == HVILLAGE ) {
                        say("Do you want to enter the building? ");
                        if( toupper( getch() ) == 'N' )
                            need_more_cmd = true;
                        else { /* */
                            pc.maplv = IN_HHUTS;
                            curlv[HVILLAGE].is_new = true;
                            pc.locr = r;
                            pc.locc = c;
                        }/* End Else */
                    }/* end HVILLAGE if */
                    else if(( pc.maplv == IN_HHUTS )
                         &&( ACTIVE_LOCATION.is_dstair == true )) {
                            say("Do you want to exit the building? ");
                            if( toupper( getch() ) == 'N' )
                                need_more_cmd = true;
                            else { /* */
                                pc.maplv = HVILLAGE;
                                curlv[IN_HHUTS].is_new = true;
                            }/* End Else */
                    }/* end HVILLAGE if */
                    else if( skill_check(pc.stats[STR],0) == false)
                        say("The door is stuck!");
                    else;
                        //open door, move char, etc...
                }/* end door if */ else /* Move Normally */ {
                    if( ACTIVE_LOCATION.is_ustair == true )
                        {say("You found some ascending stairs!");}
                    else if( ACTIVE_LOCATION.is_dstair == true )
                        {say("You found some descending stairs!");}
                    else if( ACTIVE_LOCATION.is_trap == true )
                        {say("You have stepped into a trap!");}
                    else if( ACTIVE_LOCATION.litter.is_ == true )
                        {say("You have found an item!");}
                    move(pc.locr,pc.locc);
                    addch(get_map_icon(curlv[pc.maplv].map[pc.locr][pc.locc]));//MAYBE DEFINE TO PREVIOUS_LOCATION
                    pc.locr = r;
                    pc.locc = c;
                }/*end normal move else*/
            }/* end cmd in MOVING range if */
            else if( cmd == WAIT ){/*DO NOTHING*/say("You wait...");}
            else if( cmd == QUIT ) {
                say("Are you sure you want to quit? ");
                if( toupper(getch()) == 'Y' ) run = false;
//                else say("Oh ok, then we wont kick you out of here.");
            }/* end QUIT */
            else if( ( cmd >= INVENTORY )&&( cmd <= DESTROY_ITEM) ) {
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
                    else ERROR(NULL, "Slot machine error!", i );
                }/* end MAX_SLOTS for */
                for( i = 1; i < MAX_ITEM_WINDOWS; i++ )
                {
                    box(cmd_list[i],'|','+');
                    overwrite(cmd_list[i],stdscr);
                    wrefresh(cmd_list[i]);
                }/* end MAX_ITEM_WINDOWS for */
                /* SUB CMD */
                done_with_sub = false;
                do{
                    /* Get Input */
                    if( ( cmd > INVENTORY )&&( cmd < PICK_UP ) ); /* All i sub cmds accessed directly in this range */
                    else while( ( cmd = get_subi_cmd() ) == NO_ACTION );
                    /* Process sub cmd */
                    if( cmd == REMOVE_ITEM )
                    {
                        /* count how many items we have equipped */
                        cmd = 0; /* initialize for temporary iterative use */
                        for( i = 0; i < MAX_SLOTS; i++ ) if( pc.equip[i] != NULL ) { cmd += 1; itmptr = pc.equip[i]; }

                        if( cmd == 0 ) { say("You have nothing equipped."); }
                        else if( cmd == 1 ) /* assume unequipping that one */
                        {
                            /* confirm */
                            say("Do you really want to remove that item? ");
                            if( toupper( getch() ) == 'Y' )
                            {
                                for( i = 0; i < MAX_SLOTS; i++ ) if( itmptr == pc.equip[i] ) pc.equip[i] = NULL;//TODO TEST
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
                            else if( pc.equip[cmd] == NULL ) say("You have no item equipped for that slot");
                            else{
                                pc.equip[cmd]->is_equipped = false;
                                pc.equip[cmd] = NULL;
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
                        else itmptr = &pc.inventory[cmd];
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
                                    if( pc.equip[slot_of(itmptr)] != NULL )
                                    {
                                        say("Are you sure you want to replace the item you currently have equipped? ");//TODO Takes 2 turns//make skip turn flag
                                        if( toupper(getch()) == 'Y' )
                                        {
                                            pc.equip[slot_of(itmptr)]->is_equipped = false;//Unequip old item
                                            itmptr->is_equipped = true;
                                            pc.equip[slot_of(itmptr)] = itmptr;
                                            say("Equipment swapped!");
                                            done_with_sub = true;
                                        }/* end yes if */
                                        else /* assume no */ say("Canceled. You did not replace your equipped item.");
                                    }/* end slot is ! empty if */
                                    else /* slot is empty */
                                    {
                                        itmptr->is_equipped = true;
                                        pc.equip[slot_of(itmptr)] = itmptr;
                                        say("Item equipped!");
                                        done_with_sub = true;
                                    }/* end empty slot else */
                                    break;
                                case WEP: /* can be in WEP or OFF */
                                /* if unarmed arm without hesitation in 1st slot */
                                    if( pc.equip[WEP] == NULL )
                                    {
                                        itmptr->is_equipped = true;
                                        pc.equip[WEP] = itmptr;
                                        say("Item equipped!");
                                        done_with_sub = true;
                                    }/* end unarmed if */
                                /* else if !have secondary equipment arm in offhand without hesitation */
                                    else if( pc.equip[OFF] == NULL )
                                    {
                                        itmptr->is_equipped = true;
                                        pc.equip[OFF] = itmptr;
                                        say("Item equipped!");
                                        done_with_sub = true;
                                    }/* end off-hand unarmed if */
                                /* else ask if want to replace 1st or 2nd */
                                    else{
                                        do{say("Replace which item?");}while( ( cmd = get_hand() ) == NOT_PLACED);
                                        if( cmd == WEP )
                                        {
                                            pc.equip[WEP]->is_equipped = false;
                                            itmptr->is_equipped = true;
                                            pc.equip[WEP] = itmptr;
                                            say("Equipment swapped!");
                                            done_with_sub = true;
                                        }/* end replace WEP hand if */
                                        else if( cmd == OFF )
                                        {
                                            pc.equip[OFF]->is_equipped = false;
                                            itmptr->is_equipped = true;
                                            pc.equip[OFF] = itmptr;
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
                            if( pc.inventory[cmd].is_ == false )
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
                                    set_empty_item(&pc.inventory[cmd]);
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
                            if( pc.equip[cmd] == NULL )
                            {
                                say("You have no item equipped in that slot.");
                            }else /* item exists */
                            {
                            /* confirm destruction -- last chance */
                                say("Last chance: Are you sure you want to destroy that item? ");
                                if( toupper(getch()) == 'Y' )
                                {
                                /* destroy item */
                                    set_empty_item(pc.equip[cmd]);
                                    pc.equip[cmd] = NULL;
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
                            if( pc.inventory[cmd].is_ == false )
                            {
                                say("You have no item in that slot.");
                            }/* end no item if */
                            else/* There is a real item there */
                            {
                            /* Drop item */
                                // copy item to floor//remove from player//    set_empty_item();
                                if( ACTIVE_LOCATION.litter.is_ == false )
                                {
                                    swap_item( &ACTIVE_LOCATION.litter, &pc.inventory[cmd] );
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
                            if( pc.equip[cmd] == NULL )
                            {
                                say("You have no item equipped in that slot.");
                            }else /* item exists */
                            {
                            /* Drop item */
                                // copy item to floor// remove from player //    set_empty_item(pc.equip[cmd]);
                                if( ACTIVE_LOCATION.litter.is_ == false )
                                {
                                    pc.equip[cmd]->is_equipped = false;
                                    swap_item( &ACTIVE_LOCATION.litter, pc.equip[cmd] );
                                    pc.equip[cmd] = NULL;
                                    say("Item dropped!");
                                    if( slot_of( &ACTIVE_LOCATION.litter ) != ARM ) need_more_cmd = true;//equipped items that are non Armor are free to drop
                                }/* end empty floor location */
                                else say("You can't drop that here.");//TODO
                                done_with_sub = true;
                            }/* end item exists else */
                        }/* end equip select if */
                        else/* Handle Impossible Errors */
                            ERROR(NULL,"You broke it!", FAIL);
                    }/* end DROP_ITEM sub cmd */
                    else /* CANCELED */
                    {
                        need_more_cmd = true;
                        done_with_sub = true;
                    }/* end canceled else */
                }while(done_with_sub == false);/* end sub cmd */
            /* Replace Map *///TODO: Make function and find duplicate usage
                for( r = 0; r < MAX_ROW; r++ )
                    for( c = 0; c < MAX_COL; c++ )
                    {
                        move(r,c);
                        addch( get_map_icon( ACTIVE_LOCATION ) );
                    }/* end [r][c] screen initialization */
                move(pc.locr,pc.locc); addch('@');
            /* Clean Up */
                for( i = MAX_ITEM_WINDOWS-1; i <= 0; i-- ) delwin(cmd_list[i]);
                touchwin(stdscr);
            }/* end INVENTORY cmd */
////////////////////////////////////////////////
else if( cmd == PICK_UP )
{
    if( ACTIVE_LOCATION.litter.is_ == false )
    {
        say("There is nothing to pick up.");
    }/* end no litter if */
    else if( pc.inventory[MAX_SLOTS-1].is_ != false )
    {
        say("You're inventory is full!");
    }/* end inventory full if */
    else{
        for(i = 0; i < MAX_SLOTS; i++ )
        {
            if( pc.inventory[i].is_ == false )
            {
                swap_item(&ACTIVE_LOCATION.litter, &pc.inventory[i]);
                say("Item picked up!");
                break;
            }/* end got empty slot if */
        }/* end MAX_SLOTS for */
    }/* end pickup else */
}/* end PICK_UP if */
////////////////////////////////////////////////
            else if( cmd == DEBUG_ITEM )
            {
#ifdef DEBUG
    //DELETED//UNUSED
#endif
                need_more_cmd = true;
            }/* end debug_item cmd if */
            else exit( ERROR(NULL, "cmd processing", cmd) );
        }while( need_more_cmd == true );/* end need more cmd do */

//check for end of turn stuff
        score.turn += 1;
    }/* end run while */

/* Display And Save High Score Report */

/* TERMINATE PROGRAM */
//    endwin();/* End curses mode */
//    fclose(indata);
    return 0;
}/* end main func */
