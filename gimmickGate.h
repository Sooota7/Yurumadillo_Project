
#pragma once
#include <DirectXMath.h>
#include "field.h"
#include "gimmickButton.h"
using namespace DirectX;

enum GATE_STATE
{
    GATE_NONE,
    GATE_CLOSED,
    GATE_OPENING,
    GATE_OPEN,
    GATE_CLOSING,
	GATE_MAX
};

class GIMMICK_GATE
{
public:
    XMFLOAT3 m_FirstPosition;
    XMFLOAT3 m_Position;
    XMFLOAT3 m_Rotation;
    XMFLOAT3 m_Scaling;
    XMFLOAT3 m_Velocity;
    float    m_Count;

private:
    int       m_Channel; // 連動チャンネル（下1桁）
    GATE_STATE m_State;
    float      m_Open; // 0.0=閉, 1.0=全開

public:
    void  GimmickGate_Initialize(XMFLOAT3 pos);
    void  GimmickGate_Finalize(void);

    void  GimmickGate_Update(bool isOn);

    void  GimmickGate_SetPosition(XMFLOAT3 pos) { m_Position = pos; }
    XMFLOAT3 GimmickGate_GetPosition() { return m_Position; }
    XMFLOAT3 GimmickGate_GetFirstPosition() { return m_FirstPosition; }

    void       GimmickGate_SetChannel(int ch) { m_Channel = ch; }
    int        GimmickGate_GetChannel() const { return m_Channel; }
    void       GimmickGate_SetState(GATE_STATE s) { m_State = s; }
    GATE_STATE GimmickGate_GetState() const { return m_State; }
    float      GimmickGate_GetOpen() const { return m_Open; }

    GIMMICK_GATE* GetGimmickGate() { return this; }
};
