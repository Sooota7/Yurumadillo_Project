
//Game.h
#pragma once

#include "direct3d.h"
#include "Ball.h"
#include "collision.h"
#include "camera.h"

class GAME
{
private:
	BALL m_Ball;
	MAPDATA m_Map;
	CAMERA m_Camera;
//	BOMB m_Bomb;
//	ENEMY m_Enemy;

	COLLISION collision;

public:
	void Game_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Game_Finalize();
	void Game_Update();
	void Game_Draw();


};
