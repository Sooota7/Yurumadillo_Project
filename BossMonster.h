#pragma once

//Bossmonster.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"model.h"
using namespace DirectX;

//bossmonstar用アニメーション
#include "BossAnimation.h"

#define GENSUI (0.98f)
#define STOP_VELO (0.0002f)
#define BOSSMONSTER_HP (100.0f)			 //HP

// フェーズ等
#define BOSSMONSTER_IDLE_SECONDS (3)//攻撃待機
#define BOSSMONSTER_PHASE2_TIME (6)
#define BOSSMONSTER_DEATH_SECONDS (4.6)//撃破演出(合わせて調整)

#define BOSS_OBJECT_MAX 3

#define BOSS_START_POS_Y (5.0f)     //BOSSの初期位置Y
#define BOSS_START_POS_Z (50.0f)     //BOSSの初期位置Z

#include "enemySpawner.h" // 参照: 敵生成用
// forward declare BOMB to avoid heavy include in header
class BOMB;
#include "BossObj.h"      // 参照: ボス攻撃オブジェクト

// ステート
enum BOSSMONSTER_STATE
{
	BOSSMONSTER_STATE_IDLE = 0,
	BOSSMONSTER_STATE_PHASE1,
	BOSSMONSTER_STATE_PHASE2_1,
	BOSSMONSTER_STATE_PHASE2_2,
	BOSSMONSTER_STATE_PHASE3,
	BOSSMONSTER_STATE_DEATH,
	BOSSMONSTER_STATE_END,
	BOSSMONSTER_STATE_MAX
};

class BOSSMONSTER
{
private:
	XMFLOAT3	m_Position;
	XMFLOAT3	m_Rotation;
	XMFLOAT3	m_Scaling;
	XMFLOAT3	m_Velocity;
	XMFLOAT3	m_Acceleration;

	BOSSMONSTER_STATE	m_State;
	MODEL * m_Model[5];

	float m_Hp;

	BOSSANIMATION m_BossAnim;

private:
	void	Bossmonster_Idle();
	void    Bossmonster_Phase1();
	void    Bossmonster_Phase2_1();
	void    Bossmonster_Phase2_2();
	void    Bossmonster_Phase3();
	void    Bossmonster_Death();

	XMFLOAT3 m_LastPos;

	int m_IdleCounter;
	int deathCounter;
	bool m_Phase2_1Fired = false;
	bool m_Phase2_2Fired = false;

	// スポナー参照
	ENEMYSPAWNER* m_pSpawner = nullptr;

	// Bomb (RunBomb) 参照（フェーズ3で横一列に生成）
	BOMB* m_pBomb = nullptr;

	// フェーズ3: そのフェーズ内で一度だけ生成するためのフラグ。
	// フェーズから離れる（IDLE になる）際にリセットされ、次回 Phase3 に入るたびに再生成される。
	bool m_Phase3Fired = false;

	// BOSSOBJ
	BOSSOBJ m_BossObjs[BOSS_OBJECT_MAX];

public:
	void	Bossmonster_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Bossmonster_Finalize();
	void	Bossmonster_Update();
	void	Bossmonster_Draw();

	void SetBossmonsterPosition(XMFLOAT3 pos) { m_Position = pos; };
	XMFLOAT3 GetBossmonsterPosition() { return m_Position; };

	void SetBossmonsterRotation(XMFLOAT3 rotate) { m_Rotation = rotate; };
	XMFLOAT3 GetBossmonsterRotation() { return m_Rotation; };

	void SetBossmonsterScaling(XMFLOAT3 sca) { m_Scaling = sca; };
	XMFLOAT3 GetBossmonsterScaling() { return m_Scaling; };

	void SetBossmonsterVelocity(XMFLOAT3 vel) { m_Velocity = vel; };
	XMFLOAT3 GetBossmonsterVelocity() { return m_Velocity; };

	void SetBossmonsterHp(float hp ) { m_Hp = hp; };
	FLOAT GetBossmonsterHp() { return m_Hp; };

	BOSSMONSTER_STATE GetBossmonsterState() { return m_State; };

	BOSSMONSTER* GetBossmonster();

	BOSSOBJ* GetBossObjs() { return m_BossObjs; }

	// スポナーセット
	void SetEnemySpawner(ENEMYSPAWNER* spawner) { m_pSpawner = spawner; }

	// Bomb をセット（BOSS 側で m_bomb を渡す）
	void SetBomb(BOMB* bomb) { m_pBomb = bomb; }

	void BossHeadAnimation_Update(XMFLOAT3 playerPos) { m_BossAnim.BossAnimation_UpdateHead(playerPos); };
};








