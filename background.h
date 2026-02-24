#pragma once

// background.h

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

#include "Model.h"
#include "field.h"

// 天球オブジェクト
class BACKGROUND
{
public:
	XMFLOAT3	Position;
	XMFLOAT3	Rotation;
	XMFLOAT3	Scaling;

	float		Speed;		// 回転量（多少雲が動く）

	MODEL* Model;			// 配列にしてステージごとに切り替える

public:
	void Background_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO fn);
	void Background_Finalize(void);
	void Background_Draw(void);
	void Background_Update(void);

	BACKGROUND* GetBackground();
	void SetBackground(FIELD_NO fn);
};


