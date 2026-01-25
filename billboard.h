// ============================================================================
// 
// billboard.h             制作者: 杉森奏太
// 日付: 12/15
//
// ============================================================================

#pragma once

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;

#include "Dictionary.h"

class Billboard
{
public:
	static constexpr int NUM_VERTEX = 6; // 使用できる最大頂点数
	XMFLOAT3	m_Pos{};
	XMFLOAT2	m_Size{};
	XMFLOAT4	m_Color{};
	int			m_Bno = 1;
	int			m_Wc = 1;
	int			m_Hc = 1;

	BILLBOARD_TEXTURE m_Texture;

public:
	Billboard()
	{
		Billboard_Initialize();
	};

	Billboard(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color, int bno, int wc, int hc,
		BILLBOARD_TEXTURE tex)
		:m_Pos(pos), m_Size(size), m_Color(color), m_Bno(bno), m_Wc(wc), m_Hc(hc),m_Texture(tex)
	{
		Billboard_Initialize();
	};

	void Billboard_Initialize();
	void Billboard_Finalize();
	void Billboard_Draw();

	const BILLBOARD_TEXTURE GetTexture() { return m_Texture; }

};