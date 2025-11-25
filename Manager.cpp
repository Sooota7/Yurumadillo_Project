
//Manager.cpp

#include "direct3d.h"
#include "Manager.h"
#include "keyboard.h"

#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "fade.h"


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
	case SCENE_GAME:
		m_Game.Game_Update();
		break;
	case SCENE_RESULT:
		m_Result.Result_Update();
		break;
	default:
		break;
	}

	m_Fade.Fade_Update();

	if (m_Fade.GetFadeState() == FADE_OUT && m_Fade.Fade_GetColorW() > 1.0f)
	{
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
		case SCENE_GAME:
			m_Game.Game_Draw();
			break;
		case SCENE_RESULT:
			m_Result.Result_Draw();
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
		case SCENE_GAME:
			m_Game.Game_Finalize();
			break;
		case SCENE_RESULT:
			m_Result.Result_Finalize();
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
		case SCENE_GAME:
			m_Game.Game_Initialize( Direct3D_GetDevice(), Direct3D_GetDeviceContext());
			break;
		case SCENE_RESULT:
			m_Result.Result_Initialize(Direct3D_GetDevice(), Direct3D_GetDeviceContext(), &m_Fade);
			break;
		default:
			break;
	}

}
