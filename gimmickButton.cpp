// gimmickButton.cpp
#include "gimmickButton.h"
#include <cmath>

void GIMMICK_BUTTON::GimmickButton_Initialize(XMFLOAT3 pos)
{
    m_FirstPosition = pos;
    m_Position = pos;
    m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Touch = false;
    m_Channel = 0;

}

void GIMMICK_BUTTON::GimmickButton_Finalize(void)
{

}

void GIMMICK_BUTTON::GimmickButton_Update(void)
{

    XMFLOAT3 preview = m_Position; // 直前位置を保持

    XMFLOAT3 target = m_FirstPosition;
	// 押されている場合
    if (m_Touch)
    {
        target.y = m_FirstPosition.y - BUTTON_PRESS_DEPTH;
    }

	// 移動量の計算

    float step = BUTTON_MOVE_SPEED * (1.0f / 60.0f);

    XMFLOAT3 diff(
        target.x - m_Position.x,
        target.y - m_Position.y,
        target.z - m_Position.z
    );


    float lenSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float stepSq = step * step;

    if (lenSq <= stepSq)
    {
        m_Position = target;
    }
    else
    {
        float len = std::sqrt(lenSq);
        if (len > 0.0f)
        {
            diff.x /= len; diff.y /= len; diff.z /= len;
            m_Position.x += diff.x * step;
            m_Position.y += diff.y * step;
            m_Position.z += diff.z * step;
        }
    }

    // 今フレームの移動差分（速度）を保存
    m_Velocity.x = m_Position.x - preview.x;
    m_Velocity.y = m_Position.y - preview.y;
    m_Velocity.z = m_Position.z - preview.z;

}
