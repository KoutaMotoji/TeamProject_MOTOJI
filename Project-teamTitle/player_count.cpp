//===============================================================================
//
//  3D�`�[������(player_count.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "player_count.h"
#include "title_select.h"
#include "input.h"

#define SELECT_TEXCOUNT	(11)		//�e�N�X�`���̐�
#define SELECT_USE_POLY	(7)
#define SELECT_BASE_POS_X	(SCREEN_WIDTH * 0.5+ 320)

//�e�N�X�`����
static const char* PSelCnt[SELECT_TEXCOUNT]
{
	"data\\TEXTURE\\SelectCnt\\pSelect.png",	//���t������
	"data\\TEXTURE\\SelectCnt\\gear_l.png",		//���̎���
	"data\\TEXTURE\\SelectCnt\\gear_r.png",		//�E�Ɏ���
	"data\\TEXTURE\\SelectCnt\\p1_on.png",		//��l�F�t��
	"data\\TEXTURE\\SelectCnt\\p2_on.png",		//��l�F�t��
	"data\\TEXTURE\\SelectCnt\\p3_on.png",		//�O�l�F�t��
	"data\\TEXTURE\\SelectCnt\\p4_on.png",		//�l�l�F�t��
	"data\\TEXTURE\\SelectCnt\\p1_off.png",		//��l�F�Ȃ�
	"data\\TEXTURE\\SelectCnt\\p2_off.png",		//��l�F�Ȃ�
	"data\\TEXTURE\\SelectCnt\\p3_off.png",		//�O�l�F�Ȃ�
	"data\\TEXTURE\\SelectCnt\\p4_off.png",		//�l�l�F�Ȃ�
};

//�e�N�X�`�����蓖��(���͏�ɁV)
typedef enum
{
	TEXTYPE_BASE = 0,
	TEXTYPE_GEAR_L,
	TEXTYPE_GEAR_R,
	TEXTYPE_PLAYER1_ON,
	TEXTYPE_PLAYER2_ON,
	TEXTYPE_PLAYER3_ON,
	TEXTYPE_PLAYER4_ON,
	TEXTYPE_PLAYER1_OFF,
	TEXTYPE_PLAYER2_OFF,
	TEXTYPE_PLAYER3_OFF,
	TEXTYPE_PLAYER4_OFF,
	TEXTYPE_MAX
}SELECT_TYPE;

//�v���C���[�Z���N�g�p�\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	float fHeight;
	float fWidth;
	float fAngle;
	float fLength;
	int Type;
}PLAYERSELECT;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePlayerCount[SELECT_TEXCOUNT] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayerCount = NULL;
int nSelectPlayerCnt;		//�l���I��p�ϐ�
float nZoomSelectPCnt;		//�Y�[���C���E�A�E�g�p
bool bSelectPlayerAnim;		//�A�j���[�V��������p
bool AnimFinish;			//�A�j���[�V�����I������
bool bSelPCnt;
PLAYERSELECT PlayerSelect[SELECT_USE_POLY];			//�v���C���[�I���\����

//===============================================================================
//�l���I����ʂ̏���������
//===============================================================================
void InitPlayerCount(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	for (int i = 0; i < SELECT_TEXCOUNT; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			PSelCnt[i],
			&g_pTexturePlayerCount[i]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SELECT_USE_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayerCount,
		NULL);

	for (int i = 0; i < SELECT_USE_POLY; i++)
	{
		switch (i)
		{
		case TEXTYPE_BASE:		
			PlayerSelect[i].pos = D3DXVECTOR3(SELECT_BASE_POS_X, -(SCREEN_HEIGHT * 0.5f), 0);
			PlayerSelect[i].fWidth  = 460;
			PlayerSelect[i].fHeight = SCREEN_HEIGHT;
			PlayerSelect[i].Type = i;
			break;
		case TEXTYPE_GEAR_L:
			PlayerSelect[i].pos = D3DXVECTOR3(SELECT_BASE_POS_X - 250.0f, 0.0f, 0);
			PlayerSelect[i].fWidth  = 120;
			PlayerSelect[i].fHeight = 120;
			PlayerSelect[i].Type = i;
			break;
		case TEXTYPE_GEAR_R:
			PlayerSelect[i].pos = D3DXVECTOR3(SELECT_BASE_POS_X + 250.0f, 0.0f, 0);
			PlayerSelect[i].fWidth  = 120;
			PlayerSelect[i].fHeight = 120;
			PlayerSelect[i].Type = i;
			break;
		case TEXTYPE_PLAYER1_ON:
			PlayerSelect[i].pos = D3DXVECTOR3(SELECT_BASE_POS_X, -300, 0);
			PlayerSelect[i].fWidth  = 180;
			PlayerSelect[i].fHeight = 80;
			PlayerSelect[i].Type = TEXTYPE_PLAYER1_ON;
			break;
		case TEXTYPE_PLAYER2_ON:
			PlayerSelect[i].pos = D3DXVECTOR3(SELECT_BASE_POS_X, -300, 0);
			PlayerSelect[i].fWidth  = 180;
			PlayerSelect[i].fHeight = 80;
			PlayerSelect[i].Type = TEXTYPE_PLAYER2_OFF;
			break;
		case TEXTYPE_PLAYER3_ON:
			PlayerSelect[i].pos = D3DXVECTOR3(SELECT_BASE_POS_X, -300, 0);
			PlayerSelect[i].fWidth  = 180;
			PlayerSelect[i].fHeight = 80;
			PlayerSelect[i].Type = TEXTYPE_PLAYER3_OFF;
			break;
		case TEXTYPE_PLAYER4_ON:
			PlayerSelect[i].pos = D3DXVECTOR3(SELECT_BASE_POS_X, -300, 0);
			PlayerSelect[i].fWidth  = 180;
			PlayerSelect[i].fHeight = 80;
			PlayerSelect[i].Type = TEXTYPE_PLAYER4_OFF;
			break;
		}
		PlayerSelect[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ꉞ������
		PlayerSelect[i].fAngle = atan2f(PlayerSelect[i].fWidth, PlayerSelect[i].fHeight);
		PlayerSelect[i].fLength = sqrtf(PlayerSelect[i].fWidth * PlayerSelect[i].fWidth + PlayerSelect[i].fHeight * PlayerSelect[i].fHeight) / 2;
	}
	VERTEX_2D* pVtx;	//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayerCount->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SELECT_USE_POLY; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
		pVtx[0].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
		pVtx[1].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
		pVtx[2].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
		pVtx[3].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
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
	g_pVtxBuffPlayerCount->Unlock();
	nSelectPlayerCnt = 0;
	nZoomSelectPCnt = 0;
	bSelectPlayerAnim = false;
	AnimFinish = false;
	bSelPCnt = false;
}
//===============================================================================
//�l���I����ʂ̏I������
//===============================================================================
void UninitPlayerCount(void)
{
	for (int nCnt = 0; nCnt < SELECT_TEXCOUNT; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturePlayerCount[nCnt] != NULL)
		{
			g_pTexturePlayerCount[nCnt]->Release();
			g_pTexturePlayerCount[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayerCount != NULL)
	{
		g_pVtxBuffPlayerCount->Release();
		g_pVtxBuffPlayerCount = NULL;
	}
}

//===============================================================================
//�l���I����ʂ̍X�V����
//===============================================================================
void UpdatePlayerCount(void)
{
	enum
	{
		SELECTTYPE_START = 0,
		SELECTTYPE_TUTORIAL,
		SELECTTYPE_RANKING,
		SELECTTYPE_FINISH
	};
	bSelPCnt = ReturnNowSelect();
	if (bSelPCnt == true)
	{
		//����������؂�ւ���
		if (GetJoypadTrigger(JOYKEY_UP,0) == true)//�㉟�����Ƃ�
		{
			if (nSelectPlayerCnt == SELECTTYPE_START)
			{//������ԏ�Ȃ�
				nSelectPlayerCnt = SELECTTYPE_FINISH;//���s���܂�
			}
			else
			{
				nSelectPlayerCnt--;
			}
			nZoomSelectPCnt = 0;

		}
		if (GetJoypadTrigger(JOYKEY_DOWN,0) == true)//���������Ƃ�
		{
			if (nSelectPlayerCnt == SELECTTYPE_FINISH)
			{//������ԉ��Ȃ�
				nSelectPlayerCnt = SELECTTYPE_START;//��s���܂�
			}
			else
			{
				nSelectPlayerCnt++;
			}
			nZoomSelectPCnt = 0;

		}
		//�ǂꂪ�I������Ă邩
		switch (nSelectPlayerCnt)
		{
		case SELECTTYPE_START:
			PlayerSelect[TEXTYPE_PLAYER1_ON].Type = TEXTYPE_PLAYER1_ON;
			PlayerSelect[TEXTYPE_PLAYER2_ON].Type = TEXTYPE_PLAYER2_OFF;
			PlayerSelect[TEXTYPE_PLAYER3_ON].Type = TEXTYPE_PLAYER3_OFF;
			PlayerSelect[TEXTYPE_PLAYER4_ON].Type = TEXTYPE_PLAYER4_OFF;
			break;
		case SELECTTYPE_TUTORIAL:
			PlayerSelect[TEXTYPE_PLAYER1_ON].Type = TEXTYPE_PLAYER1_OFF;
			PlayerSelect[TEXTYPE_PLAYER2_ON].Type = TEXTYPE_PLAYER2_ON;
			PlayerSelect[TEXTYPE_PLAYER3_ON].Type = TEXTYPE_PLAYER3_OFF;
			PlayerSelect[TEXTYPE_PLAYER4_ON].Type = TEXTYPE_PLAYER4_OFF;
			break;
		case SELECTTYPE_RANKING:
			PlayerSelect[TEXTYPE_PLAYER1_ON].Type = TEXTYPE_PLAYER1_OFF;
			PlayerSelect[TEXTYPE_PLAYER2_ON].Type = TEXTYPE_PLAYER2_OFF;
			PlayerSelect[TEXTYPE_PLAYER3_ON].Type = TEXTYPE_PLAYER3_ON;
			PlayerSelect[TEXTYPE_PLAYER4_ON].Type = TEXTYPE_PLAYER4_OFF;
			break;
		case SELECTTYPE_FINISH:
			PlayerSelect[TEXTYPE_PLAYER1_ON].Type = TEXTYPE_PLAYER1_OFF;
			PlayerSelect[TEXTYPE_PLAYER2_ON].Type = TEXTYPE_PLAYER2_OFF;
			PlayerSelect[TEXTYPE_PLAYER3_ON].Type = TEXTYPE_PLAYER3_OFF;
			PlayerSelect[TEXTYPE_PLAYER4_ON].Type = TEXTYPE_PLAYER4_ON;
			break;
		}

		//�Y�[���C���E�A�E�g�̏���
		if (nZoomSelectPCnt < 12)
		{
			nZoomSelectPCnt += 1.2f;
		}

		//�����������̃A�j���[�V����
		if (bSelectPlayerAnim == false)
		{//�I�����~��Ă���
			if (PlayerSelect[0].pos.y < SCREEN_HEIGHT * 0.5f)
			{
				PlayerSelect[0].pos.y += 48.0f;
				PlayerSelect[1].rot.z -= 0.3f;
				PlayerSelect[2].rot.z += 0.3f;
			}
			if (PlayerSelect[0].pos.y >= SCREEN_HEIGHT * 0.5f)
			{
				AnimFinish = true;
			}
		}//�I�����߂�
		else
		{
			if (PlayerSelect[0].pos.y > -SCREEN_HEIGHT * 0.5f)
			{
				PlayerSelect[0].pos.y -= 48.0f;
				PlayerSelect[1].rot.z += 0.3f;
				PlayerSelect[2].rot.z -= 0.3f;
			}
			else if (PlayerSelect[0].pos.y <= -SCREEN_HEIGHT * 0.5f)
			{
				SwitchSelectMode();
			}
		}
		//��ʏ�ł̐l���\���̑��Βl�ݒ�
		PlayerSelect[3].pos.y = PlayerSelect[0].pos.y - 165.0f;
		PlayerSelect[4].pos.y = PlayerSelect[0].pos.y - 40.0f;
		PlayerSelect[5].pos.y = PlayerSelect[0].pos.y + 80.0f;
		PlayerSelect[6].pos.y = PlayerSelect[0].pos.y + 210.0f;

		VERTEX_2D* pVtx;	//���_���̃|�C���^

		//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
		g_pVtxBuffPlayerCount->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < SELECT_USE_POLY; nCnt++)
		{
			if (nCnt > 2)
			{
				if (PlayerSelect[nCnt].Type == nCnt)
				{
					//���_���W�̐ݒ�
					pVtx[0].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength - nZoomSelectPCnt;
					pVtx[0].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength - nZoomSelectPCnt;
					pVtx[0].pos.z = 0.0f;

					pVtx[1].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength + nZoomSelectPCnt;
					pVtx[1].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength - nZoomSelectPCnt;
					pVtx[1].pos.z = 0.0f;

					pVtx[2].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength - nZoomSelectPCnt;
					pVtx[2].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength + nZoomSelectPCnt;
					pVtx[2].pos.z = 0.0f;

					pVtx[3].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength + nZoomSelectPCnt;
					pVtx[3].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength + nZoomSelectPCnt;
					pVtx[3].pos.z = 0.0f;
				}
				else
				{
					//���_���W�̐ݒ�
					pVtx[0].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
					pVtx[0].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
					pVtx[0].pos.z = 0.0f;

					pVtx[1].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
					pVtx[1].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
					pVtx[1].pos.z = 0.0f;

					pVtx[2].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
					pVtx[2].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
					pVtx[2].pos.z = 0.0f;

					pVtx[3].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
					pVtx[3].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
					pVtx[3].pos.z = 0.0f;
				}
			}
			else
			{
			
				//���_���W�̐ݒ�
				pVtx[0].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
				pVtx[0].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
				pVtx[1].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + (D3DX_PI - PlayerSelect[nCnt].fAngle)) * PlayerSelect[nCnt].fLength;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
				pVtx[2].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z - PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = PlayerSelect[nCnt].pos.x + sinf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
				pVtx[3].pos.y = PlayerSelect[nCnt].pos.y + cosf(PlayerSelect[nCnt].rot.z + PlayerSelect[nCnt].fAngle) * PlayerSelect[nCnt].fLength;
				pVtx[3].pos.z = 0.0f;
				
			}

			pVtx += 4;
		}

	}
	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffPlayerCount->Unlock();
	if (GetJoypadTrigger(JOYKEY_A,0) == true && AnimFinish == true)//����I
	{
		int Mode = ReturnSelectedMode();
		SetMode((MODE)Mode);
	}
	if (GetJoypadTrigger(JOYKEY_B,0) == true && AnimFinish == true)//�R�I
	{
		bSelectPlayerAnim = true;
		AnimFinish = false;
	}

}

//===============================================================================
//�l���I����ʌn�̕`��
//===============================================================================
void DrawPlayerCount(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayerCount, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	for (int nCnt = 0; nCnt < SELECT_USE_POLY; nCnt++)
	{
		pDevice->SetTexture(0, g_pTexturePlayerCount[PlayerSelect[nCnt].Type]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			4 * nCnt,
			2);
	}
}

//===============================================================================
//�I������Ă���l����Ԃ��֐�
//===============================================================================
int ReturnSelectPlayerCount(void)
{
	return nSelectPlayerCnt;		//����� int i = ReturnSelectPlayerCount()���邾���Ől���擾�ł��܂�(0�`3)
}

//===============================================================================
//�A�j���[�V�������I��������Ƃ�Ԃ��֐�
//===============================================================================
bool ReturnAnimFinish(void)
{
	return AnimFinish;
}

//===============================================================================
//�ʂ�cpp����A�j���[�V�������쓮������֐�
//===============================================================================
void AnimSwitch(void)
{
	bSelectPlayerAnim = false;
}

