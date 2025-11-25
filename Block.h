
//field.h
#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "direct3d.h"
#include "sprite.h"
#include "shader.h"
using namespace DirectX;

// マクロ定義
#define		BOX_RADIUS		(0.5f)

//MAP構成ブロックの種類
enum FIELD
{
	FIELD_BOX = 0,

	FIELD_OBT_0, // 障害物

	FIELD_MAX
};

//MAPデータ構造体
class MAP
{
private:
	//	BOMB* m_Bomb;
	XMFLOAT3	position;	//ブロックの座標
	FIELD		no;		//ブロックの種類
	//その他必要な物は追加する

public:

	void MapData_Initialize(XMFLOAT3 pPos, FIELD pNo);
	void MapData_Finalize(void);
	void MapData_Update(void);

	MAP* GetFieldMap();
	FIELD MapData_GetNo() { return no; };
	XMFLOAT3 MapData_GetPosition() { return position; };
};

