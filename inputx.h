//=============================================================================
//
// 入力処理 [input.h]
// Author : 
//
//=============================================================================
#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;
#include <XInput.h>
#pragma comment (lib, "xinput.lib")
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//ボタン一覧

// XINPUT_GAMEPAD_DPAD_UP			十字キー上
// XINPUT_GAMEPAD_DPAD_DOWN			十字キー下
// XINPUT_GAMEPAD_DPAD_LEFT			十字キー左
// XINPUT_GAMEPAD_DPAD_RIGHT		十字キー右
// XINPUT_GAMEPAD_START
// XINPUT_GAMEPAD_BACK
// XINPUT_GAMEPAD_LEFT_THUMB		Lスティック押し込み
// XINPUT_GAMEPAD_RIGHT_THUMB		Rスティック押し込み
// XINPUT_GAMEPAD_LEFT_SHOULDER		Lボタン
// XINPUT_GAMEPAD_RIGHT_SHOULDER	Rボタン
// XINPUT_GAMEPAD_A
// XINPUT_GAMEPAD_B
// XINPUT_GAMEPAD_X
// XINPUT_GAMEPAD_Y

//スティックの範囲
// X軸 左-1.0　右 1.0
// Y軸 上 1.0　下-1.0

//トリガーの範囲
// 0 ～ 255

// バイブレーションの範囲
// 0 ～ 255

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

float GetThumbLeftX(int padNo);	//Lスティック
float GetThumbLeftY(int padNo);
float GetThumbRightX(int padNo);//Rスティック
float GetThumbRightY(int padNo);

int GetLeftTrigger(int padNo);
int GetRightTrigger(int padNo);

void SetLeftVibration(int padNo, int speed);
void SetRightVibration(int padNo, int speed);
void SetVibration(int padNo, int speed);
void StopVibration(int padNo);

//デジタルボタン
BOOL IsButtonPressed(int padNo,DWORD button);
BOOL IsButtonTriggered(int padNo,DWORD button);

