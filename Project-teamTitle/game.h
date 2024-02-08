//===============================================================================
//
//  2DアクションMaster版(game.h)
//								制作：元地弘汰
// 
//===============================================================================

#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"


//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void GameFinish(void);
void TypeFrag(bool bType);
bool GameType(void);
#endif 
