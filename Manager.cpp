
//Manager.cpp

#include "direct3d.h"
#include "Manager.h"
#include "keyboard.h"

#include "Game.h"
#include "Title.h"
#include "Pause.h"
#include "Result.h"
#include "fade.h"
#include "Menu.h"
#include "Tutorial.h"
#include "Tutorial2.h"
#include "Tutorial3.h"
#include "prologue.h"
#include "StageSelection.h"
#include "Ending.h"
#include "Gimmick.h"
#include "EnemyLush.h"
#include "Boss.h"

void	MANAGER::Manager_BGM_Initialize(SCENE scene)
{
	if (scene != SCENE_STAGESELECTION)
	{//サウンドのリセット
		Manager_BGM_Finalize();
	}

	//ロードチェック
	bool load = false;

	switch (scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		g_BgmID = LoadAudio("asset\\Audio\\Title_Test.wav"); load = true;
		break;
	case SCENE_PROLOGUE:
		g_BgmID = LoadAudio("asset\\Audio\\Intro_Test.wav"); load = true;//サウンドロード
		break;
	case SCENE_MENU:
		g_BgmID = LoadAudio("asset\\Audio\\Menu_Test.wav"); load = true;//サウンドロード
		break;
	case SCENE_TUTORIAL:
		break;
	case SCENE_TUTORIAL2:
		break;
	case SCENE_TUTORIAL3:
		break;
	case SCENE_GAME:
		g_BgmID = LoadAudio("asset\\Audio\\Kinoko_Stage_Test.wav"); load = true;//サウンドロード
		break;
	case SCENE_PAUSE:
		break;
	case SCENE_RESULT:
		break;
	case SCENE_STAGESELECTION:
		break;
	case SCENE_ENDING:
		break;
	case SCENE_GIMMICK:
		g_BgmID = LoadAudio("asset\\Audio\\Gimmick_Stage_Test.wav"); load = true;//サウンドロード
		break;
	case SCENE_ENEMYLUSH:
		g_BgmID = LoadAudio("asset\\Audio\\Lush_Stage_Test.wav"); load = true;//サウンドロード
		break;
	case SCENE_BOSS:
		break;
	default:
		break;
	}

	if (load)
	{
		PlayAudio(g_BgmID, true);		//再生開始（ループあり）
	}

}

void	MANAGER::Manager_BGM_Finalize()
{
	UnloadAudio(g_BgmID);//サウンドの解放
}


void	MANAGER::Manager_Initialize()
{ 
	//Fade_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());

	////本来はtitleの初期化でフェードインをセットする
	//XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//SetFade(60.0f, color, FADE_STATE::FADE_IN, SCENE_GAME);
	//SetScene(SCENE_GAME);	//最初に動かすシーンに切り替える


	//本来の形
	m_Fade.Fade_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext());
	SetScene(SCENE_TITLE);	//最初に動かすシーンに切り替える

	g_BgmID = LoadAudio("asset\\Audio\\Title_Test.wav");	//サウンドロード
	PlayAudio(g_BgmID, true);		//再生開始（ループあり）

}

void	MANAGER::Manager_Finalize()
{ 
	m_Fade.Fade_Finalize();
	SetScene(SCENE_NONE);



}

void	MANAGER::Manager_Update()
{
	switch (m_Scene)	//現在シーンのアップデート関数を呼び出す
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		m_Title.Title_Update();
		break;
	case SCENE_PROLOGUE:
		m_Prologue.Prologue_Update();
		break;
	case SCENE_MENU:
		m_Menu.Menu_Update();
		break;
	case SCENE_STAGESELECTION:
		m_StageSelection.StageSelection_Update();
		break;
	case SCENE_TUTORIAL:
		m_Tutorial.Tutorial_Update();
		break;
	case SCENE_TUTORIAL2:
		m_Tutorial2.Tutorial2_Update();
		break;
	case SCENE_TUTORIAL3:
		m_Tutorial3.Tutorial3_Update();
		break;
	
	case SCENE_GAME:
		m_Game.Game_Update();
		break;
	case SCENE_GIMMICK:
		m_Gimmick.Gimmick_Update();
		break;
	case SCENE_ENEMYLUSH:
		m_EnemyLush.Enemylush_Update();
		break;
	case SCENE_BOSS:
		m_Boss.Boss_Update();
		break;
	case SCENE_PAUSE:
		m_Pause.Pause_Update();
		break;
	case SCENE_RESULT:
		m_Result.Result_Update();
		break;
	case SCENE_ENDING:
		m_Ending.Ending_Update();
		break;
	default:
		break;
	}

	m_Fade.Fade_Update();

	if (m_Fade.GetFadeState() == FADE_OUT && m_Fade.Fade_GetColorW() > 1.0f)
	{
		Manager_BGM_Initialize(m_Fade.Fade_GetScene());
		SetScene(m_Fade.Fade_GetScene());
	}



}

void	MANAGER::Manager_Draw()
{ 

	switch (m_Scene)	//現在シーンの描画関数を呼び出す
	{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			m_Title.Title_Draw();	
			break;
		case SCENE_PROLOGUE:
			m_Prologue.Prologue_Draw();	
			break;
		case SCENE_MENU:
			m_Menu.Menu_Draw();
			break;
		case  SCENE_STAGESELECTION :
			m_StageSelection.StageSelection_Draw();
			break;
		case SCENE_TUTORIAL:
			m_Tutorial.Tutorial_Draw();
			break;
		case SCENE_TUTORIAL2:
			m_Tutorial2.Tutorial2_Draw();
			break;
		case SCENE_TUTORIAL3:
			m_Tutorial3.Tutorial3_Draw();
			break;
		case SCENE_GAME:
			m_Game.Game_Draw();
			break;
		case SCENE_GIMMICK:
			m_Gimmick.Gimmick_Draw();
			break;
		case SCENE_ENEMYLUSH:
			m_EnemyLush.Enemylush_Draw();
			break;
		case SCENE_BOSS:
			m_Boss.Boss_Draw();
			break;
		case SCENE_PAUSE:
			m_Pause.Pause_Draw();
			break;
		case SCENE_RESULT:
			m_Result.Result_Draw();
			break;
		case SCENE_ENDING:
			m_Ending.Ending_Draw();
			break;
		default:
			break;
	}

	m_Fade.Fade_Draw();

}

void	MANAGER::SetScene(SCENE scene) //シーンを切り替える
{
	//実行中のシーンを終了させる
	switch (m_Scene)	//現在シーンの終了関数を呼び出す
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		m_Title.Title_Finalize();
		break;
	case SCENE_PROLOGUE:
		m_Prologue.Prologue_Finalize();
		break;
	case SCENE_MENU:
		m_Menu.Menu_Finalize();
		break;
	case SCENE_STAGESELECTION:
		m_StageSelection.StageSelection_Finalize();
		break;
	case SCENE_TUTORIAL:
		m_Tutorial.Tutorial_Finalize();
		break;
	case SCENE_TUTORIAL2:
		m_Tutorial2.Tutorial2_Finalize();
		break;
	case SCENE_TUTORIAL3:
		m_Tutorial3.Tutorial3_Finalize();
		break;
	case SCENE_GAME:
		if (scene != SCENE_PAUSE)
		{
			m_Game.Game_Finalize();
			m_GameInitialized = false;
		}
		break;
	case SCENE_GIMMICK:
		if (scene != SCENE_PAUSE)
		{
			m_Gimmick.Gimmick_Finalize();
			m_GimmickInitialized = false;
		}
		break;
	case SCENE_ENEMYLUSH:
		if (scene != SCENE_PAUSE)
		{
			m_EnemyLush.Enemylush_Finalize();
			m_EnemyLushInitialized = false;
		}
		break;
	case SCENE_BOSS:
		if (scene != SCENE_PAUSE)
		{
			m_Boss.Boss_Finalize();
			m_BossInitialized = false;
		}
		break;
	case SCENE_PAUSE:
		m_Pause.Pause_Finalize();
		break;
	case SCENE_RESULT:
		m_Result.Result_Finalize();
		break;
	case SCENE_ENDING:
		m_Ending.Ending_Finalize();
		break;
	default:
		break;
	}


	m_Scene = scene;	//指定のシーンへ切り替える


	
	//次のシーンを初期化する
	switch (m_Scene)	//現在シーンの初期化関数を呼び出す
	{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			m_Title.Title_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(),&m_Fade);
			break;
		case SCENE_PROLOGUE:
			m_Prologue.Prologue_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(),&m_Fade);
			break;
		case SCENE_MENU:
			m_Menu.Menu_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), &m_Fade);
			break;
		case SCENE_STAGESELECTION:
			m_StageSelection.StageSelection_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), &m_Fade,this);
			break;
		
		case SCENE_TUTORIAL:
			if (!m_TutorialInitialized)
			{
				m_Tutorial.Tutorial_Initialize(
					Direct3D_GetDevice(),
					Direct3D_GetDeviceContext(),
					this
				);
				m_TutorialInitialized = true;
			}
			break;
		case SCENE_TUTORIAL2:
			if (!m_TutorialInitialized2)
			{
				m_Tutorial2.Tutorial2_Initialize(
					Direct3D_GetDevice(),
					Direct3D_GetDeviceContext(),
					this
				);
				m_TutorialInitialized = true;
			}
			break;
		case SCENE_TUTORIAL3:
			if (!m_TutorialInitialized3)
			{
				m_Tutorial3.Tutorial3_Initialize(
					Direct3D_GetDevice(),
					Direct3D_GetDeviceContext(),
					this
				);
				m_TutorialInitialized = true;
			}
			break;
		case SCENE_GAME:
			if (!m_GameInitialized)
			{
				m_Game.Game_Initialize(
					Direct3D_GetDevice(),
					Direct3D_GetDeviceContext(),
					this
				);
				m_GameInitialized = true;
			}
			break;		
		case SCENE_GIMMICK:
			if (!m_GimmickInitialized)
			{
				m_Gimmick.Gimmick_Initialize(
					Direct3D_GetDevice(),
					Direct3D_GetDeviceContext(),
					this
				);
				m_GimmickInitialized = true;
			}
			break;
		case SCENE_ENEMYLUSH:
			if (!m_EnemyLushInitialized)
			{
				m_EnemyLush.Enemylush_Initialize(
					Direct3D_GetDevice(),
					Direct3D_GetDeviceContext(),
					this
				);
				m_EnemyLushInitialized = true;
			}
			break;
		case SCENE_BOSS:
			if (!m_BossInitialized)
			{
				m_Boss.Boss_Initialize(
					Direct3D_GetDevice(),
					Direct3D_GetDeviceContext(),
					this
				);
				m_BossInitialized = true;
			}
			break;
		case SCENE_PAUSE:
			m_Pause.Pause_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), &m_Fade);
			break;
		case SCENE_RESULT:
			m_Result.Result_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), &m_Fade,this);
			break;
		case SCENE_ENDING:
			m_Ending.Ending_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), &m_Fade);
			break;
		default:
			break;
	}


}
