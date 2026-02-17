#include "EnemyGround.h"

static ID3D11Device* g_pDevice;
static ID3D11DeviceContext* g_pContext;

void ENEMY_GROUND::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	SetEnemyPosition(XMFLOAT3(1.0f, 4.0f, 7.0f));
	SetEnemyRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetEnemyVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetEnemyScaling(XMFLOAT3(1.0f, 1.0f, 1.0f));

	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);
	m_FrameCnt = 0.0f;
	m_AttackCool = 0.0f;

	//m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
	SetEnemyHp(100);

	SetEnemyGroundState(ENEMY_GROUND_STATE_IDLE);

	float downSize = 1.0f;

	for (int i = 0; i < EG_PARTS_MAX; i++)
	{
		switch (i)
		{
		case EG_PARTS_BODY:
			m_EG_Parts[i].PartsInitialize(pDevice, pContext);
			m_EG_Parts[i].SetPartsScaling(XMFLOAT3(1.0f * downSize, 1.0f * downSize, 1.0f * downSize));
			break;
		case EG_PARTS_ARM_RIGHT:
			m_EG_Parts[i].PartsInitialize(pDevice, pContext);
			m_EG_Parts[i].SetPartsScaling(XMFLOAT3(1.0f * downSize, 1.0f * downSize, 1.0f * downSize));
			break;
		case EG_PARTS_ARM_LEFT:
			m_EG_Parts[i].PartsInitialize(pDevice, pContext);
			m_EG_Parts[i].SetPartsScaling(XMFLOAT3(1.0f * downSize, 1.0f * downSize, 1.0f * downSize));
			break;
		case EG_PARTS_LEG_RIGHT:
			m_EG_Parts[i].PartsInitialize(pDevice, pContext);
			m_EG_Parts[i].SetPartsScaling(XMFLOAT3(1.0f * downSize, 1.0f * downSize, 1.0f * downSize));
			break;
		case EG_PARTS_LEG_LEFT:
			m_EG_Parts[i].PartsInitialize(pDevice, pContext);
			m_EG_Parts[i].SetPartsScaling(XMFLOAT3(1.0f * downSize, 1.0f * downSize, 1.0f * downSize));
			break;
		default:
			break;
		}

	}
	
	XMFLOAT3 pos = XMFLOAT3(0.25f, 0.0f, 0.25f);

	for (int i = 0; i < EG_PARTS_MAX; i++)
	{

		switch (i)
		{
		case EG_PARTS_BODY:
			m_EG_Parts[i].SetInisPosition(XMFLOAT3(0.0f, 0.3f, 0.0f));
			break;
		case EG_PARTS_ARM_RIGHT:
			m_EG_Parts[i].SetInisPosition(XMFLOAT3(-0.3f, 0.3f, 0.0f));
			break;
		case EG_PARTS_ARM_LEFT:
			m_EG_Parts[i].SetInisPosition(XMFLOAT3(0.3f, 0.3f, 0.0f));
			break;
		case EG_PARTS_LEG_RIGHT:
			m_EG_Parts[i].SetInisPosition(XMFLOAT3(-0.2f, 0.0f, 0.0f));
			break;
		case EG_PARTS_LEG_LEFT:
			m_EG_Parts[i].SetInisPosition(XMFLOAT3(0.2f, 0.0f, 0.0f));
			break;
		default:
			break;
		}
	}


}

void ENEMY_GROUND::Finalize()
{
}

void ENEMY_GROUND::Update(XMFLOAT3 chasePos)
{

	// 敵の向きをプレイヤーに向ける
	XMFLOAT3 direction;

	direction.x = chasePos.x - m_Position.x;
	direction.y = 0.0f;
	direction.z = chasePos.z - m_Position.z;

	// 距離
	float length = sqrtf((direction.x * direction.x) +
		(direction.z * direction.z));

	if (length != 0.0f)
	{// 正規化
		direction.x /= length;
		direction.z /= length;

		float yaw = atan2(direction.x, direction.z);


		// [-π, π] に正規化
		if (yaw > XM_PI) yaw -= XM_2PI;
		if (yaw < -XM_PI) yaw += XM_2PI;


		m_Rotation.y = yaw;
	}

	if (length < 4)
	{
		SetEnemyGroundState(ENEMY_GROUND_STATE_ATTACK);
	}

	switch (m_State)
	{
	case ENEMY_GROUND_STATE_IDLE:
		Enemy_Ground_Idle();
		break;
	case ENEMY_GROUND_STATE_MOVE:
		Enemy_Ground_Move();
		break;
	case ENEMY_GROUND_STATE_ATTACK:
		Enemy_Ground_Attack(chasePos);
		break;
	case ENEMY_GROUND_STATE_DEAD:
		Enemy_Ground_Dead();
		break;
	default:
		break;
	}

	m_FrameCnt += 1.0f ;
	if (m_FrameCnt >= 60.0f)
	{
		//一旦ループ前提
		m_FrameCnt = 0.0f;
	}

}

void ENEMY_GROUND::Draw()
{
}




//==========================================================
//ステートごとの処理
//==========================================================

void ENEMY_GROUND::Enemy_Ground_Idle()
{
	SetEnemyGroundState(ENEMY_GROUND_STATE_MOVE);

}

void ENEMY_GROUND::Enemy_Ground_Move()
{


	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;


	//落下判定
	if (m_Position.y < -15.0f)
	{
		m_State = ENEMY_GROUND_STATE_DEAD;
		return;
	}

	

	
}

void ENEMY_GROUND::Enemy_Ground_SetDirection()
{
	float yaw = m_Rotation.y;

	float pVecX = sinf(yaw);
	float pVecZ = cosf(yaw);

	// 正規化
	float len = sqrtf(pVecX * pVecX + pVecZ * pVecZ);
	if (len > 0.0f) {
		pVecX /= len;
		pVecZ /= len;
	}

	float speed = ENEMY_SPEED * 0.05f;

	m_Velocity.x = (pVecX)*speed;
	//BombVel.y = 0;//BOMB_THROW_POWER;  // 上方向成分（好みで調整）
	m_Velocity.z = (pVecZ)*speed;

	//SetEnemyGroundState(ENEMY_GROUND_STATE_MOVE);
	m_Velocity.y -= ENEMY_GRAVITY;
	if (m_Velocity.y < -0.2)
	{
		m_Velocity.y = -0.2;
	}


}

void ENEMY_GROUND::Enemy_Ground_Attack(XMFLOAT3 chasePos)
{
	m_AttackCool += 1.0f / 60.0;

	if (m_AttackCool > 2.0f)
	{
		SetEnemyGroundState(ENEMY_GROUND_STATE_MOVE);
		m_AttackCool = 0.0f;
	}

}

void ENEMY_GROUND::Enemy_Ground_Dead()
{
}
