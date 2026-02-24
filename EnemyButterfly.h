// ====================================================================
// 
// EnemyButterfly.h             制作者: 杉森奏太
// 日付: 12/30
//
// ====================================================================

#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"

#include	"Enemy.h"

//敵（蝶）の状態
enum ENEMY_BUTTERFLY_STATE
{
	ENEMY_BUTTERFLY_STATE_NONE = -1,	//状態なし
	ENEMY_BUTTERFLY_STATE_IDLE = 0,	//何もしない
	ENEMY_BUTTERFLY_STATE_MOVE,		//移動
	ENEMY_BUTTERFLY_STATE_DIRECTION,	//方向指示
	ENEMY_BUTTERFLY_STATE_ATTACK,	//方向指示
	ENEMY_BUTTERFLY_STATE_DEAD			//死亡
};

class ENEMY_BUTTERFLY : public ENEMY
{
private:
	constexpr static int ENEMY_BUTTERFLY_IDLE_CNT = 120;
	constexpr static float ENEMY_BUTTERFLY_POWER = 2.0f; // 移動量
	constexpr static float SWING_WIDTH = 0.005f; // 移動量

	ENEMY_BUTTERFLY_STATE m_State;		// 状態
	int m_FrameCnt;					// フレームカウント
	XMFLOAT3	m_ChasePos;			// 追跡する相手
	ENEMY_TYPE  m_Type;

	float cycle = 0.0f;
private:
	void	Enemy_Butterfly_Idle();
	void	Enemy_Butterfly_Move();
	void	Enemy_Butterfly_Direction(XMFLOAT3 chasePos);
	void	Enemy_Butterfly_Attack(XMFLOAT3 chasePos);
	void	Enemy_Butterfly_Dead();


public:
	void	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) override;
	void	Finalize()override;
	void	Update(XMFLOAT3 chasePos)override;
	void	Draw()override;

	ENEMY* GetEnemy()override { return this; };

	void SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE state) { m_State = state; };
	ENEMY_BUTTERFLY_STATE GetEnemyButterflyState() { return m_State; };

	void SetEnemyButterflyType(ENEMY_TYPE type) { m_Type = type; };
	ENEMY_TYPE GetEnemyButterflyType() { return m_Type; };
};