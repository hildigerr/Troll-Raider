/******************************************************************************
 *                             rmhv_stdlib.h                                  *
 * Created by:      Roberto Morrel HildigerR Vergaray <moonsdad@gmail.com>    *
 *                                                                            *
 *    Includes memory allocation macros from the book                         *
 *      "Advanced C Techniques & Applications"                                *
 *          by Sobelman, Gerald E. & Krekelberg, David E.                     *
 ******************************************************************************/

#pragma once

/* Stystem Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Precompiler Constants */
#ifdef DEBUG
    #define DEBUG_DISPLAY 1
#else
    #define DEBUG_DISPLAY 0
#endif

/* Precompiler Constants */
#define FAIL 1
#define KEY_ESC 27
#define READ_ONLY "r"

/* For Bit Manipulation */ /* Opposite of Traditional Value */
#define ONN 0
#ifndef OFF //TODO: change reused OFF to OFH or this to BOFF and BONN
    #define OFF 1
#endif

/* Macros */
#define ERROR(w,b,s) _ERROR(w,b,s,DEBUG_DISPLAY)
#define BARF(m,i)    _ERROR("BARF",m,i,1)
/* From Sobelman & Krekelberg: */
#define MALLOC(x)   ((x*)malloc(sizeof(x)))
#define CALLOC(n,x) ((x*)calloc(n,sizeof(x)))

/* Prototypes */
inline int _ERROR( const char* who, const char* barf, int status, int boo );
size_t stricpy( char* dest, const char* src, size_t n );
inline void mypause ( int );


/************************************EOF***************************************/
