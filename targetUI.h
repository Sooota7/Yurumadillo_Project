// ====================================================================
// 
// PlyerUI.h             êßçÏé“: êôêXëtëæ
// ì˙ït: 02/08
// 
// ====================================================================


#ifndef TARGETUI_H
#define TARGETUI_H

#include <d3d11.h>
#include "direct3d.h"
#include <DirectXMath.h>
using namespace DirectX;

class TargetUI
{
private:
	static constexpr int T_UI_MAX = 2;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	XMFLOAT2 m_Position;
	XMFLOAT2 m_Velocity;
	XMFLOAT2 m_Scaling;
	XMFLOAT4 m_Color;

	float m_Count = 0.0f;
	bool m_IsView = true;

	ID3D11ShaderResourceView* m_Texture[T_UI_MAX] = { NULL };
public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Finalize();
	void Update();
	void Draw();
private:
	void Draw_Banner();
};

#endif  // TARGETUI_H
