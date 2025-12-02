#include "bombSource.h"
#include	"keyboard.h"
#include	"collision.h"


void BOMBSOURCE::BombSource_Initialize(XMFLOAT3 pos, BOMB_STATE state)
{
	m_FirstPosition = pos;
	m_Position = pos;
	m_State = state;
	m_Count = 0;
	m_Touch = false;
}

void BOMBSOURCE::BombSource_Finalize(void)
{
}


void BOMBSOURCE::BombSource_Safe()
{
	
	m_Position.y -= BOMB_GRAVITY;
	
	if (Keyboard_IsKeyDownTrigger(KK_V)&&m_Touch)
	{
		m_State = BOMB_STATE::BOMB_ACTIVE_HAVE;
		m_Touch = false;
		m_Count = 0;
	}
}

void BOMBSOURCE::BombSource_Active_Have(XMFLOAT3 pPlayerPos,XMFLOAT3 pPlayerRot)
{
	m_Position = pPlayerPos;
	m_Position.y += 0.5;

	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = BOMB_STATE::BOMB_EXPLOSION;
		m_Count = 0;
	}

	//動き
	if (Keyboard_IsKeyDownTrigger(KK_B))//トリガーでチェック！
	{

		m_Velocity.x = sinf(XMConvertToRadians(pPlayerRot.y));
		//m_Velocity.y = 1.0f;
		m_Velocity.z = cosf(XMConvertToRadians(pPlayerRot.y));

		XMFLOAT3	power = XMFLOAT3(1.0f, 1.0f, 1.0f);
		power.z *= BOMB_SPEED_MAX * 0.1f;

		m_Velocity.x *= power.z;
		/*if (power.z < 0.16f)
		{
			m_Velocity.y = 0.0f;
		}
		else*/
		//{//それなりのパワーの場合、ボール浮かす
			m_Velocity.y = power.z * 1.5f;
		//}
		m_Velocity.z *= power.z;

		
		m_State = BOMB_STATE::BOMB_ACTIVE_THROW;
	}
	


}


void BOMBSOURCE::BombSource_Active_Throw()
{
	m_Velocity.y -= BOMB_GRAVITY;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	//落下判定
	if (m_Position.y < -10.0f)
	{
		m_State = BOMB_STATE::BOMB_COOL;
		return;
	}

	m_Velocity.x *= 0.98f;//速度を適当に減衰させる
	m_Velocity.y *= 0.98f;//追加する
	m_Velocity.z *= 0.98f;

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

}

void BOMBSOURCE::BombSource_Cool()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = BOMB_STATE::BOMB_ITEM;
		m_Count = 0;
		m_Position = m_FirstPosition;
	}
}

void BOMBSOURCE::BombSource_Explosion()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = BOMB_STATE::BOMB_COOL;
		m_Count = 0;
	}
}

