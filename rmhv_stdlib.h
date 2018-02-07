/******************************************************************************
 *                             rmhv_stdlib.h                                  *
 * Created by:      Roberto Morrel HildigerR Vergaray                         *
 *                                                                            *
 *    Includes memory allocation macros from the book                         *
 *      "Advanced C Techniques & Applications"                                *
 *          by Sobelman, Gerald E. & Krekelberg, David E.                     *
 ******************************************************************************/

#pragma once

/* Stystem Headers */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

//TODO: #define NDEBUG when complete
//#define NDEBUG
#include <assert.h>

#ifndef NDEBUG
    #define DEBUG_BARF
#endif

/* Allow All-Cap Booleans */
#ifndef TRUE
    #define TRUE true
#endif
#ifndef FALSE
    #define FALSE false
#endif

/* Global Variable Define/Init Helper */
#ifdef MAIN
    #define EXTERN
    #define INIT(x) = x
#else
    #define EXTERN extern
    #define INIT(x)
#endif

/* Precompiler Constants */
#define FAIL 1
#define KEY_ESC 27
#define READ_ONLY "r"

/* Macros */
#define ERROR(w,b,s) _ERROR(w,b,s,FALSE)
#define Error(b,s)   _ERROR(__FUNCTION__,b,s,FALSE)
#define BARF(b,s)    _ERROR("BARF",b,s,TRUE)
#define Barf(b,s)    _ERROR(NULL,b,s,TRUE)
#define WARN(b,s)    _ERROR("WARNING",b,s,TRUE)
/* From Sobelman & Krekelberg: */
#define MALLOC(x)   ((x*)malloc(sizeof(x)))
#define CALLOC(n,x) ((x*)calloc(n,sizeof(x)))

/* Prototypes */
int _ERROR( const char * who, const char * barf, int status, bool debug );
size_t stricpy( char * dest, const char * src, size_t n );
void mypause( bool display_prompt );


/************************************EOF***************************************/
