// ============================================================================
// 
// billboardManager.h             êßçÏé“: êôêXëtëæ
// ì˙ït: 12/16
//
// ============================================================================

#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

#include "billboard.h"
#include "Dictionary.h"

class BillboardManager
{
private:
	static constexpr int BILLBOARD_MAX = 256; // égópÇ≈Ç´ÇÈç≈ëÂ
	int m_Count;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pContext;

	Billboard* m_pBillboard[BILLBOARD_MAX];

	ID3D11ShaderResourceView* m_pSRV[BILLBOARD_TEXTURE::MAX];

public:
	void Initialize(ID3D11Device* m_pDevice, ID3D11DeviceContext* m_pContext, FIELD_NO fn);
	void Finalize();
	void Draw();
	void Update();

	void Register(Billboard* pBillboard);

	void Initialize_BombEffect(TexMetadata* tm, ScratchImage* si, FIELD_NO fn);
};

