#pragma once

//Player.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"
#include    "bombSource.h"

#define PLAYER_SPEEDMAX (0.05f)      //最高速度
#define PLAYER_RADIUS (0.2f)
#define GENSUI (0.98f)
#define STOP_VELO (0.0002f)
#define PLAYER_ACCELERATION (0.01f)  //加速度
#define PLAYER_GRAVITY (0.01f)       //重力
<<<<<<< HEAD
#define PLAYER_JUMP (0.3f)           //ジャンプ力
=======
#define PLAYER_FALLMAX (-0.2f)        //落下最高速度
#define PLAYER_JUMP (0.2f)           //ジャンプ力
>>>>>>> e5e005faf78d341748218fb44b15d22f971cf71d
#define PLAYER_DEATH (-3.0f)         //これ以上下に行くと死
#define PLAYER_MAGICRANGE (0.03f)    //魔法範囲

//ボールの状態
enum PLAYER_STATE
{
	PLAYER_STATE_IDLE = 0,	//何もしない
	PLAYER_STATE_MOVE,		//移動
	PLAYER_STATE_JUMP,
	PLAYER_STATE_RESPAWN,   //リスポーン
	
};

//ボール構造体
class PLAYER
{
private:
	XMFLOAT3	m_Position;	//表示座標
	XMFLOAT3	m_Rotation;	//回転角
	XMFLOAT3	m_Scaling;	//拡大率
	XMFLOAT3	m_Velocity;	//速度
	XMFLOAT3	m_Acceleration;	// 落下速度

	PLAYER_STATE	m_State;		//状態
	MODEL* m_Model[2];		//モデルデータ

	 BOMBSOURCE* m_Bomb;

	 bool JumpCount;

private:
	void	Player_Idle();
	void	Player_Move();
	void    Player_Jump();
	void    Player_BombMagic(BOMBSOURCE* pBomb);
	void    Player_Respawn();

public:
	void	Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Player_Finalize();
	void	Player_Update();
	void	Player_Draw();

	void SetPlayerPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetPlayerPosition() { return m_Position; };

	void SetPlayerRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetPlayerRotation() { return m_Rotation; };

	void SetPlayerScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetPlayerScaling() { return m_Scaling; };

	void SetPlayerVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetPlayerVelocity() { return m_Velocity; };

	void SetPlayerJump(bool jump) { JumpCount = jump; };
	BOOL GetPlayerJump() { return JumpCount; };

	PLAYER* GetPlayer();
};







