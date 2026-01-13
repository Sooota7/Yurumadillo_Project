

#include "bomb.h"
#include "camera.h"
#include "Dictionary.h"
#include "player.h"


//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;



int GetBomb(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return Field_pos_row[y][x];

		break;
	case(1):
		return Field_pos_nor[y][x];


		break;
	case(2):
		return Field_pos_high[y][x];


		break;
	default:
		break;
	}


}
int GetBomb2(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return Field2_pos_row[y][x];

		break;
	case(1):
		return Field2_pos_nor[y][x];


		break;
	case(2):
		return Field2_pos_high[y][x];


		break;
	default:
		break;
	}


}

int GetBomb3(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return tutorial_pos_row[y][x];

		break;
	case(1):
		return tutorial_pos_nor[y][x];


		break;
	case(2):
		return tutorial_pos_high[y][x];


		break;
	default:
		break;
	}


}
int CheckBomb(int x, int y, int z,FIELD_NO no)
{
	switch (no)
	{
	case NO_NONE:
		break;
	case NO_1:
		return GetBomb(x, y, z);
		break;
	case NO_2:
		return GetBomb2(x, y, z);
		break;
	case NO_3:
		return GetBomb3(x, y, z);
		break;
	default:
		break;
	}


}

void BOMB::Bomb_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	for (int i = 0; i < BOMB_STATE::BOMB_MAX; i++)
	{
		switch (i)
		{
		case BOMB_NONE:
			break;
		case BOMB_ITEM:
			m_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case BOMB_ACTIVE_HAVE:
			m_Model[i] = ModelLoad("asset\\model\\test_bomb.fbx");
			break;
		case BOMB_EXPLOSION:
			m_Model[i] = ModelLoad("asset\\model\\test_explosion.fbx");
			break;
		case BOMB_MAX:
			break;
		default:
			break;
		}
	}

	
	/*for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_Bomb[i].BombSource_Initialize(XMFLOAT3(0.0f, 2.0f, 0.0f), BOMB_STATE::BOMB_SAFE);
		m_Model[BOMB_ACTIVE] = ModelLoad("asset\\model\\ball.fbx");
	}*/

	int a = 0;
	for (int q = 0; q < 3; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{

				switch (CheckBomb(l, i, q, no))
				{
				case 0:
					break;
				case 1:
					break;
				case 3:
					m_Bomb[a].BombSource_Initialize(XMFLOAT3(l, q, i), BOMB_STATE::BOMB_ITEM);
					a++;
					break;
				}


			}
		}
	}
	
}

void BOMB::Bomb_Finalize(void)
{
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_Bomb[i].BombSource_Finalize();
	}

	for (int i = 0; i < BOMB_STATE::BOMB_MAX; i++)
	{
		if (m_Model[i] != NULL)
		{
			ModelRelease(m_Model[i]);
			m_Model[i] = NULL;
		}

	}

	SAFE_RELEASE(g_VertexBuffer);
	SAFE_RELEASE(g_IndexBuffer);
	SAFE_RELEASE(g_Texture);
}

void BOMB::Bomb_Draw(void)
{

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();


	//プロジェクション行列作成
	XMMATRIX	Projection = GetProjectionMatrix();
	//ビュー行列作成
	XMMATRIX	View = GetViewMatrix();
	//先にVP変換行列を作っておく
	XMMATRIX	VP = View * Projection;

	
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
		case BOMB_ITEM:
			ModelDraw(m_Model[BOMB_ITEM]);
			break;
		case BOMB_ACTIVE_HAVE:
			ModelDraw(m_Model[BOMB_ACTIVE_HAVE]);
			break;
		case BOMB_ACTIVE_THROW:
			ModelDraw(m_Model[BOMB_ACTIVE_HAVE]);
			break;

		case BOMB_EXPLOSION:
			ModelDraw(m_Model[BOMB_EXPLOSION]);//テストはツリー
			break;

		case BOMB_COOL:
			
			break;
		}


	}
}

void BOMB::Bomb_Update(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
{
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		switch (m_Bomb[i].BombSource_GetState())
		{
		case BOMB_NONE:
			break;
		case BOMB_ITEM:
			m_Bomb[i].BombSource_Safe();
			break;
		case BOMB_ACTIVE_HAVE:
			m_Bomb[i].BombSource_Active_Have(pPlayerPos,pPlayerRot);
			break;
		case BOMB_ACTIVE_THROW:
			m_Bomb[i].BombSource_Active_Throw();
			break;
		case BOMB_EXPLOSION:
			m_Bomb[i].BombSource_Explosion();
			break;
		case BOMB_COOL:
			m_Bomb[i].BombSource_Cool();
			break;
		default:
			break;
		}
	}
}

BOMBSOURCE* BOMB::Bomb_GetBomb()
{
	return m_Bomb->BombSource_GetBombSource();
}






