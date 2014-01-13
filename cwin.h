/* $Id: cwin.h,v 1.3 2014/01/13 06:26:02 moonsdad Exp $ */
/******************************************************************************
 * cwin -- Curses Window Functions.                                           *
 ******************************************************************************/
#pragma once

/* System Header */
#include <curses.h>

/* Local Headers */
#include "rmhv_stdlib.h"
#include "calc.h"
#include "types.h"

/* Display Window Definitions */
#define BTM_SUB_ROWS 4
#define BTM_SUB_COLS 80
#define RT_SUB_ROWS  20      /* should be equal to MAX_ROW */
#define RT_SUB_COLS  20


/******************************************************************************
 * FUNCTION:    wsay                                                          *
 * ARGUMENTS:   WINDOW* where   -- The window to which we will write          *
 *              char*   string  -- The string which will be written           *
 ******************************************************************************/
void wsay( WINDOW* where, char* string );
#define say(x) wsay(display_btm,x)


/******************************************************************************
 * FUNCTION:    init_display_right                                            *
 * ARGUMENTS:   WINDOW*     d       -- Reference to the window to initialize  *
 *              PLAYER*     p       -- The player whose statistics to display *
 *              STAT_DAT*   s       -- Game statistics to display             *
 * RETURNS:     bool                                                           *
 * NOTE:        d expects the right window.                                   *
 ******************************************************************************/
bool init_display_right( WINDOW* d, PLAYER* p, STAT_DAT* s );



/************************************EOF***************************************/
