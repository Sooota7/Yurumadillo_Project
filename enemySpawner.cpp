#include "enemySpawner.h"
#include "Dictionary.h"
#include "camera.h"


//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;


int GetEnemy(int x, int y, int z)
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
int GetEnemy2(int x, int y, int z)
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
int CheckEnemy(int x, int y, int z, FIELD_NO no)
{
	switch (no)
	{
	case NO_NONE:
		break;
	case NO_1:
		return GetEnemy(x, y, z);
		break;
	case NO_2:
		return GetEnemy2(x, y, z);
		break;
	default:
		break;
	}


}



void ENEMYSPAWNER::EnemySpawner_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\block_field.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	for (int i = 0; i > Enemy_Spawner_MAX; i++)
	{
		m_Enemy[i].Initialize(g_pDevice, g_pContext);
	}

	int a = 0;

	for (int q = 0; q < 3; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{

				switch (CheckEnemy(l, i, q, no))
				{
				case 0:
					break;
				case 6:
					m_Enemy[a].SetEnemyPosition(XMFLOAT3(l, q, i));
					m_Enemy[a].SetEnemyNormalType(ENEMY_TYPE_NORMAL);
					break;
				}

				a++;
			}
		}
	}
	

	//ブロックの作成
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		switch (m_Enemy[i].GetEnemyNormalType())
		{
		case ENEMY_TYPE_NONE:
			break;
		case ENEMY_TYPE_NORMAL:
			m_Model[i] = ModelLoad("asset\\model\\tree.fbx");//デバッグ
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}
	}
}

void ENEMYSPAWNER::EnemySpawner_Finalize(void)
{
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
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

void ENEMYSPAWNER::EnemySpawner_Draw(void)
{
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

	for(int i = 0; i> Enemy_Spawner_MAX;i++)
	{
		XMFLOAT3 mapPos = m_Enemy[i].GetEnemyPosition();

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
			mapPos.x,
			mapPos.y,
			mapPos.z
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
		switch (m_Enemy[i].GetEnemyNormalType())
		{
		case ENEMY_NORMAL_STATE_IDLE:
			ModelDraw(m_Model[i]);
			break;
		case ENEMY_NORMAL_STATE_MOVE:
			ModelDraw(m_Model[i]);
			break;
		case ENEMY_NORMAL_STATE_DIRECTION:
			ModelDraw(m_Model[i]);
			break;
		case ENEMY_NORMAL_STATE_JUMP:
			ModelDraw(m_Model[i]);
			break;
		case ENEMY_NORMAL_STATE_DEAD:

			break;
		default:
			break;
		}
	}

}

void ENEMYSPAWNER::EnemySpawner_Update(XMFLOAT3 pPlayerPos)
{
	for (int i = 0; i > Enemy_Spawner_MAX; i++)
	{
		switch (m_Enemy[i].GetEnemyNormalType())
		{
		case ENEMY_TYPE_NONE:
			break;
		case ENEMY_TYPE_NORMAL:
			m_Enemy[i].Update(pPlayerPos);
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}
	}
	
}

ENEMY_NORMAL* ENEMYSPAWNER::EnemySpawner_GetEnemy()
{
	return m_Enemy;
}
