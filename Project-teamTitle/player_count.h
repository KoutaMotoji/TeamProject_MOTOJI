//===============================================================================
//
//  2D�A�N�V����Master��(gameover_select.h)
//								����F���n�O��
// 
//===============================================================================

#ifndef _PLAYERCOUNT_H_
#define _PLAYERCOUNT_H_

#include "main.h"



//�v���g�^�C�v�錾
void InitPlayerCount(void);
void UninitPlayerCount(void);
void UpdatePlayerCount(void);
void DrawPlayerCount(void);
void SwitchSelectMode(void);
int ReturnSelectPlayerCount(void);
bool ReturnAnimFinish(void);
void AnimSwitch(void);

#endif 
