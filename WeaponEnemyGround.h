#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"player.h"
using namespace DirectX;

#define		EG_WEAPON_RADIUS		(0.5f)
#define		EG_WEAPON_SPEED_MAX		(1.0f)
#define		EG_WEAPON_GRAVITY		(1.0f/60.0f)
#define		EG_WEAPON_THROW_POWER		(0.2f)

enum EG_WEAPON_STATE
{
	EG_WEAPON_NONE,
	EG_WEAPON_MOVE,
	EG_WEAPON_DIRECTION,
	EG_WEAPON_POWER,
	EG_WEAPON_GROUND,
	EG_WEAPON_COOL,
	EG_WEAPON_MAX

};

class WEAPONENEMYGROUND
{
private:

	XMFLOAT3			m_Position;		//爆弾の座標
	XMFLOAT3			m_Velocity;		//速度
	XMFLOAT3			m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);	//落下速度
	float				m_Count;		//爆発までのカウント
	float				m_StopTime;		//静止するまでの時間
	EG_WEAPON_STATE		m_State;		//現在の状態

	bool m_isDamage;
public:

	void		Weapon_EG_Initialize(XMFLOAT3 pos, EG_WEAPON_STATE state);
	void		Weapon_EG_Finalize(void);


	void		Weapon_EG_Move();
	void		Weapon_EG_Direction(XMFLOAT3 pPlayerPos);
	void		Weapon_EG_Power();
	void		Weapon_EG_Ground();
	void		Weapon_EG_Cool();

	void		Weapon_EG_SetPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3	Weapon_EG_GetPosition() { return m_Position; };

	void		Weapon_EG_SetVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3	Weapon_EG_GetVelocity() { return m_Velocity; };

	void		Weapon_EG_SetState(EG_WEAPON_STATE state) { m_State = state; };
	EG_WEAPON_STATE	Weapon_EG_GetState() { return m_State; };

	float		Weapon_EG_GetCount() { return m_Count; };

	WEAPONENEMYGROUND* GetWeapon_EG() { return this; };

	void		Weapon_EG_SetIsDamage(bool set) { m_isDamage = set; };
	bool		Weapon_EG_GetIsDamage() { return m_isDamage; };
	

};