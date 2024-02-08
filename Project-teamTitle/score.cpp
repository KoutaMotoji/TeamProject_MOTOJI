//===============================================================================
//
//  弾出して爆発するプログラム(bullet.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "main.h"
#include <time.h>
#include "score.h"
#include "player_count.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
int g_nScore;
//弾の初期化処理
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
		//頂点バッファの生成
	pDevice = GetDevice();


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\SelectCnt\\counter.png",
		&g_pTextureScore);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);
	VERTEX_2D* pVtx;	//頂点情報のポインタ

	//頂点バッファをロックし、頂点バッファデータへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
		//頂点座標の設
		
	pVtx[0].pos = D3DXVECTOR3(1080, 570, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280, 570, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(1080, 720, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280, 720, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
		//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
	
	g_pVtxBuffScore->Unlock();
	g_nScore = ReturnSelectPlayerCount();
}

//弾の終了処理
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	
	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//弾の更新処理
void UpdateScore(void)
{
	float ppp = g_nScore * 0.25;
	VERTEX_2D* pVtx;	//頂点情報のポインタ
	//頂点バッファをロックし、頂点バッファデータへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + ppp, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + ppp, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + ppp, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + ppp, 1.0f);
	g_pVtxBuffScore->Unlock();

}

//弾の描画処理
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);
	//ポリゴンの描画処理
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);
}