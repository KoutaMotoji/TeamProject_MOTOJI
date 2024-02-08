//===============================================================================
//
//  3Dゲーム基礎(input.cpp)
//								制作：元地弘汰
// 
//===============================================================================
#include "input.h"
//マクロ定義
#define NUM_KEY_MAX (256)

//グローバル変数
LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];
XINPUT_STATE g_joyKeyState;
XINPUT_STATE g_joyKeyStateTrigger;

//キーボードの初期化設定
HRESULT InitKeyboard(HINSTANCE hInstace, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstace, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//強調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	//キーボードのアクセス権を獲得
	g_pDevKeyboard->Acquire();
	return S_OK;
}
//キーボードの終了処理
void UninitKeyboard(void)
{
	//入力デバイスの破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}
//キーボードの更新設定
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;
	//入力デバイスからデータを取得
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
//キーボードのプレス情報を取得
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) != 0;
}
//キーボードのプレス情報を取得
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) != 0;
}

HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));

	//XInputEnableのステートを設定(有効にする)
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
	//XInputEnableのステートを設定(無効にする)
	XInputEnable(false);
}

void UpdateJoypad(void)
{
	XINPUT_STATE JoykeyState;
	//ジョイパッドの状態を取得
	if (XInputGetState(0, &JoykeyState) == ERROR_SUCCESS)
	{
		WORD Button = JoykeyState.Gamepad.wButtons;
		BYTE LeftTrigger = JoykeyState.Gamepad.bLeftTrigger;    // 左トリガーアナログコントロール
		BYTE RightTrigger = JoykeyState.Gamepad.bRightTrigger;   // 右トリガーアナログコントロール
		SHORT ThumbLX = JoykeyState.Gamepad.sThumbLX;       // 左スティックの X 軸
		SHORT ThumbLY = JoykeyState.Gamepad.sThumbLY;       // 左スティックの Y 軸
		SHORT ThumbRX = JoykeyState.Gamepad.sThumbRX;       // 右スティックの X 軸
		SHORT ThumbRY = JoykeyState.Gamepad.sThumbRY;       // 右スティックの Y 軸
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