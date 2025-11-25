

#include "bomb.h"
<<<<<<< HEAD
#include "camera.h"

=======
>>>>>>> 6d8c39e6b7e5d7419fc0ee9a596a9b39278503eb

//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;




void BOMB::Bomb_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

<<<<<<< HEAD
	for (int i; i < BOMB_STATE::BOMB_MAX; i++)
	{
		switch (i)
		{
		case BOMB_NONE:
			break;
		case BOMB_SAFE:
			break;
		case BOMB_ACTIVE:
			m_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case BOMB_EXPLOSION:
			m_Model[i] = ModelLoad("asset\\model\\tree.fbx");
			break;
		case BOMB_MAX:
			break;
		default:
			break;
		}
	}

	
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_Bomb[i].BombSource_Initialize(XMFLOAT3(0.0f, 2.0f, 0.0f),BOMB_STATE::BOMB_SAFE);
=======
	m_Model[BOMB_ACTIVE] = ModelLoad("asset\\model\\ball.fbx");

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_bomb[i].BombSource_Initialize(XMFLOAT3(0.0f, 2.0f, 0.0f),BOMB_STATE::BOMB_SAFE);
>>>>>>> 6d8c39e6b7e5d7419fc0ee9a596a9b39278503eb
	}
}

void BOMB::Bomb_Finalize(void)
{

}

void BOMB::Bomb_Draw(void)
{
<<<<<<< HEAD

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();


	//プロジェクション行列作成
	XMMATRIX	Projection = GetProjectionMatrix();
	//ビュー行列作成
	XMMATRIX	View = GetViewMatrix();
	//先にVP変換行列を作っておく
	XMMATRIX	VP = View * Projection;

	//MAPの表示
	int i = 0;

	static float rot = 0.0f;
	rot -= 0.5f;

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{

		XMFLOAT3 bombPos = m_Bomb[i].BombSource_GetPosition();

		//スケーリング行列の作成
		XMMATRIX	ScalingMatrix = XMMatrixScaling
		(
			1.0f,
			1.0f,
			1.0f
		);
		//平行移動行列の作成
		XMMATRIX	TranslationMatrix = XMMatrixTranslation
		(
			bombPos.x,
			bombPos.y,
			bombPos.z
		);

		//回転行列の作成
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
		(
			XMConvertToRadians(0.0f),
			//XMConvertToRadians(rot),
			//XMConvertToRadians(rot),
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f)
		);
		//ワールド行列の作成
		XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
		//最終的な変換行列を作成
		XMMATRIX WVP = World * VP;//(VP = View*Projection)
		//DirectXへ行列をセット
		Shader_SetMatrix(WVP);

		//テクスチャをセット
		g_pContext->PSSetShaderResources(0, 1, &g_Texture);

		//頂点バッファをセット
		UINT	stride = sizeof(Vertex3D);	//頂点１個のデータサイズ
		UINT	offset = 0;
		g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		//インデックスバッファをセット
		g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//描画するポリゴンの種類をセット 3頂点でポリゴン１枚として表示
		g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//描画リクエスト
		switch (m_Bomb[i].BombSource_GetState())
		{
		case BOMB_NONE:
			g_pContext->DrawIndexed(6 * 6, 0, 0);
			break;
		case BOMB_SAFE:
			g_pContext->DrawIndexed(6 * 6, 0, 0);
			break;
		case BOMB_ACTIVE:
			ModelDraw(m_Model[m_Bomb[i].BombSource_GetState()]);
			break;
		case BOMB_EXPLOSION:
			g_pContext->DrawIndexed(6 * 6, 0, 0);
=======
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		switch (m_bomb[i].BombSource_GetState())
		{
		case BOMB_NONE:

			break;
		case BOMB_SAFE:

			break;
		case BOMB_ACTIVE:

			break;
		case BOMB_EXPLOSION:

>>>>>>> 6d8c39e6b7e5d7419fc0ee9a596a9b39278503eb
			break;
		default:
			break;
		}
	}
}

void BOMB::Bomb_Update(void)
{
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
<<<<<<< HEAD
		switch (m_Bomb[i].BombSource_GetState())
=======
		switch (m_bomb[i].BombSource_GetState())
>>>>>>> 6d8c39e6b7e5d7419fc0ee9a596a9b39278503eb
		{
		case BOMB_NONE:

			break;
		case BOMB_SAFE:
<<<<<<< HEAD
			m_Bomb[i].BombSource_Safe();
			break;
		case BOMB_ACTIVE:
			m_Bomb[i].BombSource_Active();
			break;
		case BOMB_EXPLOSION:
			m_Bomb[i].BombSource_Explosion();
=======

			break;
		case BOMB_ACTIVE:
			if (m_bomb[i].BombSource_GetCount() > limitTime)
			{

			}
			break;
		case BOMB_EXPLOSION:

>>>>>>> 6d8c39e6b7e5d7419fc0ee9a596a9b39278503eb
			break;
		default:
			break;
		}
	}
}





