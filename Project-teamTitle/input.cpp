//===============================================================================
//
//  3D�Q�[����b(input.cpp)
//								����F���n�O��
// 
//===============================================================================
#include "input.h"
//�}�N����`
#define NUM_KEY_MAX (256)

//�O���[�o���ϐ�
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];
XINPUT_STATE g_joyKeyState;
XINPUT_STATE g_joyKeyStateTrigger;

//�L�[�{�[�h�̏������ݒ�
HRESULT InitKeyboard(HINSTANCE hInstace, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstace, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//�L�[�{�[�h�̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();
	return S_OK;
}
//�L�[�{�[�h�̏I������
void UninitKeyboard(void)
{
	//���̓f�o�C�X�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}
//�L�[�{�[�h�̍X�V�ݒ�
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;
	//���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey] ) & aKeyState[nCntKey];
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}
//�L�[�{�[�h�̃v���X�����擾
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) != 0;
}
//�L�[�{�[�h�̃v���X�����擾
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) != 0;
}

HRESULT InitJoypad(void)
{
	//�������̃N���A
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	//XInputEnable�̃X�e�[�g��ݒ�(�L���ɂ���)
	XInputEnable(true);
	//// Zero value if thumbsticks are within the dead zone
	//if ((state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	//	state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
	//	(state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
	//		state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	//{
	//	state.Gamepad.sThumbLX = 0;
	//	state.Gamepad.sThumbLY = 0;
	//}

	return S_OK;
}

void UninitJoypad(void)
{
	//XInputEnable�̃X�e�[�g��ݒ�(�����ɂ���)
	XInputEnable(false);
}

void UpdateJoypad(void)
{
	XINPUT_STATE JoykeyState;
	//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &JoykeyState) == ERROR_SUCCESS)
	{
		WORD Button = JoykeyState.Gamepad.wButtons;
		BYTE LeftTrigger = JoykeyState.Gamepad.bLeftTrigger;    // ���g���K�[�A�i���O�R���g���[��
		BYTE RightTrigger = JoykeyState.Gamepad.bRightTrigger;   // �E�g���K�[�A�i���O�R���g���[��
		SHORT ThumbLX = JoykeyState.Gamepad.sThumbLX;       // ���X�e�B�b�N�� X ��
		SHORT ThumbLY = JoykeyState.Gamepad.sThumbLY;       // ���X�e�B�b�N�� Y ��
		SHORT ThumbRX = JoykeyState.Gamepad.sThumbRX;       // �E�X�e�B�b�N�� X ��
		SHORT ThumbRY = JoykeyState.Gamepad.sThumbRY;       // �E�X�e�B�b�N�� Y ��
		WORD OldButton = g_joyKeyState.Gamepad.wButtons;
		g_joyKeyStateTrigger.Gamepad.wButtons = ~OldButton & Button;

		g_joyKeyState = JoykeyState;
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		// Zero value if thumbsticks are within the dead zone
		if ((state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			state.Gamepad.sThumbLX = 0;
			state.Gamepad.sThumbLY = 0;
		}
		// Simply get the state of the controller from XInput.
		DWORD dwResult = XInputGetState(0, &state);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
		}
		else
		{
			// Controller is not connected
		}
	}
}
WORD ThumbToDPad(SHORT sThumbX, SHORT sThumbY, SHORT sDeadZone)
{
	WORD wButtons = 0;

	if (sThumbY >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_UP;
	}
	else if (sThumbY <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
	}

	if (sThumbX <= -sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if (sThumbX >= sDeadZone)
	{
		wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
	}

	return wButtons;
}
bool GetJoypadPress(JOYKEY key)
{
	g_joyKeyState.Gamepad.wButtons |= ThumbToDPad(g_joyKeyState.Gamepad.sThumbLX, g_joyKeyState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	if (key == JOYPAD_LEFT_SHOULDER)
	{
		return (g_joyKeyState.Gamepad.bLeftTrigger &  key) ? true : false;
	}
	else if(key == JOYPAD_RIGHT_SHOULDER)
	{
		return (g_joyKeyState.Gamepad.bRightTrigger &  key) ? true : false;
	}
	else
	{
		return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
	}
}

bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}