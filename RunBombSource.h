#pragma once


#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"player.h"
using namespace DirectX;

enum RUNBOMB_STATE
{
	RUNBOMB_NONE,
	RUNBOMB_ENEMY,
	RUNBOMB_ITEM,
	RUNBOMB_ACTIVE_HAVE,
	RUNBOMB_ACTIVE_THROW,
	RUNBOMB_EXPLOSION,
	RUNBOMB_COOL,
	RUNBOMB_MAX

};

class RUNBOMBSOURCE
{
	XMFLOAT3	m_FirstPosition;		//爆弾の座標
	XMFLOAT3	m_Position;		//爆弾の座標
	XMFLOAT3	m_Velocity;		//速度
	XMFLOAT3	m_Rotation;		//速度
	XMFLOAT3	m_Acceleration;	//落下速度
	float		m_Count;		//爆発までのカウント
	float		m_StopTime;		//静止するまでの時間
	RUNBOMB_STATE	m_State;		//現在の状態
	float		m_LimitCount;	//
	bool		m_Touch;

	bool		m_fieldColision;

	//BOMB_TYPE	m_Type;

public:

	void		Runbombsource_Initialize(XMFLOAT3 pos, RUNBOMB_STATE state);
	void		Runbombsource_Finalize(void);


	void		Runbombsource_Safe();
	void		Runbombsource_Active_Have(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	void		Runbombsource_Active_Throw();
	void		Runbombsource_Cool();
	void		Runbombsource_Explosion();
	void		Runbombsource_Enemy(XMFLOAT3 pPlayerPos);

	//void		Runbombsource_Active_Type();


	//セッター及びゲッター
	void		Runbombsource_SetPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3	Runbombsource_GetPosition() { return m_Position; };

	void		Runbombsource_SetRotation(XMFLOAT3 rot) { m_Rotation = rot; };
	XMFLOAT3	Runbombsource_GetRotation() { return m_Rotation; };

	void		Runbombsource_SetVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3	Runbombsource_GetVelocity() { return m_Velocity; };

	void		Runbombsource_SetState(RUNBOMB_STATE state) { m_State = state; };
	RUNBOMB_STATE	Runbombsource_GetState() { return m_State; };

	void		Runbombsource_SetCount(float count) { m_Count = count; };
	float		Runbombsource_GetCount() { return m_Count; };

	void		Runbombsource_SetTouch(bool check) { m_Touch = check; };
	bool		Runbombsource_GetTouch() { return m_Touch; };

	void		Runbombsource_SetFieldCollision(bool touch) { m_fieldColision = touch; };
	bool		Runbombsource_GetFieldCollision() { return m_fieldColision; };


	RUNBOMBSOURCE* Runbombsource_GetRunbombsource() { return this; };
};
