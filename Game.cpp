
//Game.cpp

#include	"Manager.h"
#include	"sprite.h"
#include	"Game.h"
#include	"keyboard.h"

#include	"player.h"
#include	"Block.h"
#include	"field.h"
#include	"Effect.h"
#include	"score.h"
#include	"Audio.h"

#include	"Polygon3D.h"
#include	"Camera.h"

#include "Ball.h"

#include	"direct3d.h"//<<<<<<<<<<<<<<<<<<<

LIGHTOBJECT		Light;//<<<<<<ライト管理オブジェクト


static	int		g_BgmID = NULL;	//サウンド管理ID

void GAME::Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager)
{
	
	m_Player.Player_Initialize(pDevice, pContext); // ボールの初期化
	Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
	m_Map.Field_Initialize(pDevice, pContext); // フィールドの初期化
	m_EnemyNormal.Initialize(pDevice, pContext);
	m_bomb.Bomb_Initialize(pDevice, pContext);

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
	m_Player.Player_Finalize();	// ボールの終了処理
	m_EnemyNormal.Finalize();
	m_bomb.Bomb_Finalize();
	//Block_Finalize();
	//Effect_Finalize();
	//Score_Finalize();
	//Polygon3D_Finalize();
	Camera_Finalize();	//カメラ終了処理

	UnloadAudio(g_BgmID);//サウンドの解放
}

void GAME::Game_Update()
{
	//更新処理
	Camera_Update(m_Player.GetPlayerPosition());	//カメラ更新処理
	m_Player.Player_Update();
	m_EnemyNormal.Update();
	m_Map.Field_Update();

	m_bomb.Bomb_Update(m_Player.GetPlayerPosition(),m_Player.GetPlayerRotation());

	collision.PlayerFieldCollision(&m_Player, &m_Map);
	collision.EnemyFieldCollision(&m_EnemyNormal, &m_Map);
	collision.PlayerEnemyCollision(&m_Player, &m_EnemyNormal);
	collision.PlayerBombCollision(&m_Player, &m_bomb);
	collision.BombFieldCollision(&m_bomb, &m_Map);
	collision.BombEnemyCollision(&m_bomb, &m_EnemyNormal);

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

}

void GAME::Game_Draw()
{ 
	Light.SetEnable(TRUE);			//ライティングON
	Shader_SetLight(Light.Light);	//ライト構造体をシェーダーへセット
	SetDepthTest(TRUE);

	Camera_Draw();		//Drawの最初で呼ぶ！
	m_Player.Player_Draw();
	m_Map.Field_Draw();
	m_Player.Player_Draw();
	m_EnemyNormal.Draw();
	m_bomb.Bomb_Draw();

	//2D描画
	Light.SetEnable(FALSE);			//ライティングOFF
	Shader_SetLight(Light.Light);	//ライト構造体をシェーダーへセット
	SetDepthTest(FALSE);



	//Block_Draw();
	//Effect_Draw();
	//Score_Draw();

	//Polygon3D_Draw();

}

