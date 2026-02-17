
//Game.h
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
class MANAGER;

class GAME
{
private:
	MANAGER* m_Manager;
	PLAYER m_Player;
	MAPDATA m_Map;
	CAMERA m_Camera;
	BOMB	m_bomb;
	ENEMYSPAWNER m_EnemyNormal;
	WEAPON m_Weapon;

	COLLISION collision;

	FIELD_NO m_NowField;

	BillboardManager m_BillboardManager;
	BACKGROUND m_Background;

	PlayerUI m_PlayerUI;
	BombUI m_BombUI;
	TargetUI m_TargetUI;
public:
	void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager);
	void Game_Finalize();
	void Game_Update();
	void Game_Draw();

	void Game_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no );

};
