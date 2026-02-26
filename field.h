
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

enum FIELD_GATE
{
	FIELD_GATE_RIGHT,
	FIELD_GATE_LEFT_NO,
	FIELD_GATE_LEFT,
	FIELD_GATE_MAX
};

//MAPデータ構造体
class MAPDATA
{
private:
//	BOMB* m_Bomb;
	MAP		m_Map[(FIELD_WIDTH_X * FIELD_WIDTH_Z) * FIELD_HEIGHT_Y + 1];
	MODEL* Model[FIELD_MAX] = { NULL };//デバッグ
	//その他必要な物は追加する

	XMFLOAT3 m_GatePositionRight;
	XMFLOAT3 m_GatePositionLeft;
	MODEL* m_GateModel[FIELD_GATE_MAX];

public:

	void Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no);
	void Field_Finalize(void);
	void Field_Draw(void);
	void Field_Update(bool GateTrue = false);

	MAP* GetFieldMap();

	void SetGatePosition(FIELD_GATE gate, XMFLOAT3 pos) { 
		switch (gate)
		{
		case FIELD_GATE_RIGHT:
			m_GatePositionRight = pos;
			break;
		case FIELD_GATE_LEFT:
			m_GatePositionLeft = pos;
			break;
		default:
			break;
		}
	};

	XMFLOAT3 GetGatePosition(FIELD_GATE gate) {
		switch (gate)
		{
		case FIELD_GATE_RIGHT:
			return m_GatePositionRight;
			break;
		case FIELD_GATE_LEFT:
			return m_GatePositionLeft;
			break;
		default:
			return XMFLOAT3(0, 0, 0);
			break;
		}
	};

};

