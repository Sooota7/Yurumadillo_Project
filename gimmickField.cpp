// gimmickField.cpp
#include "gimmickField.h"
#include <cmath>

void GIMMICK_FIELD::GimmickField_Initialize(XMFLOAT3 pos)
{
	m_FirstPosition = pos;
	m_Position = pos;
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Count = 0.0f;
    m_FirstPosition = pos;
    m_Position = pos;
    m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
    m_Count = 0.0f;

    m_Channel = 0;
    m_TargetPosition = XMFLOAT3(pos.x, pos.y + 1.0f, pos.z); // マップチップで設定したい
    m_MoveSpeed = FIELD_MOVE_SPEED;

}

void GIMMICK_FIELD::GimmickField_Finalize(void)
{

}

void GIMMICK_FIELD::GimmickField_Update(bool isOn)
{
	// 目標地点の設定
    XMFLOAT3 dst;
    if (isOn)
    {
        dst = m_TargetPosition;
    }
    else
    {
        dst = m_FirstPosition;
    }

	// 移動量の計算
    const float step = m_MoveSpeed * (1.0f / 60.0f);

	// 移動ベクトルの計算
    XMFLOAT3 diff(
        dst.x - m_Position.x,
        dst.y - m_Position.y,
        dst.z - m_Position.z
    );
    
	// 移動処理
    float lenSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
    float stepSq = step * step;

	// 目標地点に到達したら、目標地点に座標を合わせる
    if (lenSq <= stepSq)
    {
        m_Position = dst;
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

}
