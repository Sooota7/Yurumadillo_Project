// ============================================================================
// 
// billboard.h             êßçÏé“: êôêXëtëæ
// ì˙ït: 12/15
//
// ============================================================================

#pragma once

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;

class Billboard
{
private:
	XMFLOAT3	m_Pos{};
	XMFLOAT2	m_Size{};
	XMFLOAT4	m_Color{};
	int			m_Bno = 1;
	int			m_Wc = 1;
	int			m_Hc = 1;

public:
	Billboard()
	{
		Billboard_Initialize();
	};

	Billboard(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color, int bno, int wc, int hc)
		:m_Pos(pos), m_Size(size), m_Color(color), m_Bno(bno), m_Wc(wc), m_Hc(hc)
	{
		Billboard_Initialize();
	};

	void Billboard_Initialize();
	void Billboard_Finalize();
	void Billboard_Draw();
};