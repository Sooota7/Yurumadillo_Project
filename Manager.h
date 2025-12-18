
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
#include "StageSelection.h"
#include "Ending.h"
#include "Gimmick.h"
#include "EnemyLush.h"
#include "Boss.h"

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
	STAGESELECTION m_StageSelection;
	ENDING m_Ending;
	GIMMICK m_Gimmick;
	ENEMYLUSH m_EnemyLush;
	BOSS m_Boss;
	bool m_GameInitialized = false;
	bool m_TutorialInitialized = false;
	bool m_GimmickInitialized = false;
	bool m_EnemyLushInitialized = false;
	bool m_BossInitialized = false;

	int m_ClearCount = 0;

public:
	void	Manager_Initialize();
	void	Manager_Finalize();
	void	Manager_Update();
	void	Manager_Draw();

	void	SetScene(SCENE scene);

	void IncrementClearCount() { ++m_ClearCount; };
	
	int  GetClearCount() const { return m_ClearCount; };
	
};