#ifndef _GAME_H_
#define _GAME_H_



#define START_X				0.0
#define START_Y				0.0

#define N_COL	24
#define N_ROW	6



#define SX		1.0
#define SY		1.0

#define HSX		((float)SX * 0.5)
#define HSY		((float)SY * 0.5)

#define SCREEN_LEFT		(-SX)
#define SCREEN_RIGHT	(SX * (N_COL+1) + 10)
#define SCREEN_BOTTOM	(-SY)
#define SCREEN_TOP		(SY * (N_ROW+1))



#define BOX_MAX	(N_COL*(N_ROW-0))

#define BOX_X_INIT	(N_COL+2)
#define BOX_Y_INIT	(N_ROW+2)

#define BOX_OFF		0
#define BOX_FALLS	1
#define BOX_LIES	2

#define MAN_STAYS		3
#define MAN_JUMPS		4
#define MAN_FLAG_UP_0	5
#define MAN_FLAG_UP_1	6
#define MAN_FLAG_UP_2	7


#define MAN_DANGLEY		20

typedef struct
{
	int x;
	int y;
	unsigned int status;
} t_box;

typedef struct 
{
	float x; 
	float y;
	float ay;
	unsigned int flag_up;
	unsigned int status;

} t_man;

/***********/
#define GAME		0
#define GAMEOVER	1
#define GAMEPAUSE	2
#define GAMEMENU	3
/***********/
#define TEXT_LINES_X	(START_X + (N_COL+2)*SX)

#define RES_LINES_X		TEXT_LINES_X
#define RES_LINES_Y		(SY * 2)

#define RES_POBASHKE_X	TEXT_LINES_X
#define RES_POBASHKE_Y	(SY * 3)

/***********/
#define MENU_COLOR_TEXT_R	1
#define MENU_COLOR_TEXT_G	1
#define MENU_COLOR_TEXT_B	1

#define MENU_COLOR_RECT_R	0
#define MENU_COLOR_RECT_G	0.5
#define MENU_COLOR_RECT_B	0

#define MENU_NEW_LEFT	(SCREEN_LEFT+(SCREEN_RIGHT - SCREEN_LEFT) * 0.2)
#define MENU_NEW_RIGHT	(SCREEN_LEFT+(SCREEN_RIGHT - SCREEN_LEFT) * 0.7)
#define MENU_NEW_TOP	(SCREEN_BOTTOM+(SCREEN_BOTTOM - SCREEN_TOP) * 0.9)
#define MENU_NEW_BOTTOM	(SCREEN_BOTTOM+(SCREEN_BOTTOM - SCREEN_TOP) * 0.8)

/***********/
#define TICS_MIN		40	
#define DTICS_MAX		80

#define DTICS_BOX_FALLS	10
#define DTICS_BOX_NEW	30

#define DTICS_FMOD_NEW		0
#define DTICS_FMOD_MOVE		1
#define DTICS_FMOD2			2
#define DTICS_FMOD3			3
#define DTICS_FMOD_FALL		(DTICS_BOX_FALLS/2)

void timer(void);

#endif