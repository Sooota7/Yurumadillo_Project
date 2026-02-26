//Bossmonster.cpp

#include	"keyboard.h"
#include	"BossMonster.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"
#include	"Audio.h"

#include	"billboard.h"
#include "enemySpawner.h"
#include "BossObj.h"
#include "bomb.h"               // 追加: RunBomb を生成するため
#include "RunBombSource.h"      // RUNBOMBSOURCE のアクセス

//ボールオブジェクト

ID3D11Device* g_pDeviceBoss;
ID3D11DeviceContext* g_pContextBoss;

static ID3D11ShaderResourceView* g_Texture = NULL;

// SEの静的メンバ初期化
int BOSSMONSTER::m_AttackSE_ID = -1;
bool BOSSMONSTER::m_SEInitialized = false;

void	BOSSMONSTER::Bossmonster_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDeviceBoss = pDevice;
	g_pContextBoss = pContext;

	float downSize = 10.0f;

	m_Position = XMFLOAT3(FIELD_WIDTH_X / 2, 7.0f, FIELD_WIDTH_Z/4*3);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);


	m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE;

	m_LastPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Hp = BOSSMONSTER_HP;

	// Idle カウンタ初期化（追加）
	m_IdleCounter = 0;

	// フェーズフラグ初期化
	m_Phase2_1Fired = false;
	m_Phase2_2Fired = false;
	m_Phase3Fired = false;

	//テクスチャ画像読み込み
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\diamond.png",
		WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(g_pDeviceBoss, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		m_BossObjs[i].BossObj_Initialize(pDevice, pContext);
		m_BossObjs[i].SetActive(false);
	}

	m_BossAnim.BossAnimation_Initialize(pDevice, pContext);

	// SEの初期化（最初のインスタンスでのみ実行）
	if (!m_SEInitialized)
	{
		m_AttackSE_ID = LoadAudio("asset\\Audio\\SE\\attack.wav");

		SetAudioVolume(m_AttackSE_ID, 0.7f);
		m_SEInitialized = true;
	}
}
void	BOSSMONSTER::Bossmonster_Finalize()
{
	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		m_BossObjs[i].BossObj_Finalize();
	}

	m_BossAnim.BossAnimation_Finalize();

	// SE解放
	if (m_SEInitialized && m_AttackSE_ID != -1)
	{
		UnloadAudio(m_AttackSE_ID);
		m_AttackSE_ID = -1;
		m_SEInitialized = false;
	}
}


void	BOSSMONSTER::Bossmonster_Update()
{
	// アニメーション状態更新
	switch (m_State)
	{
	case BOSSMONSTER_STATE_IDLE:
		m_BossAnim.SetBossAnimState(BOSS_ANIMATION_STATE::BOSS_STATE_IDLE);
		break;
	case BOSSMONSTER_STATE_PHASE1:
		m_BossAnim.SetBossAnimState(BOSS_ANIMATION_STATE::BOSS_STATE_PHASE01);
		break;
	case BOSSMONSTER_STATE_PHASE2_1:
		m_BossAnim.SetBossAnimState(BOSS_ANIMATION_STATE::BOSS_STATE_PHASE02_01);
		break;
	case BOSSMONSTER_STATE_PHASE2_2:
		m_BossAnim.SetBossAnimState(BOSS_ANIMATION_STATE::BOSS_STATE_PHASE02_02);
		break;
	case BOSSMONSTER_STATE_PHASE3:
		m_BossAnim.SetBossAnimState(BOSS_ANIMATION_STATE::BOSS_STATE_PHASE03);
		break;
	case BOSSMONSTER_STATE_DEATH:
		m_BossAnim.SetBossAnimState(BOSS_ANIMATION_STATE::BOSS_STATE_DEATH);
		break;
	default:
		break;
	}

	m_BossAnim.BossAnimation_Update(m_Position, m_Rotation);

	switch (m_State)
	{
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE:
		Bossmonster_Idle();
		break;
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE1:
		Bossmonster_Phase1();
		break;
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_1:
		Bossmonster_Phase2_1();
		break;
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_2:
		Bossmonster_Phase2_2();
		break;
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE3:
		Bossmonster_Phase3();
		break;
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_DEATH:
		Bossmonster_Death();
		break;

	}
	if (Keyboard_IsKeyDownTrigger(KK_ENTER))
	{
		m_Hp -= 40.0f;
	}

	if (m_Hp <= 0.0f && m_State != BOSSMONSTER_STATE::BOSSMONSTER_STATE_DEATH
		&& m_State != BOSSMONSTER_STATE::BOSSMONSTER_STATE_END)
	{
		m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_DEATH;
		deathCounter = 0;
	}


	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		m_BossObjs[i].BossObj_Update();
	}

}
void	BOSSMONSTER::Bossmonster_Draw()
{
	// モデル（アニメ）描画
	switch (m_State)
	{
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE:
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE1:
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_1:
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_2:
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE3:
	case BOSSMONSTER_STATE::BOSSMONSTER_STATE_DEATH:
		m_BossAnim.BossAnimation_Draw();
		break;
	default:
		break;
	}

	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		m_BossObjs[i].BossObj_Draw();
	}

	{
		XMFLOAT3 pos = m_Position;
		pos.y += 1.0f;
		XMFLOAT2 size = XMFLOAT2(0.5f, 0.5f);
		XMFLOAT4 col = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		int bno = 1;
		int wc = 1;
		int hc = 1;

		Billboard* bb = new Billboard(pos, size, col, bno, wc, hc, BILLBOARD_TEXTURE::TEST);
	}
}


void	BOSSMONSTER::Bossmonster_Idle()
{
	m_IdleCounter++;

	if (m_IdleCounter >= (BOSSMONSTER_IDLE_SECONDS * 60))
	{
		m_IdleCounter = 0;
		if (m_Hp > BOSSMONSTER_HP / 3 * 2)
		{
			// 次回 Phase3 入るときは再生成させたい -> フラグをリセット
			m_Phase3Fired = false;
			m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE1;
		}
		else if (m_Hp > BOSSMONSTER_HP / 3)
		{
			m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_1;
		}
		else
		{
			// 次回 Phase3 入るときは再生成させたい -> フラグをリセット
			m_Phase3Fired = false;
			m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE3;
		}
	}
}

void BOSSMONSTER::Bossmonster_Phase1()
{
	if (m_pSpawner != nullptr)
	{
		// 攻撃SE再生
		if (m_AttackSE_ID != -1)
		{
			PlayAudio(m_AttackSE_ID, false);
		}

		int spawned = 0;

		XMFLOAT3 spawnPos[2];
		spawnPos[0] = XMFLOAT3(m_Position.x - 2.0f, 2.0f, FIELD_WIDTH_Z / 2);
		spawnPos[1] = XMFLOAT3(m_Position.x + 2.0f, 2.0f, FIELD_WIDTH_Z / 2);

		for (int s = 0; s < 2; ++s)
		{
			if (m_pSpawner->EnemySpawner_SpawnButterfly(spawnPos[s]))
			{
				spawned++;
			}
		}

		m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE;
	}
}

// フェーズ2は既存ロジック（省略）
// フェーズ2実装はそのまま（省略）
void BOSSMONSTER::Bossmonster_Phase2_1()
{
	// まだ撃ってない時だけ弾生成
	if (!m_Phase2_1Fired)
	{
		// 攻撃SE再生
		if (m_AttackSE_ID != -1)
		{
			PlayAudio(m_AttackSE_ID, false);
		}

		int bulletsToSpawn = 5;
		float spread = 8.0f;//オブジェクトどうしの幅？

		for (int i = 0; i < BOSS_OBJECT_MAX && bulletsToSpawn > 0; i++)
		{
			if (!m_BossObjs[i].IsActive())
			{
				XMFLOAT3 spawnPos = m_Position;
				spawnPos.y = 1.0f;
				spawnPos.z -= BOSS_PHASE2_BACKRANGE;
				spawnPos.x += (bulletsToSpawn - 4) * spread;

				m_BossObjs[i].SetBossObjPosition(spawnPos);
				m_BossObjs[i].SetBossObjVelocity(XMFLOAT3(0.0f, 0.0f, 0.50f));
				m_BossObjs[i].SetActive(true);

				bulletsToSpawn--;
			}
		}

		m_Phase2_1Fired = true; // ★もう撃たない
	}

	// ★ 弾が全部消えたかチェック
	bool anyActive = false;
	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		if (m_BossObjs[i].IsActive())
		{
			anyActive = true;
			break;
		}
	}

	// 全弾消えたらフェーズ2_2へ
	if (!anyActive)
	{
		m_Phase2_interval++;
		if(m_Phase2_interval>= (BOSSMONSTER_PHASE2_INTERVAL * 60))
		{
			m_Phase2_interval = 0;
			m_Phase2_1Fired = false;              // 次回用リセット
			m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_2; // フェーズ移行！
		}
	}
}

void BOSSMONSTER::Bossmonster_Phase2_2()
{
	if (!m_Phase2_2Fired)
	{
		// 攻撃SE再生
		if (m_AttackSE_ID != -1)
		{
			PlayAudio(m_AttackSE_ID, false);
		}

		int bulletsToSpawn = 5;
		float spread = 8.0f;

		for (int i = 0; i < BOSS_OBJECT_MAX && bulletsToSpawn > 0; i++)
		{
			if (!m_BossObjs[i].IsActive())
			{
				XMFLOAT3 spawnPos = m_Position;
				spawnPos.y = 1.0f;
				spawnPos.z -= 10.0f;
				spawnPos.x += (bulletsToSpawn - 4) * spread;

				m_BossObjs[i].SetBossObjPosition(spawnPos);
				m_BossObjs[i].SetBossObjVelocity(XMFLOAT3(0.0f, 0.0f, -0.50f));
				m_BossObjs[i].SetActive(true);

				bulletsToSpawn--;
			}
		}

		m_Phase2_2Fired = true;
	}

	// 弾が全部消えたら Idle に戻すなど
	bool anyActive = false;
	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		if (m_BossObjs[i].IsActive())
		{
			anyActive = true;
			break;
		}
	}

	if (!anyActive)
	{
		m_Phase2_2Fired = false;
		m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE;
	}
}

void BOSSMONSTER::Bossmonster_Phase3()
{
	// フェーズ3 で RunBomb（走る爆弾）を横一列に生成する
	// 同フェーズ内で何度も呼ばれるのを防ぐフラグチェック
	if (m_Phase3Fired) return;

	// 攻撃SE再生
	if (m_AttackSE_ID != -1)
	{
		PlayAudio(m_AttackSE_ID, false);
	}

	if (m_pBomb == nullptr) return; // Bomb 管理クラスがないと生成できない

	// RunBomb 配列を取得
	RUNBOMBSPAWNER* runArr = m_pBomb->Bomb_GetRunBomb();
	if (runArr == nullptr) return;

	const int spawnCount = 20;         // 横一列に出す数
	const float spacing = 1.1f;       // X 軸間隔（調整可）
	const float zOffset = -40.0f;     // ボスからの Z オフセット
	const float yPos = 1.0f;          // Y 座標（地面高に合わせる）

	// 開始 X を中央揃えで計算
	float startX = m_Position.x - ((spawnCount - 1) * spacing) * 0.5f;
	int spawned = 0;

	for (int i = 0; i < BOMB_NUM_MAX && spawned < spawnCount; ++i)
	{
		// spawner が使えるか（地形配置によっては GetUse() が false の場合がある）
		// ここでは state が NONE / COOL / ITEM のものを再利用する
		RUNBOMBSOURCE* src = runArr[i].GetRunBombSource__RunBombSpawner();
		if (src == nullptr) continue;

		RUNBOMB_STATE st = src->Runbombsource_GetState();
		if (st == RUNBOMB_NONE || st == RUNBOMB_COOL || st == RUNBOMB_ITEM)
		{
			// 生成位置を計算（左→右）
			XMFLOAT3 pos = XMFLOAT3(startX + spawned * spacing, yPos, FIELD_WIDTH_Z / 2);

			// 敵走る爆弾として初期化（横方向に進むタイプを使う -> RIGHT）
			src->Runbombsource_Initialize(pos, RUNBOMB_ENEMY, RUNBOMB_TYPE_DOWN);

			// spawner を有効にしておく（既に true の可能性あり）
			runArr[i].SetUse(true);

			// 重要: spawner の m_Active を true にして Update_RunBombSpawner が
			// Runbombsource_Enemy を呼ぶようにする
			runArr[i].SetActive(true);
			// クールタイムもリセットしておく（安全）
			runArr[i].ResetRCoolTime();

			spawned++;
		}
	}

	// 同フェーズ内の二重生成を防ぐ
	m_Phase3Fired = true;

	// フェーズ終了または Idle に戻すなど挙動を追加（必要ならここで状態遷移）
	m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE;
}

void BOSSMONSTER::Bossmonster_Death()
{
	deathCounter++;
	if (deathCounter >= (BOSSMONSTER_DEATH_SECONDS * 60))
	{
		deathCounter = 0;
		m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_END;
	}
}


