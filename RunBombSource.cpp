#include"RunBombSource.h"
#include	"keyboard.h"
#include	"collision.h"
#include	"mouse.h"

void RUNBOMBSOURCE::Runbombsource_Initialize(XMFLOAT3 pos, BOMB_STATE state)
{
	m_FirstPosition = pos;
	m_Position = pos;
	m_Rotation = XMFLOAT3(0, 0, 0);
	m_Velocity = XMFLOAT3(0, 0, 0);
	m_State = state;
	m_Count = 0;
	m_Touch = false;
}

void RUNBOMBSOURCE::Runbombsource_Finalize(void)
{
}

void RUNBOMBSOURCE::Runbombsource_Safe()
{
	m_Position.y -= BOMB_GRAVITY;
	if (Keyboard_IsKeyDownTrigger(KK_V) && m_Touch)
	{
		m_State = BOMB_STATE::BOMB_ACTIVE_HAVE;
		m_Touch = false;
		m_Count = 0;
	}
}

void RUNBOMBSOURCE::Runbombsource_Active_Have(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
{
	m_Position = pPlayerPos;
	m_Position.y += 1.0f;



	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = BOMB_STATE::BOMB_EXPLOSION;
		m_Count = 0;
	}


	if (Mouse_IsLeftDownTrigger())
	{
		// プレイヤーの向き
		float yaw = pPlayerRot.y;

		// プレイヤーの正面方向ベクトル
		float pVecX = sinf(yaw);
		float pVecZ = cosf(yaw);

		// 正規化
		float len = sqrtf(pVecX * pVecX + pVecZ * pVecZ);
		if (len > 0.0f) {
			pVecX /= len;
			pVecZ /= len;
		}

		float speed = BOMB_SPEED_MAX * BOMB_THROW_POWER;

		// 投げる速度
		m_Velocity.x = pVecX * speed;
		m_Velocity.y = pPlayerPos.y + 0.5f;
		m_Velocity.z = pVecZ * speed;

		m_State = BOMB_STATE::BOMB_ACTIVE_THROW;
	}


}

void RUNBOMBSOURCE::Runbombsource_Active_Throw()
{
	m_Position.x += m_Velocity.x;
	m_Position.y = m_Velocity.y;
	m_Position.z += m_Velocity.z;

	
	if (m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y > 0.0001f)
	{
		m_Rotation.y = atan2(m_Velocity.x, m_Velocity.z);
	}

	//float rotY = 0;

	//if (m_Velocity.z > 0)
	//{
	//	rotY = 0;
	//}
	//else 
	//{
	//	rotY = 180;
	//}

	////ifで0チェック後ｚとｘで角度取る
	//if (m_Velocity.x > 0)
	//{
	//	rotY += (90/m_Velocity.x);
	//}
	//else
	//{
	//	rotY += (90/m_Velocity.x);
	//}

	//m_Rotation.y = XMConvertToRadians(rotY);

	//落下判定
	if (m_Position.y < -10.0f)
	{
		m_State = BOMB_STATE::BOMB_COOL;
		return;
	}

	//m_Velocity.x *= 0.98f;//速度を適当に減衰させる
	m_Velocity.y = 0.98f;//追加する
	//m_Velocity.z *= 0.98f;

	//静止チェック
	float	len =
		(
			m_Velocity.x * m_Velocity.x +
			m_Velocity.y * m_Velocity.y +
			m_Velocity.z * m_Velocity.z
			);

	if (len <= 0.0002f)//静止とみなす速度
	{
		m_StopTime++;
		if (m_StopTime > (60.0f * 2))//２秒間続いている
		{
			m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_StopTime = 0.0f;
		}
	}

	//当たり判定
	//float hit = BallField_Collision();

	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = BOMB_STATE::BOMB_EXPLOSION;
		m_Count = 0;
	}


	m_Velocity.y -= BOMB_GRAVITY;



}

void RUNBOMBSOURCE::Runbombsource_Cool()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = BOMB_STATE::BOMB_ITEM;
		m_Count = 0;
		m_Position = m_FirstPosition;
	}
}

void RUNBOMBSOURCE::Runbombsource_Explosion()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 2.0f)
	{
		m_State = BOMB_STATE::BOMB_COOL;
		m_Count = 0;
	}
}

void RUNBOMBSOURCE::Runbombsource_Active_Type()
{
	switch (m_Type)
	{
	case TYPE_NORMAL:

		break;
	case TYPE_FLOW:

		break;
	case TYPE_RUN:

		break;

	default:
		break;
	}

}
