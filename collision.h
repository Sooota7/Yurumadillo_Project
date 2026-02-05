#pragma once

// collision.h

#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
#include	"sprite.h"
#include	"shader.h"
using namespace DirectX;


#include	"field.h"
#include	"Ball.h"
#include	"Enemy.h"
#include	"EnemyNormal.h"
#include	"enemySpawner.h"
#include	"bomb.h"
#include	"weapon.h"
#include	"BossObj.h"
#include    "player.h"

#define COE		(0.5f)


enum COLLISION_HIT
{
	HIT_NONE = 0,	// “–‚½‚Á‚Ä‚¢‚È‚¢
	HIT_GROUND,		// ’n–Ê‚É“–‚½‚Á‚½
	HIT_WALL_0,		// •Ç‚É-Z‚©‚ç“–‚½‚Á‚½
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
	float	BossObjPlayerCollision(BOSSOBJ* pBossObj, PLAYER* pPlayer);
};