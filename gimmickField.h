// gimmickField.h

#pragma once
#include <DirectXMath.h>
#include "field.h"
#include "gimmickButton.h"
using namespace DirectX;

#define FIELD_MOVE_SPEED (3.0f)

class GIMMICK_FIELD
{
public:
    XMFLOAT3 m_FirstPosition; // OFF時の帰還先
    XMFLOAT3 m_Position;      // 現在位置
    XMFLOAT3 m_Scaling;
    XMFLOAT3 m_Velocity;
    float    m_Count;


    int      m_Channel;           // 連動チャンネル（下1桁）
    XMFLOAT3 m_TargetPosition;    // ON時の目標
    float    m_MoveSpeed;         // unit/sec
	bool	 m_IsOn;              // ON/OFF状態

public:
    void     GimmickField_Initialize(XMFLOAT3 pos);
    void     GimmickField_Finalize(void);

    void     GimmickField_Update(bool isOn);

    void     GimmickField_SetPosition(XMFLOAT3 pos) { m_Position = pos; }
    XMFLOAT3 GimmickField_GetPosition() { return m_Position; }
    XMFLOAT3 GimmickField_GetFirstPosition() { return m_FirstPosition; }

    void     GimmickField_SetChannel(int ch) { m_Channel = ch; }
    int      GimmickField_GetChannel() const { return m_Channel; }

    void     GimmickField_SetTargetPosition(XMFLOAT3 pos) { m_TargetPosition = pos; }
    XMFLOAT3 GimmickField_GetTargetPosition() const { return m_TargetPosition; }

    void     GimmickField_SetMoveSpeed(float s) { m_MoveSpeed = s; }

    GIMMICK_FIELD* GetGimmickField() { return this; }
};
