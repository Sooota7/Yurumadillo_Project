#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;


#include	"model.h"
#include	"weaponSource.h"
#include	"WeaponEnemyGround.h"
#include	"Dictionary.h"
#include	"player.h"
#include	"enemySpawner.h"

#include	"EffectHit.h"
#include    "EffectSlash.h"
// マクロ定義

#define		WEAPON_NUM_MAX	(100)

class ENEMYSPAWNER;

class WEAPON
{
private:

	WEAPONSOURCE	m_Weapon[WEAPON_NUM_MAX];
	WEAPONENEMYGROUND m_EG_Weapon[WEAPON_NUM_MAX];
	MODEL* m_Model[WEAPON_MAX] = { NULL };//デバッグ
	MODEL* m_EG_Model = { NULL };//デバッグ
	MODEL* m_Nor_Model = { NULL };//デバッグ

	BillboardManager* m_pBm;
	EffectHit m_pEffectHit;
	EffectSlash m_pEffectSlash;
public:

	void	Weapon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BillboardManager* pBm);
	void	Weapon_Finalize(void);
	void	Weapon_Draw(void);
	void	Weapon_Update(XMFLOAT3 playerPos, ENEMYSPAWNER* enemySpawner);
	WEAPONSOURCE* Weapon_GetWeapon();
	WEAPONENEMYGROUND* EG_Weapon_GetWeapon();

	void	SetWeapon(XMFLOAT3 pos);
	void	SetWeaponEG(XMFLOAT3 pos);
	void	SetWeaponNor(XMFLOAT3 pos);
};
