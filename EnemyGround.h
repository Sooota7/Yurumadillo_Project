// ====================================================================
// 
// EnemyGround.h             制作者: 藤原海斗
// 日付: 02/12
//
// ====================================================================

#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"

#include	"Enemy.h"

#include	"parts.h"

#define ENEMY_SPEED (0.25f)
#define ENEMY_GRAVITY (0.1f/60.0f)

//敵（地）の状態
enum ENEMY_GROUND_STATE
{
	ENEMY_GROUND_STATE_NONE = 0,	//何もしない
	ENEMY_GROUND_STATE_IDLE,	//何もしない
	ENEMY_GROUND_STATE_MOVE,		//移動
	ENEMY_GROUND_STATE_ATTACK,	//方向指示
	ENEMY_GROUND_STATE_CREATE_WEAPON,	//方向指示
	ENEMY_GROUND_STATE_COOL,	//方向指示
	ENEMY_GROUND_STATE_DEAD,			//死亡
	ENEMY_GROUND_STATE_MAX			//死亡
};

class ENEMY_GROUND_WEAPON
{
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMFLOAT3 m_Scaling;
	XMFLOAT3 m_Velocity;
};


enum ENEMY_GROUND_PARTS
{
	EG_PARTS_BODY,
	EG_PARTS_ARM_RIGHT,
	EG_PARTS_ARM_LEFT,
	EG_PARTS_LEG_RIGHT,
	EG_PARTS_LEG_LEFT,
	EG_PARTS_MAX
};

class ENEMY_GROUND_ANIM
{
public:
	ANIM anim[EG_PARTS_MAX];
};



class ENEMY_GROUND :public ENEMY
{
private:

	ENEMY_GROUND_STATE m_State;		// 状態
	float m_FrameCnt;					// フレームカウント
	float m_AttackCool;
	XMFLOAT3	m_ChasePos;			// 追跡する相手
	ENEMY_TYPE  m_Type;

	ENEMY_GROUND_WEAPON m_Weapon;

	PARTS m_EG_Parts[EG_PARTS_MAX];
	
private:
	void	Enemy_Ground_Idle();
	void	Enemy_Ground_Move();
	
	void	Enemy_Ground_Attack(XMFLOAT3 chasePos);
	void	Enemy_Ground_CreateWeapon();
	void	Enemy_Ground_Cool();
	void	Enemy_Ground_Dead();

	void	Enemy_Ground_SetAnimMove();


public:

	void	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) override;
	void	Finalize()override;
	void	Update(XMFLOAT3 chasePos)override;
	void	Draw()override;

	void	Enemy_Ground_SetDirection();//Collisionで呼ぶ

	
	ENEMY* GetEnemy()override { return this; };

	void SetEnemyGroundFrame(float frame) { m_FrameCnt = frame; };
	float GetEnemyGroundFrame() { return m_FrameCnt; };

	void SetEnemyGroundState(ENEMY_GROUND_STATE state) { m_State = state; };
	ENEMY_GROUND_STATE GetEnemyGroundState() { return m_State; };

	void SetEnemyGroundType(ENEMY_TYPE type) { m_Type = type; };
	ENEMY_TYPE GetEnemyGroundType() { return m_Type; };

	PARTS* GetEnemyGroundParts() { return m_EG_Parts; };

};