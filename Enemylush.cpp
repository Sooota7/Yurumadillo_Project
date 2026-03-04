
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

static	ID3D11ShaderResourceView* g_TexturePhase = NULL;	//テクスチャ１枚を表すオブジェクト
static	ID3D11ShaderResourceView* g_TextureNumber = NULL;	//テクスチャ１枚を表すオブジェクト
static	ID3D11ShaderResourceView* g_TextureCrear = NULL;	//テクスチャ１枚を表すオブジェクト


static	int		g_BgmID = NULL;	//サウンド管理ID

void ENEMYLUSH::Enemylush_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager)
{
	//m_NowField = FIELD_NO::NO_4;

	m_NowField = FIELD_NO::NO_ENEMYLUSH;

	g_pDevice_EL = pDevice;
	g_pContext_EL = pContext;

	//テクスチャ読み込みなど
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\EnemyLush\\Phase.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_TexturePhase);
	assert(g_TexturePhase);//読み込み失敗時にダイアログを表示

	LoadFromWICFile(L"asset\\texture\\EnemyLush\\Number.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_TextureNumber);
	assert(g_TextureNumber);//読み込み失敗時にダイアログを表示

	LoadFromWICFile(L"asset\\texture\\EnemyLush\\Crear.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_TextureCrear);
	assert(g_TextureCrear);//読み込み失敗時にダイアログを表示


	m_PhaseEnd = false;

	m_Manager = manager;

	m_SceneLoad.Load_Initialize(g_pDevice_EL, g_pContext_EL);

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


void ENEMYLUSH::Enemylush_LoadUpdate()
{

	switch (m_SceneLoad.GetLoadCount())
	{
	case 0:
		m_Player.Player_Initialize(g_pDevice_EL, g_pContext_EL); // ボールの初期化
		break;
	case 1:
		Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
		break;
	case 2:
		m_Map.Field_Initialize(g_pDevice_EL, g_pContext_EL, m_NowField); // フィールドの初期化
		break;
	case 3:
		m_Background.Background_Initialize(g_pDevice_EL, g_pContext_EL, m_NowField);
		break;
	case 4:
		Enemylush_Phase_Initialize();
		break;
	case 5:
		m_EnemyNormal.EnemySpawner_Initialize(g_pDevice_EL, g_pContext_EL, m_NowField);
		break;
	case 6:
		m_bomb.Bomb_Initialize(g_pDevice_EL, g_pContext_EL, m_NowField);
		break;
	case 7:
		m_Weapon.Weapon_Initialize(g_pDevice_EL, g_pContext_EL,&m_BillboardManager);
		break;
	case 8:
		m_Goal.Goal_Initialize(g_pDevice_EL, g_pContext_EL, m_NowField);
		break;
	case 9:
		m_BillboardManager.Initialize(g_pDevice_EL, g_pContext_EL, m_NowField);
		break;
	case 10:
		m_PlayerUI.Initialize(g_pDevice_EL, g_pContext_EL, &m_Player);
		break;
	case 11:
		m_BombUI.Initialize(g_pDevice_EL, g_pContext_EL, &m_bomb);
		break;
	case 12:
		m_TargetUI.Initialize(g_pDevice_EL, g_pContext_EL,m_NowField);
		break;
	case 13:
		m_GimmickData.Gimmick_Data_Initialize(g_pDevice_EL, g_pContext_EL, m_NowField);
		break;
	case 14:
		m_SpawnTime = 0;
		break;
	case 15:
		g_BgmID = LoadAudio("asset\\Audio\\bgm.wav");	//サウンドロード
		break;
	/*case 16:
		
		break;*/
	default:
		m_SceneLoad.SetLoadComplete(true);
		break;
	}

	m_SceneLoad.Load_Update();

}


void ENEMYLUSH::Enemylush_Finalize()
{
	SAFE_RELEASE(g_TexturePhase);
	SAFE_RELEASE(g_TextureNumber);
	SAFE_RELEASE(g_TextureCrear);

	m_SceneLoad.Load_Finalize();

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
	m_Goal.Goal_Finalize();
	m_BillboardManager.Finalize();
	m_PlayerUI.Finalize();
	m_BombUI.Finalize();
	m_TargetUI.Finalize();

	UnloadAudio(g_BgmID);//サウンドの解放
}

void ENEMYLUSH::Enemylush_Update()
{
	if (m_SceneLoad.GetLoadActive()){
		Enemylush_LoadUpdate();
	}
	else {

		//更新処理
		Camera_Update(m_Player.GetPlayerPosition());	//カメラ更新処理
		m_Player.Player_Update();
		m_EnemyNormal.EnemySpawner_Update(m_Player.GetPlayerPosition());
		m_Map.Field_Update(m_PhaseEnd);
		m_Background.Background_Update();

		m_bomb.Bomb_Update(m_Player.GetPlayerPosition(), m_Player.GetPlayerRotation());
		m_Weapon.Weapon_Update(m_Player.GetPlayerPosition(), &m_EnemyNormal);

		m_GimmickData.Gimmick_Data_Update(m_Player.GetPlayerPosition(), m_Player.GetPlayerRotation());

		m_Goal.Goal_Update();
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

		Enemylush_Phase_Update();

//#ifdef _DEBUG
//
//		if (Keyboard_IsKeyDownTrigger(KK_C))
//		{
//			if (m_Manager->GetClearCount() == 2)
//			{
//				m_Manager->IncrementClearCount();
//			};
//
//			m_Manager->SetScene(SCENE_STAGESELECTION);
//		}
//#endif // _DEBUG

		// 追加：プレイヤーとゴールの当たり判定
		if (collision.PlayerGoalCollision(&m_Player, &m_Goal) == COLLISION_HIT::HIT_WALL_CREAR)
		{
			if (m_Manager->GetClearCount() == 2)
			{
				m_Manager->IncrementClearCount();
			}
			m_Manager->SetScene(SCENE_STAGESELECTION);
			return; // シーン遷移するので更新処理を止める
		}
	}
}

void ENEMYLUSH::Enemylush_Draw()
{ 
	if (m_SceneLoad.GetLoadActive()) {
		m_SceneLoad.Load_Draw();
	}
	else {
		m_Background.Background_Draw();
		Light5.SetEnable(TRUE);			//ライティングON
		//Shader_SetLight(Light5.Light);	//ライト構造体をシェーダーへセット
		SetDepthTest(TRUE);

		Camera_Draw();		//Drawの最初で呼ぶ！

		m_Map.Field_Draw();
		m_Player.Player_Draw(&m_BillboardManager);
		m_EnemyNormal.EnemySpawner_Draw();
		m_bomb.Bomb_Draw(&m_BillboardManager);
		m_Weapon.Weapon_Draw();

		m_GimmickData.Gimmick_Data_Draw();

		m_Goal.Goal_Draw();
		//2D描画
		Light5.SetEnable(FALSE);			//ライティングOFF
		//Shader_SetLight(Light5.Light);	//ライト構造体をシェーダーへセット
		
		m_BillboardManager.Draw();
		SetDepthTest(FALSE);
		m_PlayerUI.Draw();
		m_BombUI.Draw();
		m_TargetUI.Draw();

		Enemylush_DrawPhase();
	}

}

void ENEMYLUSH::Enemylush_DrawPhase()
{
	Shader_Begin();

	// 画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	// 頂点シェーダーに変換行列を設定
	Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(
		0.0f,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0.0f,
		0.0f,
		1.0f));
	//---------------------------------------------------


	if (m_EL_State != EL_STATE_END) {

		//テクスチャをセット
		g_pContext_EL->PSSetShaderResources(0, 1, &g_TexturePhase);//g_Textureを使うように設定する


		//スプライト描画
		SetBlendState(BLENDSTATE_ALFA);//ブレンド無し
		XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
		XMFLOAT2 pos = { 300, 100 };
		XMFLOAT2 size = { 300,100 };
		DrawSprite(pos, size, col);//1枚絵を表示

		{

			// シェーダーを描画パイプラインに設定
			Shader_Begin();

			const float START_WIDTH = 600;
			const float START_HEIGHT = 100;

			// 頂点シェーダーに変換行列を設定
			Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(
				0.0f,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				0.0f,
				0.0f,
				1.0f));
			//---------------------------------------------------

			//テクスチャをセット
			g_pContext_EL->PSSetShaderResources(0, 1, &g_TextureNumber);//g_Textureを使うように設定する

			//スプライト描画
			SetBlendState(BLENDSTATE_ALFA);//ブレンド無し


			XMFLOAT4 col2 = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
			XMFLOAT2 pos2 = { 500 , 100 };
			XMFLOAT2 size2 = { 100,100 };
			DrawSpriteEx(pos2, size2, col2, (int)(m_EL_State + 1), 10, 1);//1枚絵を表示
		}
	}
	else {
		//テクスチャをセット
		g_pContext_EL->PSSetShaderResources(0, 1, &g_TextureCrear);//g_Textureを使うように設定する


		//スプライト描画
		SetBlendState(BLENDSTATE_ALFA);//ブレンド無し
		XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
		XMFLOAT2 pos = { 300, 100 };
		XMFLOAT2 size = { 300,100 };
		DrawSprite(pos, size, col);//1枚絵を表示
	}

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
	m_Goal.Goal_Finalize();
	Camera_Finalize();	//カメラ終了処理



	m_Player.Player_Initialize(pDevice, pContext); // ボールの初期化
	Camera_Initialize(m_Player.GetPlayerPosition());	//カメラ初期化
	m_Map.Field_Initialize(pDevice, pContext,no); // フィールドの初期化
	m_Background.Background_Initialize(pDevice, pContext, m_NowField);
	m_EnemyNormal.EnemySpawner_Initialize(pDevice, pContext,no);
	m_bomb.Bomb_Initialize(pDevice, pContext,no);
	m_Weapon.Weapon_Initialize(pDevice, pContext,&m_BillboardManager);

	m_GimmickData.Gimmick_Data_Initialize(pDevice, pContext, no);

	m_BillboardManager.Initialize(pDevice, pContext, m_NowField);
	m_PlayerUI.Initialize(pDevice, pContext, &m_Player);
	m_BombUI.Initialize(pDevice, pContext, &m_bomb);
	m_TargetUI.Initialize(pDevice, pContext,m_NowField);
	m_Goal.Goal_Initialize(pDevice, pContext, m_NowField);
}



//===================================================================================================
// EnemyLushPhase
//===================================================================================================

void ENEMYLUSH::Enemylush_Phase_Initialize()
{
	m_EL_State = EL_STATE_PHASE01;
	m_EnemyNormal.EnemySpawner_ResetKillNum();
	m_EnemyNormal.EnemySpawner_ResetEnemyNum();

	for (int i = 0; i < EL_STATE_MAX; i++)
	{
		switch (i)
		{
		case EL_STATE_PHASE01:
			Enemylush_Phase_SetPhase01();
			break;
		case EL_STATE_PHASE02:
			Enemylush_Phase_SetPhase02();
			break;
		case EL_STATE_PHASE03:
			Enemylush_Phase_SetPhase03();
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
	int EN_NUM = m_Phase[m_EL_State].Get_EN_TOTAL();
	int EF_NUM = m_Phase[m_EL_State].Get_EF_TOTAL();
	int EG_NUM = m_Phase[m_EL_State].Get_EG_TOTAL();
	
	if (EN_NUM <= 0 && EF_NUM <= 0 && EG_NUM <= 0 && 
		m_EnemyNormal.EnemySpawner_GetKillNum() >= m_EnemyNormal.EnemySpawner_GetEnemyNum())//出尽くしてかつ全滅でゲート開く
	{
		switch (m_EL_State)
		{
		case EL_STATE_PHASE01:
			m_EL_State = EL_STATE_PHASE02; m_SpawnTime = 0; m_NextSpawn = false;
			m_EnemyNormal.EnemySpawner_ResetKillNum();
			m_EnemyNormal.EnemySpawner_ResetEnemyNum();
			return;
		case EL_STATE_PHASE02:
			m_EL_State = EL_STATE_PHASE03; m_SpawnTime = 0; m_NextSpawn = false;
			m_EnemyNormal.EnemySpawner_ResetKillNum();
			m_EnemyNormal.EnemySpawner_ResetEnemyNum();
			return;
		case EL_STATE_PHASE03:
			m_EL_State = EL_STATE_END; m_SpawnTime = 0; m_NextSpawn = false;
			m_EnemyNormal.EnemySpawner_ResetKillNum();
			m_EnemyNormal.EnemySpawner_ResetEnemyNum();
			return;
		case EL_STATE_END:
			m_PhaseEnd = true;
			return;
		default:
			break;
		}

		

	}
	
	if (m_SpawnTime > 5.0f&&!m_NextSpawn)
	{
		srand(time(NULL));

		m_SpawnTime = 0;
		m_NextSpawn = true;
		int ramd = rand();
		m_SpawnMax = (ramd % 3) + 1;
	}

	m_SpawnTime += (1.0f / 60.0f);
	
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
				EN_NUM--;m_SpawnMax--;
			}
			else if (EnemyType == 2 && EF_NUM > 0) {
				m_EnemyNormal.EnemySpawner_Spawn(m_SpawnPos[SpawnPos], EL_EF);
				EF_NUM--; m_SpawnMax--;
			}
			else if (EnemyType == 3 && EG_NUM > 0) {
				m_EnemyNormal.EnemySpawner_Spawn(m_SpawnPos[SpawnPos], EL_EG);
				EG_NUM--; m_SpawnMax--;
			}
			else {
			
			}

			m_Phase[m_EL_State].Set_EN_TOTAL(EN_NUM);
			m_Phase[m_EL_State].Set_EF_TOTAL(EF_NUM);
			m_Phase[m_EL_State].Set_EG_TOTAL(EG_NUM);

			
		}
		else { m_NextSpawn = false; }

	}

}

void ENEMYLUSH::Enemylush_Phase_Draw()
{
}

void ENEMYLUSH::Enemylush_Phase_SetPhase01()
{
	m_Phase[EL_STATE_PHASE01].Set_EN_TOTAL(0);
	m_Phase[EL_STATE_PHASE01].Set_EF_TOTAL(1);
	m_Phase[EL_STATE_PHASE01].Set_EG_TOTAL(2);
}

void ENEMYLUSH::Enemylush_Phase_SetPhase02()
{
	m_Phase[EL_STATE_PHASE02].Set_EN_TOTAL(2);
	m_Phase[EL_STATE_PHASE02].Set_EF_TOTAL(1);
	m_Phase[EL_STATE_PHASE02].Set_EG_TOTAL(2);

}

void ENEMYLUSH::Enemylush_Phase_SetPhase03()
{
	m_Phase[EL_STATE_PHASE03].Set_EN_TOTAL(2);
	m_Phase[EL_STATE_PHASE03].Set_EF_TOTAL(2);
	m_Phase[EL_STATE_PHASE03].Set_EG_TOTAL(2);
}

