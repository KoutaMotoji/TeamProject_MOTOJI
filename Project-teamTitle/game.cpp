//===============================================================================
//
//  2D�A�N�V����Master��(game.cpp)
//								����F���n�O��
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
//�Q�[����ʂ̏�����
void InitGame(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\game.png",
		&g_pTextureGame);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGame,
		NULL);
	VERTEX_2D* pVtx;	//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 720, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280, 720, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);



	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffGame->Unlock();

	InitScore();
	PlayerCnt;
	g_f = 0.0f;
}

//�I������
void UninitGame(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureGame != NULL)
	{
		g_pTextureGame->Release();
		g_pTextureGame = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGame != NULL)
	{
		g_pVtxBuffGame->Release();
		g_pVtxBuffGame = NULL;
	}
	UninitScore();
}

//�Q�[���X�V
void UpdateGame(void)
{
	UpdateScore();

	VERTEX_2D* pVtx;	//���_���̃|�C���^

//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f +g_f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f +g_f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f +g_f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f +g_f, 1.0f);

	g_f += 0.04f;

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffGame->Unlock();
	if (GetJoypadTrigger(JOYPAD_A) == true || GetKeyboardTrigger(DIK_RETURN) == true)
	{
		SetMode(MODE_RESULT);
	}
}

//�Q�[���`��
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGame, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGame);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		4);
	DrawScore();
}





