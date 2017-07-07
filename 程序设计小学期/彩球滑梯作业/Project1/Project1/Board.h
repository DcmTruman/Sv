#include"LinkList.h"
#include<Windows.h>


//∞Â◊”÷÷¿‡
#define bNormal			    0
#define bWithTrap			1
#define bMoving		        2
#define bJumping	        3
#define bBroken		        4

typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;