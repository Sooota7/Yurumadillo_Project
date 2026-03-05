#pragma once

//Player.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"parts.h"

#include	"billboardManager.h"
#include	"EffectDamage.h"
#include	"Audio.h"

#define PLAYER_SPEEDMAX (0.1f)      //
#define PLAYER_RADIUS (0.2f)
#define GENSUI (0.98f)
#define STOP_VELO (0.0002f)
#define PLAYER_ACCELERATION (0.01f)  //加速度
#define PLAYER_GRAVITY (0.01f)       //重力
#define PLAYER_JUMP (0.25f)           //ジャンプ力
#define PLAYER_FALLMAX (-0.2f)        //落下最高速度
#define PLAYER_RESPAWN (-3.0f)         //これ以上下に行くと死
#define PLAYER_MAGICRANGE (0.03f)    //魔法範囲
#define PLAYER_HP (5.0f)			 //HP
#define PLAYER_BALLOON_SPEED (0.1)        //風船の上昇速度
#define PLAYER_BALLOON_FALLSPEED (-0.05f)   //風船の下降速度
#define PLAYER_START_POS_X (8.0f)     //プレイヤーの初期位置X
#define PLAYER_START_POS_Y (5.0f)     //プレイヤーの初期位置Y
#define PLAYER_START_POS_Z (0.0f)     //プレイヤーの初期位置Z

//プレイヤーのステート
enum PLAYER_STATE
{
	PLAYER_STATE_IDLE = 0,	//通常
	PLAYER_STATE_MOVE,		//ムーブ
	PLAYER_STATE_JUMP,		//ジャンプ
	PLAYER_STATE_BALLOON,	//風船持ってる
	PLAYER_STATE_RESPAWN,   //リスポーン
	PLAYER_STATE_DEATH,		//死
	PLAYER_STATE_ATTACK_AFTER,		//攻撃後
	PLAYER_STATE_MAX
};

enum PLAYER_PARTS
{
	PARTS_HEAD,
	PARTS_BODY,
	PARTS_ARM_RIGHT,
	PARTS_ARM_LEFT,
	PARTS_LEG_RIGHT,
	PARTS_LEG_LEFT,
	PARTS_MAX
};

class PLAYER_ANIM
{
public:
	ANIM anim[PARTS_MAX];
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
	PARTS m_Parts[PARTS_MAX];					//モデル(必要な数)
	PLAYER_ANIM m_anim[PLAYER_STATE_MAX];
	MODEL* m_ModelData[PARTS_MAX];

	FIELD_NO m_No;

	bool BombHave;

	bool TransBombFlag;
	 bool JumpCount;
	 float m_Hp;
	 bool BalloonFlag;
	 bool BalloomUp;
	 bool BalloomNow;

	 bool isDamage;			// プレイヤーステートにDAMAGEを作ってもいい
	 int DamageCount;
	 EffectDamage m_EDamage;
	 int rBno;
	 float cnt;

	 // SE関連
	 int m_BombFireSE_ID;		// 爆弾を持っているときのSE
	 bool m_IsBombFireSEPlaying;	// SE再生中フラグ
	 int m_BombChangeSE_ID;		// 爆弾を持った瞬間のSE (change1.wav)
	 int m_DamageSE_ID;			// ダメージを受けたときのSE (damage.wav)
	 bool m_PreviousBombHave;		// 前フレームの爆弾所持状態
private:
	void	Player_Idle();
	void	Player_Move();
	void    Player_Jump();
	void    Player_Balloon();
	void    Player_Respawn();
	void    Player_Death();
	void    Player_Attack_After();

	void	Player_SetParts();
	
	XMFLOAT3	Player_AnimPos(PLAYER_STATE state,PLAYER_PARTS part,PARTS* parts, XMFLOAT3 rot,int frame);
	XMFLOAT3	Player_AnimRot(PLAYER_STATE state,PLAYER_PARTS part,PARTS* parts, XMFLOAT3 rot,int frame);
	void		Player_SetAnimInis();

	void	Player_SetAnim();
	void	Player_SetAnimIdle();
	void	Player_SetAnimMove();
	void	Player_SetAnimJunp();
	void	Player_SetAnimHave_MAX();
	void	Player_SetAnimBaloon();
	void	Player_SetAnimRespawn();
	void	Player_SetAnimDeath();
	void	Player_SetAnimHokan(PLAYER_STATE parts);

	//使うならセッターゲッター用意
	float	m_flame;

	XMFLOAT3 m_LastPos;

	int		m_AttackAfterCounter; // 追加: 攻撃後の強制移動をフレームで管理するカウンタ

public:
	void	Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no=FIELD_NO::NO_NONE);
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

	void SetPlayerBombHave(bool have) { BombHave = have; };
	bool GetPlayerBombave() { return BombHave; };

	void SetPlayerTransBombFlag(bool have) { TransBombFlag = have; };
	bool GetPlayerTransBombFlag() { return TransBombFlag; };

	void SetIsDamage(bool set) { isDamage = set; };

	// ダメージSEを再生する関数
	void PlayDamageSE();

	PLAYER_STATE GetPlayerState() { return m_State; };
	// 追加：ステートを外部から設定できるようにする
	void SetPlayerState(PLAYER_STATE state) { m_State = state; };

	PLAYER* GetPlayer();
};