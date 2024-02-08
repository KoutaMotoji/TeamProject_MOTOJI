//===============================================================================
//
//  2DアクションMaster版(game.h)
//								制作：元地弘汰
// 
//===============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"


//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void RankingFinish(void);
void TypeFrag(bool bType);
bool RankingType(void);
#endif 
