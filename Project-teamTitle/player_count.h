//===============================================================================
//
//  2DアクションMaster版(gameover_select.h)
//								制作：元地弘汰
// 
//===============================================================================

#ifndef _PLAYERCOUNT_H_
#define _PLAYERCOUNT_H_

#include "main.h"



//プロトタイプ宣言
void InitPlayerCount(void);
void UninitPlayerCount(void);
void UpdatePlayerCount(void);
void DrawPlayerCount(void);
void SwitchSelectMode(void);
int ReturnSelectPlayerCount(void);
bool ReturnAnimFinish(void);
void AnimSwitch(void);

#endif 
