#pragma once

//Player.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"

#include	"billboardManager.h"

#define PLAYER_SPEEDMAX (0.05f)      //
#define PLAYER_RADIUS (0.2f)
#define GENSUI (0.98f)
#define STOP_VELO (0.0002f)
#define PLAYER_ACCELERATION (0.01f)  //加速度
#define PLAYER_GRAVITY (0.01f)       //重力
#define PLAYER_JUMP (0.25f)           //ジャンプ力
#define PLAYER_FALLMAX (-0.2f)        //落下最高速度
#define PLAYER_RESPAWN (-3.0f)         //これ以上下に行くと死
#define PLAYER_MAGICRANGE (0.03f)    //魔法範囲
#define PLAYER_HP (100.0f)			 //HP
#define PLAYER_BALLOON_SPEED (0.1)        //風船の上昇速度
#define PLAYER_BALLOON_FALLSPEED (-0.05f)   //風船の下降速度

//プレイヤーのステート
enum PLAYER_STATE
{
	PLAYER_STATE_IDLE = 0,	//通常
	PLAYER_STATE_MOVE,		//ムーブ
	PLAYER_STATE_JUMP,		//ジャンプ
	PLAYER_STATE_BALLOON,	//風船持ってる
	PLAYER_STATE_RESPAWN,   //リスポーン
	PLAYER_STATE_DEATH,		//死
};

//プレイヤーのクラス
class PLAYER
{
private:
	XMFLOAT3	m_Position;	//ポジション
	XMFLOAT3	m_Rotation;	//回転
	XMFLOAT3	m_Scaling;	//大きさ
	XMFLOAT3	m_Velocity;	//進行方向
	XMFLOAT3	m_Acceleration;	// 加速

	PLAYER_STATE	m_State;		//ステート
	MODEL* m_Model[2];		//モデル


	 bool JumpCount;
	 float m_Hp;
	 bool BalloonFlag;
	 bool BalloomUp;
	 bool BalloomNow;

private:
	void	Player_Idle();
	void	Player_Move();
	void    Player_Jump();
	void    Player_Balloon();
	void    Player_Respawn();
	void    Player_Death();

public:
	void	Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Player_Finalize();
	void	Player_Update();
	void	Player_Draw(BillboardManager* billboardManager);

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

	void SetPlayerBalloon(bool balloon) { BalloonFlag = balloon; };
	BOOL GetPlayerBallon() { return BalloonFlag; };

	void SetPlayerHp(float hp ) { m_Hp = hp; };
	FLOAT GetPlayerHp() { return m_Hp; };

	PLAYER_STATE GetPlayerState() { return m_State; };

	PLAYER* GetPlayer();
};







