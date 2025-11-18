
//Ball.cpp

#include	"keyboard.h"
#include	"Ball.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"

//ボールオブジェクト
BALL	g_Ball;

ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pContext;

float g_StopTime = 0.0f;	// ボールが制止するまでの時間

void	Ball_Idle();
void	Ball_Move();
void	Ball_Power();
void	Ball_Direction();

void	BallInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	g_Ball.Model = ModelLoad("asset\\model\\ball.fbx");

	g_Ball.Position = XMFLOAT3(0.0f, 2.2f, 0.0f);
	g_Ball.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Ball.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Ball.Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	g_Ball.Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	g_Ball.State = BALL_STATE::BALL_STATE_DIRECTION;

	g_StopTime = 0.0f;

}
void	BallFinalize()
{


	ModelRelease(g_Ball.Model);

}
void	BallUpdate()
{
	switch (g_Ball.State)
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
	//g_Ball.Velocity = XMFLOAT3(0, 0, 0);
	//if (Keyboard_IsKeyDown(KK_UP))
	//{
	//	g_Ball.Velocity.z = 1.0f / 60.0f;
	//}
	//if (Keyboard_IsKeyDown(KK_DOWN))
	//{
	//	g_Ball.Velocity.z = -1.0f / 60.0f;
	//}
	//if (Keyboard_IsKeyDown(KK_LEFT))
	//{
	//	g_Ball.Velocity.x = -1.0f / 60.0f;
	//}
	//if (Keyboard_IsKeyDown(KK_RIGHT))
	//{
	//	g_Ball.Velocity.x = 1.0f / 60.0f;
	//}
	//g_Ball.Position.x += g_Ball.Velocity.x;
	//g_Ball.Position.y += g_Ball.Velocity.y;
	//g_Ball.Position.z += g_Ball.Velocity.z;


}
void	BallDraw() 
{
	//ワールド行列作成
	XMMATRIX	scale = XMMatrixScaling(
		g_Ball.Scaling.x,
		g_Ball.Scaling.y,
		g_Ball.Scaling.z);
	XMMATRIX	rotation = XMMatrixRotationRollPitchYaw(
		g_Ball.Rotation.x,
		g_Ball.Rotation.y,
		g_Ball.Rotation.z);
	XMMATRIX	translation = XMMatrixTranslation(
		g_Ball.Position.x,
		g_Ball.Position.y,
		g_Ball.Position.z);
	XMMATRIX	world = scale * rotation * translation;

	//変換行列作成
	XMMATRIX	view = GetViewMatrix();
	XMMATRIX	projection = GetProjectionMatrix();
	XMMATRIX	wvp = world * view * projection;

	//シェーダーへ行列をセット
	Shader_SetWorldMatrix(world);
	Shader_SetMatrix(wvp);

	//モデルの描画リクエスト
	ModelDraw(g_Ball.Model);

}

XMFLOAT3 GetBallPosition()
{
	return g_Ball.Position;
}

void	Ball_Idle()
{

}

void	Ball_Move()
{
	g_Ball.Velocity.x += g_Ball.Acceleration.x;
	g_Ball.Velocity.y += g_Ball.Acceleration.y;
	g_Ball.Velocity.z += g_Ball.Acceleration.z;

	g_Ball.Position.x += g_Ball.Velocity.x;
	g_Ball.Position.y += g_Ball.Velocity.y;
	g_Ball.Position.z += g_Ball.Velocity.z;

	g_Ball.Velocity.x *= GENSUI;	// 速度を適当に減衰する
	//g_Ball.Velocity.y *= GENSUI;
	g_Ball.Velocity.z *= GENSUI;

	// 静止チェック
	float	len = (g_Ball.Velocity.x * g_Ball.Velocity.x +
		g_Ball.Velocity.y * g_Ball.Velocity.y +
		g_Ball.Velocity.z * g_Ball.Velocity.z);

	if (len <= STOP_VELO) // 静止とみなす速度
	{
		g_StopTime++;
		if (g_StopTime > 60.0f * 2) // 2秒間続いている
		{
			g_Ball.Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			g_Ball.State = BALL_STATE::BALL_STATE_DIRECTION;
			g_StopTime = 0.0f;
		}
	}

	float hit = BallFieldCollision();
}

void	Ball_Power()
{
	// 打ち出すスピードを決める
	float	power = BALL_SPEEDMAX * 0.12f; // とりあえず固定値

	g_Ball.Velocity.x *= power;
	g_Ball.Velocity.y *= power;
	g_Ball.Velocity.z *= power;

	g_Ball.State = BALL_STATE::BALL_STATE_MOVE;
}

void	Ball_Direction()
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

		g_Ball.Velocity = Direction;
		g_Ball.State = BALL_STATE::BALL_STATE_POWER;
	}



}


BALL* GetBall()
{
	return &g_Ball;
}




