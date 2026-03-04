#include    "weaponSource.h"
#include	"collision.h"


void WEAPONSOURCE::WeaponSource_Initialize(XMFLOAT3 pos, WEAPON_STATE state)
{
	m_Position = pos;
	m_State = state;
	m_Count = 0;
	m_isDamage = false;
}

void WEAPONSOURCE::WeaponSource_Finalize(void)
{

}

void WEAPONSOURCE::WeaponSource_Move()
{
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	m_Velocity.x *= 0.99f;	// 速度を適当に減衰する
	m_Velocity.y *= 0.99f;
	m_Velocity.z *= 0.99f;

	m_Position.y -= 1.0 / 40.0f;

	// 静止チェック
	float	len = (m_Velocity.x * m_Velocity.x +
		m_Velocity.y * m_Velocity.y +
		m_Velocity.z * m_Velocity.z);
	if (len <= 0.0002f)
	{
		m_Velocity = XMFLOAT3(0, 0, 0);
		WeaponSource_SetState(WEAPON_GROUND);
	}


	if (m_Position.y < -10.0f)
	{
		WeaponSource_SetState(WEAPON_NONE);
	}
}

XMFLOAT3 WEAPONSOURCE::WeaponSource_Direction(XMFLOAT3 pPlayerPos)
{
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
	WeaponSource_SetState(WEAPON_POWER);
	return m_Velocity;
}

void WEAPONSOURCE::WeaponSource_Power()
{
	float power = WEAPON_THROW_POWER * 0.5f;

	m_Velocity.x *= power;
	//m_Velocity.y += 0.15f;
	m_Velocity.z *= power;

	WeaponSource_SetState(WEAPON_MOVE);
}

void WEAPONSOURCE::WeaponSource_Ground()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 3.0f)
	{
		m_Count = 0.0f;
		m_isDamage = false;
		WeaponSource_SetState(WEAPON_NONE);
	}
}

void WEAPONSOURCE::WeaponSource_Cool()
{

}
