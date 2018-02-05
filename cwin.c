/* $Id: cwin.c,v 1.5 2014/01/13 06:26:02 moonsdad Exp $ */
/******************************************************************************
 * 7drl0 :  _ Troll Raider _   by Roberto Morrel HildigerR Vergaray           *
 * cwin.c -- Curses Window Functions.                                         *
 ******************************************************************************/

#include "cwin.h"
#include "map.h"

WINDOW  *display_btm;

/******************************************************************************
 * FUNCTION:    wsay                                                          *
 * ARGUMENTS:   WINDOW* where   -- The window to which we will write          *
 *              char*   string  -- The string which will be written           *
 ******************************************************************************/
void wsay( WINDOW* where, char* string )
{
    int i = 0;

    /* Move all lines up */
    wmove(where,0,0);
    if( wdeleteln(where) == ERR )
        exit( ERROR( "wsay", "wdeleteln err", FAIL ) );
    /* print string at end */
    else while( string[i] != '\0' ) {
        wmove(where,BTM_SUB_ROWS-1,i);
        if( winsch(where,string[i++]) == ERR )
            exit( ERROR( "wsay", "message insert display", string[--i] ));
    }/* End string !terminated While */

    wrefresh(where);
}/* End wsay Func */


/******************************************************************************
 * FUNCTION:    draw_map                                                      *
 * ARGUMENTS:   LEVEL* curlv   -- The Level to draw.                          *
 ******************************************************************************/
void draw_map( LEVEL * curlv )
{
    int r, c;
    for( r = 0; r < MAX_ROW; r++ ) for( c = 0; c < MAX_COL; c++ ) {
        move(r,c);
        addch( get_map_icon( curlv->map[r][c] ) );
    }/* end [r][c] screen initialization */
}


/******************************************************************************
 * FUNCTION:    init_display_right                                            *
 * ARGUMENTS:   WINDOW*     d       -- Reference to the window to initialize  *
 *              PLAYER*     p       -- The player whose statistics to display *
 *              STAT_DAT*   s       -- Game statistics to display             *
 * RETURNS:     bool                                                          *
 * NOTE:        d expects the right window.                                   *
 ******************************************************************************/
bool init_display_right( WINDOW* d, PLAYER* p, STAT_DAT* s )
{
    int i;
    char* dflav[DFMAX] ={ /* for each dungeon lv an arbitrary 5 letter name */
                             "Vault",
                             "Jails",
                             "Depth"    };

    wmove(d,1,1);
    wprintw( d, " %s  LV %.2f   $%d\n\n  HP %d/%d\n",
        p->name, p->explv, p->money, p->hp[1], p->hp[0] );//[0]MAX//[1]CUR
    for( i = 0; i < MAX_STATS; i++ ) {
        wprintw( d, "\n" );
        switch(i) {
            case STR:    wprintw(d,"  STR ");        break;
            case AGI:    wprintw(d,"  AGI ");        break;
            case MEN:    wprintw(d,"  MEN ");        break;
            case CON:    wprintw(d,"  CON ");        break;
            case CHA:    wprintw(d,"  CHA ");        break;
            case LUC:    wprintw(d,"  LUC ");        break;
            default:    return false;
        }/* End i Switch */
        wprintw( d, "%d", p->stats[i] );
    }/* End MAX_STATS i For */
    wprintw( d, "\n\n  Food: %d\n  Family Saved: %d ", s->food, s->family );

    /* Initialize i for Flavor */
    i = rng(DFMAX) - 1;

    switch(p->maplv) {
//        case TVILLAGE:    wprintw(d,"\n\n Troll Village ");    break;
        case HVILLAGE:    wprintw(d,"\n\n Human Village ");    break;
        case IN_HHUTS:    wprintw(d,"\n\n In a Hut      ");    break;
        case CASL_GRD:    wprintw(d,"\n\n Castle Ground ");    break;
        case CASL_UP0:    wprintw(d,"\n\n Tower 1st LV  ");    break;
        case CASL_UP1:    wprintw(d,"\n\n Tower 2nd LV  ");    break;
        case CASL_UP2:    wprintw(d,"\n\n Tower 3rd LV  ");    break;
        case CASL_DN0:    case CASL_DN1:    case CASL_DN2:
            wprintw(d,"\n\n Dungeon %s ", dflav[i] );       break;
        default:        wprintw(d,"\n\n ERROR ");           break;
    }/*end switch*/
    wprintw(d,"\n  Turn: %d\n",s->turn);
    box(d,'|','-');//
    return true;
}/* end init_display_right func*/



/************************************EOF***************************************/
