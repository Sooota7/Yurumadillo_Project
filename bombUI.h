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

enum BOMBUI_TEXTURE
{
	BOMBUI_HOLDER = 0,
	BOMBUI_ROPE,
	BOMBUI_FIRE,
	BOMBUI_MAX,
};

class BombUI
{
private:
	static constexpr int TEXTURE_MAX = 3;
	static constexpr int BOMB_TEXTURE_MAX = 3;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	BOMB* m_pBomb = { NULL };

	ID3D11ShaderResourceView* m_Texture[TEXTURE_MAX] = { NULL };
	ID3D11ShaderResourceView* m_BombTexture[BOMB_TEXTURE_MAX] = { NULL };

	int m_BombType = 0;		// 1 = 通常爆弾, 2 = 風船爆弾, 3 = ネズミ爆弾
	float m_Count = 0.0f;	// 導火線の長さを決めるためにボムのm_Countをここに代入
	float m_limit = 0.0f;	// 爆弾の起爆時間
public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOMB* pBomb);
	void Finalize();
	void Update();
	void Draw();

private:
	float CheckBombState();		// 返値 爆弾のカウント
	void SetBombTexture();
};

#endif  // BOMBUI_H