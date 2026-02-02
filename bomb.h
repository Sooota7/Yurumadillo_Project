#pragma once
#ifndef _BOMB_H_
#define _BOMB_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

#include	"model.h"
#include	"bombSource.h"
#include	"RunBombSource.h"
#include	"FlowtBombSource.h"
#include	"Dictionary.h"
#include "player.h"
#include     "billboardManager.h"
// マクロ定義

#define		BOMB_NUM_MAX	(100)
#define		BOMB_TYPE_MAX	(2)


class BOMB
{
private:

	//5秒後爆発
	const int limitTime{ 5 };


	BOMBSOURCE		m_Bomb[BOMB_NUM_MAX];
	int				m_Bbno[BOMB_NUM_MAX];
	RUNBOMBSOURCE	m_RunBomb[BOMB_NUM_MAX];
	int				m_Rbno[BOMB_NUM_MAX];
	FLOWTBOMBSOURCE m_FlowtBomb[BOMB_NUM_MAX];
	int				m_Fbno[BOMB_NUM_MAX];

	MODEL*			m_Model[BOMB_MAX] = { NULL };//デバッグ

public:

	void	Bomb_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,FIELD_NO no);
	void	Bomb_Finalize(void);
	void	Bomb_Draw(BillboardManager* billboardManager);
	void	Bomb_Update(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot);
	BOMBSOURCE* Bomb_GetBomb();
	RUNBOMBSOURCE* Bomb_GetRunBomb();
	FLOWTBOMBSOURCE* Bomb_GetFlowtBomb();
	
};

#endif // !_BOMB_H_


