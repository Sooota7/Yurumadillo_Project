
////Polygon3D.cpp

#include	"d3d11.h"
#include	"DirectXMath.h"
using namespace DirectX;
#include	"direct3d.h"
#include	"shader.h"
#include	"keyboard.h"
#include	"sprite.h"

#include	"Camera.h"

//グローバル変数
static	ID3D11Device* g_pDevice = NULL;
static	ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static	ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static	ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;

//ポリゴン表示座標
static	XMFLOAT3	Position;
//ポリゴン回転角度
static	XMFLOAT3	Rotation;
//ポリゴン拡大率
static	XMFLOAT3	Scaling;

#define		NUM_VERTEX	(100)

static	Vertex vdata[NUM_VERTEX] =
{
	//-Z面
	{//頂点０　LEFT-TOP
		XMFLOAT3(-0.5f, 0.5f, -0.5f),		//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点１　RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),			//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点２　LEFT-BOTTOM
		XMFLOAT3(-0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},
	//{//頂点3　LEFT-BOTTOM
	//	XMFLOAT3(-0.5f, -0.5f, -0.5f),			//座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
	//	XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	//},
	//{//頂点4　RIGHT-TOP
	//	XMFLOAT3(0.5f, 0.5f, -0.5f),			//座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
	//	XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	//},
	{//頂点3　RIGHT-BTTOM
		XMFLOAT3(0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 1.0f)				//テクスチャ座標
	},

	//+X面
	{//頂点4　LEFT-TOP
		XMFLOAT3(0.5f, 0.5f, -0.5f),			//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点5　RIGHT-TOP
		XMFLOAT3(0.5f, 0.5f, 0.5f),			//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	},
	{//頂点6　LEFT-BTTOM
		XMFLOAT3(0.5f, -0.5f, -0.5f),			//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	},
	//{//頂点9　LEFT-BTTOM
	//	XMFLOAT3(0.5f, -0.5f, -0.5f),			//座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
	//	XMFLOAT2(0.0f, 1.0f)				//テクスチャ座標
	//},
	//{//頂点10　RIGHT-TOP
	//	XMFLOAT3(0.5f, 0.5f, 0.5f),			//座標
	//	XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
	//	XMFLOAT2(1.0f, 0.0f)				//テクスチャ座標
	//},
	{//頂点7　RIGHT-;BOTTOM
		XMFLOAT3(0.5f, -0.5f, 0.5f),			//座標
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),	//色
		XMFLOAT2(1.0f, 1.0f)				//テクスチャ座標
	},

	//残りの4面を作ってみよう！

	//+Z面

	//-X面

	//+Y面

	//-Y面







	//後で増えます


};

//インデックス配列
static UINT idxdata[6 * 6] =
{
	0,  1,  2,  2,  1,  3,	//-Z面
	4,  5,  6,  6,  5,  7,	//+X面


};





void	Polygon3D_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{ 
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//頂点バッファ作成
	D3D11_BUFFER_DESC	bd;
	ZeroMemory(&bd, sizeof(bd));//０でクリア
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;//格納できる頂点数*頂点サイズ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	g_pDevice = pDevice;
	g_pContext = pContext;


	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\texture.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);


	//インデックスバッファ作成
	{
		D3D11_BUFFER_DESC	bd;
		ZeroMemory(&bd, sizeof(bd));//０でクリア
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(UINT) * 6 * 6;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		pDevice->CreateBuffer(&bd, NULL, &g_IndexBuffer);

		//インデックスバッファへ書き込み
		D3D11_MAPPED_SUBRESOURCE   msr;
		pContext->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		UINT* index = (UINT*)msr.pData;

		//インデックスデータをバッファへコピー
		CopyMemory(&index[0], &idxdata[0], sizeof(UINT) * 6 * 6);
		pContext->Unmap(g_IndexBuffer, 0);

	}
}
void	Polygon3D_Finalize()
{
	if (g_IndexBuffer != NULL)
	{
		g_IndexBuffer->Release();
		g_IndexBuffer = NULL;
	}

	if (g_VertexBuffer != NULL)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}


}
void	Polygon3D_Update()
{ 
	//ポリゴンの移動
	if (Keyboard_IsKeyDown(KK_UP))
	{
		Rotation.x += 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_DOWN))
	{
		Rotation.x -= 1.0f;
	}

	//ポリゴンの回転
	if (Keyboard_IsKeyDown(KK_LEFT))
	{
		Rotation.y += 1.0f;
	}
	if (Keyboard_IsKeyDown(KK_RIGHT))
	{
		Rotation.y -= 1.0f;
	}


}
void	Polygon3D_Draw()
{ 
	//====================
	//ワールド行列の作成
	//====================

	//スケーリング行列の作成
	XMMATRIX	ScalingMatrix = XMMatrixScaling
				(
					Scaling.x,
					Scaling.y,
					Scaling.z
				);
	//平行移動行列の作成
	XMMATRIX	TranslationMatrix = XMMatrixTranslation
				(
					Position.x,
					Position.y,
					Position.z
				);
	//回転行列の作成
	XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
				(
					XMConvertToRadians(Rotation.x),
					XMConvertToRadians(Rotation.y),
					XMConvertToRadians(Rotation.z)
				);

	//乗算の順番に注意！！
	XMMATRIX	WorldMatrix =   ScalingMatrix *
								RotationMatrix *
								TranslationMatrix;
	
	//プロジェクション行列作成
	XMMATRIX	Projection = GetProjectionMatrix();
	//ビュー行列作成
	XMMATRIX	View = GetViewMatrix();

	//最終的な変換行列を作成 乗算の順番に注意！！
	XMMATRIX	WVP = WorldMatrix * View * Projection;

	//変換行列を頂点シェーダーへセット
	Shader_SetMatrix(WVP);

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();


	//頂点データを頂点バッファへコピーする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	Vertex* vertex = (Vertex*)msr.pData;
	//頂点データをコピーする
	CopyMemory(&vertex[0], &vdata[0], sizeof(Vertex) * NUM_VERTEX);
	//コピー完了
	g_pContext->Unmap(g_VertexBuffer, 0);

	//テクスチャをセット
	g_pContext->PSSetShaderResources(0, 1, &g_Texture);

	//頂点バッファをセット
	UINT	stride = sizeof(Vertex);	//頂点１個のデータサイズ
	UINT	offset = 0;
	g_pContext->IASetVertexBuffers(0,1, &g_VertexBuffer, &stride, &offset);

	//インデックスバッファをセット
	g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//描画するポリゴンの種類をセット 3頂点でポリゴン１枚として表示
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//描画リクエスト
	//g_pContext->Draw(NUM_VERTEX, 0);

	g_pContext->DrawIndexed(6 * 6, 0, 0);


}



