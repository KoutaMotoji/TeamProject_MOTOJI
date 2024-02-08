//===============================================================================
//
//  3D�`�[������(title_select.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "title_anim.h"
#include "input.h"

#define SELECT_USE_POLY	(11)
#define TITLE_ANIM_POS_X	(SCREEN_WIDTH * 0.5)
#define TITLE_ANIM_GEAR_HALFSIZE	(150)

//�e�N�X�`�����蓖��(���͏�ɁV)
typedef enum
{
	TEXTYPE_TWALLL = 0,
	TEXTYPE_TWALLR,
	TEXTYPE_GEAR_L,
	TEXTYPE_GEAR_R,
	TEXTYPE_GEAR_CHAIN,
	TEXTYPE_WALLGEAR,
	TEXTYPE_TITLE,
	TEXTYPE_MAX
}TEXTYPE_ANIM;
//�e�N�X�`����
static const char* TAnimCnt[TEXTYPE_MAX]
{
	"data\\TEXTURE\\Title\\TwallL.png",			//����
	"data\\TEXTURE\\Title\\TwallR.png",			//�E��
	"data\\TEXTURE\\SelectCnt\\gear_l.png",		//���M�A
	"data\\TEXTURE\\SelectCnt\\gear_r.png",		//�E�M�A
	"data\\TEXTURE\\Title\\gear_chain.png",		//��
	"data\\TEXTURE\\Title\\gear.png",			//�����M�A
	"data\\TEXTURE\\Title\\TitleText.png",		//�^�C�g����
};

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
}TITEL_ANIM;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTitleAnim[TEXTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleAnim = NULL;
float TitleAnimGear[2];
bool bUseTitleAnim;		//�A�j���[�V��������p
bool TitleAnimFinish;
TITEL_ANIM TitleAnim[SELECT_USE_POLY];			//�v���C���[�I���\����

//===============================================================================
//�l���I����ʂ̏���������
//===============================================================================
void InitTitleAnim(void)
{

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	for (int i = 0; i < TEXTYPE_MAX; i++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TAnimCnt[i],
			&g_pTextureTitleAnim[i]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * SELECT_USE_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleAnim,
		NULL);

	for (int i = 0; i < SELECT_USE_POLY; i++)
	{
		TitleAnim[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�ꉞ������
		switch (i)
		{
		case 0:
			TitleAnim[i].pos = D3DXVECTOR3(TITLE_ANIM_POS_X, SCREEN_HEIGHT/2, 0);
			TitleAnim[i].fWidth = SCREEN_WIDTH;
			TitleAnim[i].fHeight = SCREEN_HEIGHT;
			TitleAnim[i].Type = TEXTYPE_TWALLL;
			break;
		case 1:
			TitleAnim[i].pos = D3DXVECTOR3(TITLE_ANIM_POS_X, SCREEN_HEIGHT/2, 0);
			TitleAnim[i].fWidth = SCREEN_WIDTH;
			TitleAnim[i].fHeight = SCREEN_HEIGHT;
			TitleAnim[i].Type = TEXTYPE_TWALLR;
			break;
		case 2:
			TitleAnim[i].pos = D3DXVECTOR3(0, 0.0f, 0);
			TitleAnim[i].fWidth = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].fHeight = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].Type = TEXTYPE_GEAR_L;
			break;
		case 3:
			TitleAnim[i].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0);
			TitleAnim[i].fWidth = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].fHeight = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].Type = TEXTYPE_GEAR_R;
			break;
		case 4:
			TitleAnim[i].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0);
			TitleAnim[i].fWidth = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].fHeight = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].Type = TEXTYPE_GEAR_R;
			break;
		case 5:
			TitleAnim[i].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
			TitleAnim[i].fWidth = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].fHeight = TITLE_ANIM_GEAR_HALFSIZE;
			TitleAnim[i].Type = TEXTYPE_GEAR_L;
			break;
		case 6:
			TitleAnim[i].pos = D3DXVECTOR3(TITLE_ANIM_GEAR_HALFSIZE/3, SCREEN_HEIGHT / 2, 0);
			TitleAnim[i].fWidth = 20;
			TitleAnim[i].fHeight = SCREEN_HEIGHT;
			TitleAnim[i].Type = TEXTYPE_GEAR_CHAIN;
			break;
		case 7:
			TitleAnim[i].pos = D3DXVECTOR3(SCREEN_WIDTH - (TITLE_ANIM_GEAR_HALFSIZE / 3), SCREEN_HEIGHT / 2, 0);
			TitleAnim[i].fWidth = 20;
			TitleAnim[i].fHeight = SCREEN_HEIGHT;
			TitleAnim[i].Type = TEXTYPE_GEAR_CHAIN;
			break;
		case 8:
			TitleAnim[i].pos = D3DXVECTOR3(TITLE_ANIM_POS_X -30, SCREEN_HEIGHT/2 - 20, 0);
			TitleAnim[i].fWidth = 350;
			TitleAnim[i].fHeight = 350;
			TitleAnim[i].Type = TEXTYPE_WALLGEAR;
			break;
		case 9:
			TitleAnim[i].pos = D3DXVECTOR3(300, -200, 0);
			TitleAnim[i].fWidth = 500;
			TitleAnim[i].fHeight = 250;
			TitleAnim[i].Type = TEXTYPE_TITLE;
			break;	
		case 10:
			TitleAnim[i].pos = D3DXVECTOR3(TITLE_ANIM_POS_X+20, SCREEN_HEIGHT / 2 + 30, 0);
			TitleAnim[i].fWidth = 320;
			TitleAnim[i].fHeight = 320;
			TitleAnim[i].Type = TEXTYPE_WALLGEAR;

		}
		TitleAnim[i].fAngle = atan2f(TitleAnim[i].fWidth, TitleAnim[i].fHeight);
		TitleAnim[i].fLength = sqrtf(TitleAnim[i].fWidth * TitleAnim[i].fWidth + TitleAnim[i].fHeight * TitleAnim[i].fHeight) / 2;
	}
	VERTEX_2D* pVtx;	//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleAnim->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SELECT_USE_POLY; nCnt++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z - (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[0].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z - (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z + (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[1].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z + (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z - TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
		pVtx[2].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z - TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z + TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
		pVtx[3].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z + TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
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
	g_pVtxBuffTitleAnim->Unlock();
	TitleAnimGear[0] = 0.0f;
	TitleAnimGear[1] = 0.0f;
	bUseTitleAnim = false;
	TitleAnimFinish = false;
}
//===============================================================================
//�l���I����ʂ̏I������
//===============================================================================
void UninitTitleAnim(void)
{
	for (int nCnt = 0; nCnt < TEXTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTitleAnim[nCnt] != NULL)
		{
			g_pTextureTitleAnim[nCnt]->Release();
			g_pTextureTitleAnim[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitleAnim != NULL)
	{
		g_pVtxBuffTitleAnim->Release();
		g_pVtxBuffTitleAnim = NULL;
	}
}

//===============================================================================
//�l���I����ʂ̍X�V����
//===============================================================================
void UpdateTitleAnim(void)
{
	
	
	if (TitleAnim[8].rot.z < 3.14f*1.5f)
	{
		TitleAnim[8].rot.z  += (3.14f * 0.018f);
		TitleAnim[10].rot.z -= (3.14f * 0.018f);

	}
	else
	{
		if (TitleAnim[0].pos.x > -(SCREEN_WIDTH / 2) + 256)
		{
			TitleAnim[0].pos.x -= 12.8f;
			TitleAnim[2].rot.z -= 0.06f;
			TitleAnim[4].rot.z -= 0.06f;
			TitleAnimGear[0] += 0.025f;
		}
		if (TitleAnim[1].pos.x < (SCREEN_WIDTH / 2) + (SCREEN_WIDTH - 256))
		{
			TitleAnim[1].pos.x += 12.8f;
			TitleAnim[3].rot.z += 0.06f;
			TitleAnim[5].rot.z += 0.06f;
			TitleAnimGear[1] -= 0.025f;
		}
		if (TitleAnim[0].pos.x < -(SCREEN_WIDTH / 2) + 256 && TitleAnim[1].pos.x >(SCREEN_WIDTH / 2) - 256)
		{
			if (TitleAnim[9].pos.y < 75)
			{
				TitleAnim[9].pos.y += 7.5f;
			}
			else
			{
				TitleAnimFinish = true;
			}
		}
	}
	//�����ǂƃM�A�̑��΍��W
	TitleAnim[8].pos = TitleAnim[0].pos + D3DXVECTOR3(-120, -80, 0);
	TitleAnim[10].pos = TitleAnim[1].pos + D3DXVECTOR3(80, +120, 0);

	VERTEX_2D* pVtx;	//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���āA���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleAnim->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < SELECT_USE_POLY; nCnt++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z - (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[0].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z - (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z + (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[1].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z + (D3DX_PI - TitleAnim[nCnt].fAngle)) * TitleAnim[nCnt].fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z - TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
		pVtx[2].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z - TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = TitleAnim[nCnt].pos.x + sinf(TitleAnim[nCnt].rot.z + TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
		pVtx[3].pos.y = TitleAnim[nCnt].pos.y + cosf(TitleAnim[nCnt].rot.z + TitleAnim[nCnt].fAngle) * TitleAnim[nCnt].fLength;
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

		if (nCnt == 6 || nCnt == 7)
		{
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + TitleAnimGear[nCnt - 6]);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + TitleAnimGear[nCnt - 6]);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f + TitleAnimGear[nCnt - 6]);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f + TitleAnimGear[nCnt - 6]);
		}
		else
		{
			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		pVtx += 4;
	}
}
//===============================================================================
//�l���I����ʌn�̕`��
//===============================================================================
void DrawTitleAnim(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleAnim, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	for (int nCnt = 0; nCnt < SELECT_USE_POLY; nCnt++)
	{
		pDevice->SetTexture(0, g_pTextureTitleAnim[TitleAnim[nCnt].Type]);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			4 * nCnt,
			2);
	}
}

//===============================================================================
//�A�j���[�V�������I��������Ƃ�Ԃ��֐�
//===============================================================================
bool ReturnTitleAnimFinish(void)
{
	return TitleAnimFinish;
}


