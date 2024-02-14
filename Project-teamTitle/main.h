//===============================================================================
//
//  3Dゲーム基礎(main.h)
//								制作：元地弘汰
// 
//===============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_
#include <Windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)
#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"
//ライブラリのリンク	
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要
#pragma comment(lib,"xinput.lib")
//マクロ定義
#define SCREEN_WIDTH	(1980)
#define SCREEN_HEIGHT	(1080)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1)
#define FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//画面(モード)の種類
typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_RANKING,
	MODE_MAX
}MODE;

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_3D;
//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
void SetExitGame(void);
int GetFPS(void);
#endif 

