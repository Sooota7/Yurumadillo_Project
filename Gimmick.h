
//Gimmick.h
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

#include "playerUI.h"
#include "bombUI.h"
#include "targetUI.h"
#include "Goal.h"

#include "load.h"

class MANAGER;

class GIMMICK
{
private:
	MANAGER* m_Manager;
	PLAYER m_Player;
	MAPDATA m_Map;
	GIMMICK_DATA m_GimmickData;
	CAMERA m_Camera;
	BOMB	m_bomb;
	ENEMYSPAWNER m_EnemyNormal;
	WEAPON m_Weapon;
	GOAL m_Goal;

	COLLISION collision;

	FIELD_NO m_NowField;

	BillboardManager m_BillboardManager;
	BACKGROUND m_Background;

	PlayerUI m_PlayerUI;
	BombUI m_BombUI;
	TargetUI m_TargetUI;

	LOAD_MANAGER m_SceneLoad;

public:
	void Gimmick_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager);
	void Gimmick_Finalize();
	void Gimmick_Update();
	void Gimmick_Draw();

	void Gimmick_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no );

	void Gimmick_LoadUpdate();

};
