
//Manager.h
#pragma once

#include "Game.h"
#include "Title.h"
#include "Pause.h"
#include "Result.h"
#include "fade.h"
#include "dictionary.h"
#include "Menu.h"
#include "Tutorial.h"
#include "prologue.h"

class MANAGER
{
private:
	//シーン移動に使用
	SCENE m_Scene;//現在のシーン
	FadeObject m_Fade;

	//シーン
	TITLE m_Title;
	PROLOGUE m_Prologue;
	MENU  m_Menu;
	TUTORIAL m_Tutorial;
	GAME  m_Game;
	PAUSE m_Pause;
	RESULT m_Result;
	bool m_GameInitialized = false;
	bool m_TutorialInitialized = false;

public:
	void	Manager_Initialize();
	void	Manager_Finalize();
	void	Manager_Update();
	void	Manager_Draw();

	void	SetScene(SCENE scene);

};