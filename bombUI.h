// ====================================================================
// 
// PlyerUI.h             制作者: 杉森奏太
// 日付: 12/18
// 
// ====================================================================

#pragma once
#ifndef BOMBUI_H
#define BOMBUI_H

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "bomb.h"


class BombUI
{
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	BOMB* m_pBomb = { NULL };

	ID3D11ShaderResourceView* m_Texture[2] = { NULL };

	int m_BombType = 0; // 仮置き　ボムの種類の型（があれば）に変更
public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOMB* pBomb);
	void Finalize();
	void Update();
	void Draw();

private:
	void CheckBombState();
	void SetBombTexture();
};

#endif  // BOMBUI_H