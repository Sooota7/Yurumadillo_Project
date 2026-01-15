// ====================================================================
// 
// EnemyButterfly.cpp            制作者: 杉森奏太
// 日付: 11/23
//
// ====================================================================
#include	"EnemyButterfly.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"
#include	"player.h"



static ID3D11Device* g_pDevice;
static ID3D11DeviceContext* g_pContext;


void ENEMY_BUTTERFLY::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	SetEnemyPosition(XMFLOAT3(1.0f, 4.0f, 7.0f));
	SetEnemyRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetEnemyVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetEnemyScaling(XMFLOAT3(1.0f, 1.0f, 1.0f));

	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);
	m_FrameCnt = 0;

	//m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
	SetEnemyHp(100);

	SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE_MOVE);
}

void ENEMY_BUTTERFLY::Finalize()
{


}

void ENEMY_BUTTERFLY::Update(XMFLOAT3 chasePos)
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

	// ホバリング
	m_Position.y += sinf(cycle) * SWING_WIDTH;
	cycle += 0.05f;

	switch (m_State)
	{
	case ENEMY_BUTTERFLY_STATE_IDLE:
		Enemy_Butterfly_Idle();
		break;
	case ENEMY_BUTTERFLY_STATE_MOVE:
		Enemy_Butterfly_Move();
		break;
	case ENEMY_BUTTERFLY_STATE_DIRECTION:
		Enemy_Butterfly_Direction(chasePos);
		break;
	case ENEMY_BUTTERFLY_STATE_ATTACK:
		Enemy_Butterfly_Attack(chasePos);
		break;
	case ENEMY_BUTTERFLY_STATE_DEAD:
		Enemy_Butterfly_Dead();
		break;
	default:
		break;
	}
}

void ENEMY_BUTTERFLY::Draw()
{

}

void ENEMY_BUTTERFLY::Enemy_Butterfly_Idle()
{
	m_FrameCnt++;			// カウントアップ
	if (m_FrameCnt >= ENEMY_BUTTERFLY_IDLE_CNT)
	{
		m_State = ENEMY_BUTTERFLY_STATE::ENEMY_BUTTERFLY_STATE_DIRECTION;
		m_FrameCnt = 0;
	}

}

void ENEMY_BUTTERFLY::Enemy_Butterfly_Move()
{
	m_Velocity.x += m_Acceleration.x;
	//m_Velocity.y += m_Acceleration.y;
	m_Velocity.z += m_Acceleration.z;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	m_Velocity.x *= GENSUI;	// 速度を適当に減衰する
	m_Velocity.z *= GENSUI;

	// 静止チェック
	float	len = (m_Velocity.x * m_Velocity.x +
		m_Velocity.y * m_Velocity.y +
		m_Velocity.z * m_Velocity.z);
	if (len <= 0.0002f)
	{
		m_Velocity = XMFLOAT3(0, 0, 0);
		m_State = ENEMY_BUTTERFLY_STATE::ENEMY_BUTTERFLY_STATE_IDLE;
	}

}

void ENEMY_BUTTERFLY::Enemy_Butterfly_Direction(XMFLOAT3 chasePos)
{
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

		/*const float yaw = atan2f(direction.x, direction.z);
		m_Rotation.y = yaw;*/
	}

	// プレイヤーの方向を向く

	if (length < 2.5f) // アルファテスト用
	{// プレイヤーとの距離が近すぎたら
		m_Velocity.x = -direction.x * 0.1f; // 反転
		m_Velocity.z = -direction.z * 0.1f;

		SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE_MOVE);
	}
	else if (length > 4.0f) // アルファテスト用
	{// 遠すぎたら
		m_Velocity.x = direction.x * 0.1f;
		m_Velocity.y = direction.y * 0.1f;
		m_Velocity.z = direction.z * 0.1f;

		SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE_MOVE);
	}
	else
	{// 攻撃範囲
		SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE_ATTACK);  // 攻撃
	}
}

void ENEMY_BUTTERFLY::Enemy_Butterfly_Attack(XMFLOAT3 chasePos)
{
	// 武器はweapon側で生成
	SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE_IDLE);
}

void ENEMY_BUTTERFLY::Enemy_Butterfly_Dead()
{
	m_Type = (ENEMY_TYPE_DEAD);
}
