#pragma once


#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"player.h"
#include	"bombSource.h"
using namespace DirectX;


class RUNBOMBSOURCE
{
	XMFLOAT3	m_FirstPosition;		//爆弾の座標
	XMFLOAT3	m_Position;		//爆弾の座標
	XMFLOAT3	m_Velocity;		//速度
	XMFLOAT3	m_Rotation;		//速度
	XMFLOAT3	m_Acceleration;	//落下速度
	float		m_Count;		//爆発までのカウント
	float		m_StopTime;		//静止するまでの時間
	BOMB_STATE	m_State;		//現在の状態
	float		m_LimitCount;	//
	bool		m_Touch;

	BOMB_TYPE	m_Type;

public:

	void		Runbombsource_Initialize(XMFLOAT3 pos, BOMB_STATE state);
	void		Runbombsource_Finalize(void);


	void		Runbombsource_Safe();
	void		Runbombsource_Active_Have(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	void		Runbombsource_Active_Throw();
	void		Runbombsource_Cool();
	void		Runbombsource_Explosion();

	void		Runbombsource_Active_Type();


	//セッター及びゲッター
	void		Runbombsource_SetPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3	Runbombsource_GetPosition() { return m_Position; };

	void		Runbombsource_SetRotation(XMFLOAT3 rot) { m_Rotation = rot; };
	XMFLOAT3	Runbombsource_GetRotation() { return m_Rotation; };

	void		Runbombsource_SetVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3	Runbombsource_GetVelocity() { return m_Velocity; };

	void		Runbombsource_SetState(BOMB_STATE state) { m_State = state; };
	BOMB_STATE	Runbombsource_GetState() { return m_State; };

	float		Runbombsource_GetCount() { return m_Count; };

	void		Runbombsource_SetTouch(bool check) { m_Touch = check; };
	bool		Runbombsource_GetTouch() { return m_Touch; };

	RUNBOMBSOURCE* Runbombsource_GetRunbombsource() { return this; };
};
