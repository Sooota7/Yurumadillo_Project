
//Manager.h
#pragma once

#include "Game.h"
#include "Title.h"
#include "Pause.h"
#include "Result.h"
#include "fade.h"
#include "dictionary.h"


class MANAGER
{
private:
	//シーン移動に使用
	SCENE m_Scene;//現在のシーン
	FadeObject m_Fade;

	//シーン
	TITLE m_Title;
	GAME  m_Game;
	PAUSE m_Pause;
	RESULT m_Result;
	bool m_GameInitialized = false;

public:
	void	Manager_Initialize();
	void	Manager_Finalize();
	void	Manager_Update();
	void	Manager_Draw();

	void	SetScene(SCENE scene);

};