// gimmickGate.cpp
#include "gimmickGate.h"
#include <cmath>

void GIMMICK_GATE::GimmickGate_Initialize(XMFLOAT3 pos)
{
    m_FirstPosition = pos;
    m_Position = pos;
    m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Count = 0.0f;

    m_Channel = 0;
    m_State = GATE_CLOSED;
    m_Open = 0.0f;
    m_Side = GATE_SIDE_RIGHT;
}

void GIMMICK_GATE::GimmickGate_Finalize(void)
{

}

void GIMMICK_GATE::GimmickGate_Update(bool isOn)
{
	
    float speed = 2.0f * (1.0f / 60.0f);


    if (!isOn)
	{// ŠJ‚­•ûŒü
        if (m_Open < 1.0f)
        {
            m_Open += speed;
            if (m_Open > 1.0f) { m_Open = 1.0f; }
            m_State = GATE_OPENING;
            if (m_Open >= 1.0f) { m_State = GATE_OPEN; }
        }
        else
        {
            m_Open = 1.0f;
            m_State = GATE_OPEN;
        }
    }
    else
    {
        if (m_Open > 0.0f)
        {
            m_Open -= speed;
            if (m_Open < 0.0f) { m_Open = 0.0f; }
            m_State = GATE_CLOSING;
            if (m_Open <= 0.0f) { m_State = GATE_CLOSED; }
        }
        else
        {
            m_Open = 0.0f;
            m_State = GATE_CLOSED;
        }
    }

}