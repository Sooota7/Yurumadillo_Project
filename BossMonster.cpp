//Bossmonster.cpp

#include	"keyboard.h"
#include	"BossMonster.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"

#include	"billboard.h"
#include "enemySpawner.h" // 追加: Enemy 配列操作のため
#include "BossObj.h"      // ボス攻撃オブジェクト

//ボールオブジェクト

ID3D11Device* g_pDeviceBoss;
ID3D11DeviceContext* g_pContextBoss;

static ID3D11ShaderResourceView* g_Texture = NULL;

void	BOSSMONSTER::Bossmonster_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDeviceBoss = pDevice;
	g_pContextBoss = pContext;

	float downSize = 10.0f;

	m_Position = XMFLOAT3(7.0f, 10.0f, 100.0f);//ボスの位置　モデルにあわせて調整
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	m_Scaling = XMFLOAT3(100.0f, 100.0f, 100.0f);//ボスの大きさ　モデルにあわせて調整	


	m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE;

	m_LastPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Hp = BOSSMONSTER_HP;

	// Idle カウンタ初期化（追加）
	m_IdleCounter = 0;

	// フェーズフラグ初期化
	m_Phase1Spawned = false;
	m_Phase2Spawned = false;

	

	//テクスチャ画像読み込み
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\diamond.png",
		WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
	CreateShaderResourceView(g_pDeviceBoss, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);//読み込み失敗時にダイアログを表示

	m_Model[1] = ModelLoad("asset\\model\\test.fbx");
	m_Model[2] = ModelLoad("asset\\model\\test.fbx");
	m_Model[3] = ModelLoad("asset\\model\\test.fbx");
	m_Model[4] = ModelLoad("asset\\model\\test.fbx");

	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		m_BossObjs[i].BossObj_Initialize(pDevice, pContext);
		m_BossObjs[i].SetActive(false);
	}
	
}
void	BOSSMONSTER::Bossmonster_Finalize()
{
	for (int i = 0; i < 4; i++) {
		ModelRelease(m_Model[i+1]);
	}

	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		m_BossObjs[i].BossObj_Finalize();
	}
}


void	BOSSMONSTER::Bossmonster_Update()
{
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
		deathCounter = 0;   // ここでリセットすると完璧
	}


	for (int i = 0; i < BOSS_OBJECT_MAX; i++)
	{
		m_BossObjs[i].BossObj_Update();
	}

}
void	BOSSMONSTER::Bossmonster_Draw()
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
		switch (m_State)
		{
		case BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE:
			ModelDraw(m_Model[1]);
			break;
		case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE1:
			ModelDraw(m_Model[2]);
			break;
		case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_1:
			ModelDraw(m_Model[3]);
			break;
		case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_2:
			ModelDraw(m_Model[3]);
			break;
		case BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE3:
			ModelDraw(m_Model[4]);
			break;
		case BOSSMONSTER_STATE::BOSSMONSTER_STATE_DEATH:
			//ModelDraw(m_Model[4]);
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
	// 毎フレーム呼ばれる想定なのでフレームカウントで秒数を測る（60FPS 前提）
	m_IdleCounter++;

	// 指定秒数を過ぎたら次のステートへ
	if (m_IdleCounter >= (BOSSMONSTER_IDLE_SECONDS * 60))
	{
		m_IdleCounter = 0;
		if (m_Hp > BOSSMONSTER_HP / 3 * 2) //HP が 2/3 より大きいならフェーズ1へ
		{
			m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE1;
		}
		else if (m_Hp > BOSSMONSTER_HP / 3) //HP が 1/3 より大きいならフェーズ2へ
		{
			m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_1;
		}
		else                                //HP が 1/3 以下ならフェーズ3へ
		{
			m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE3;
		}
	}
}

void BOSSMONSTER::Bossmonster_Phase1()
{
	// フェーズ1開始時に敵をスポーン（ENEMYSPAWNER に空きがあれば）
	if ( m_pSpawner != nullptr)
	{
		ENEMY_BUTTERFLY* arr = m_pSpawner->EnemySpawner_GetEnemyButterfly();
		int spawned = 0;

		// 指定場所（例: ボスの左右に出現）
		XMFLOAT3 spawnPos[2];
		spawnPos[0] = XMFLOAT3(m_Position.x - 2.0f, 2.0f, m_Position.z - 80.0f);
		spawnPos[1] = XMFLOAT3(m_Position.x + 2.0f, 2.0f, m_Position.z - 80.0f);

		for (int s = 0; s < 2; ++s)
		{
			for (int i = 0; i < Enemy_Spawner_MAX; ++i)
			{
				if (arr[i].GetEnemyButterflyType() == ENEMY_TYPE::ENEMY_TYPE_NONE)
				{
					arr[i].SetEnemyPosition(spawnPos[s]);
					arr[i].SetEnemyButterflyType(ENEMY_TYPE::ENEMY_TYPE_BUTTERFLY);
					// 必要なら初期化を呼ぶ（EnemySpawner_Initialize ですでに Initialize されている前提）
					spawned++;
					break;
				}
			}
		}

		m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_IDLE;
	}

	// フェーズ1中の挙動（未実装） — 必要ならここに行動を追加
}

// フェーズ2実装はそのまま（省略）
void BOSSMONSTER::Bossmonster_Phase2_1()
{
	// まだ撃ってない時だけ弾生成
	if (!m_Phase2_1Fired)
	{
		int bulletsToSpawn = 5;
		float spread = 8.0f;//オブジェクトどうしの幅？

		for (int i = 0; i < BOSS_OBJECT_MAX && bulletsToSpawn > 0; i++)
		{
			if (!m_BossObjs[i].IsActive())
			{
				XMFLOAT3 spawnPos = m_Position;
				spawnPos.y = 1.0f;
				spawnPos.z -= 150.0f;
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
		m_Phase2_1Fired = false;              // 次回用リセット
		m_State = BOSSMONSTER_STATE::BOSSMONSTER_STATE_PHASE2_2; // フェーズ移行！
	}
}

void BOSSMONSTER::Bossmonster_Phase2_2()
{
	if (!m_Phase2_2Fired)
	{
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


