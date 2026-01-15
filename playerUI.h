// ====================================================================
// 
// PlyerUI.h             êßçÏé“: êôêXëtëæ
// ì˙ït: 12/18
// 
// ====================================================================

#pragma once
#ifndef PLAYERUI_H
#define PLAYERUI_H

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;
#include "player.h"


class PlayerUI
{
private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	PLAYER* m_pPlayer = { NULL };

	int m_HpDigit;

	ID3D11ShaderResourceView* m_Texture = NULL;
public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,PLAYER* pPlayer);
	void Finalize();
	void Update();
	void Draw();

private:
	void Update_HpDigit();		//	åªç›ÇÃëÃóÕÇçXêV
	void Draw_HpDigit();
};

#endif  // PLAYERUI_H
