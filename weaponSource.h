#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"player.h"
using namespace DirectX;

#define		WEAPON_RADIUS		(0.5f)
#define		WEAPON_SPEED_MAX		(1.0f)
#define		WEAPON_GRAVITY		(1.0f/60.0f)
#define		WEAPON_THROW_POWER		(0.2f)

enum WEAPON_STATE
{
	WEAPON_NONE,
	WEAPON_MOVE,
	WEAPON_DIRECTION,
	WEAPON_POWER,
	WEAPON_GROUND,
	WEAPON_COOL,
	WEAPON_MAX

};



class WEAPONSOURCE
{
private:

	XMFLOAT3	m_Position;		//爆弾の座標
	XMFLOAT3	m_Velocity;		//速度
	XMFLOAT3	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);	//落下速度
	float		m_Count;		//爆発までのカウント
	float		m_StopTime;		//静止するまでの時間
	WEAPON_STATE	m_State;		//現在の状態

	bool m_isDamage;
public:

	void		WeaponSource_Initialize(XMFLOAT3 pos, WEAPON_STATE state);
	void		WeaponSource_Finalize(void);


	void		WeaponSource_Move();
	XMFLOAT3		WeaponSource_Direction(XMFLOAT3 pPlayerPos);
	void		WeaponSource_Power();
	void		WeaponSource_Ground();
	void		WeaponSource_Cool();

	void		WeaponSource_SetPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3	WeaponSource_GetPosition() { return m_Position; };

	void		WeaponSource_SetVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3	WeaponSource_GetVelocity() { return m_Velocity; };

	void		WeaponSource_SetState(WEAPON_STATE state) { m_State = state; };
	WEAPON_STATE	WeaponSource_GetState() { return m_State; };

	float		WeaponSource_GetCount() { return m_Count; };

	WEAPONSOURCE* GetWeaponSource() { return this; };

	void		WeaponSource_SetIsDamage(bool set) { m_isDamage = set; };
	bool		WeaponSource_GetIsDamage() { return m_isDamage; };
};