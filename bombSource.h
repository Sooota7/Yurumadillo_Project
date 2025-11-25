#pragma once


#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#define		BOMB_RADIUS		(0.25f)


enum BOMB_STATE
{
	BOMB_NONE,
	BOMB_SAFE,
	BOMB_ACTIVE,
	BOMB_EXPLOSION,
	BOMB_MAX

};



class BOMBSOURCE
{
private:

	
	XMFLOAT3	m_Position;		//爆弾の座標
	float		m_Count;		//爆発までのカウント
	BOMB_STATE	m_State;		//現在の状態
	float		m_LimitCount;	//
	
public:

	void		BombSource_Initialize(XMFLOAT3 pos, BOMB_STATE state);
	void		BombSource_Finalize(void);
	
	void		BombSource_Update(void);


	void		BombSource_Safe();
	void		BombSource_Active();
	void		BombSource_Explosion();

	void		BombSource_SetState();

	void		BombSource_SetPosition(XMFLOAT3 pos)	{ m_Position = pos; };
	XMFLOAT3	BombSource_GetPosition()	{ return m_Position; };

	void		BombSource_SetState(BOMB_STATE state) { m_State = state; };
	BOMB_STATE	BombSource_GetState()		{ return m_State; };

	float		BombSource_GetCount() { return m_Count; };

};
