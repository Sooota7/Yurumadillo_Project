#include "WeaponEnemyGround.h"

void WEAPONENEMYGROUND::Weapon_EG_Initialize(XMFLOAT3 pos, EG_WEAPON_STATE state)
{
	m_Position = pos;
	m_State = state;
	m_Count = 0;
	m_isDamage = false;

	m_NormalWeapon = false;
}

void WEAPONENEMYGROUND::Weapon_EG_Finalize(void)
{
}

void WEAPONENEMYGROUND::Weapon_EG_Move()
{
	

	m_Position.x += m_Velocity.x;
	//m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	//m_Velocity.x *= GENSUI;	// 速度を適当に減衰する
	//m_Velocity.y *= GENSUI;
	//m_Velocity.z *= GENSUI;

	//m_Position.y -= 1.0 / 40.0f;

	//// 静止チェック
	//float	len = (m_Velocity.x * m_Velocity.x +
	//	m_Velocity.y * m_Velocity.y +
	//	m_Velocity.z * m_Velocity.z);
	//if (len <= 0.0002f)
	//{
	//	m_Velocity = XMFLOAT3(0, 0, 0);
	//	Weapon_EG_SetState(EG_WEAPON_GROUND);
	//}

	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_Count = 0.0f;
		m_isDamage = false;
		Weapon_EG_SetState(EG_WEAPON_NONE);
	}

	if (m_Position.y < -10.0f)
	{
		Weapon_EG_SetState(EG_WEAPON_NONE);
	}
}

void WEAPONENEMYGROUND::Weapon_EG_Direction(XMFLOAT3 pPlayerPos)
{
	{
		// 敵の向きをプレイヤーに向ける
		XMFLOAT3 direction;

		direction.x = pPlayerPos.x - m_Position.x;
		direction.y = 0.0f;
		direction.z = pPlayerPos.z - m_Position.z;

		// 距離
		float length = sqrtf((direction.x * direction.x) +
			(direction.z * direction.z));

		if (length != 0.0f)
		{// 正規化
			direction.x /= length;
			direction.z /= length;

			float yaw = atan2(direction.x, direction.z);


			// [-π, π] に正規化
			if (yaw > XM_PI) yaw -= XM_2PI;
			if (yaw < -XM_PI) yaw += XM_2PI;


			m_Rotation.y = yaw;
		}
	}

	XMFLOAT3 direction;

	direction.x = pPlayerPos.x - m_Position.x;
	direction.y = 0.0f;
	direction.z = pPlayerPos.z - m_Position.z;

	// 距離
	float length = sqrtf((direction.x * direction.x) +
		(direction.z * direction.z));

	if (length != 0.0f)
	{// 正規化
		direction.x /= length;
		direction.z /= length;
	}

	m_Velocity = direction;
	Weapon_EG_SetState(EG_WEAPON_POWER);
	
	
}

void WEAPONENEMYGROUND::Weapon_EG_Power()
{
	float power = 0.0f;

	if (!m_NormalWeapon) {
		power = EG_WEAPON_THROW_POWER * 0.7f;
	}
	else {
		power = EG_WEAPON_THROW_POWER * 1.2f;
	}

	m_Velocity.x *= power;
	//m_Velocity.y += 0.15f;
	m_Velocity.z *= power;

	Weapon_EG_SetState(EG_WEAPON_MOVE);
}

void WEAPONENEMYGROUND::Weapon_EG_Cool()
{
}
