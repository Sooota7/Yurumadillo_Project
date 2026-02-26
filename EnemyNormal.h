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
	ENEMY_NORMAL_STATE_NONE = -1,	//状態なし
	ENEMY_NORMAL_STATE_IDLE = 0,	//何もしない
	ENEMY_NORMAL_STATE_MOVE,		//移動
	ENEMY_NORMAL_STATE_DIRECTION,	//方向指示
	ENEMY_NORMAL_STATE_JUMP,		//ジャンプ
	ENEMY_NORMAL_STATE_ATTACK,		//攻撃
	ENEMY_NORMAL_STATE_COOL,		//クール
	ENEMY_NORMAL_STATE_DEAD			//死亡
};

class ENEMY_NORMAL : public ENEMY
{
private:
	constexpr static int ENEMY_NORMAL_IDLE_CNT = 120;
	constexpr static float ENEMY_NORMAL_POWER = 2.0f; // 移動量

	ENEMY_NORMAL_STATE m_State;		// 状態
	float m_FrameCnt;					// フレームカウント
	float m_AttackCool;

	XMFLOAT3	m_ChasePos;			// 追跡する相手
	ENEMY_TYPE  m_Type;

private:
	void	Enemy_Normal_Idle();
	void	Enemy_Normal_Move();
	void	Enemy_Normal_Direction(XMFLOAT3 chasePos);
	void	Enemy_Normal_Jump();
	void	Enemy_Normal_Attack();
	void	Enemy_Normal_Cool();
	void	Enemy_Normal_Dead();


public:
	void	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) override;
	void	Finalize()override;
	void	Update(XMFLOAT3 chasePos)override;
	void	Draw()override;

	ENEMY* GetEnemy()override { return this; };

	void SetEnemyNormalState(ENEMY_NORMAL_STATE state) { m_State = state; };
	ENEMY_NORMAL_STATE GetEnemyNormalState() { return m_State; };

	void SetEnemyNormalType(ENEMY_TYPE type) { m_Type = type; };
	ENEMY_TYPE GetEnemyNormalType() { return m_Type; };
};