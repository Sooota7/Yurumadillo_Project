#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;


#include	"model.h"
#include	"weaponSource.h"
#include	"Dictionary.h"
#include	"player.h"
#include	"enemySpawner.h"
// マクロ定義

#define		WEAPON_NUM_MAX	(100)

class ENEMYSPAWNER;

class WEAPON
{
private:

	WEAPONSOURCE	m_Weapon[WEAPON_NUM_MAX];
	MODEL* m_Model[WEAPON_MAX] = { NULL };//デバッグ

public:

	void	Weapon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Weapon_Finalize(void);
	void	Weapon_Draw(void);
	void	Weapon_Update(XMFLOAT3 playerPos, ENEMYSPAWNER* enemySpawner);
	WEAPONSOURCE* Weapon_GetWeapon();

	void	SetWeapon(XMFLOAT3 pos);

};
