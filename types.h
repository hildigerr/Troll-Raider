/* $Id: types.h,v 1.6 2014/01/13 08:50:50 moonsdad Exp $ */
/******************************************************************************
 ******************************************************************************/
#pragma once

/* Local Header */
#include "rmhv_stdlib.h"

/* Precompiler Constants */
#define CANCEL      -1
#define NOT_PLACED  99
#define CHECK_FLOAT 99.9

/* get_cmd return values *//* get_subi_cmd return values */
#define NO_ACTION    0
#define NORTH        1
#define SOUTH        2
#define EAST         3
#define WEST         4
#define NORTH_EAST   5
#define NORTH_WEST   6
#define SOUTH_EAST   7
#define SOUTH_WEST   8
#define WAIT         9
#define QUIT         10
#define SUB_FIN      10
#define INVENTORY    11
#define EQUIPMENT    12
#define DROP_ITEM    13
#define REMOVE_ITEM  14
#define DESTROY_ITEM 15
#define PICK_UP      16
#define DEBUG_ITEM   -9

/* Character Abilities */
#define STR         0 /*Strength*/
#define AGI         1 /*Agility*/
#define MEN         2 /*Mental*/
#define CON         3 /*Constitution*/
#define CHA         4 /*Charisma*/
#define LUC         5 /*Luck*/
#define MAX_STATS   6
#define MAX_STAT_VAL 10

/* Item Statistics */
#define TO_HIT          0
#define TO_DAM          1
#define MAX_ITEM_STATS  2

#define ITEM_DAT "dat/item.dat"
#define MAX_ITEM_NAME_LEN   10
#define MAX_LV_ITEM         10

/* Equipment Types */
#define MONEY           0
#define MELEE           1
#define REACH           2
#define RANGE1          3
#define RANGE2          4
#define SHIELD          5
#define TYPHAT          6
#define TYPARM          7
#define MAX_ITEM_TYPES  8

/* NPCs */
#define MON_DAT "dat/human.dat"
#define MAX_NAME_LEN 11
#define MAX_FAM 45
#define MAX_NPC 45

/* Equipment Slots */
#define WEP         0 /*Weapon*//* Main Hand */
#define OFF         1 /*Off-Hand (shield or weapon)*/
#define ARM         2 /*Armor*/
#define HAT         3 /*Head*/
#define MAX_SLOTS   4

#define MAX_HOLD 10

/* Map/LV Size Definitions */
#define MAX_ROW 20
#define MAX_COL 60

#define MAX_HUTS     5
#define MAX_HUT_WID  6
#define MAX_HUT_HGT  4
#define MIN_HUT_WID  1
#define MIN_HUT_HGT  1
#define MIN_HUT_DIST 4.0

#define EAST_WEST   0
#define NORTH_SOUTH 1

/* Current Level Array Access Definitions */
#define HVILLAGE    0 /* Human Village */
#define IN_HHUTS    1 /* Inside Human Huts */
#define CASL_GRD    2 /* Castles Ground Level */
#define CASL_UP0    3 /* Castles 1st Upper Level */
#define CASL_UP1    4 /* Castles 2nd Upper Level */
#define CASL_UP2    5 /* Castles 3rd Upper Level *///current "highest" level
#define CASL_DN0    6 /* Castles 1st Lower Level */
#define CASL_DN1    7 /* Castles 2nd Lower Level */
#define CASL_DN2    8 /* Castles 3rd Lower Level *///current "lowest" level
#define DFMAX       3 /* Maximum Dungeon Name Flavors */
#define MAX_MAPS    9 /* TOTAL NUMBER OF MAPS PER GAME */

typedef struct {
    char name[MAX_ITEM_NAME_LEN + 1];
    int stats[MAX_ITEM_STATS];
    int worth;
    unsigned is_ : 1;
    unsigned is_equipped :1;
    unsigned is_2handed : 1;
    unsigned is_XXX : 1; /* unused */
    unsigned short int type;
} ITEM;

typedef struct {
    char name[MAX_NAME_LEN+1];
    int stats[MAX_STATS];
    ITEM inventory[MAX_HOLD];
    ITEM* equip[MAX_SLOTS];
    int hp[2];//[0]MAX//[1]CUR
    float explv; /*lv is int val, xp is decimal*/
    int money;
    int locr, locc, maplv;
    unsigned is_main  : 1;
    unsigned is_alive : 1;
    unsigned is_human : 1;
    unsigned is_awake : 1;
} PLAYER;

typedef struct {
    int food;
    unsigned long turn;
    int family;//TODO: make family types and rank score based on who saved
    unsigned short hut_qt;
} STAT_DAT;

typedef struct {
    int colx;
    int rowy;
} COORD;

typedef struct {
    COORD a;
    COORD b;
} RECT;

typedef struct {
    ITEM litter; /* TODO: make ptr, malloc as needed */
    unsigned is_floor : 1;
    unsigned is_occupied : 1;
    unsigned is_visible : 1;
    unsigned is_wall : 1;
    unsigned is_door : 1;
    unsigned is_ustair : 1;
    unsigned is_dstair : 1;
    unsigned is_trap : 1;
} LOC;

typedef struct level_struct {
    LOC map[MAX_ROW][MAX_COL];
    int type; /* Current Level Array Access */
    unsigned is_new : 1; /* Unexplored */
}LEVEL;
