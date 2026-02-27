//Gimmick.cpp

#include	"Manager.h"
#include	"sprite.h"
#include	"Gimmick.h"
#include	"keyboard.h"

#include	"player.h"
#include	"Block.h"
#include	"field.h"
#include	"Effect.h"
#include	"background.h"
#include	"score.h"
#include	"Audio.h"

#include	"Polygon3D.h"
#include	"Camera.h"

#include "Ball.h"

#include "billboard.h"

#include	"direct3d.h"//<<<<<<<<<<<<<<<<<<<

LIGHTOBJECT		Light3;//<<<<<<ライト管理オブジェクト

static ID3D11Device* g_pDevice_GM = NULL;
static ID3D11DeviceContext* g_pContext_GM = NULL;


static	int		g_BgmID = NULL;	//サウンド管理ID

void GIMMICK::Gimmick_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager)
{
	m_NowField = FIELD_NO::NO_2;

	m_SceneLoad.Load_Initialize(pDevice, pContext);

	;//３Dテスト初期化

	m_Manager = manager;
	g_pDevice_GM = pDevice;
	g_pContext_GM = pContext;


//ライト初期化
	XMFLOAT4	para;

	para = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);//環境光の色
	Light3.SetAmbient(para);

	para = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);//光の色
	Light3.SetDiffuse(para);

	para = XMFLOAT4(0.5f, -1.0f, 0.0f, 1.0f);//光方向
	float	len = sqrtf(para.x * para.x + para.y * para.y + para.z * para.z);
	para.x /= len;
	para.y /= len;
	para.z /= len;
	Light3.SetDirection(para);//光の方向（正規化済）

}


void GIMMICK::Gimmick_LoadUpdate()
{
	switch (m_SceneLoad.GetLoadCount())
	{
	case 0:
		m_Player.Player_Initialize(g_pDevice_GM, g_pContext_GM); // ボールの初期化
		break;
	case 1:
		Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
		break;
	case 2:
		m_Map.Field_Initialize(g_pDevice_GM, g_pContext_GM, m_NowField); // フィールドの初期化
		break;
	case 3:
		m_Background.Background_Initialize(g_pDevice_GM, g_pContext_GM, m_NowField);
		break;
	case 4:
		m_GimmickData.Gimmick_Data_Initialize(g_pDevice_GM, g_pContext_GM, m_NowField);
		break;
	case 5:
		m_EnemyNormal.EnemySpawner_Initialize(g_pDevice_GM, g_pContext_GM, m_NowField);
		break;
	case 6:
		m_bomb.Bomb_Initialize(g_pDevice_GM, g_pContext_GM, m_NowField);
		break;
	case 7:
		m_Weapon.Weapon_Initialize(g_pDevice_GM, g_pContext_GM);
		break;
	case 8:
		m_Goal.Goal_Initialize(g_pDevice_GM, g_pContext_GM, m_NowField);
		break;
	case 9:
		m_BillboardManager.Initialize(g_pDevice_GM, g_pContext_GM, m_NowField);
		break;
	case 10:
		m_PlayerUI.Initialize(g_pDevice_GM, g_pContext_GM, &m_Player);
		break;
	case 11:
		m_BombUI.Initialize(g_pDevice_GM, g_pContext_GM, &m_bomb);
		break;
	case 12:
		m_TargetUI.Initialize(g_pDevice_GM, g_pContext_GM);
		break;
	case 13:
		g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
		break;
	default:
		m_SceneLoad.SetLoadComplete(true);
		break;
	}

	m_SceneLoad.Load_Update();
}

void GIMMICK::Gimmick_Finalize()
{
	m_Map.Field_Finalize();	// フィールドの終了処理
	m_Background.Background_Finalize();
	m_GimmickData.Gimmick_Data_Finalize();
	m_Player.Player_Finalize();	// ボールの終了処理
	m_EnemyNormal.EnemySpawner_Finalize();
	m_bomb.Bomb_Finalize();
	m_Weapon.Weapon_Finalize();
	m_Goal.Goal_Finalize();

	m_SceneLoad.Load_Finalize();

	Camera_Finalize();	//カメラ終了処理

	m_BillboardManager.Finalize();
	m_PlayerUI.Finalize();
	m_BombUI.Finalize();
	m_TargetUI.Finalize();

	UnloadAudio(g_BgmID);//サウンドの解放
}

void GIMMICK::Gimmick_Update()
{
	if (m_SceneLoad.GetLoadActive()) {
		Gimmick_LoadUpdate();
	}
	else {
		//更新処理
		Camera_Update(m_Player.GetPlayerPosition());	//カメラ更新処理
		m_Player.Player_Update();
		m_EnemyNormal.EnemySpawner_Update(m_Player.GetPlayerPosition());
		m_bomb.Bomb_Update(m_Player.GetPlayerPosition(), m_Player.GetPlayerRotation());
		m_Map.Field_Update();
		m_Background.Background_Update();
		m_Goal.Goal_Update();
		collision.PlayerMovingFieldCollision(&m_Player, &m_GimmickData);
		collision.EnemyMovingFieldCollision(&m_EnemyNormal, &m_GimmickData);
		collision.BombMovingFieldCollision(&m_bomb, &m_GimmickData);

		collision.PlayerGimmickCollision(&m_Player, &m_GimmickData);
		collision.EnemyGimmickCollision(&m_EnemyNormal, &m_GimmickData);
		collision.BombGimmickCollision(&m_bomb, &m_GimmickData);

		m_GimmickData.Gimmick_Data_Update(m_Player.GetPlayerPosition(), m_Player.GetPlayerRotation());
		collision.PlayerGateCollision(&m_Player, &m_GimmickData);

		collision.BombGateCollision(&m_bomb, &m_GimmickData);

		m_Weapon.Weapon_Update(m_Player.GetPlayerPosition(), &m_EnemyNormal);

		m_PlayerUI.Update();
		m_BombUI.Update();
		m_TargetUI.Update();

		if (m_Player.GetPlayerState() == PLAYER_STATE::PLAYER_STATE_DEATH)
		{
			m_Manager->SetScene(SCENE_GAMEOVER);
			return; // ← ここで即座に抜ける：Finalize 後のアクセスを防止
		}

		// 以下はプレイヤー等のメンバにアクセスするコード
		collision.PlayerFieldCollision(&m_Player, &m_Map);
		collision.EnemyFieldCollision(&m_EnemyNormal, &m_Map);
		collision.PlayerEnemyCollision(&m_Player, &m_EnemyNormal);
		collision.PlayerBombCollision(&m_Player, &m_bomb);
		collision.BombFieldCollision(&m_bomb, &m_Map);
		collision.EXPLOSIONFieldCollision(&m_bomb, &m_Map);
		collision.BombEnemyCollision(&m_bomb, &m_EnemyNormal);
		collision.EXPLOSIONEnemyCollision(&m_bomb, &m_EnemyNormal);
		collision.WeaponFieldCollision(&m_Weapon, &m_Map);
		collision.PlayerWeaponCollision(&m_Player, &m_Weapon);

		//キー入力チェック
	//スタートボタンが押されたらシーンを切り替え
	//フェード処理中はキーを受け付けない
		if (Keyboard_IsKeyDownTrigger(KK_P))
		{
			m_Manager->SetScene(SCENE_PAUSE);
		}

		//Block_Update();
		//Effect_Update();
		//Score_Update();
		//Polygon3D_Update();



		if (Keyboard_IsKeyDownTrigger(KK_C))
		{
			if (m_Manager->GetClearCount() == 1)
			{
				m_Manager->IncrementClearCount();
			};

			m_Manager->SetScene(SCENE_STAGESELECTION);
		}

		// 追加：プレイヤーとゴールの当たり判定
		if (collision.PlayerGoalCollision(&m_Player, &m_Goal) == COLLISION_HIT::HIT_WALL_CREAR)
		{
			if (m_Manager->GetClearCount() == 1)
			{
				m_Manager->IncrementClearCount();
			}
			m_Manager->SetScene(SCENE_STAGESELECTION);
			return; // シーン遷移するので更新処理を止める
		}
	}

}

void GIMMICK::Gimmick_Draw()
{
	if (m_SceneLoad.GetLoadActive()) {
		m_SceneLoad.Load_Draw();
	}
	else {
		m_Background.Background_Draw();
		Light3.SetEnable(TRUE);			//ライティングON
		//Shader_SetLight(Light3.Light);	//ライト構造体をシェーダーへセット
		SetDepthTest(TRUE);

		Camera_Draw();		//Drawの最初で呼ぶ！

		m_Map.Field_Draw();
		m_GimmickData.Gimmick_Data_Draw();
		m_Player.Player_Draw(&m_BillboardManager);
		m_EnemyNormal.EnemySpawner_Draw();
		m_bomb.Bomb_Draw(&m_BillboardManager);
		m_Weapon.Weapon_Draw();
		m_Goal.Goal_Draw();
		//2D描画
		Light3.SetEnable(FALSE);			//ライティングOFF
		Shader_SetLight(Light3.Light);	//ライト構造体をシェーダーへセット

		m_BillboardManager.Draw();
		SetDepthTest(FALSE);
		m_PlayerUI.Draw();
		m_BombUI.Draw();
		m_TargetUI.Draw();

	}

}

void GIMMICK::Gimmick_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
	m_GimmickData.Gimmick_Data_Finalize();
	m_Map.Field_Finalize();	// フィールドの終了処理
	m_Background.Background_Finalize();
	m_Player.Player_Finalize();	// ボールの終了処理
	m_EnemyNormal.EnemySpawner_Finalize();
	m_bomb.Bomb_Finalize();
	m_Weapon.Weapon_Finalize();
	m_BillboardManager.Finalize();
	m_PlayerUI.Initialize(pDevice, pContext, &m_Player);
	m_BombUI.Initialize(pDevice, pContext, &m_bomb);
	m_TargetUI.Initialize(pDevice, pContext);
	m_Goal.Goal_Finalize();
	Camera_Finalize();	//カメラ終了処理



	m_Player.Player_Initialize(pDevice, pContext); // ボールの初期化
	Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
	m_Map.Field_Initialize(pDevice, pContext, no); // フィールドの初期化
	m_Background.Background_Initialize(pDevice, pContext, m_NowField);
	m_GimmickData.Gimmick_Data_Initialize(pDevice, pContext, no);
	m_EnemyNormal.EnemySpawner_Initialize(pDevice, pContext, no);
	m_bomb.Bomb_Initialize(pDevice, pContext, no);
	m_Weapon.Weapon_Initialize(pDevice, pContext);
	m_BillboardManager.Initialize(pDevice, pContext, m_NowField);
	m_PlayerUI.Initialize(pDevice, pContext, &m_Player);
	m_BombUI.Initialize(pDevice, pContext, &m_bomb);
	m_TargetUI.Initialize(pDevice, pContext);
	m_Goal.Goal_Initialize(pDevice, pContext, no);
}


