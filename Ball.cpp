
//Ball.cpp

#include	"keyboard.h"
#include	"Ball.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"

//ボールオブジェクト

ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pContext;

float g_StopTime = 0.0f;	// ボールが制止するまでの時間

void	BALL::BallInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Model = ModelLoad("asset\\model\\ball.fbx");

	m_Position = XMFLOAT3(0.0f, 2.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_State = BALL_STATE::BALL_STATE_MOVE;

	g_StopTime = 0.0f;

}
void	BALL::BallFinalize()
{


	ModelRelease(m_Model);

}
void	BALL::BallUpdate()
{
	switch (m_State)
	{
	case BALL_STATE::BALL_STATE_IDLE:
		Ball_Idle();
		break;
	case BALL_STATE::BALL_STATE_MOVE:
		Ball_Move();
		break;
	case BALL_STATE::BALL_STATE_DIRECTION:
		Ball_Direction();
		break;
	case BALL_STATE::BALL_STATE_POWER:
		Ball_Power();
		break;
	}

	////デバッグ
	//Velocity = XMFLOAT3(0, 0, 0);
	//if (Keyboard_IsKeyDown(KK_UP))
	//{
	//	Velocity.z = 1.0f / 60.0f;
	//}
	//if (Keyboard_IsKeyDown(KK_DOWN))
	//{
	//	Velocity.z = -1.0f / 60.0f;
	//}
	//if (Keyboard_IsKeyDown(KK_LEFT))
	//{
	//	Velocity.x = -1.0f / 60.0f;
	//}
	//if (Keyboard_IsKeyDown(KK_RIGHT))
	//{
	//	Velocity.x = 1.0f / 60.0f;
	//}
	//Position.x += Velocity.x;
	//Position.y += Velocity.y;
	//Position.z += Velocity.z;


}
void	BALL::BallDraw()
{
	//ワールド行列作成
	XMMATRIX	scale = XMMatrixScaling(
		m_Scaling.x,
		m_Scaling.y,
		m_Scaling.z);
	XMMATRIX	rotation = XMMatrixRotationRollPitchYaw(
		m_Rotation.x,
		m_Rotation.y,
		m_Rotation.z);
	XMMATRIX	translation = XMMatrixTranslation(
		m_Position.x,
		m_Position.y,
		m_Position.z);
	XMMATRIX	world = scale * rotation * translation;

	//変換行列作成
	XMMATRIX	view = GetViewMatrix();
	XMMATRIX	projection = GetProjectionMatrix();
	XMMATRIX	wvp = world * view * projection;

	//シェーダーへ行列をセット
	Shader_SetWorldMatrix(world);
	Shader_SetMatrix(wvp);

	//モデルの描画リクエスト
	ModelDraw(m_Model);

}


void	BALL::Ball_Idle()
{

}

void	BALL::Ball_Move()
{
	m_Velocity.x += m_Acceleration.x;
	m_Velocity.y += m_Acceleration.y;
	m_Velocity.z += m_Acceleration.z;

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	m_Velocity.x *= GENSUI;	// 速度を適当に減衰する
	//Velocity.y *= GENSUI;
	m_Velocity.z *= GENSUI;

	// 静止チェック
	float	len = (m_Velocity.x * m_Velocity.x +
		m_Velocity.y * m_Velocity.y +
		m_Velocity.z * m_Velocity.z);

	if (len <= STOP_VELO) // 静止とみなす速度
	{
		g_StopTime++;
		if (g_StopTime > 60.0f * 2) // 2秒間続いている
		{
			m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_State = BALL_STATE::BALL_STATE_DIRECTION;
			g_StopTime = 0.0f;
		}
	}

	//float hit = BallFieldCollision(); 
}

void	BALL::Ball_Power()
{
	// 打ち出すスピードを決める
	float	power = BALL_SPEEDMAX * 0.12f; // とりあえず固定値

	m_Velocity.x *= power;
	m_Velocity.y *= power;
	m_Velocity.z *= power;

	m_State = BALL_STATE::BALL_STATE_MOVE;
}

void	BALL::Ball_Direction()
{
	// とりあえずかめらの向いている方向へ転がす
	// スペースキーを押したら転がる
	if (Keyboard_IsKeyDownTrigger(KK_SPACE)) // トリガーでチェック
	{
		// カメラの向きを取得
		XMFLOAT3	v1 = GetCameraAtPosition();
		XMFLOAT3	v2 = GetCameraPosition();
		XMFLOAT3	Direction;
		// カメラが向いている方向ベクトル（水平方向）
		Direction.x = v1.x - v2.x;
		Direction.y = 0.0f;
		Direction.z = v1.z - v2.z;
		// Direction を正規化する
		float	len = sqrtf(Direction.x * Direction.x +
			Direction.y * Direction.y +
			Direction.z * Direction.z);

		Direction.x /= len;
		Direction.z /= len;

		m_Velocity = Direction;
		m_State = BALL_STATE::BALL_STATE_POWER;
	}



}


BALL* BALL::GetBall()
{
	return this;
}




