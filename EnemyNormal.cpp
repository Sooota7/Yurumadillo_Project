// ====================================================================
// 
// EnemyNormal.cpp            制作者: 杉森奏太
// 日付: 11/23
//
// ====================================================================
#include	"EnemyNormal.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"
#include	"player.h"

static ID3D11Device* g_pDevice;
static ID3D11DeviceContext* g_pContext;

void	ENEMY_NORMAL::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Model = ModelLoad("asset\\model\\test.fbx");

	SetEnemyPosition(XMFLOAT3(1.0f, 4.0f, 7.0f));
	SetEnemyRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetEnemyVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetEnemyScaling(XMFLOAT3(1.0f, 1.0f, 1.0f));
	
	/*m_Position = XMFLOAT3(0.0f, 2.0f, 1.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);*/

	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);
	m_FrameCnt = 0;

	//m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);
	SetEnemyHp(100);

	SetEnemyNormalState(ENEMY_NORMAL_STATE_MOVE);
}

void	ENEMY_NORMAL::Finalize()
{
	ModelRelease(m_Model);
}

void	ENEMY_NORMAL::Update(XMFLOAT3 chasePos)
{
	switch (m_State)
	{
	case ENEMY_NORMAL_STATE_IDLE:
		Enemy_Normal_Idle();
		break;
	case ENEMY_NORMAL_STATE_MOVE:
		Enemy_Normal_Move();
		break;
	case ENEMY_NORMAL_STATE_DIRECTION:
		Enemy_Normal_Direction(chasePos);
		break;
	case ENEMY_NORMAL_STATE_JUMP:
		Enemy_Normal_Jump();
		break;
	case ENEMY_NORMAL_STATE_DEAD:
		Enemy_Normal_Dead();
		break;
	default:
		break;
	}
}

void	ENEMY_NORMAL::Draw()
{
//{
//	//ワールド行列作成
//	XMMATRIX	scale = XMMatrixScaling(
//		m_Scaling.x,
//		m_Scaling.y,
//		m_Scaling.z);
//	XMMATRIX	rotation = XMMatrixRotationRollPitchYaw(
//		m_Rotation.x,
//		m_Rotation.y,
//		m_Rotation.z);
//	XMMATRIX	translation = XMMatrixTranslation(
//		m_Position.x,
//		m_Position.y,
//		m_Position.z);
//	XMMATRIX	world = scale * rotation * translation;
//
//	//変換行列作成
//	XMMATRIX	view = GetViewMatrix();
//	XMMATRIX	projection = GetProjectionMatrix();
//	XMMATRIX	wvp = world * view * projection;
//
//	//シェーダーへ行列をセット
//	Shader_SetWorldMatrix(world);
//	Shader_SetMatrix(wvp);
//
//	//モデルの描画リクエスト
//	//ModelDraw(m_Model);
//	switch (m_State)
//	{
//	case ENEMY_NORMAL_STATE_IDLE:
//		ModelDraw(m_Model);
//		break;
//	case ENEMY_NORMAL_STATE_MOVE:
//		ModelDraw(m_Model);
//		break;
//	case ENEMY_NORMAL_STATE_DIRECTION:
//		ModelDraw(m_Model);
//		break;
//	case ENEMY_NORMAL_STATE_JUMP:
//		ModelDraw(m_Model);
//		break;
//	case ENEMY_NORMAL_STATE_DEAD:
//		
//		break;
//	default:
//		break;
//	}
}

void	ENEMY_NORMAL::Enemy_Normal_Idle()
{
	m_FrameCnt++;			// カウントアップ
	if (m_FrameCnt >= ENEMY_NORMAL_IDLE_CNT)
	{
		m_State = ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_DIRECTION;
		m_FrameCnt = 0;
	}

}

void	ENEMY_NORMAL::Enemy_Normal_Move()
{
	m_Velocity.x += m_Acceleration.x;
	m_Velocity.y += m_Acceleration.y;
	m_Velocity.z += m_Acceleration.z;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	m_Velocity.x *= GENSUI;	// 速度を適当に減衰する
	m_Velocity.y *= GENSUI;
	m_Velocity.z *= GENSUI;

	m_Position.y -= 1.0 / 60.0f;

	// 静止チェック
	float	len = (m_Velocity.x * m_Velocity.x +
		m_Velocity.y * m_Velocity.y +
		m_Velocity.z * m_Velocity.z);
	if (len <= 0.0002f)
	{
		m_Velocity = XMFLOAT3(0, 0, 0);
		m_State = ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_IDLE;
	}


}

void	ENEMY_NORMAL::Enemy_Normal_Direction(XMFLOAT3 chasePos)
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
	}
	
	
	if (length < 2.5f) // アルファテスト用
	{// プレイヤーとの距離が近すぎたら
		m_Velocity.x = -direction.x; // 反転
		m_Velocity.z = -direction.z;

		m_State = ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_JUMP;
	}
	else if (length > 4.0f) // アルファテスト用
	{// 遠すぎたら
		m_Velocity = direction;

		m_State = ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_JUMP;
	}
	else
	{// 攻撃範囲
		//m_State = ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_ATTACK;  // 攻撃
	}
	
}	

void	ENEMY_NORMAL::Enemy_Normal_Jump()
{
	float power = ENEMY_NORMAL_POWER * 0.03f;

	m_Velocity.x *= power;
	m_Velocity.y += 0.15f;
	m_Velocity.z *= power;

	m_State = ENEMY_NORMAL_STATE::ENEMY_NORMAL_STATE_MOVE;
}

void	ENEMY_NORMAL::Enemy_Normal_Dead()
{
	delete this;
}