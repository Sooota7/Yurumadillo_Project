//Boss.h
#pragma once

#include "direct3d.h"
#include "player.h"
#include "collision.h"
#include "camera.h"
#include "enemySpawner.h"
#include "bomb.h"
#include "weapon.h"
#include "Dictionary.h"
#include "billboardManager.h"
#include "BossMonster.h" // �ǉ�: BOSSMONSTER�^�̐錾���K�v
#include "BossObj.h"

#include "background.h"

#include "playerUI.h"
#include "bombUI.h"
#include "targetUI.h"

#include "load.h"
#include "gimmickData.h"
class MANAGER;

class BOSS
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

	BOSSMONSTER m_BossMonster;
	COLLISION collision;

	FIELD_NO m_NowField;

	BillboardManager m_BillboardManager;
	BACKGROUND m_Background;

	PlayerUI m_PlayerUI;
	BombUI m_BombUI;
	TargetUI m_TargetUI;

	LOAD_MANAGER m_SceneLoad;
public:
	void Boss_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager);
	void Boss_Finalize();
	void Boss_Update();
	void Boss_Draw();

	void Boss_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no );

	void Boss_LoadUpdate();
};
