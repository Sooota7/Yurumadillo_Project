
#include "Block.h"
#include "camera.h"
#include "model.h"

MODEL* Model[FIELD_MAX] = { NULL };//デバッグ


//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;

#define		BOX_NUM_VERTEX (24)


//BOX頂点データ
static	Vertex3D Box_vdata[BOX_NUM_VERTEX] =
{
	//-Z
	{//頂点０　LEFT-TOP
		XMFLOAT3(-0.5f,0.5f,-0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点１　RIGHT-TOP
		XMFLOAT3(0.5f,0.5f,-0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点２　LEFT-BOTTOM
		XMFLOAT3(-0.5f,-0.5f,-0.5f),	//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点3　RIGHT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,-0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},

	//+X面
	{//頂点4　LEFT-TOP
		XMFLOAT3(0.5f,0.5f,-0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点5　RIGHT-TOP
		XMFLOAT3(0.5f,0.5f,0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点6　LEFT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,-0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点7　RIGHT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},

	//+Z

	{//頂点8　RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),//色
		XMFLOAT2(0.0f,0.0f)//テクスチャ座標
	},
	{//頂点9　LEFT-TOP
		XMFLOAT3(-0.5f,0.5f,0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点10　LEFT-BOTTOM
		XMFLOAT3(0.5f,-0.5f,0.5f),	//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点11　RIGHT-TOP
		XMFLOAT3(-0.5f,-0.5f,0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},
	//-X

	{//頂点12　LEFT-TOP
		XMFLOAT3(-0.5f,0.5f,0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点13　LEFT-BOTTOM
		XMFLOAT3(-0.5f,0.5f,-0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,0.0f)				//テクスチャ座標
	},
	{//頂点14　RIGHT-TOP
		XMFLOAT3(-0.5f,-0.5f,0.5f),		//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(0.0f,1.0f)				//テクスチャ座標
	},
	{//頂点15　RIGHT-TOP
		XMFLOAT3(-0.5f,-0.5f,-0.5f),	//座標
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//色
		XMFLOAT2(1.0f,1.0f)				//テクスチャ座標
	},

	//+Y

	{//頂点16 LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,0.0f)
	},
	{//頂点17 RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,0.0f)
	},
	{//頂点18 LEFT-BOTTOM
		XMFLOAT3(-0.5f, 0.5f, -0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,0.25f)
	},
	{//頂点19 RIGHT-BOTTOM
		XMFLOAT3(0.5f, 0.5f, -0.5f),
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,0.25f)
	},
	//-Y面
	{//頂点20 LEFT-TOP
		XMFLOAT3(-0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,0.75f)
	},
	{//頂点21 RIGHT-TOP
		XMFLOAT3(0.5f, -0.5f, -0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,0.75f)
	},
	{//頂点22 LEFT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(0.0f,1.0f)
	},
	{//頂点23 RIGHT-BOTTOM
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(0.5f,0.5f,0.5f),		//法線
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT2(1.0f,1.0f)
	},

	//後で増えます

};

//インデックス配列
static UINT Box_idxdata[6 * 6] =
{
	 0, 1, 2, 2, 1, 3,	//-Z面
	 4, 5, 6, 6, 5, 7,	//+X面
	 8, 9,10,10, 9,11,	//+Z面
	12,13,14,14,13,15,	//-X面
	16,17,18,18,17,19,	//+Y面
	20,21,22,22,21,23,	//-Y面
};

void MAP::MapData_Initialize(XMFLOAT3 pPos,FIELD pNo)
{
	position = pPos;
	no = pNo;
}

void  MAP::MapData_Finalize(void)
{

}


void  MAP::MapData_Update(void)
{
}


////BOXデータを作成する
//void CreateBox()
//{
//
//}

MAP* MAP::GetFieldMap()
{
	//return Mapとも書けるが配列と分かりずらいかも
	return this;
}
