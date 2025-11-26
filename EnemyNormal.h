// ====================================================================
// 
// EnemyNormal.h             制作者: 杉森奏太
// 日付: 11/23
//
// ====================================================================

#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"

#include	"Enemy.h"

//敵（通常）の状態
enum ENEMY_NORMAL_STATE
{
	ENEMY_NORMAL_STATE_IDLE = 0,	//何もしない
	ENEMY_NORMAL_STATE_MOVE,		//移動
	ENEMY_NORMAL_STATE_DIRECTION,	//方向指示
	ENEMY_NORMAL_STATE_JUMP,		//ジャンプ
	ENEMY_NORMAL_STATE_DEAD			//死亡
};

class ENEMY_NORMAL : public ENEMY
{
private:
	ENEMY_NORMAL_STATE m_State;		// 状態

private:
	void	Enemy_Normal_Idle();
	void	Enemy_Normal_Move();
	void	Enemy_Normal_Direction();
	void	Enemy_Normal_Jump();
	void	Enemy_Normal_Dead();

public:
	void	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) override;
	void	Finalize()override;
	void	Update()override;
	void	Draw()override;

	ENEMY* GetEnemy()override { return this; };

	void SetEnemyNormalState(ENEMY_NORMAL_STATE state) { m_State = state; };
	ENEMY_NORMAL_STATE GetEnemyNormalState() { return m_State; };
};