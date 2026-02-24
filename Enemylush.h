
//Enemylush.h
#pragma once

#include "direct3d.h"
#include "player.h"
#include "Ball.h"
#include "collision.h"
#include "camera.h"
#include "enemySpawner.h"
#include "bomb.h"
#include "weapon.h"
#include "Dictionary.h"
#include "billboardManager.h"
#include "background.h"
#include "gimmickData.h"

#include "playerUI.h"
#include "bombUI.h"
#include "targetUI.h"
class MANAGER;

enum ENEMYLUSH_STATE
{
	EL_STATE_PHASE01,
	EL_STATE_PHASE02,
	EL_STATE_PHASE03,
	EL_STATE_END,
	EL_STATE_MAX
};

class ENEMYLUSH_PHASE
{
private:
	int EN_TOTAL;
	int EF_TOTAL;
	int EG_TOTAL;

public:
	void Set_EN_TOTAL(int max) { EN_TOTAL = max; };
	int Get_EN_TOTAL() { return EN_TOTAL; };

	void Set_EF_TOTAL(int max) { EF_TOTAL = max; };
	int Get_EF_TOTAL() { return EF_TOTAL; };

	void Set_EG_TOTAL(int max) { EG_TOTAL = max; };
	int Get_EG_TOTAL() { return EG_TOTAL; };
};

class ENEMYLUSH
{
private:
	MANAGER* m_Manager;
	PLAYER m_Player;
	MAPDATA m_Map;
	CAMERA m_Camera;
	BOMB	m_bomb;
	ENEMYSPAWNER m_EnemyNormal;
	WEAPON m_Weapon;

	GIMMICK_DATA m_GimmickData;
	ENEMYLUSH_STATE m_EnemyLushState;

	COLLISION collision;

	FIELD_NO m_NowField;

	BillboardManager m_BillboardManager;
	BACKGROUND m_Background;

	PlayerUI m_PlayerUI;
	BombUI m_BombUI;
	TargetUI m_TargetUI;

	ENEMYLUSH_PHASE m_Phase[EL_STATE_MAX];
	ENEMYLUSH_STATE m_EL_State;
	float m_SpawnTime;
	XMFLOAT3 m_SpawnPos[10];
	int m_TotalSpawn;
	bool m_NextSpawn = false;
	int m_SpawnMax = 0;



public:
	void Enemylush_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager);
	void Enemylush_Finalize();
	void Enemylush_Update();
	void Enemylush_Draw();

	void Enemylush_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no );

	//===================================================================================================
	// EnemyLushPhase
	//===================================================================================================

	void Enemylush_Phase_Initialize();
	void Enemylush_Phase_Finalize();
	void Enemylush_Phase_Update();
	void Enemylush_Phase_Draw();

};
