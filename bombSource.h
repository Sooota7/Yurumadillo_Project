#pragma once


#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"player.h"
using namespace DirectX;
#include "BossMonster.h"

#define		BOMB_RADIUS		(0.5f)
#define		BOMB_SPEED_MAX		(1.0f)
#define		BOMB_GRAVITY		(1.0f/60.0f)
#define		BOMB_THROW_POWER	(0.2f)
#define		BOMB_DAMAGE_BOSS	(50.0f)//対ボスダメージ
#define		BOMB_EXPLOSION_AREA_  (60.0f) //koreyorisakidebakuhatu
enum BOMB_STATE
{
	BOMB_NONE,
	BOMB_ITEM,
	BOMB_ACTIVE_HAVE,
	BOMB_ACTIVE_THROW,
	BOMB_EXPLOSION, 
	BOMB_EXPLOSION_BOSS, 
	BOMB_COOL,
	BOMB_MAX

};

enum BOMB_TYPE
{
	TYPE_NORMAL,
	TYPE_FLOW,
	TYPE_RUN,
	TYPE_MAX
};

class BOMBSOURCE
{
private:

	XMFLOAT3	m_FirstPosition;		//爆弾の座標
	XMFLOAT3	m_Position;		//爆弾の座標
	XMFLOAT3	m_Velocity;		//速度
	XMFLOAT3	m_Acceleration;	//落下速度
	float		m_Count;		//爆発までのカウント
	float		m_StopTime;		//静止するまでの時間
	BOMB_STATE	m_State;		//現在の状態
	float		m_LimitCount;	//
	bool		m_Touch;
	bool 		m_Exploded=false;

	BOMB_TYPE	m_Type;
	
	BOSSMONSTER* m_pBossMonster = nullptr;

public:

	void		BombSource_Initialize(XMFLOAT3 pos, BOMB_STATE state);
	void		BombSource_Finalize(void);
	

	void		BombSource_Safe();
	void		BombSource_Active_Have(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	void		BombSource_Active_Throw();
	void		BombSource_Active_Throw_Boss();
	void		BombSource_Cool();
	void		BombSource_Explosion();
	void		BombSource_Explosion_Boss();

	void		BombSource_Active_Type();


	//セッター及びゲッター
	void		BombSource_SetFirstPosition(XMFLOAT3 pos)	{ m_FirstPosition = pos; };
	XMFLOAT3	BombSource_GetFirstPosition()	{ return m_FirstPosition; };

	void		BombSource_SetPosition(XMFLOAT3 pos)	{ m_Position = pos; };
	XMFLOAT3	BombSource_GetPosition()	{ return m_Position; };

	void		BombSource_SetVelocity(XMFLOAT3 vel)	{ m_Velocity = vel; };
	XMFLOAT3	BombSource_GetVelocity()	{ return m_Velocity; };

	void		BombSource_SetState(BOMB_STATE state) { m_State = state; };
	BOMB_STATE	BombSource_GetState()		{ return m_State; };

	void		BombSource_SetCount(float count) { m_Count = count; };
	float		BombSource_GetCount() { return m_Count; };

	void		BombSource_SetTouch(bool check) { m_Touch = check; };
	bool		BombSource_GetTouch() { return m_Touch; };

	void BombSource_SetBoss(BOSSMONSTER* boss) { m_pBossMonster = boss; }

	BOMBSOURCE* BombSource_GetBombSource() { return this; };
};
