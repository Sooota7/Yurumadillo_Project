
//Game.cpp

#include	"Manager.h"
#include	"sprite.h"
#include	"Game.h"
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

LIGHTOBJECT		Light;//<<<<<<ライト管理オブジェクト



static	int		g_BgmID = NULL;	//サウンド管理ID

void GAME::Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager)
{
	m_NowField = FIELD_NO::NO_2;

	m_Player.Player_Initialize(pDevice, pContext); // ボールの初期化
	Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
	m_Map.Field_Initialize(pDevice, pContext, m_NowField); // フィールドの初期化
	m_Background.Background_Initialize(pDevice, pContext);
	m_EnemyNormal.EnemySpawner_Initialize(pDevice, pContext, m_NowField);
	m_bomb.Bomb_Initialize(pDevice, pContext, m_NowField);
	m_Weapon.Weapon_Initialize(pDevice, pContext);

	m_BillboardManager.Initialize(pDevice, pContext);
	m_PlayerUI.Initialize(pDevice, pContext, &m_Player);
	m_BombUI.Initialize(pDevice, pContext, &m_bomb);
	//Player_Initialize(pDevice, pContext); // ポリゴンの初期化
	//Block_Initialize(pDevice, pContext);//ブロックの初期化
	//Effect_Initialize(pDevice, pContext);//エフェクト初期化
	//Score_Initialize(pDevice, pContext);//スコア初期化

	//Polygon3D_Initialize(pDevice, pContext);//３Dテスト初期化

	m_Manager = manager;

	

	g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
	//PlayAudio(g_BgmID, true);	//再生開始（ループあり）
	//PlayAudio(g_BgmID);			//再生開始（ループなし）
	//PlayAudio(g_BgmID, false);	//再生開始（ループなし）

	//ライト初期化
	XMFLOAT4	para;

	para = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);//環境光の色
	Light.SetAmbient(para);

	para = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);//光の色
	Light.SetDiffuse(para);

	para = XMFLOAT4(0.5f, -1.0f, 0.0f, 1.0f);//光方向
	float	len = sqrtf(para.x * para.x + para.y * para.y + para.z * para.z);
	para.x /= len;
	para.y /= len;
	para.z /= len;
	Light.SetDirection(para);//光の方向（正規化済）

}

void GAME::Game_Finalize()
{
	m_Map.Field_Finalize();	// フィールドの終了処理
	m_Background.Background_Finalize();
	m_Player.Player_Finalize();	// ボールの終了処理
	m_EnemyNormal.EnemySpawner_Finalize();
	m_bomb.Bomb_Finalize();
	m_Weapon.Weapon_Finalize();
	//Block_Finalize();
	//Effect_Finalize();
	//Score_Finalize();
	//Polygon3D_Finalize();
	Camera_Finalize();	//カメラ終了処理

	m_BillboardManager.Finalize();
	m_PlayerUI.Finalize();
	m_BombUI.Finalize();

	UnloadAudio(g_BgmID);//サウンドの解放
}

void GAME::Game_Update()
{
	//更新処理
	Camera_Update(m_Player.GetPlayerPosition());	//カメラ更新処理
	m_Player.Player_Update();
	m_EnemyNormal.EnemySpawner_Update(m_Player.GetPlayerPosition());
	m_Map.Field_Update();
	m_Background.Background_Update();

	m_bomb.Bomb_Update(m_Player.GetPlayerPosition(), m_Player.GetPlayerRotation());
	m_Weapon.Weapon_Update(m_Player.GetPlayerPosition(),&m_EnemyNormal);

	m_PlayerUI.Update();
	m_BombUI.Update();

	if (collision.PlayerFieldCollision(&m_Player, &m_Map) == COLLISION_HIT::HIT_WALL_CREAR)
	{
		if (m_NowField == FIELD_NO::NO_1)
		{
			Game_SetNextMap(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), FIELD_NO::NO_2);
			m_NowField = FIELD_NO::NO_2;
		}
		else if (m_NowField == FIELD_NO::NO_2)
		{
			Game_SetNextMap(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), FIELD_NO::NO_1);
			m_NowField = FIELD_NO::NO_1;
		}
	}
	
	if (m_Player.GetPlayerState() == PLAYER_STATE::PLAYER_STATE_DEATH)
	{
		m_Manager->SetScene(SCENE_PAUSE);
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
	
	//倒すべき敵の数と今まで倒した敵の数を比べる
	if (m_EnemyNormal.EnemySpawner_GetKillNum() >= m_EnemyNormal.EnemySpawner_GetEnemyNum())
	{
		if (m_Manager->GetClearCount() == 0) 
		{
			m_Manager->IncrementClearCount();
		};
		
		m_Manager->SetScene(SCENE_RESULT);
		
	}
}

void GAME::Game_Draw()
{ 
	m_Background.Background_Draw();

	Light.SetEnable(TRUE);			//ライティングON
	Shader_SetLight(Light.Light);	//ライト構造体をシェーダーへセット
	SetDepthTest(TRUE);

	Camera_Draw();		//Drawの最初で呼ぶ！

	m_Map.Field_Draw();
	m_Player.Player_Draw(&m_BillboardManager);
	m_EnemyNormal.EnemySpawner_Draw();
	m_bomb.Bomb_Draw(&m_BillboardManager);
	m_Weapon.Weapon_Draw();

	//2D描画
	Light.SetEnable(FALSE);			//ライティングOFF
	Shader_SetLight(Light.Light);	//ライト構造体をシェーダーへセット
	SetDepthTest(FALSE);

	m_BillboardManager.Draw();
	m_PlayerUI.Draw();
	m_BombUI.Draw();

	//Block_Draw();
	//Effect_Draw();
	//Score_Draw();

	//Polygon3D_Draw();

}

void GAME::Game_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{

	m_Map.Field_Finalize();	// フィールドの終了処理
	m_Background.Background_Finalize();
	m_Player.Player_Finalize();	// ボールの終了処理
	m_EnemyNormal.EnemySpawner_Finalize();
	m_bomb.Bomb_Finalize();
	m_Weapon.Weapon_Finalize();
	m_BillboardManager.Finalize();
	m_PlayerUI.Finalize();
	m_BombUI.Finalize();
	Camera_Finalize();	//カメラ終了処理



	m_Player.Player_Initialize(pDevice, pContext); // ボールの初期化
	Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
	m_Map.Field_Initialize(pDevice, pContext,no); // フィールドの初期化
	m_Background.Background_Initialize(pDevice, pContext);
	m_EnemyNormal.EnemySpawner_Initialize(pDevice, pContext,no);
	m_bomb.Bomb_Initialize(pDevice, pContext,no);
	m_Weapon.Weapon_Initialize(pDevice, pContext);
	m_BillboardManager.Initialize(pDevice, pContext);
	m_PlayerUI.Initialize(pDevice, pContext, &m_Player);
	m_BombUI.Initialize(pDevice, pContext, &m_bomb);
}

