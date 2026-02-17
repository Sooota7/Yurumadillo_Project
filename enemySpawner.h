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
#include	"EnemyButterfly.h"
#include	"EnemyGround.h"
#include	"weapon.h"

// マクロ定義

#define		Enemy_Spawner_MAX	(100)


class ENEMYSPAWNER
{
private:

	//全滅数
	int MaxNum;

	int NowKillNum;

	ENEMY_NORMAL	m_Enemy[Enemy_Spawner_MAX];
	ENEMY_BUTTERFLY	m_EnemyButterfly[Enemy_Spawner_MAX];
	ENEMY_GROUND	m_EnemyGround[Enemy_Spawner_MAX];	//地面歩く敵追加
	MODEL* m_Model[ENEMY_TYPE_MAX] = { NULL };//デバッグ

	MODEL* m_EG_Model[EG_PARTS_MAX];

	//=============================================================
	// EnemyGroundAnim
	//=============================================================
	ENEMY_GROUND_ANIM m_EG_Anim[ENEMY_GROUND_STATE_MAX];
	

	void ENEMY_GROUND_SetAnimInis();
	void ENEMY_GROUND_UpdateAnim();
	XMFLOAT3 ENEMY_GROUND_AnimPos(ENEMY_GROUND_STATE state, ENEMY_GROUND_PARTS part, PARTS* parts, XMFLOAT3 pos, XMFLOAT3 rot, int frame);

	void	Enemy_Ground_SetAnimHokan(ENEMY_GROUND_STATE state);
	void	Enemy_Ground_SetAnimMove();
	void	Enemy_Ground_SetAnimAttack();

public:

	void	EnemySpawner_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no);
	void	EnemySpawner_Finalize(void);
	void	EnemySpawner_Draw(void);
	void	EnemySpawner_Update(XMFLOAT3 pPlayerPos);
	ENEMY_NORMAL* EnemySpawner_GetEnemy();
	ENEMY_BUTTERFLY* EnemySpawner_GetEnemyButterfly();
	ENEMY_GROUND* EnemySpawner_GetEnemyGround();		//追加

	int		EnemySpawner_GetEnemyNum();
	void	EnemySpawner_SetKillNum(int killnum);
	int		EnemySpawner_GetKillNum();

	bool EnemySpawner_SpawnButterfly(const XMFLOAT3& pos);
	bool EnemySpawner_SpawnNormal(const XMFLOAT3& pos);
};

#endif // !_enemySpawner_H_



