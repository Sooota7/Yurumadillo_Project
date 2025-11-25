

#include "bomb.h"

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

	m_Model[BOMB_ACTIVE] = ModelLoad("asset\\model\\ball.fbx");

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_bomb[i].BombSource_Initialize(XMFLOAT3(0.0f, 2.0f, 0.0f),BOMB_STATE::BOMB_SAFE);
	}
}

void BOMB::Bomb_Finalize(void)
{

}

void BOMB::Bomb_Draw(void)
{
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
		switch (m_bomb[i].BombSource_GetState())
		{
		case BOMB_NONE:

			break;
		case BOMB_SAFE:

			break;
		case BOMB_ACTIVE:
			if (m_bomb[i].BombSource_GetCount() > limitTime)
			{

			}
			break;
		case BOMB_EXPLOSION:

			break;
		default:
			break;
		}
	}
}





