/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * player.c -- Player Character Utility Functions.                            *
 ******************************************************************************/

#include "player.h"
#include "item.h"
#include "cwin.h"
#include "dat.h"

static DATA * data = NULL;


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
 * FUNCTION:    init_stat_data                                                *
 * ARGUMENTS:   STAT_DAT * d                                                  *
 ******************************************************************************/
 void init_stat_data( STAT_DAT * d )
{
    d->food   = 0;
    d->turn   = 0;
    d->family = 0;
    d->hut_qt = rng( MAX_HUTS );
}/* end init_stat_data func */


/******************************************************************************
 * FUNCTION:    get_i_slot      -- Get Inventory Slot from user.              *
 * ARGUMENTS:   char * prompt   -- The input prompt.                          *
 * RETURNS:     int             -- The chosen inventory slot.                 *
 ******************************************************************************/
 static int get_i_slot( char * prompt )
{
    int input;
    do{ say(prompt); input = getch();
        switch ( input ) {
            case KEY_ESC: case ' ': case 'q': case 'Q':     return CANCEL;
            case '1': case '2': case '3': case '4': case '5': case '6': case '7':
            case '8': case '9': case '0': return input - '0';
            default: input = NOT_PLACED;
        }/* End input Switch */
    } while( input == NOT_PLACED );
    return input;
}/* End get_i_slot Func */


/******************************************************************************
 * FUNCTION:    get_e_slot      -- Get equipment slot from user.              *
 * ARGUMENTS:   char * prompt   -- The input prompt.                          *
 * RETURNS:     int             -- The chosen equipment slot.                 *
 ******************************************************************************/
 static int get_e_slot( char * prompt )
{
    int input;
    do{ say(prompt); input = getch();
        switch (input) {
            case KEY_ESC: case ' ': case 'q': case 'Q':     return CANCEL;
            case 'a': case 'A': return 0;
            case 'b': case 'B': return 1;
            case 'c': case 'C': return 2;
            case 'd': case 'D': return 3;
            default:   input = NOT_PLACED; break;
        }/* End input Switch */
    } while( input == NOT_PLACED );
    return input;
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
 * FUNCTION:    outfit_me     -- Select starting equipment for a characater.  *
 * ARGUMENTS:   PLAYER * who  -- The character to equip.                      *
 *              char   * line -- The string describing how to allocate gear.  *
 * NOTE:  line format is ":I:Q:N:T:M:...:@Name:"                              *
 *   where  I = Type Number                                                   *
 *          Q = Max Qt of Starting Equipment                                  *
 *          N = The number of T:M pairs which follow.                         *
 *        T:M = Equipment Reference Values                                    *
 *          M = Maximum index of T type, or -1 if any.                        *
 * WARNING: XXX The line will have all ':' chars replaced with '\0'.          *
 ******************************************************************************/
 static bool outfit_me( PLAYER * who,  char * line )
{
    int i, qt, option_qt;
    char * cptr, * end = cptr = &line[3]; //XXX Skip Type -- Unchecked

    /* Find Max Qt of Starting Equipment */
    while( *end != ':' ) ++end; *end = '\0';
    qt = atoi( cptr );
    if( qt < 0 ) qt = 0; else if( qt > MAX_HOLD ) qt = MAX_HOLD;

    /* Determine Starting Equipment Options */
    cptr = ++end; while( *end != ':' ) ++end; *end = '\0';
    option_qt = atoi( cptr );
    if( option_qt < 0 ) option_qt = 0;

    if( qt && option_qt ) { /* Character Gets Starting Equipment */
        int * itmt = NULL, * itmm = NULL;
        if( !(itmt = CALLOC( option_qt, int )) ) {
            Error( "Failed to allocate itmt array", __LINE__ );
            return false;
        }/* End !CALLOC If */
        if( !(itmm = CALLOC( option_qt, int )) ) {
            free( itmt );
            Error( "Failed to allocate itmm array", __LINE__ );
            return false;
        }/* End !CALLOC If */

        /* Parse Equipment Reference Values */
        for( i = 0; i < option_qt; i++ ) {
            cptr = ++end;
            while( *end != ':' ) ++end; *end = '\0';
            itmt[i] = atoi( cptr );
            cptr = ++end;
            while( *end != ':' ) ++end; *end = '\0';
            itmm[i] = atoi( cptr );
        }/* End option_qt For */

        /* Retrieve Equipment */
        for( qt = rng( qt ), i=0; qt > 0; --qt,i++ ) {
            int selection = rng(option_qt)-1;
            if( !getp_item( &(who->inventory[i]), itmt[selection],
                                                  itmm[selection] ) ) {
                Error( "Failed to retrieve Equipment.", qt );
                free( itmt ); free( itmm );
                return false;
            }/* End getp_item If */
        }/* End qt For */

        //TODO: Equip items

        /* Clean Up */
        free( itmt ); free( itmm );
    }/* End Starting Equipment If */

    return true;
}/* End outfit_me Func */


/******************************************************************************
 * FUNCTION:    init_player                                                   *
 * ARGUMENTS:   PLAYER * p                                                    *
 * RETURNS:     bool                                                          *
 * NOTE:        Expects player's name to already be set.                      *
 ******************************************************************************/
 bool init_player( PLAYER * p )
{
    int i = 0;

    /* Load Item Data */
    if( !data && !(data = load_data( MON_DAT )) ) {
        Error( "Unable to load data", (int)false );
        return false;
    }/* End Load Data If */

    for( i = 0; i < MAX_STATS; i++ ) p->stats[i] = rng(MAX_STAT_VAL);
    for( i = 0; i < MAX_HOLD; i++ ) set_empty_item( &p->inventory[i] );

    for( i = 0; i < MAX_SLOTS; i++ ) p->equip[i] = NULL;

    p->hp[0] = p->stats[CON];
    p->hp[1] = p->hp[0];
    p->explv = 0.0;
    p->money = 0;
    p->locr = NOT_PLACED;
    p->locc = NOT_PLACED;
    p->maplv = HVILLAGE;

    p->is_main = true;
    p->is_alive = true;
    p->is_human = false;
    p->is_awake = true;

    /* GIVE PC A STARTING WEAPON */
    if( !outfit_me( p, data->lines[0] ) )
        return (bool)
            Error( "Failed to get starting equipment", false );

    return true;
}/* end init_player func */


/******************************************************************************
 * FUNCTION:    init_mon        -- Initialize Monsters (NPCS)                 *
 * ARGUMENTS:   PLAYER *  who   -- The thing being initialized.               *
 *              int        t    -- The type of monster to generate.           *
 * RETURNS:     bool            -- True on success.                           *
 *  NPC types are: NPC_TTROLL, HUMAN_INCT, HUMAN_COMN, and HUMAN_HERO         *
 * NOTE: NPC_TTROLL[0] is the Player.                                         *
 * TODO: Perhaps do rng(data->qt[QT_LINES]) and remove lines for uniques.     *
 ******************************************************************************/
 #define MAX_LINE_LEN 80
 bool init_mon( PLAYER * who, int t )
{
    int m, i, j, stat_seed;
    char ** s, * cptr, * end, line[MAX_LINE_LEN];

    /* Load Item Data */
    if( !data && !(data = load_data( MON_DAT )) ) {
        Error( "Unable to load data", (int)false );
        return false;
    } else s = data->lines;

    /* Verify t Param */
    if( ( t < 0 )||( t >  data->qt[QT_TYPES] ) ) {
        Error( "Invalid t input parameter", t );
        //TODO: Suggest possible datafile corruption
        return false;
    }/* End Verify t If */

    /* Select Random member of NPC Type */
    do{ m = rng(data->max[t]) - 1; } while( ( t == NPC_TTROLL )&&( m == 0 ) );
    for( i=0; i < t; i++ ) for( j=0; j < data->max[i]; j++ ) ++s;
    for( i=0; i < m; i++ ) ++s;

    /* Perform Initiliazations */
    who->locr = NOT_PLACED; who->locc = NOT_PLACED;
    who->maplv = NOT_PLACED;
    who->is_main = false;
    who->is_alive = true;
    who->is_awake = false;
    who->is_human = !( t == NPC_TTROLL );
    who->money = ( t > HUMAN_INCT )? rng( 10 * t ) : 0;//TODO Make meaningful, adjust based on class
    for( i = 0; i < MAX_HOLD; i++ ) set_empty_item( &who->inventory[i] );
    for( i = 0; i < MAX_SLOTS; i++ ) who->equip[i] = NULL;

    /* Get Character Stats */
    switch( t ) {
        case NPC_TTROLL:
            stat_seed =  ( MAX_STAT_VAL /2 ) +1;
            who->explv = 0.0;
            break;
        case HUMAN_INCT:
            stat_seed = MAX_STAT_VAL /2;
            who->explv = 0.05;
            break;
        case HUMAN_COMN:
            stat_seed = MAX_STAT_VAL;
            who->explv = 0.1;
            break;
        case HUMAN_HERO:
            stat_seed = MAX_STAT_VAL + ( MAX_STAT_VAL /2 );
            who->explv = 0.15;//TODO Make meaningful, adjust based on dificulty
    }/* End t Switch */
    for( i = 0; i < MAX_STATS; i++ ) who->stats[i] = rng(stat_seed);
    who->hp[1] = who->hp[0] = who->stats[CON];

    /* Slice Line XXX Line validity Unchecked */
    if( (i = snprintf( line, MAX_LINE_LEN, "%s", *s )) < 1 ) {
        Error( strerror(i), i ); //TODO: Test
        return false;
    }/* End Copy Line If */

    if( !outfit_me( who, line ) ) {
        Error( "Failed to outfit NPC.", t );
        return false;
    }/* End euip_me If */

    /* Get NPC Name */
    cptr = line;   while( *cptr != '@' ) ++cptr;
    end  = ++cptr; while( *end  != ':' ) ++end; *end = '\0';
    stricpy( who->name, cptr, MAX_NAME_LEN+1 );

    return true;
}/* end init_mon func */


/******************************************************************************
 * FUNCTION:    equip_me           -- Unequip an equipped item.               *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              int      slot      -- Which item will be removed?             *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
bool equip_me( PLAYER * who, int slot, bool verbose )
{
    ITEM * itmptr;
    int slot_of_itmptr;
    bool prompt = ( slot < 0 );

    /* Must ask User Which Item? */
    if(( prompt )&&( (slot = get_i_slot("Equip which item? ")) == CANCEL )) {
        say("Equip Item Canceled.");
        return false;
    }/* End prompt and Cancel If */

    itmptr = &(who->inventory[slot]);
    slot_of_itmptr = slot_of(itmptr);

    /* Verify Equipability */
    if( !is_equipable(itmptr) )
        vsay( "You cannot equip that %s.", itmptr->name );
    else if( itmptr->is_equipped )
        vsay("That %s is already equipped.", itmptr->name );

    /* Check item type and slot if needed */
    else switch( slot_of_itmptr ) {

        case HAT: /* must equip to HAT slot */
        case ARM: /* must equip to ARM slot */
            if( who->equip[slot_of_itmptr] != NULL ) {
                vsay( "Are you sure you want to replace the %s "
                      "you currently have equipped? ",
                        who->equip[slot_of_itmptr]->name );
                if( toupper(getch()) == 'Y' ) {
                    /* Unequip old item */
                    unequip_me( who, slot_of_itmptr, verbose );
                    //TODO Takes 2 turns//use skip turn flag
                    //  or force user to perform remove command
                    //  -- Maybe set via opt or difficulty lv?
                } else /* assume no */ {
                    vsay( "Canceled. You did not replace your %s.",
                            who->equip[slot_of_itmptr]->name );
                    return false;
                }/* End Y/N If-Else */
            }/* End !empty Slot If */
            if( verbose ) vsay( "%s equipped!", itmptr->name );
            itmptr->is_equipped = true;
            who->equip[slot_of_itmptr] = itmptr;
            return true;
            break;

        case WEP: { /* can be in WEP or OFF */
            /* Select Weapon Slot */
            slot = NOT_PLACED;
            /* if unarmed arm without hesitation in 1st slot */
            if( who->equip[WEP] == NULL ) slot = WEP;
            /* else arm in offhand without hesitation if possible */
            else if( who->equip[OFF] == NULL ) slot = OFF;

            else { /* Otherwise we Have to Ask */
                if( who->equip[OFF]->is_2handed ) {
                    vsay( "Are you sure you want to replace the %s "
                          "you currently have equipped? ",
                                who->equip[OFF]->name );
                    if( toupper(getch()) == 'Y' ) slot = WEP;

                } else { /* Dual Wielding */
                    /* Ask if want to replace 1st or 2nd Slot */
                    say("Replace which item?");
                    slot = get_hand();
                }/* End Dual Wield Else */

                /* Unequip Old Item  */
                if( ( slot == WEP )||( slot == OFF ) ) {
                    unequip_me( who, slot, verbose );
                    //TODO: takes extra turn.
                }/* End unequiping If */
            }/* End Ask Else */

            /* Equip New Item */
            if( ( slot == WEP )||( slot == OFF ) ) {
                if( verbose ) vsay( "%s equipped!", itmptr->name );
                itmptr->is_equipped = true;
                who->equip[slot] = itmptr;
                return true;
            } else /* Canceled */
                vsay( "Canceled: %s not equipped.", itmptr->name );
        } /* End WEP Case */ break;

        case OFF: { /* Two-handed Weapons */
            #define EMPTY_HANDED 0
            #define WEP_ONLY     1
            #define OFF_ONLY     2
            #define DUAL_WIELD   3
            #define TWO_HANDED   7
            int determinent = ( (who->equip[WEP])? 1 : 0 ) +
                ( (who->equip[OFF])?
                    ( (who->equip[OFF]->is_2handed)? 6 : 2 ) : 0 );
            char yn = 'A'; /* Ask */

            assert( itmptr->is_2handed );

            if( determinent == DUAL_WIELD ) {
                vsay( "Are you sure you want to replace "
                      "the %s and %s with your %s? ",
                        who->equip[WEP]->name, who->equip[OFF]->name,
                            itmptr->name );
                yn = toupper(getch());
            } else if( determinent > EMPTY_HANDED ) {
                vsay( "Are you sure you want to replace your %s with your %s? ",
                        (determinent == WEP_ONLY)?
                            who->equip[WEP]->name :
                            who->equip[OFF]->name,
                            itmptr->name );
                yn = toupper(getch());
            } else yn = 'Y'; /* EMPTY_HANDED -- Assume Yes */

            if( yn == 'Y' ) { /* Unequip old and Equip new */

                /* Uneqip Old Item *///TODO take extra turn(s)
                if( who->equip[WEP] ) unequip_me( who, WEP, verbose );
                if( who->equip[OFF] ) unequip_me( who, OFF, verbose );

                /* Equip New Item */
                if( verbose ) vsay( "%s equipped!", itmptr->name );
                itmptr->is_equipped = true;
                who->equip[WEP] = itmptr; //XXX Is this how I want to do it?
                who->equip[OFF] = itmptr; //XXX Point both hands to the same item.
                return true;

            } else  /* Canceled */
                vsay( "Canceled: %s not equipped.", itmptr->name );

        }/* End OFF Case */ break;

        default: /* This should be dead code: */
            ERROR( "Invalid item",
                   "verify datafile not corrupted!", slot_of_itmptr );
            assert( slot_of_itmptr == MAX_SLOTS ); /* Should Pass */
            assert( slot_of_itmptr != MAX_SLOTS ); /* Force Fail  */
    }/* end slot switch */
    return false;
}/* end EQUIPMENT sub cmd */


/******************************************************************************
 * FUNCTION:    unequip_me         -- Unequip an equipped item.               *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              int      slot      -- Which equipment slot will be removed?   *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
 bool unequip_me( PLAYER * who, int slot, bool verbose )
{
    ITEM * itmptr = NULL;
    int i, j, cnt = 0;
    bool prompt = ( slot < 0 );

    if( prompt ) { /* Must ask User which Slot to Unequip */
        /* count how many items we have equipped */
        for( i = 0; i < MAX_SLOTS; i++ ) if( who->equip[i] != NULL ) {
            cnt += 1;
            /* In case there is only 1 Save reference to it */
            itmptr = who->equip[i]; j = i;
        }/* End equip For If */

        if( cnt == 0 ) { /* Nothing Equipped */
            say("You have nothing equipped.");
            return false;
        } else if(( cnt == 1 )|| /* assume unequipping that one */
                 (( cnt == 2 )&&( itmptr->is_2handed ))) {
            vsay( "Do you really want to remove your %s?", itmptr->name );
            if( toupper( getch() ) == 'Y' ) slot = j;
            else { /*  Canceled */
                vsay( "Canceled! %s still equipped.", itmptr->name );
                return false;
            }/* End Canceled Else */
        } else { /* Select Slot */
            slot = get_e_slot("Remove which item? ");
            if( slot == CANCEL ) {
                say("Canceled: Items still equipped.");
                return false;
            } else if( who->equip[slot] == NULL ) {
                say("You have no item equipped for that slot");
                return false;
            }/* End Check Slot If-Else */
        }/* End Select Slot Else */
    }/* End prompt If */

    /* Unequip the Item */
    if( verbose ) vsay( "%s removed", who->equip[slot]->name );
    who->equip[slot]->is_equipped = false;
    if( who->equip[slot]->is_2handed ) who->equip[(slot==WEP)?OFF:WEP] = NULL;
    who->equip[slot] = NULL;
    return true;
}/* End unequip_me Func */


/************************************EOF***************************************/
