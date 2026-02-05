
//Tutorial2.h
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

class MANAGER;

class TUTORIAL2
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
public:
	void Tutorial2_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager);
	void Tutorial2_Finalize();
	void Tutorial2_Update();
	void Tutorial2_Draw();

	void Tutorial2_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no );

};
