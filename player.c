/* $Id: player.c,v 1.6 2014/01/13 08:50:50 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * player.c -- Player Character Utility Functions.                            *
 ******************************************************************************/

/* Local Headers */
#include "player.h"
#include "item.h"
#include "dat.h"

static DATA * data = NULL;

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
 * FUNCTION:    init_player                                                   *
 * ARGUMENTS:   PLAYER * p                                                    *
 * RETURNS:     bool                                                          *
 * NOTE:        Expects player's name to already be set.                      *
 ******************************************************************************/
 bool init_player( PLAYER * p )
{
    int i = 0;

    for( i = 0; i < MAX_STATS; i++ ) p->stats[i] = rng(MAX_STAT_VAL);
    for( i = 0; i < MAX_HOLD; i++ )
        if( set_empty_item( &p->inventory[i] ) != true ) return false;

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
    if( getp_item( &(p->inventory[0]), 1, 0 ) == false )
        return((bool) ERROR( NULL, "Failed to get starting equipment", false ));

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
 bool init_mon( PLAYER* who, int t )
{
    int m, i, j, stat_seed;
    char ** s, * cptr, * end, line[MAX_LINE_LEN];

    /* Load Item Data */
    if( !data && !(data = load_data( MON_DAT )) ) {
        ERROR( "init_mon", "Unable to load data", (int)false );
        return false;
    } else s = data->lines;

    /* Verify t Param */
    if( ( t < 0 )||( t >  data->qt[QT_TYPES] ) ) {
        ERROR( "getp_item", "Invalid t input parameter", t );
        //TODO: Suggest possible atafile corruption
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
    who->money = ( t > HUMAN_INCT )? rng( 10 * t ) : 0;//TODO Make meaningful

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
    for( i = 0; i < MAX_HOLD; i++ ) {
        if( !set_empty_item( &who->inventory[i] ) ) {
            ERROR( "init_mon", "Unable to init empty item.", i );
            return false;
        }/* End !set_empty_item If */
    }/* End MAX_HOLD For */

    for( i = 0; i < MAX_SLOTS; i++ ) who->equip[i] = NULL;

    /* Slice Line XXX Line validity Unchecked */
    if( (i = snprintf( line, MAX_LINE_LEN, "%s", *s )) < 1 ) {
        ERROR( "init_mon", strerror(i), i ); //TODO: Test
        return false;
    }/* End Copy Line If */
    end = cptr = &line[3]; //XXX Skip Type -- Unchecked

    /* Find Max Qt of Starting Equipment */
    while( *end != ':' ) ++end; *end = '\0';
    i = atoi( cptr );
    if( i < 0 ) i = 0; else if( i > MAX_HOLD ) i = MAX_HOLD;

    /* Determine Starting Equipment Options */
    cptr = ++end;
    while( *end != ':' ) ++end; *end = '\0';
    j = atoi( cptr );
    if( j < 0 ) j = 0;
    if( j > 0 ) { /* Starting Equipment */
        int * itmt = NULL, * itmm = NULL;
        if( !(itmt = CALLOC( j, int )) ) {
            ERROR( "init_mon", "Failed to allocate itmt array", __LINE__ );
            return false;
        }/* End !CALLOC If */
        if( !(itmm = CALLOC( j, int )) ) {
            free( itmt );
            ERROR( "init_mon", "Failed to allocate itmm array", __LINE__ );
            return false;
        }/* End !CALLOC If */

        /* Parse Equipment Reference Values */
        for( m = 0; m < j; m++ ) {
            cptr = ++end;
            while( *end != ':' ) ++end; *end = '\0';
            itmt[m] = atoi( cptr );
            cptr = ++end;
            while( *end != ':' ) ++end; *end = '\0';
            itmm[m] = atoi( cptr );
        }/* End Equipment For */

        /* Retrieve Equipment */
        for( i = rng( i ), m=0; i > 0; --i,m++ ) {
            int selection = ( itmm[i]  < 0 )? -1 :
                            ( itmm[i] == 0 )?  0 : rng( itmm[i] ) -1;
            if( !getp_item( &(who->inventory[m]), itmt[i], selection ) ) {
                ERROR( "init_mon", "Failed to retrieve Equipment.", i );
                free( itmt ); free( itmm );
                return false;
            }/* End getp_item If */
        }/* End Equipment i For */

        //TODO: Equip items, convert coins to who->money

        /* Clean Up */
        free( itmt ); free( itmm );
    }/* End Starting Equipment If */

    /* Get NPC Name */
    end += 2; cptr = end; while( *end != ':' ) ++end; *end = '\0';
    if( !stricpy( who->name, cptr, MAX_NAME_LEN+1 ) ) {
        ERROR( "init_mon", "Failed to record NPC Name.", MAX_NAME_LEN );
        return false;
    }/* End strncpy Name If */

    return true;
}/* end init_mon func */



/************************************EOF***************************************/
