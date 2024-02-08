//===============================================================================
//
//  2D�A�N�V����Master��(gameover_select.h)
//								����F���n�O��
// 
//===============================================================================

#ifndef _TITLESELECT_H_
#define _TITLESELECT_H_

#include "main.h"

typedef enum
{
	SELECT_TITLE_START_ON = 0,
	SELECT_TITLE_TUTORIAL_ON,
	SELECT_TITLE_RANKING_ON,
	SELECT_TITLE_FINISH_ON,
	SELECT_TITLE_START_OFF,
	SELECT_TITLE_TUTORIAL_OFF,
	SELECT_TITLE_RANKING_OFF,
	SELECT_TITLE_FINISH_OFF,
	SELECT_TITLE_MAX
}TITLE_SELECT;

//�v���g�^�C�v�錾
void InitTitleSelect(void);
void UninitTitleSelect(void);
void UpdateTitleSelect(void);
void DrawTitleSelect(void);
void SwitchSelectMode(void);
bool ReturnNowSelect(void);
int ReturnSelectedMode(void);
#endif 
