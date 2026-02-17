//Manager.h
#pragma once

#include "Game.h"
#include "Title.h"
#include "Pause.h"
#include "Result.h"
#include "GameOver.h"
#include "fade.h"
#include "dictionary.h"
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
#include "enemySpawner.h"

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
	TUTORIAL2 m_Tutorial2;
	TUTORIAL3 m_Tutorial3;
	GAME  m_Game;
	PAUSE m_Pause;
	RESULT m_Result;
	GAMEOVER m_GameOver;
	STAGESELECTION m_StageSelection;
	ENDING m_Ending;
	GIMMICK m_Gimmick;
	ENEMYLUSH m_EnemyLush;
	BOSS m_Boss;
	bool m_GameInitialized = false;
	bool m_TutorialInitialized = false;
	bool m_TutorialInitialized2 = false;
	bool m_TutorialInitialized3 = false;
	bool m_GimmickInitialized = false;
	bool m_EnemyLushInitialized = false;
	bool m_BossInitialized = false;

	int m_ClearCount =3;

	int	g_BgmID{ NULL };

	ENEMYSPAWNER m_EnemySpawner;

public:
	void	Manager_Initialize();
	void	Manager_Finalize();
	void	Manager_Update();
	void	Manager_Draw();

	void	SetScene(SCENE scene);
	SCENE	GetScene() const { return m_Scene; };

	void IncrementClearCount() { ++m_ClearCount; };
	
	void ResetClearCount() { m_ClearCount = 0; };

	int  GetClearCount() const { return m_ClearCount; };
	
	void Manager_BGM_Initialize(SCENE scene);

	void Manager_BGM_Finalize();
};