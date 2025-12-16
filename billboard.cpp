// ============================================================================
// 
// billboard.cpp             制作者: 杉森奏太
// 日付: 12/16
//
// ============================================================================


#include "billboard.h"
#include "Camera.h"
#include "shader.h"

//グローバル変数

static ID3D11Buffer* g_pVertexBuffer = nullptr; // 頂点バッファ
// 注意！初期化で外部から設定されるもの。Release不要。
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


void Billboard::Billboard_Initialize()
{
	g_pDevice = Direct3D_GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex3D) * NUM_VERTEX;//<<<<<<<格納する最大頂点数
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
}

void Billboard::Billboard_Finalize()
{
	g_pVertexBuffer->Release();	//頂点バッファの解放
}

void Billboard::Billboard_Draw()
{

	g_pDevice = Direct3D_GetDevice();
	g_pContext = Direct3D_GetDeviceContext();

	// 頂点バッファをロックする
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// 頂点バッファへの仮想ポインタを取得
	Vertex3D* v = (Vertex3D*)msr.pData;

	//ブロックの縦横サイズを計算
	float w = 1.0f / m_Wc;
	float h = 1.0f / m_Hc;
	//m_Bnoの左上のテクスチャ座標を計算
	float x = (m_Bno % m_Wc) * w;
	float y = (m_Bno / m_Wc) * h;

	// 指定の位置に指定のサイズ、色の四角形を描画する /////////テクスチャ追加
	v[0].position = { -(m_Size.x / 2), (m_Size.y / 2), 0.0f };
	v[0].color = m_Color;
	v[0].texCoord = { x, y };

	v[1].position = { (m_Size.x / 2), (m_Size.y / 2), 0.0f };
	v[1].color = m_Color;
	v[1].texCoord = { x + w, y };

	v[2].position = { -(m_Size.x / 2), -(m_Size.y / 2), 0.0f };
	v[2].color = m_Color;
	v[2].texCoord = { x, y + h };

	v[3].position = { (m_Size.x / 2), -(m_Size.y / 2), 0.0f };
	v[3].color = m_Color;
	v[3].texCoord = { x + w, y + h };


	// 頂点バッファのロックを解除
	g_pContext->Unmap(g_pVertexBuffer, 0);


	// 頂点バッファを描画パイプラインに設定
	UINT stride = sizeof(Vertex3D);//頂点１つあたりのサイズを指定
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定　ポリゴンの描画ルール的なもの
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//変換行列作成
	XMMATRIX	view = GetViewMatrix();
	XMMATRIX	projection = GetProjectionMatrix();

	XMMATRIX	vm = GetViewMatrix(); // カメラの行列
	vm.r[3].m128_f32[0] = 0.0f;  // カメラの位置座標をリセット
	vm.r[3].m128_f32[1] = 0.0f;
	vm.r[3].m128_f32[2] = 0.0f;
	vm.r[3].m128_f32[3] = 1.0f;
	vm = XMMatrixTranspose(vm);  // 転置する（逆行列）
	vm.r[3].m128_f32[0] = m_Pos.x;  // ビルボードの位置座標
	vm.r[3].m128_f32[1] = m_Pos.y;
	vm.r[3].m128_f32[2] = m_Pos.z;
	vm.r[3].m128_f32[3] = 1.0f;

	XMMATRIX wvp = vm * view * projection;  // 変換行列作成
	Shader_SetMatrix(wvp);

	// ポリゴン描画命令発行
	g_pContext->Draw(4, 0);//表示に使用する頂点数を指定

}
