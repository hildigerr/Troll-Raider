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
    return ( ( 1 + rng(MAX_STAT_VAL) + adjust ) < ability )? true : false;
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
 * NOTE:  line format is ":T:N:M|K:X|...|@Name:"                              *
 *   where  T = Type Number                                                   *
 *          N = Min Qt of Starting Equipment                                  *
 *          M = Max Qt of Starting Equipment                                  *
 *              The number of K:X pairs which follow.                         *
 *        K:X = Equipment Reference Values                                    *
 *          X = Maximum index of K item type, or -1 if any.                   *
 * WARNING: XXX The line will have some elements replaced with '\0'.          *
 *          XXX The line is expected to be valid.                             *
 ******************************************************************************/
 static bool outfit_me( PLAYER * who,  char * line )
{
    unsigned int t, i, min_qt, max_qt;
    int itmt, itmm;
    char * cptr, * end = cptr = &line[1];

    /* Retrieve Player Type */
    while( *end != ':' ) ++end; *end = '\0';
    t = atoi( cptr );

    /* Find Min Qt of Starting Equipment */
    cptr = ++end; while( *end != ':' ) ++end; *end = '\0';
    min_qt = atoi( cptr );
    if( min_qt > MAX_HOLD ) min_qt = 0;

    /* Find Max Qt of Starting Equipment */
    cptr = ++end; while( *end != '|' ) ++end; *end = '\0';
    max_qt = atoi( cptr ); if( max_qt ) max_qt = 1 + rng( max_qt );
    if(( max_qt < min_qt )||( max_qt > MAX_HOLD )) max_qt = min_qt;

    /* Give Starting Equipment */
    for( i = 0; i < max_qt; i++ ) {
        /* Determine Options */
        cptr = ++end;
        while( *end != ':' ) ++end; *end = '\0';
        itmt = atoi( cptr );
        cptr = ++end;
        while( *end != '|' ) ++end; *end = '\0';
        itmm = atoi( cptr );
        if( itmm > 0 ) itmm = rng( itmm ); /* Pehaps get item lower in list */

        /* Retrieve Equipment */
        if( !(who->inventory[i] = getp_item( itmt, itmm )) ) {
            Error( "Failed to retrieve Equipment.", t );
            return false;
        }/* End getp_item If */

        /* NPCs Equip items */
        if( !who->is_main ) equip_me( who, i, false );
        //TODO: Maybe: Unequip and equip again if failed, randomly
        //              and destroy any extra equipment.
    }/* End Starting Equipment For */

    for(; i < MAX_HOLD; i++ ) who->inventory[i] = NULL;

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

    for( i = 0; i < MAX_STATS; i++ ) p->stats[i] = 1 + rng(MAX_STAT_VAL);
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
    do{ m = rng( data->max[t] ); } while( ( t == NPC_TTROLL )&&( m == 0 ) );
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
    for( i = 0; i < MAX_STATS; i++ ) who->stats[i] = 1 + rng(stat_seed);
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

    //TODO Re-sort inventory: bring nulls to end of list

    /* Get NPC Name */
    cptr = line;   while( *cptr != '@' ) ++cptr;
    end  = ++cptr; while( *end  != ':' ) ++end; *end = '\0';
    stricpy( who->name, cptr, MAX_NAME_LEN+1 );

    return true;
}/* end init_mon func */


/******************************************************************************
 * FUNCTION:    equip_me           -- Unequip an equipped item.               *
 * ARGUMENTS:   PLAYER * who       -- Who will perform the action?            *
 *              int      slot      -- Which item will be equiped?             *
 *              bool     verbose   -- Should we notify the player on success? *
 * RETURNS:     bool               -- TRUE if the action was performed.       *
 ******************************************************************************/
bool equip_me( PLAYER * who, int slot, bool verbose )
{
    ITEM * itmptr;
    int i, slot_of_itmptr;
    bool prompt = ( slot < 0 );

    assert( (prompt)? (verbose == true) : true );

    /* Must ask User Which Item? */
    if(( prompt )&&( (slot = get_i_slot("Equip which item? ")) == CANCEL )) {
        say("Equip Item Canceled.");
        return false;
    }/* End prompt and Cancel If */

    itmptr = who->inventory[slot];

    /* Verify Equipability */
    if( !itmptr ) {
        if( verbose ) say( "You don't have that many items." );
        return false;
    }/* End !exists if */
    if( !is_equipable(itmptr) ) {
        if( verbose ) vsay( "You cannot equip that %s.", itmptr->name );
        return false;
    }/* End !equipable If */

    slot_of_itmptr = slot_of(itmptr);

    /* Check item type and slot if needed */
    switch( slot_of_itmptr ) {

        case HAT: /* must equip to HAT slot */
        case ARM: /* must equip to ARM slot */
            if( who->equip[slot_of_itmptr] != NULL ) {
                if( !verbose ) return false;
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
            if( verbose ) vsay( "%s equipped!", itmptr->name ); //TODO describe better based on body part
            who->equip[slot_of_itmptr] = itmptr;
            for( i = slot; i < MAX_HOLD-1; i++ )
                who->inventory[i] = who->inventory[i+1];
            who->inventory[i] = NULL;
            return true;
            break;

        case WEP: { /* can be in WEP or OFF */
            /* Select Weapon Slot */
            int target = NOT_PLACED;
            /* if unarmed arm without hesitation in 1st slot */
            if( who->equip[WEP] == NULL ) target = WEP;
            /* else arm in offhand without hesitation if possible */
            else if( who->equip[OFF] == NULL ) target = OFF;

            else { /* Otherwise we Have to Ask */
                if( !verbose ) return false;
                if( who->equip[OFF]->is_2handed ) {
                    vsay( "Are you sure you want to replace the %s "
                          "you currently have equipped? ",
                                who->equip[OFF]->name );
                    if( toupper(getch()) == 'Y' ) target = WEP;

                } else { /* Dual Wielding */
                    /* Ask if want to replace 1st or 2nd Slot */
                    say("Replace which item?");
                    target = get_hand();
                }/* End Dual Wield Else */

                /* Unequip Old Item  */
                if( ( target == WEP )||( target == OFF ) ) {
                    unequip_me( who, target, verbose );//TODO Handle Overflow XXX
                    //TODO: takes extra turn.
                }/* End unequiping If */
            }/* End Ask Else */

            /* Equip New Item */
            if( ( target == WEP )||( target == OFF ) ) {
                if( verbose ) vsay( "%s equipped!", itmptr->name );
                who->equip[target] = itmptr;
                for( i = slot; i < MAX_HOLD-1; i++ )
                    who->inventory[i] = who->inventory[i+1];
                who->inventory[i] = NULL;
                return true;
            } else /* Canceled */
                if(verbose) vsay( "Canceled: %s not equipped.", itmptr->name );
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
                if( !verbose ) return false;
                vsay( "Are you sure you want to replace "
                      "the %s and %s with your %s? ",
                        who->equip[WEP]->name, who->equip[OFF]->name,
                            itmptr->name );
                yn = toupper(getch());
            } else if( determinent > EMPTY_HANDED ) {
                if( !verbose ) return false;
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
                //TODO: handle overflow XXX

                /* Equip New Item */
                if( verbose ) vsay( "%s equipped!", itmptr->name );
                who->equip[WEP] = itmptr; //XXX Is this how I want to do it?
                who->equip[OFF] = itmptr; //XXX Point both hands to the same item.
                for( i = slot; i < MAX_HOLD-1; i++ )
                    who->inventory[i] = who->inventory[i+1];
                who->inventory[i] = NULL;

                return true;

            } else  /* Canceled */
                if(verbose) vsay( "Canceled: %s not equipped.", itmptr->name );

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
                say("You have no item equipped for that slot");//TODO: describe bodypart
                return false;
            }/* End Check Slot If-Else */
        }/* End Select Slot Else */
    }/* End prompt If */

    /* Find a Place for it */
    for( i = 0; i < MAX_HOLD; i++ ) if( !who->inventory[i] ) break;
    if( i == MAX_HOLD )
        return (bool) WARN( "Inventory Full", false );//XXX TODO: handle overflow

    /* Unequip the Item */
    who->inventory[i] = who->equip[slot];
    if( verbose ) vsay( "%s removed", who->inventory[i]->name );
    if( who->equip[slot]->is_2handed ) who->equip[(slot==WEP)?OFF:WEP] = NULL;
    who->equip[slot] = NULL;
    return true;
}/* End unequip_me Func */


/************************************EOF***************************************/
