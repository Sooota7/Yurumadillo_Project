#pragma once


#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"player.h"
#include	"bombSource.h"
using namespace DirectX;


class FLOWTBOMBSOURCE
{
	XMFLOAT3	m_FirstPosition;		//爆弾の座標
	XMFLOAT3	m_Position;		//爆弾の座標
	XMFLOAT3	m_Velocity;		//速度
	XMFLOAT3	m_Acceleration;	//落下速度
	float		m_Count;		//爆発までのカウント
	float		m_StopTime;		//静止するまでの時間
	BOMB_STATE	m_State;		//現在の状態
	float		m_LimitCount;	//
	bool		m_Touch;

	BOMB_TYPE	m_Type;

public:

	void		Flowtbombsource_Initialize(XMFLOAT3 pos, BOMB_STATE state);
	void		Flowtbombsource_Finalize(void);


	void		Flowtbombsource_Safe();
	void		Flowtbombsource_Active_Have(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	void		Flowtbombsource_Active_Throw();
	void		Flowtbombsource_Cool();
	void		Flowtbombsource_Explosion();

	void		Flowtbombsource_Active_Type();


	//セッター及びゲッター
	void		Flowtbombsource_SetPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3	Flowtbombsource_GetPosition() { return m_Position; };

	void		Flowtbombsource_SetVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3	Flowtbombsource_GetVelocity() { return m_Velocity; };

	void		Flowtbombsource_SetState(BOMB_STATE state) { m_State = state; };
	BOMB_STATE	Flowtbombsource_GetState() { return m_State; };

	float		Flowtbombsource_GetCount() { return m_Count; };

	void		Flowtbombsource_SetTouch(bool check) { m_Touch = check; };
	bool		Flowtbombsource_GetTouch() { return m_Touch; };

	FLOWTBOMBSOURCE* Flowtbombsource_GetFlowtbombsource() { return this; };
};
