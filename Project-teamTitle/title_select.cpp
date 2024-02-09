//===============================================================================
//
//  3D�`�[������(title_select.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "title.h"
#include "title_select.h"
#include "title_anim.h"
#include "player_count.h"
#include "input.h"
#include "game.h"

#define DEFTITLE_SELECT	(10)
#define TITLE_SELECT_COUNT	(5)

#define TITLE_ANY_HEIGHT			(100)
#define TITLE_START_WIDTH			(200)
#define TITLE_TUTORIAL_WIDTH		(400)
#define TITLE_FINISH_WIDTH			(150)
#define TITLE_ZOOM_SIZE				(15)

//�e�N�X�`����
static const char* TitleSelectName[DEFTITLE_SELECT]
{
	"data\\TEXTURE\\Title\\start0.png",			//�X�^�[�g�I��
	"data\\TEXTURE\\Title\\tutorial0.png",		//�`���[�g���A���I��
	"data\\TEXTURE\\Title\\ranking0.png",		//�`���[�g���A���I��
	"data\\TEXTURE\\Title\\finish0.png",		//�I���I��
	"data\\TEXTURE\\Title\\start1.png",			//�X�^�[�g��I��
	"data\\TEXTURE\\Title\\tutorial1.png",		//�`���[�g���A����I��
	"data\\TEXTURE\\Title\\ranking1.png",		//�`���[�g���A���I��
	"data\\TEXTURE\\Title\\finish1.png",		//�I����I��
	"data\\TEXTURE\\Title\\finishselect0.png",		//�I���I����(�͂�)
	"data\\TEXTURE\\Title\\finishselect1.png",		//�I���I����(������)
};
//�I�����̗�
enum
{//�����I������Ă邩�ȁH
	SELECTED_START = 0,
	SELECTED_TUTORIAL,
	SELECTED_RANKING,
	SELECTED_FINISH
};
//�I�����̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	float fAngle;
	float fLength;
	int Type;
}TITLEINFO;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTitleSelect[DEFTITLE_SELECT] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleSelect = NULL;
int nSelectTitle;
int nSelectFinish;
float nZoomTitle;
bool bSelectMode;
bool bSelectFinish;
TITLEINFO TitleInfo[TITLE_SELECT_COUNT];
//===============================================================================
//�^�C�g���I���̏���������
//===============================================================================
void InitTitleSelect(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	for (int i = 0; i < DEFTITLE_SELECT; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TitleSelectName[i],
			&g_pTextureTitleSelect[i]);
	}
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_SELECT_COUNT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleSelect,
		NULL);
	for (int i = 0; i < TITLE_SELECT_COUNT; i++)
	{
		TitleInfo[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		switch (i)
		{
		case SELECTED_START:		//�I���F�X�^�[�g
			TitleInfo[i].fAngle = atan2f(TITLE_START_WIDTH, TITLE_ANY_HEIGHT);
			TitleInfo[i].fLength = sqrtf(TITLE_START_WIDTH * TITLE_START_WIDTH + TITLE_ANY_HEIGHT * TITLE_ANY_HEIGHT) / 2.0f;
			TitleInfo[i].Type = SELECT_TITLE_START_ON;
			TitleInfo[i].pos = D3DXVECTOR3(300, 300, 0);
			break;
		case SELECTED_TUTORIAL:		//�I���F�`���[�g���A��
			TitleInfo[i].fAngle = atan2f(TITLE_TUTORIAL_WIDTH, TITLE_ANY_HEIGHT);
			TitleInfo[i].fLength = sqrtf(TITLE_TUTORIAL_WIDTH * TITLE_TUTORIAL_WIDTH + TITLE_ANY_HEIGHT * TITLE_ANY_HEIGHT) / 2.0f;
			TitleInfo[i].Type = SELECT_TITLE_TUTORIAL_OFF;
			TitleInfo[i].pos = D3DXVECTOR3(300, 400, 0);
			break;
		case SELECTED_RANKING:		//�I���F�����L���O
			TitleInfo[i].fAngle = atan2f(TITLE_TUTORIAL_WIDTH, TITLE_ANY_HEIGHT);
			TitleInfo[i].fLength = sqrtf(TITLE_TUTORIAL_WIDTH * TITLE_TUTORIAL_WIDTH + TITLE_ANY_HEIGHT * TITLE_ANY_HEIGHT) / 2.0f;
			TitleInfo[i].Type = SELECT_TITLE_RANKING_OFF;
			TitleInfo[i].pos = D3DXVECTOR3(300, 500, 0);
			break;
		case SELECTED_FINISH:		//�I���F�I��
			TitleInfo[i].fAngle = atan2f(TITLE_FINISH_WIDTH, TITLE_ANY_HEIGHT);
			TitleInfo[i].fLength = sqrtf(TITLE_FINISH_WIDTH * TITLE_FINISH_WIDTH + TITLE_ANY_HEIGHT * TITLE_ANY_HEIGHT) / 2.0f;
			TitleInfo[i].Type = SELECT_TITLE_FINISH_OFF;
			TitleInfo[i].pos = D3DXVECTOR3(300, 600, 0);
			break;
		case 4:
			TitleInfo[i].fAngle = atan2f(SCREEN_WIDTH, SCREEN_HEIGHT);
			TitleInfo[i].fLength = sqrtf(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT) / 2.0f;
			TitleInfo[i].Type = 8;
			TitleInfo[i].pos = D3DXVECTOR3((SCREEN_WIDTH*1.25f), (SCREEN_HEIGHT / 2), 0);
		}
	}
	VERTEX_2D* pVtx;	//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < TITLE_SELECT_COUNT; nCnt++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z - (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
		pVtx[0].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z - (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z + (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
		pVtx[1].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z + (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z - TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
		pVtx[2].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z - TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z + TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
		pVtx[3].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z + TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
		pVtx[3].pos.z = 0.0f;

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

		pVtx += 4;
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffTitleSelect->Unlock();
	nSelectTitle = 0;
	nSelectFinish = 0;
	nZoomTitle = 0;
	bSelectMode = false;
	bSelectFinish = false;
}

//===============================================================================
//�^�C�g���I���I������
//===============================================================================
void UninitTitleSelect(void)
{
	for (int nCnt = 0; nCnt < SELECT_TITLE_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTitleSelect[nCnt] != NULL)
		{
			g_pTextureTitleSelect[nCnt]->Release();
			g_pTextureTitleSelect[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleSelect != NULL)
	{
		g_pVtxBuffTitleSelect->Release();
		g_pVtxBuffTitleSelect = NULL;
	}
}

//===============================================================================
//�^�C�g���I���X�V����
//===============================================================================
void UpdateTitleSelect(void)
{
	if (ReturnTitleAnimFinish() == true)
	{
		if (bSelectMode == false)
		{
			if (bSelectFinish == false)
			{
				if (TitleInfo[4].pos.x < (SCREEN_WIDTH*1.25))
				{
					TitleInfo[4].pos.x += SCREEN_WIDTH * 0.025f;
				}
				if (GetJoypadTrigger(JOYKEY_UP,0) == true)
				{
					if (nSelectTitle == SELECTED_START)
					{
						nSelectTitle = SELECTED_FINISH;
					}
					else
					{
						nSelectTitle--;
					}
					nZoomTitle = 0;
				}
				else if (GetJoypadTrigger(JOYKEY_DOWN,0) == true)
				{
					if (nSelectTitle == SELECTED_FINISH)
					{
						nSelectTitle = SELECTED_START;
					}
					else
					{
						nSelectTitle++;
					}
					nZoomTitle = 0;
				}
				switch (nSelectTitle)
				{
				case SELECTED_START:		//�I���F�X�^�[�g
					TitleInfo[0].Type = SELECT_TITLE_START_ON;
					TitleInfo[1].Type = SELECT_TITLE_TUTORIAL_OFF;
					TitleInfo[2].Type = SELECT_TITLE_RANKING_OFF;
					TitleInfo[3].Type = SELECT_TITLE_FINISH_OFF;
					break;
				case SELECTED_TUTORIAL:		//�I���F�`���[�g���A��
					TitleInfo[0].Type = SELECT_TITLE_START_OFF;
					TitleInfo[1].Type = SELECT_TITLE_TUTORIAL_ON;
					TitleInfo[2].Type = SELECT_TITLE_RANKING_OFF;
					TitleInfo[3].Type = SELECT_TITLE_FINISH_OFF;
					break;
				case SELECTED_RANKING:		//�I���F�����L���O
					TitleInfo[0].Type = SELECT_TITLE_START_OFF;
					TitleInfo[1].Type = SELECT_TITLE_TUTORIAL_OFF;
					TitleInfo[2].Type = SELECT_TITLE_RANKING_ON;
					TitleInfo[3].Type = SELECT_TITLE_FINISH_OFF;
					break;
				case SELECTED_FINISH:		//�I���F�I��
					TitleInfo[0].Type = SELECT_TITLE_START_OFF;
					TitleInfo[1].Type = SELECT_TITLE_TUTORIAL_OFF;
					TitleInfo[2].Type = SELECT_TITLE_RANKING_OFF;
					TitleInfo[3].Type = SELECT_TITLE_FINISH_ON;
					break;
				}
			}
			else
			{
				if (TitleInfo[4].pos.x > (SCREEN_WIDTH *0.5f))
				{
					TitleInfo[4].pos.x -= SCREEN_WIDTH * 0.025f;
				}
				if (GetJoypadTrigger(JOYKEY_UP,0) == true || GetJoypadTrigger(JOYKEY_DOWN,0) == true)
				{
					if (nSelectFinish == 0)
					{
						nSelectFinish = 1;
					}
					else
					{
						nSelectFinish = 0;
					}
				}
				if (nSelectFinish == 0)
				{
					TitleInfo[4].Type = 8;
				}
				else
				{
					TitleInfo[4].Type = 9;
				}
				if (GetJoypadTrigger(JOYKEY_A,0) == true)
				{
					if (nSelectFinish == 0)
					{
						SetExitGame();
					}
					else
					{
						bSelectFinish = false;
						return;
					}
				}
			}
		}
		if (nZoomTitle < TITLE_ZOOM_SIZE)//�Y�[������
		{
			nZoomTitle += TITLE_ZOOM_SIZE * 0.1f;;
		}
		VERTEX_2D* pVtx;	//���_���̃|�C���^

		//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
		g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < TITLE_SELECT_COUNT; nCnt++)
		{

			if (TitleInfo[nCnt].Type == nCnt)
			{//�I������Ă�����̂��g��\��
				//���_���W�̐ݒ�
				pVtx[0].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z - (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength - nZoomTitle;
				pVtx[0].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z - (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength - nZoomTitle;
				pVtx[0].pos.z = 0.0f;
				pVtx[1].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z + (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength + nZoomTitle;
				pVtx[1].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z + (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength - nZoomTitle;
				pVtx[1].pos.z = 0.0f;
				pVtx[2].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z - TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength - nZoomTitle;
				pVtx[2].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z - TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength + nZoomTitle;
				pVtx[2].pos.z = 0.0f;
				pVtx[3].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z + TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength + nZoomTitle;
				pVtx[3].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z + TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength + nZoomTitle;
				pVtx[3].pos.z = 0.0f;
			}
			else
			{//��I�����k��
				//���_���W�̐ݒ�
				pVtx[0].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z - (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
				pVtx[0].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z - (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
				pVtx[0].pos.z = 0.0f;
				pVtx[1].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z + (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
				pVtx[1].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z + (D3DX_PI - TitleInfo[nCnt].fAngle)) * TitleInfo[nCnt].fLength;
				pVtx[1].pos.z = 0.0f;
				pVtx[2].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z - TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
				pVtx[2].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z - TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
				pVtx[2].pos.z = 0.0f;
				pVtx[3].pos.x = TitleInfo[nCnt].pos.x + sinf(TitleInfo[nCnt].rot.z + TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
				pVtx[3].pos.y = TitleInfo[nCnt].pos.y + cosf(TitleInfo[nCnt].rot.z + TitleInfo[nCnt].fAngle) * TitleInfo[nCnt].fLength;
				pVtx[3].pos.z = 0.0f;
			}

			pVtx += 4;
		}

		//���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffTitleSelect->Unlock();
		//���[�h����
		if (GetJoypadTrigger(JOYKEY_A,0) == true && bSelectMode == false)
		{
			if (nSelectTitle == SELECTED_FINISH)
			{//�����I�����I������Ă�����
				bSelectFinish = true;			//�Q�[���I���̑I����������
			}
			else if (nSelectTitle == SELECTED_RANKING)
			{
				MovetoRanking();
			}
			else
			{//����ȊO�Ȃ�
				SwitchSelectMode();		//�I�����[�h�؂�ւ��֐�
				AnimSwitch();			//�A�j���[�V�����؂�ւ��֐�
			}
		}
	}
}

//===============================================================================
//�^�C�g���I��`��
//===============================================================================
void DrawTitleSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleSelect, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	for (int nCnt = 0; nCnt < TITLE_SELECT_COUNT; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureTitleSelect[TitleInfo[nCnt].Type]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			4 * nCnt,
			2);
	}
}

//===============================================================================
//�I����ʂ�؂�ւ��鏈��(�Q�[���ƃ`���[�g���A��or�l���I��)
//===============================================================================
void SwitchSelectMode(void)
{
	bSelectMode = bSelectMode ? false : true;
}

//===============================================================================
//���ݐl���I�������[�h�I�����𔻒f����ׂɎg���܂�
//===============================================================================
bool ReturnNowSelect(void)
{
	return bSelectMode;
}

//===============================================================================
//�I�����ꂽ���[�h�𔻕ʂ���������Ԃ��֐�
//===============================================================================
int ReturnSelectedMode(void)
{
	if (nSelectTitle == 0)
	{
		return MODE_GAME;		//���Ɉڍs�����ʂ̓Q�[��
	}
	else
	{
		return MODE_TUTORIAL;	//���Ɉڍs�����ʂ̓`���[�g���A��
	}
}


