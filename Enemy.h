// ====================================================================
// 
// Enemy.h             制作者: 杉森奏太
// 日付: 11/23
//
// ====================================================================

#pragma once

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"

enum ENEMY_TYPE
{
	ENEMY_TYPE_NONE = 0,

	ENEMY_TYPE_NORMAL,

	ENEMY_TYPE_DEAD,

	ENEMY_TYPE_MAX
};

//落下の底、死ぬライン
#define	FALL_MAX (-3)

class ENEMY
{
protected:
	XMFLOAT3	m_Position;	//表示座標
	XMFLOAT3	m_Rotation;	//回転角
	XMFLOAT3	m_Scaling;	//拡大率
	XMFLOAT3	m_Velocity;	//速度
	XMFLOAT3	m_Acceleration;	// 落下速度
	int			m_Hp;		// 体力
	
	MODEL* m_Model;		//モデルデータ

public:
	virtual void	Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) = 0;
	virtual void	Finalize() = 0;
	virtual void	Update(XMFLOAT3 playerPos) = 0;
	virtual void	Draw() = 0;
	virtual ENEMY*	GetEnemy() = 0;

	virtual ~ENEMY() {};  // 仮想デストラクタ

	virtual void SetEnemyPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetEnemyPosition() { return m_Position; };

	virtual void SetEnemyRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetEnemyRotation() { return m_Rotation; };

	virtual void SetEnemyScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetEnemyScaling() { return m_Scaling; };

	virtual void SetEnemyVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetEnemyVelocity() { return m_Velocity; };

	virtual void SetEnemyHp(int hp) { m_Hp = hp; };
	int GetEnemyHp() { return m_Hp; };

	
};

