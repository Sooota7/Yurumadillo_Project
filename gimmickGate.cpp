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

bool test2 = false;
float test = 0.0f;

void GIMMICK_GATE::GimmickGate_Update(bool isOn)
{
    if (test2) {
        test += (1.0f / 60.0f);
        m_Open = test;
    }
    else {
		test -= (1.0f / 60.0f);
        m_Open = test;
    }

    if (test > 1.0f || test < 0.0f) {
        test2 = !test2;
    }
	
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