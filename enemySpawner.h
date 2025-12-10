#pragma once
#ifndef _enemySpawner_H_
#define _enemySpawner_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

#include	"model.h"
#include	"Dictionary.h"
#include	"EnemyNormal.h"

// マクロ定義

#define		Enemy_Spawner_MAX	(100)


class ENEMYSPAWNER
{
private:

	//全滅数
	int MaxNum;

	int NowKillNum;

	ENEMY_NORMAL	m_Enemy[Enemy_Spawner_MAX];
	MODEL* m_Model[ENEMY_TYPE_MAX] = { NULL };//デバッグ

public:

	void	EnemySpawner_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no);
	void	EnemySpawner_Finalize(void);
	void	EnemySpawner_Draw(void);
	void	EnemySpawner_Update(XMFLOAT3 pPlayerPos);
	ENEMY_NORMAL* EnemySpawner_GetEnemy();

	int		EnemySpawner_GetEnemyNum();
	void	EnemySpawner_SetKillNum(int killnum);
	int		EnemySpawner_GetKillNum();

};

#endif // !_enemySpawner_H_


