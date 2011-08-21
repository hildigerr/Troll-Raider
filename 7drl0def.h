#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>


#define MALLOC(x) ((x *)malloc(sizeof(x)))
#define CALLOC(n,x) ((x *)calloc(n, sizeof(x)))

#define say(x) wsay(display_btm,x)

/* Current Level Array Access Definitions */
//#define TVILLAGE 0 /* Troll Village */
#define HVILLAGE	0 /* Human Village */
#define IN_HHUTS	1 /* Inside Human Huts *//* divide into sections accessed 1 at a time through the human village */
#define CASL_GRD	2 /* Castles Ground Level */
#define CASL_UP0	3 /* Castles 1st Upper Level */
#define CASL_UP1	4 /* Castles 2nd Upper Level */
#define CASL_UP2	5 /* Castles 3rd Upper Level *///current "highest" level
#define CASL_DN0	6 /* Castles 1st Lower Level */
#define CASL_DN1	7 /* Castles 2nd Lower Level */
#define CASL_DN2	8 /* Castles 3rd Lower Level *///current "lowest" level
#define DFMAX  3    /* Maximum Dungeon Name Flavors */
#define MAX_MAPS	9 /* TOTAL NUMBER OF MAPS PER GAME */

/* Character Abilities */
#define STR 0 /*Strength*/
#define AGI 1 /*Agility*/
#define MEN 2 /*Mental*/
#define CON 3 /*Constitution*/
#define CHA 4 /*Charisma*/
#define LUC 5 /*Luck*/
#define MAX_STATS 6
#define MAX_STAT_VAL 10

/* Item Statistics */
#define TO_HIT 0
#define TO_DAM 1
#define MAX_ITEM_STATS 2

#define MAX_ITM_NAME_LEN 10
#define MAX_ITM_TYPES 7
#define MAX_ITM_PER_TYPE 5


/* Equipment Slots */
#define WEP 0 /*Weapon*/
#define OFF 1 /*Off-Hand (shield or weapon)*///REUSED or bit manipulation with ONN
#define ARM 2 /*Armor*/
#define HAT 3 /*Head*/
#define MAX_SLOTS 4
#define MAX_HOLD 10

#define ONN 0 /*for bit manipulation with OFF*/

/* Map/LV Size Definitions */
#define MAX_ROW 20
#define MAX_COL 60

#define MAX_LV_MON 0
#define MAX_LV_ITM 10

#define MAX_HUTS 5
#define MAX_HUT_WID 4//These are
#define MAX_HUT_HT  6//Inverted for some reason//TODO:Figure out why
#define MIN_HUT_DIST 4.0

#define EAST_WEST 0
#define NORTH_SOUTH 1

/* Display Window Definitions */
#define BTM_SUB_ROWS 4
#define BTM_SUB_COLS 80
#define RT_SUB_ROWS 20//should be same as MAX_ROW
#define RT_SUB_COLS 20

#define FAIL 1
#define READ_ONLY "r"
#define NOT_PLACED -99 //creature location r,c
#define CHECK_FLOAT -99.9

/* Map Generation Macros */
#define ACTIVE_LOCATION	curlv[pc.maplv].map[r][c]

#define DISTANCE_TO_NORTH_WALL dist( hutspot[0][i], 0            , hutspot[1][i], hutspot[1][i] )
#define DISTANCE_TO_EAST_WALL  dist( hutspot[0][i], hutspot[0][i], hutspot[1][i], MAX_ROW       )
#define DISTANCE_TO_SOUTH_WALL dist( hutspot[0][i], MAX_COL      , hutspot[1][i], hutspot[1][i] )
#define DISTANCE_TO_WEST_WALL  dist( hutspot[0][i], hutspot[0][i], hutspot[1][i], 0             )

/* get_cmd return values */
#define NO_ACTION  0
#define NORTH      1
#define SOUTH		 2
#define EAST		 3
#define WEST		 4
#define NORTH_EAST 5
#define NORTH_WEST 6
#define SOUTH_EAST 7
#define SOUTH_WEST 8
#define WAIT		 9
#define QUIT		10

/* Storyline Time Definitions */
#define BEGINNING 0
#define MIDDLE 1
#define END 2

typedef struct boo_struct{
	unsigned bbool : 1;
}MYBOOL;

typedef struct loc_struct{
	unsigned is_floor : 1;
	unsigned is_occupied : 1;
	unsigned is_visible : 1;
	unsigned is_wall : 1;
	unsigned is_door : 1;
	unsigned is_ustair : 1;
	unsigned is_dstair : 1;
	unsigned is_trap : 1;
}LOC;

typedef struct item_struct{
	char name[MAX_ITM_NAME_LEN + 1];
	unsigned is_ : 1;
	unsigned short int type;
	int stats[MAX_ITEM_STATS];
	int worth;
}ITEM;

typedef struct statistics_structure {
	int food;	
	long turn;
	int family;//TODO: make family types and rank score based on who saved
}STAT_DAT;

typedef struct pc_struct{
	char* name;
	int stats[MAX_STATS];
	float explv; /*lv is int val, xp is decimal*/
	int money;
	int locr, locc, maplv;
	ITEM inventory[MAX_HOLD];
	ITEM* equip[MAX_SLOTS];
	short attdir; /* Attack Direction */
	unsigned is_main : 1;
	unsigned is_alive : 1;
	unsigned is_human : 1;
	unsigned is_unique : 1;
}PLAYER;

typedef struct level_struct{
	LOC map[MAX_ROW][MAX_COL];
	PLAYER mon[MAX_LV_MON];
	ITEM itm[MAX_LV_ITM];
	int type; /* Current Level Array Access */
	unsigned is_new : 1; /* Unexplored */
}LEVEL;
