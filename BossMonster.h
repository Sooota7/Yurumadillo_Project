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

// 攻撃待機時間
#define BOSSMONSTER_IDLE_SECONDS (3)

#define BOSSMONSTER_PHASE2_TIME (6) // フェーズ2_1継続時間（秒）

// 撃破演出　(いらなければ1～2に設定する(爆弾の爆発すら見えずに移行してしまうから))
#define BOSSMONSTER_DEATH_SECONDS (3)

#define BOSS_OBJECT_MAX 3

#include "enemySpawner.h" // 追加: スポーン用アクセス
#include "BossObj.h"      // 追加: ボス攻撃オブジェクト

//ボスのステート
enum BOSSMONSTER_STATE
{
	BOSSMONSTER_STATE_IDLE = 0,	//通常
	BOSSMONSTER_STATE_PHASE1,		//フェーズ1
	BOSSMONSTER_STATE_PHASE2_1,		//フェーズ2
	BOSSMONSTER_STATE_PHASE2_2,		//フェーズ2
	BOSSMONSTER_STATE_PHASE3,		//フェーズ3
	BOSSMONSTER_STATE_DEATH,		//死
	BOSSMONSTER_STATE_END,			//終了

	BOSSMONSTER_STATE_MAX
};



//ボスのクラス
class BOSSMONSTER
{
private:
	XMFLOAT3	m_Position;	//ポジション
	XMFLOAT3	m_Rotation;	//回転
	XMFLOAT3	m_Scaling;	//大きさ
	XMFLOAT3	m_Velocity;	//進行方向
	XMFLOAT3	m_Acceleration;	// 加速

	BOSSMONSTER_STATE	m_State;		//ステート
	MODEL * m_Model[5];		//モデルデータ

	 float m_Hp;

	 BOSSANIMATION m_BossAnim;		//アニメーションデータ

private:
	void	Bossmonster_Idle();
	void    Bossmonster_Phase1();
	void    Bossmonster_Phase2_1();
	void    Bossmonster_Phase2_2();
	void    Bossmonster_Phase3();
	void    Bossmonster_Death();

	
	
	XMFLOAT3 m_LastPos;

	// 追加: Idle 時のフレームカウンタ（60FPS 前提）
	int m_IdleCounter;
	 int deathCounter;
	bool m_Phase2_1Fired = false;
	bool m_Phase2_2Fired = false; // 後で使う


	// 追加: ENEMYSPAWNER への参照（フェーズで敵を生成するため）
	ENEMYSPAWNER* m_pSpawner = nullptr;
	// 追加: フェーズ1で一度だけスポーンするためのフラグ
	bool m_Phase1Spawned = false;

	// 追加: フェーズ2でスポーンする BOSSOBJ 配列
	BOSSOBJ m_BossObjs[BOSS_OBJECT_MAX];
	bool m_Phase2Spawned = false;

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



	// 追加: スポナーを設定
	void SetEnemySpawner(ENEMYSPAWNER* spawner) { m_pSpawner = spawner; }

	void BossHeadAnimation_Update(XMFLOAT3 playerPos) { m_BossAnim.BossAnimation_UpdateHead(playerPos); };// 追加: ボスの頭部アニメーションを更新する関数
};








