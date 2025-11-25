//#pragma once
#ifndef _BOMB_H_
#define _BONB_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"direct3d.h"
using namespace DirectX;

#include	"model.h"
#include	"bombSource.h"

// マクロ定義

#define		BOMB_NUM_MAX	(100)


class BOMB
{
private:

	//5秒後爆発
	const int limitTime{ 5 };


	BOMBSOURCE	m_Bomb[BOMB_NUM_MAX];
	MODEL*		m_Model[BOMB_MAX] = { NULL };//デバッグ

public:

	void	Bomb_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Bomb_Finalize(void);
	void	Bomb_Draw(void);
	void	Bomb_Update(void);


};

#endif // !_BOMB_H_


