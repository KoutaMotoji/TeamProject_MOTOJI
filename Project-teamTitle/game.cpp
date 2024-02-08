//===============================================================================
//
//  2DアクションMaster版(game.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "game.h"
#include "input.h"
#include "stdio.h"
#include "stdlib.h"
#include "score.h"

int PlayerCnt;
float g_f;
LPDIRECT3DTEXTURE9 g_pTextureGame = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;
//ゲーム画面の初期化
void InitGame(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\game.png",
		&g_pTextureGame);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGame,
		NULL);
	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックして、頂点情報へのポインタを取得
	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 720, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280, 720, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	//頂点バッファのアンロック
	g_pVtxBuffGame->Unlock();

	InitScore();
	PlayerCnt;
	g_f = 0.0f;
}

//終了処理
void UninitGame(void)
{
	//テクスチャの破棄
	if (g_pTextureGame != NULL)
	{
		g_pTextureGame->Release();
		g_pTextureGame = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGame != NULL)
	{
		g_pVtxBuffGame->Release();
		g_pVtxBuffGame = NULL;
	}
	UninitScore();
}

//ゲーム更新
void UpdateGame(void)
{
	UpdateScore();

	VERTEX_2D* pVtx;	//頂点情報のポインタ

//頂点バッファをロックして、頂点情報へのポインタを取得
	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f +g_f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f +g_f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f +g_f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f +g_f, 1.0f);

	g_f += 0.04f;

	//頂点バッファのアンロック
	g_pVtxBuffGame->Unlock();
	if (GetJoypadTrigger(JOYPAD_A) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetMode(MODE_RESULT);
	}
}

//ゲーム描画
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGame, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGame);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		4);
	DrawScore();
}





