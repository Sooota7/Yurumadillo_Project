
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
	FIELD_NONE = 0,

	FIELD_BOX,

	FIELD_OBT_0, // 障害物

	FIELD_GOAL, // ゴール

	FIELD_BREAK, // 壊れる床

	FIELD_JUMP, // 跳ねる床

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

	XMFLOAT3	InitPosition;	//ブロックの座標
	XMFLOAT3	LastPosition;	//ブロックの座標

	bool m_Gate;
	bool m_GateOpen;
	bool m_TrueR_FalseL;
	bool m_GateStop;

public:

	void MapData_Initialize(XMFLOAT3 pPos, FIELD pNo);
	void MapData_Finalize(void);
	void MapData_Update(void);

	MAP* GetFieldMap();
	FIELD MapData_GetNo() { return no; };
	XMFLOAT3 MapData_GetPosition() { return position; };
	void MapData_SetPosition(XMFLOAT3 pos) { position = pos; };

	void MapData_SetGate(bool isGate) { m_Gate = isGate; };
	bool MapData_GetGate() { return m_Gate; };

	void MapData_SetGateOpen(bool isOpen) { m_GateOpen = isOpen; };
	bool MapData_GetGateOpen() { return m_GateOpen; };

	void MapData_SetTrueR_FalseL(bool isTrue) { m_TrueR_FalseL = isTrue; };
	bool MapData_GetTrueR_FalseL() { return m_TrueR_FalseL; };

	void MapData_SetLastPosition(XMFLOAT3 pos) { LastPosition = pos; };
	XMFLOAT3 MapData_GetLastPosition() { return LastPosition; };

};

