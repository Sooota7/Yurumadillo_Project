#pragma once

//Ball.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"

#define BALL_SPEEDMAX (2.0f)
#define BALL_RADIUS (0.2f)
#define GENSUI (0.98f)
#define STOP_VELO (0.0002f)


//ボールの状態
enum BALL_STATE
{
	BALL_STATE_IDLE = 0,	//何もしない
	BALL_STATE_MOVE,		//移動
	BALL_STATE_DIRECTION,	//方向指示
	BALL_STATE_POWER,		//威力指示
};

//ボール構造体
class BALL
{
private:
	XMFLOAT3	m_Position;	//表示座標
	XMFLOAT3	m_Rotation;	//回転角
	XMFLOAT3	m_Scaling;	//拡大率
	XMFLOAT3	m_Velocity;	//速度
	XMFLOAT3	m_Acceleration;	// 落下速度

	BALL_STATE	m_State;		//状態
	MODEL* m_Model;		//モデルデータ

//	BOMB* m_Bomb;

private:
	void	Ball_Idle();
	void	Ball_Move();
	void	Ball_Power();
	void	Ball_Direction();

public:
	void	BallInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	BallFinalize();
	void	BallUpdate();
	void	BallDraw();

	void SetBallPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetBallPosition() { return m_Position; };

	void SetBallRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetBallRotation() { return m_Rotation; };

	void SetBallScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetBallScaling() { return m_Scaling; };

	void SetBallVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetBallVelocity() { return m_Velocity; };

	BALL* GetBall();
};







