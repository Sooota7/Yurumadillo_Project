#pragma once


#include	"model.h"
#include	"bombSource.h"
#include	"gimmickButton.h"
#include	"gimmickField.h"
#include	"gimmickGate.h"
#include	"player.h"

// マクロ定義
#define GIM_BUTTON_MIN 10  // 10-19
#define GIM_BUTTON_MAX 19
#define GIM_FIELD_MIN  20  // 20-29
#define GIM_FIELD_MAX  29
#define GIM_GATE_MIN   30  // 30-39
#define GIM_GATE_MAX   39


// --- channel は下1桁（0..9） ---
#define MAX_CHANNEL 10

// --- 必要ボタン数（固定） ---
#define NEED_BUTTONS_FIELD 1
#define NEED_BUTTONS_GATE  2

#define		GIMMICK_NUM_MAX	(10)
#define		GIMMICK_TYPE_MAX	(2)


enum GIMMICK_STATE
{
	GIMMICK_STATE_NONE,
	GIMMICK_STATE_BUTTON,
	GIMMICK_STATE_FIELD,
	GIMMICK_STATE_GATE,
	GIMMICK_STATE_MAX
};


class GIMMICK_DATA
{
    public:



	GIMMICK_BUTTON		m_GimmickButton[GIMMICK_NUM_MAX];
	GIMMICK_FIELD		m_GimmickField[GIMMICK_NUM_MAX];
	GIMMICK_GATE		m_GimmickGate[GIMMICK_NUM_MAX];

	MODEL* m_Model[GIMMICK_STATE_MAX] = { NULL };//デバッグ


	int  m_ButtonCount;     // ボタンの数
	int  m_FieldCount;      // 動くフィールドの数
	int  m_GateCount;       // ゲートの数

	bool m_ChannelOn[MAX_CHANNEL];       // channelごとのON状態
	int  m_ChannelOnCount[MAX_CHANNEL];  // channelごとのONカウント


public:
    void Gimmick_Data_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no);
    void Gimmick_Data_Finalize(void);
    void Gimmick_Data_Draw(void);
    void Gimmick_Data_Update(DirectX::XMFLOAT3 pPlayerPos, DirectX::XMFLOAT3 pPlayerRot);

    // channel集計
    void Channels_Reset();
    void Channels_AddCount(int ch);
    int  Channels_GetCount(int ch) const;
    bool Channels_IsOn(int ch) const; // 互換用（必要なら残す）

    // 配列アクセス
    GIMMICK_BUTTON* GetButtons() { return m_GimmickButton; }
    int             GetButtonCount() const { return m_ButtonCount; }
    GIMMICK_FIELD* GetFields() { return m_GimmickField; }
    int             GetFieldCount() const { return m_FieldCount; }
    GIMMICK_GATE* GetGates() { return m_GimmickGate; }
    int             GetGateCount() const { return m_GateCount; }

	GIMMICK_DATA* GetGimmickData() { return this; };
};