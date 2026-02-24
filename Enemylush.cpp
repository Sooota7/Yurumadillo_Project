
//Enemylush.cpp

#include	"Manager.h"
#include	"sprite.h"
#include	"Enemylush.h"
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

#include <fstream>


LIGHTOBJECT		Light5;//<<<<<<ライト管理オブジェクト

static ID3D11Device* g_pDevice_EL = NULL;
static ID3D11DeviceContext* g_pContext_EL = NULL;



static	int		g_BgmID = NULL;	//サウンド管理ID

void ENEMYLUSH::Enemylush_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager)
{
	m_NowField = FIELD_NO::NO_ENEMYLUSH;
	
	g_pDevice_EL = pDevice;
	g_pContext_EL = pContext;

	m_Player.Player_Initialize(pDevice, pContext); // ボールの初期化
	Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
	m_Map.Field_Initialize(pDevice, pContext, m_NowField); // フィールドの初期化
	m_Background.Background_Initialize(pDevice, pContext);

	Enemylush_Phase_Initialize();

	m_EnemyNormal.EnemySpawner_Initialize(pDevice, pContext, m_NowField);
	m_bomb.Bomb_Initialize(pDevice, pContext, m_NowField);
	m_Weapon.Weapon_Initialize(pDevice, pContext);

	m_BillboardManager.Initialize(pDevice, pContext, m_NowField);
	m_PlayerUI.Initialize(pDevice, pContext, &m_Player);
	m_BombUI.Initialize(pDevice, pContext, &m_bomb);
	m_TargetUI.Initialize(pDevice, pContext);

	m_GimmickData.Gimmick_Data_Initialize(pDevice, pContext, m_NowField);

	//Player_Initialize(pDevice, pContext); // ポリゴンの初期化
	//Block_Initialize(pDevice, pContext);//ブロックの初期化
	//Effect_Initialize(pDevice, pContext);//エフェクト初期化
	//Score_Initialize(pDevice, pContext);//スコア初期化

	//Polygon3D_Initialize(pDevice, pContext);//３Dテスト初期化

	m_Manager = manager;
	m_SpawnTime = 0;
	
	g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
	//PlayAudio(g_BgmID, true);	//再生開始（ループあり）
	//PlayAudio(g_BgmID);			//再生開始（ループなし）
	//PlayAudio(g_BgmID, false);	//再生開始（ループなし）

	//ライト初期化
	XMFLOAT4	para;

	para = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);//環境光の色
	Light5.SetAmbient(para);

	para = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);//光の色
	Light5.SetDiffuse(para);

	para = XMFLOAT4(0.5f, -1.0f, 0.0f, 1.0f);//光方向
	float	len = sqrtf(para.x * para.x + para.y * para.y + para.z * para.z);
	para.x /= len;
	para.y /= len;
	para.z /= len;
	Light5.SetDirection(para);//光の方向（正規化済）

	m_TotalSpawn = 0;
	for (int q = 0; q < FIELD_HEIGHT_Y; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{

				switch (CheckMap(l, i, q, m_NowField))
				{
				case 0:
					break;
				case S:
					m_SpawnPos[m_TotalSpawn] = XMFLOAT3(l, q, i);
					m_TotalSpawn++;
					break;
				default:
					break;
				}


			}
		}
	}


}

void ENEMYLUSH::Enemylush_Finalize()
{
	m_Map.Field_Finalize();	// フィールドの終了処理
	m_Background.Background_Finalize();
	m_Player.Player_Finalize();	// ボールの終了処理
	m_EnemyNormal.EnemySpawner_Finalize();
	m_bomb.Bomb_Finalize();
	m_Weapon.Weapon_Finalize();

	m_GimmickData.Gimmick_Data_Finalize();

	//Block_Finalize();
	//Effect_Finalize();
	//Score_Finalize();
	//Polygon3D_Finalize();
	Camera_Finalize();	//カメラ終了処理

	m_BillboardManager.Finalize();
	m_PlayerUI.Finalize();
	m_BombUI.Finalize();
	m_TargetUI.Finalize();

	UnloadAudio(g_BgmID);//サウンドの解放
}

void ENEMYLUSH::Enemylush_Update()
{
	//更新処理
	Camera_Update(m_Player.GetPlayerPosition());	//カメラ更新処理
	m_Player.Player_Update();
	Enemylush_Phase_Update();
	m_EnemyNormal.EnemySpawner_Update(m_Player.GetPlayerPosition());
	m_Map.Field_Update();
	m_Background.Background_Update();

	m_bomb.Bomb_Update(m_Player.GetPlayerPosition(), m_Player.GetPlayerRotation());
	m_Weapon.Weapon_Update(m_Player.GetPlayerPosition(), &m_EnemyNormal);

	m_GimmickData.Gimmick_Data_Update(m_Player.GetPlayerPosition(),m_Player.GetPlayerRotation());

	m_PlayerUI.Update();
	m_BombUI.Update();
	m_TargetUI.Update();

	if (collision.PlayerFieldCollision(&m_Player, &m_Map) == COLLISION_HIT::HIT_WALL_CREAR)
	{
		if (m_NowField == FIELD_NO::NO_1)
		{
			Enemylush_SetNextMap(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), FIELD_NO::NO_2);
			m_NowField = FIELD_NO::NO_2;
		}
		else if (m_NowField == FIELD_NO::NO_2)
		{
			Enemylush_SetNextMap(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), FIELD_NO::NO_1);
			m_NowField = FIELD_NO::NO_1;
		}
	}
	
	if (m_Player.GetPlayerState() == PLAYER_STATE::PLAYER_STATE_DEATH)
	{
		m_Manager->SetScene(SCENE_GAMEOVER);
	}

	collision.PlayerFieldCollision(&m_Player, &m_Map);
	collision.EnemyFieldCollision(&m_EnemyNormal, &m_Map);
	collision.PlayerEnemyCollision(&m_Player, &m_EnemyNormal);
	collision.PlayerBombCollision(&m_Player, &m_bomb);
	collision.BombFieldCollision(&m_bomb, &m_Map);
	collision.BombEnemyCollision(&m_bomb, &m_EnemyNormal);
	collision.EXPLOSIONEnemyCollision(&m_bomb, &m_EnemyNormal);
	collision.WeaponFieldCollision(&m_Weapon, &m_Map);
	collision.PlayerWeaponCollision(&m_Player, &m_Weapon);

	collision.PlayerGimmickCollision(&m_Player, &m_GimmickData);
	collision.BombGimmickCollision(&m_bomb, &m_GimmickData);
	collision.EnemyGimmickCollision(&m_EnemyNormal, &m_GimmickData);	


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
	//
	////倒すべき敵の数と今まで倒した敵の数を比べる
	//if (m_EnemyNormal.EnemySpawner_GetKillNum() >= m_EnemyNormal.EnemySpawner_GetEnemyNum())
	//{
	//	if (m_Manager->GetClearCount() == 2) 
	//	{
	//		m_Manager->IncrementClearCount();
	//	};
	//	
	//	m_Manager->SetScene(SCENE_RESULT);
	//	
	//}

	//if (Keyboard_IsKeyDownTrigger(KK_C))
	//{
	//	if (m_Manager->GetClearCount() == 2)
	//	{
	//		m_Manager->IncrementClearCount();
	//	};

	//	m_Manager->SetScene(SCENE_RESULT);
	//}

}

void ENEMYLUSH::Enemylush_Draw()
{ 
	m_Background.Background_Draw();
	Light5.SetEnable(TRUE);			//ライティングON
	Shader_SetLight(Light5.Light);	//ライト構造体をシェーダーへセット
	SetDepthTest(TRUE);

	Camera_Draw();		//Drawの最初で呼ぶ！

	m_Map.Field_Draw();
	m_Player.Player_Draw(&m_BillboardManager);
	m_EnemyNormal.EnemySpawner_Draw();
	m_bomb.Bomb_Draw(&m_BillboardManager);
	m_Weapon.Weapon_Draw();

	m_GimmickData.Gimmick_Data_Draw();

	//2D描画
	Light5.SetEnable(FALSE);			//ライティングOFF
	Shader_SetLight(Light5.Light);	//ライト構造体をシェーダーへセット
	
	m_BillboardManager.Draw();
	SetDepthTest(FALSE);
	m_PlayerUI.Draw();
	m_BombUI.Draw();
	m_TargetUI.Draw();
	//Block_Draw();
	//Effect_Draw();
	//Score_Draw();

	//Polygon3D_Draw();

}

void ENEMYLUSH::Enemylush_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{

	m_Map.Field_Finalize();	// フィールドの終了処理
	m_Background.Background_Finalize();
	m_Player.Player_Finalize();	// ボールの終了処理
	m_EnemyNormal.EnemySpawner_Finalize();
	m_bomb.Bomb_Finalize();
	m_Weapon.Weapon_Finalize();

	m_GimmickData.Gimmick_Data_Finalize();

	m_BillboardManager.Finalize();
	m_PlayerUI.Finalize();
	m_BombUI.Finalize();
	m_TargetUI.Finalize();
	Camera_Finalize();	//カメラ終了処理



	m_Player.Player_Initialize(pDevice, pContext); // ボールの初期化
	Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
	m_Map.Field_Initialize(pDevice, pContext,no); // フィールドの初期化
	m_Background.Background_Initialize(pDevice, pContext);
	m_EnemyNormal.EnemySpawner_Initialize(pDevice, pContext,no);
	m_bomb.Bomb_Initialize(pDevice, pContext,no);
	m_Weapon.Weapon_Initialize(pDevice, pContext);

	m_GimmickData.Gimmick_Data_Initialize(pDevice, pContext, no);

	m_BillboardManager.Initialize(pDevice, pContext, m_NowField);
	m_PlayerUI.Initialize(pDevice, pContext, &m_Player);
	m_BombUI.Initialize(pDevice, pContext, &m_bomb);
	m_TargetUI.Initialize(pDevice, pContext);
}



//===================================================================================================
// EnemyLushPhase
//===================================================================================================

void ENEMYLUSH::Enemylush_Phase_Initialize()
{
	for (int i = 0; i < EL_STATE_MAX; i++)
	{
		switch (i)
		{
		case EL_STATE_PHASE01:
			m_Phase[i].Set_EN_TOTAL(2);
			m_Phase[i].Set_EF_TOTAL(2);
			m_Phase[i].Set_EG_TOTAL(6);
			break;
		case EL_STATE_PHASE02:
			m_Phase[i].Set_EN_TOTAL(3);
			m_Phase[i].Set_EF_TOTAL(3);
			m_Phase[i].Set_EG_TOTAL(3);
			break;
		case EL_STATE_PHASE03:
			m_Phase[i].Set_EN_TOTAL(1);
			m_Phase[i].Set_EF_TOTAL(4);
			m_Phase[i].Set_EG_TOTAL(4);
			break;
		case EL_STATE_END:
			m_Phase[i].Set_EN_TOTAL(0);
			m_Phase[i].Set_EF_TOTAL(0);
			m_Phase[i].Set_EG_TOTAL(0);
			break;
		case EL_STATE_MAX:
			m_Phase[i].Set_EN_TOTAL(0);
			m_Phase[i].Set_EF_TOTAL(0);
			m_Phase[i].Set_EG_TOTAL(0);
			break;
		default:
			break;
		}
	}
}

void ENEMYLUSH::Enemylush_Phase_Finalize()
{
}

void ENEMYLUSH::Enemylush_Phase_Update()
{
	
	if (m_SpawnTime > 5.0f&&!m_NextSpawn)
	{
		srand(time(NULL));
		m_SpawnTime = 0;
		m_NextSpawn = true;
		m_SpawnMax = 3;
	}

	m_SpawnTime += (1.0f / 60.0f);
	int EN_NUM = m_Phase[m_EL_State].Get_EN_TOTAL();
	int EF_NUM = m_Phase[m_EL_State].Get_EF_TOTAL();
	int EG_NUM = m_Phase[m_EL_State].Get_EG_TOTAL();

	if (m_NextSpawn)
	{
		if (m_SpawnMax > 0)
		{
			
			int ramd = rand();
			int SpawnPos = (ramd % m_TotalSpawn);

			int ramd2 = rand();
			int EnemyType = (ramd2 % 3) + 1;

			if (EnemyType == 1 && EN_NUM > 0) {
				m_EnemyNormal.EnemySpawner_Spawn(m_SpawnPos[SpawnPos], EL_EN);
				EN_NUM--;
			}
			else if (EnemyType == 2 && EF_NUM > 0) {
				m_EnemyNormal.EnemySpawner_Spawn(m_SpawnPos[SpawnPos], EL_EF);
				EF_NUM--;
			}
			else if (EnemyType == 3 && EG_NUM > 0) {
				m_EnemyNormal.EnemySpawner_Spawn(m_SpawnPos[SpawnPos], EL_EG);
				EG_NUM--;
			}
			else {
			
			}

			m_Phase[m_EL_State].Set_EN_TOTAL(EN_NUM);
			m_Phase[m_EL_State].Set_EF_TOTAL(EF_NUM);
			m_Phase[m_EL_State].Set_EG_TOTAL(EG_NUM);

			m_SpawnMax--;
		}
		else { m_NextSpawn = false; }

	}

}

void ENEMYLUSH::Enemylush_Phase_Draw()
{
}

