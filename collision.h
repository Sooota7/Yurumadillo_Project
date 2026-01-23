#pragma once

// collision.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"sprite.h"
#include	"shader.h"
using namespace DirectX;


#include	"field.h"
#include	"gimmickData.h"
#include	"Ball.h"
#include	"Enemy.h"
#include	"EnemyNormal.h"
#include	"enemySpawner.h"
#include	"bomb.h"
#include	"weapon.h"

#include    "player.h"

#define COE		(0.5f)
#define GROUND_SLOP (0.05f) // 動くギミックの遊び幅（必要に応じて調整）


#define GATE_PANEL_HALF_W (0.45f)  // 片側パネルのX半幅（＝厚み）: モデルに合わせて
#define GATE_PANEL_HALF_H (0.50f)  // パネルのY半径
#define GATE_PANEL_HALF_D (0.50f)  // パネルのZ半径（奥行


enum COLLISION_HIT
{
	HIT_NONE = 0,	// 当たっていない
	HIT_GROUND,		// 地面に当たった
	HIT_WALL_0,		// 壁に-Zから当たった
	HIT_WALL_1,		// +X
	HIT_WALL_2,		// +Z
	HIT_WALL_3,		// -X
	HIT_WALL_CREAR,		// -X

};

class COLLISION
{
public:
	float	PlayerFieldCollision(PLAYER* pPlayer, MAPDATA* pField);
	float	EnemyFieldCollision(ENEMYSPAWNER* pEnemy, MAPDATA* pField);
	float	PlayerEnemyCollision(PLAYER* pPlayer, ENEMYSPAWNER* pEnemy);
	float	PlayerBombCollision(PLAYER* pPlayer, BOMB* pBomb);
	float	BombFieldCollision(BOMB* pBomb, MAPDATA* pField);
	float	BombEnemyCollision(BOMB* pBomb, ENEMYSPAWNER* pEnemy);
	float	EXPLOSIONEnemyCollision(BOMB* pBomb, ENEMYSPAWNER* pEnemy);
	float	WeaponFieldCollision(WEAPON* pWeapon, MAPDATA* pField);
	float	PlayerWeaponCollision(PLAYER* pPlayer, WEAPON* pWeapon);
	float	PlayerGimmickCollision(PLAYER* pPlayer, GIMMICK_DATA* pGimmick);
	float	PlayerMovingFieldCollision(PLAYER* pPlayer, GIMMICK_DATA* pGimmick);
	float	PlayerGateCollision(PLAYER* pPlayer, GIMMICK_DATA* pGimmick);
	float   EnemyGimmickCollision(ENEMYSPAWNER* pEnemy, GIMMICK_DATA* pGimmick);
	float   EnemyMovingFieldCollision(ENEMYSPAWNER* pEnemy, GIMMICK_DATA* pGimmick);
	float   BombMovingFieldCollision(BOMB* pBomb, GIMMICK_DATA* pGimmick);
	float   BombGimmickCollision(BOMB* pBomb, GIMMICK_DATA* pGimmick);
	float   BombGateCollision(BOMB* pBomb, GIMMICK_DATA* pGimmick);

};