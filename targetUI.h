// ====================================================================
// 
// PlyerUI.h             制作者: 杉森奏太
// 日付: 02/08
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
	static constexpr float T_UI_SPEED = 180.0f;

	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	XMFLOAT2 m_FirstPosition;		// 初期位置
	XMFLOAT2 m_Position;			// 現在の位置
	XMFLOAT2 m_DisplayPosition;		// 表示位置
	XMFLOAT2 m_Velocity;			// 移動速度
	XMFLOAT2 m_Scaling;				// 拡大率
	XMFLOAT4 m_Color;				// カラー

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
