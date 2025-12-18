
//Enemylush.h
#pragma once

#include "direct3d.h"
#include "player.h"
#include "Ball.h"
#include "collision.h"
#include "camera.h"
#include "enemySpawner.h"
#include "bomb.h"
#include "Dictionary.h"
#include "billboardManager.h"
class MANAGER;

class ENEMYLUSH
{
private:
	MANAGER* m_Manager;
	PLAYER m_Player;
	MAPDATA m_Map;
	CAMERA m_Camera;
	BOMB	m_bomb;
	ENEMYSPAWNER m_EnemyNormal;

	COLLISION collision;

	FIELD_NO m_NowField;

	BillboardManager m_BillboardManager;

public:
	void Enemylush_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MANAGER* manager);
	void Enemylush_Finalize();
	void Enemylush_Update();
	void Enemylush_Draw();

	void Enemylush_SetNextMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no );

};
