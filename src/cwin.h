/******************************************************************************
 *  Troll Raider                                                              *
 *      by Roberto Morrel HildigerR Vergaray                                  *
 * cwin.h -- Curses Window Functions.                                         *
 ******************************************************************************/
#pragma once

#include <curses.h>
#include <stdarg.h>
#include "types.h"

/* Display Window Definitions */
#define BTM_SUB_ROWS 4
#define BTM_SUB_COLS 80
#define RT_SUB_ROWS  20      /* should be equal to MAX_ROW */
#define RT_SUB_COLS  20

extern WINDOW * display_btm;


/******************************************************************************
 * FUNCTION:    wsay                                                          *
 * ARGUMENTS:   WINDOW *  where   -- The window to which we will write        *
 *              char   *  string  -- The string which will be written         *
 ******************************************************************************/
void wsay( WINDOW * where, const char * string );
#define say(x) wsay(display_btm,x)


/******************************************************************************
 * FUNCTION:    vsay                                                          *
 * ARGUMENTS:   const char * fmt  -- The format string.                       *
 *              ...               -- A variable quantity of arguments.        *
 * Writes a formatted string to the bottom display.                           *
 ******************************************************************************/
void vsay( const char * fmt, ... );


/******************************************************************************
 * FUNCTION:    init_display_right                                            *
 * ARGUMENTS:   WINDOW   *   d      -- Reference to the window to initialize  *
 *              PLAYER   *   p      -- The player whose statistics to display *
 *              STAT_DAT *   s      -- Game statistics to display             *
 * RETURNS:     bool                                                          *
 * NOTE:        d expects the right window.                                   *
 ******************************************************************************/
bool init_display_right( WINDOW * d, PLAYER * p, STAT_DAT * s );


/************************************EOF***************************************/
