
//field.h
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

#include "dictionary.h"
#include "Block.h"
#include "model.h"

// マクロ定義
#define		BOX_RADIUS		(0.5f)


//MAPデータ構造体
class MAPDATA
{
private:
//	BOMB* m_Bomb;
	MAP		m_Map[(FIELD_WIDTH_X * FIELD_WIDTH_Z) * 3 + 1];
	MODEL* Model[FIELD_MAX] = { NULL };//デバッグ
	//その他必要な物は追加する


public:

	void Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Field_Finalize(void);
	void Field_Draw(void);
	void Field_Update(void);

	MAP* GetFieldMap();

};

