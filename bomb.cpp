

#include "bomb.h"
#include "camera.h"
#include "Dictionary.h"
#include "player.h"
#include "BossMonster.h"

//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;



//int GetBomb(int x, int y, int z)
//{
//	switch (z)
//	{
//	case(0):
//		return Field_pos_row[y][x];
//
//		break;
//	case(1):
//		return Field_pos_nor[y][x];
//
//
//		break;
//	case(2):
//		return Field_pos_high[y][x];
//
//
//		break;
//	default:
//		break;
//	}
//
//
//}
//int GetBomb2(int x, int y, int z)
//{
//	switch (z)
//	{
//	case(0):
//		return Field2_pos_row[y][x];
//
//		break;
//	case(1):
//		return Field2_pos_nor[y][x];
//
//
//		break;
//	case(2):
//		return Field2_pos_high[y][x];
//
//
//		break;
//	default:
//		break;
//	}
//
//
//}
//
//int GetBomb3(int x, int y, int z)
//{
//	switch (z)
//	{
//	case(0):
//		return tutorial_pos_row[y][x];
//
//		break;
//	case(1):
//		return tutorial_pos_nor[y][x];
//
//
//		break;
//	case(2):
//		return tutorial_pos_high[y][x];
//
//
//		break;
//	default:
//		break;
//	}
//
//
//}
//
//int GetBomb4(int x, int y, int z)
//{
//	switch (z)
//	{
//	case(0):
//		return tutorial_balloon_pos_row[y][x];
//
//		break;
//	case(1):
//		return tutorial_balloon_pos_nor[y][x];
//
//
//		break;
//	case(2):
//		return tutorial_balloon_pos_high[y][x];
//
//
//		break;
//	default:
//		break;
//	}
//
//
//}
//
//int GetBomb5(int x, int y, int z)
//{
//	switch (z)
//	{
//	case(0):
//		return tutorial_mouse_pos_row[y][x];
//
//		break;
//	case(1):
//		return tutorial_mouse_pos_nor[y][x];
//
//
//		break;
//	case(2):
//		return tutorial_mouse_pos_high[y][x];
//
//
//		break;
//	default:
//		break;
//	}
//
//
//}
//
//int GetBomb6(int x, int y, int z)
//{
//	switch (z)
//	{
//	case(0):
//		return boss_pos_row[y][x];
//
//		break;
//	case(1):
//		return boss_pos_nor[y][x];
//
//
//		break;
//	case(2):
//		return boss_pos_high[y][x];
//
//
//		break;
//	default:
//		break;
//	}
//
//
//}
//int CheckBomb(int x, int y, int z,FIELD_NO no)
//{
//	switch (no)
//	{
//	case NO_NONE:
//		break;
//	case NO_1:
//		return GetBomb(x, y, z);
//		break;
//	case NO_2:
//		return GetBomb2(x, y, z);
//		break;
//	case NO_3:
//		return GetBomb3(x, y, z);
//		break;
//	case NO_4:
//		return GetBomb4(x, y, z);
//		break;
//	case NO_5:
//		return GetBomb5(x, y, z);
//		break;
//	case NO_6:
//		return GetBomb6(x, y, z);
//		break;
//
//	default:
//		break;
//	}
//
//
//}

void BOMB::Bomb_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_NorBombModel = ModelLoad("asset\\model\\AliceObjCool.fbx");

	for (int i = 0; i < BOMB_TYPE::TYPE_MAX; i++)
	{
		switch (i)
		{
		case TYPE_NORMAL:
			m_BombModel[i] = ModelLoad("asset\\model\\AliceBomb.fbx");
			break;
		case TYPE_FLOW:
			m_BombModel[i] = ModelLoad("asset\\model\\AliceFlowtBomb.fbx");
			break;
		case TYPE_RUN:
			m_BombModel[i] = ModelLoad("asset\\model\\AliceRunBomb.fbx");
			break;
		case TYPE_MAX:
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < BOMB_TYPE::TYPE_MAX; i++)
	{
		switch (i)
		{
		case TYPE_NORMAL:
			m_ItemModel[i] = ModelLoad("asset\\model\\AliceObj.fbx");
			break;
		case TYPE_FLOW:
			m_ItemModel[i] = ModelLoad("asset\\model\\AliceFlowtBomb.fbx");
			break;
		case TYPE_RUN:
			m_ItemModel[i] = ModelLoad("asset\\model\\AliceRunObj.fbx");
			break;
		case TYPE_MAX:
			break;
		default:
			break;
		}
	}

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
			m_Model[i] = ModelLoad("asset\\model\\test_Bomb.fbx");
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
	int b = 0;
	int c = 0;
	for (int q = 0; q < FIELD_HEIGHT_Y; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{

				switch (CheckMap(l, i, q, no))
				{
				case 0:
					break;
				case 1:
					break;
				case 3:
					m_Bomb[a].BombSource_Initialize(XMFLOAT3(l, q, i), BOMB_STATE::BOMB_ITEM);
					a++;
					break;
				case 4:
					m_RunBomb[b].Initialize_RunBombSpawner(XMFLOAT3(l, q, i));
					b++;
					break;
				case 7:
					m_FlowtBomb[c].Flowtbombsource_Initialize(XMFLOAT3(l, q, i), BOMB_STATE::BOMB_ITEM);
					c++;
					break;
				}


			}
		}
	}
	
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_Bbno[i] = 0;
		m_Rbno[i] = 0;
		m_Fbno[i] = 0;
	}
}

void BOMB::Bomb_Finalize(void)
{
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_Bomb[i].BombSource_Finalize();
	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_RunBomb[i].Finalize_RunBombSpawner();
	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_FlowtBomb[i].Flowtbombsource_Finalize();
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

void BOMB::Bomb_Draw(BillboardManager* billboardManager)
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
			ModelDraw(m_ItemModel[BOMB_TYPE::TYPE_NORMAL]);
			break;
		case BOMB_ACTIVE_HAVE:
			ModelDraw(m_BombModel[BOMB_TYPE::TYPE_NORMAL]);
			m_Bbno[i] = 0;
			break;
		case BOMB_ACTIVE_THROW:
			ModelDraw(m_BombModel[BOMB_TYPE::TYPE_NORMAL]);
			break;

		case BOMB_EXPLOSION:
			//ModelDraw(m_Model[BOMB_EXPLOSION]);//テストはツリー
			{
				XMFLOAT3 pos = m_Bomb[i].BombSource_GetPosition();
				XMFLOAT2 size = XMFLOAT2(3.2f, 3.2f);
				float cnt = m_Bomb[i].BombSource_GetCount();

				int wc = 3;
				int hc = 3;

				if (cnt > (1.0f / (wc * hc)) * (m_Bbno[i] + 1))
				{
					m_Bbno[i]++;
				}
				
				XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				if (m_Bbno[i] < wc * hc)
				{
					Billboard* bb = new Billboard(pos, size, col, m_Bbno[i], wc, hc, BILLBOARD_TEXTURE::EXPLOSION);
					billboardManager->Register(bb);
				}
				

			}

			break;

		case BOMB_EXPLOSION_BOSS:
			//ModelDraw(m_Model[BOMB_EXPLOSION]);//テストはツリー
		{
			XMFLOAT3 pos = m_Bomb[i].BombSource_GetPosition();
			XMFLOAT2 size = XMFLOAT2(30.2f, 30.2f);
			float cnt = m_Bomb[i].BombSource_GetCount();

			int wc = 3;
			int hc = 3;

			if (cnt > (1.0f / (wc * hc)) * (m_Bbno[i] + 1))
			{
				m_Bbno[i]++;
			}

			XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			if (m_Bbno[i] < wc * hc)
			{
				Billboard* bb = new Billboard(pos, size, col, m_Bbno[i], wc, hc, BILLBOARD_TEXTURE::EXPLOSION);
				billboardManager->Register(bb);
			}
		}

		break;

		case BOMB_COOL:

			break;
		}
	}

	//ボムの抜け殻（クールタイム中の描画）
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{

		XMFLOAT3 bombPos = m_Bomb[i].BombSource_GetFirstPosition();

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
			break;
		case BOMB_ITEM:
			break;
		case BOMB_ACTIVE_HAVE:
			ModelDraw(m_NorBombModel);
			break;
		case BOMB_ACTIVE_THROW:
			ModelDraw(m_NorBombModel);
			break;

		case BOMB_EXPLOSION:
			ModelDraw(m_NorBombModel);
			break;

		case BOMB_COOL:
			ModelDraw(m_NorBombModel);
			break;
		}

	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		RUNBOMBSOURCE* runBomb = m_RunBomb[i].GetRunBombSource__RunBombSpawner();


		XMFLOAT3 bombPos = runBomb->Runbombsource_GetPosition();
		XMFLOAT3 bombRot = runBomb->Runbombsource_GetRotation();

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
			bombRot.x,
			bombRot.y,
			bombRot.z
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
		switch (runBomb->Runbombsource_GetState())
		{
		case RUNBOMB_NONE:
			g_pContext->DrawIndexed(6 * 6, 0, 0);
			break;
		case RUNBOMB_ENEMY:
			ModelDraw(m_ItemModel[BOMB_TYPE::TYPE_RUN]);
			break;
		case RUNBOMB_ITEM:
			ModelDraw(m_ItemModel[BOMB_TYPE::TYPE_RUN]);
			break;
		case RUNBOMB_ACTIVE_HAVE:
			ModelDraw(m_BombModel[BOMB_TYPE::TYPE_RUN]);
			m_Rbno[i] = 0;
			break;
		case RUNBOMB_ACTIVE_THROW:
			ModelDraw(m_BombModel[BOMB_TYPE::TYPE_RUN]);
			break;

		case RUNBOMB_EXPLOSION:
			ModelDraw(m_Model[BOMB_EXPLOSION]);//テストはツリー
			{
				XMFLOAT3 pos = runBomb->Runbombsource_GetPosition();
				XMFLOAT2 size = XMFLOAT2(3.2f, 3.2f);
				float cnt = runBomb->Runbombsource_GetCount();

				int wc = 3;
				int hc = 3;

				if (cnt > (1.0f / (wc * hc)) * (m_Rbno[i] + 1))
				{
					m_Rbno[i]++;
				}

				XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				if (m_Rbno[i] < wc * hc)
				{
					Billboard* bb = new Billboard(pos, size, col, m_Rbno[i], wc, hc, BILLBOARD_TEXTURE::EXPLOSION);
					billboardManager->Register(bb);
				}


			}

			break;

		case RUNBOMB_COOL:

			break;
		}


	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{

		XMFLOAT3 bombPos = m_FlowtBomb[i].Flowtbombsource_GetPosition();

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
		switch (m_FlowtBomb[i].Flowtbombsource_GetState())
		{
		case BOMB_NONE:
			g_pContext->DrawIndexed(6 * 6, 0, 0);
			break;
		case BOMB_ITEM:
			ModelDraw(m_ItemModel[BOMB_TYPE::TYPE_FLOW]);
			break;
		case BOMB_ACTIVE_HAVE:
			ModelDraw(m_BombModel[BOMB_TYPE::TYPE_FLOW]);
			m_Fbno[i] = 0;
			break;
		case BOMB_ACTIVE_THROW:
			ModelDraw(m_BombModel[BOMB_TYPE::TYPE_FLOW]);
			break;

		case BOMB_EXPLOSION:
			ModelDraw(m_Model[BOMB_EXPLOSION]);//テストはツリー
			{
				XMFLOAT3 pos = m_FlowtBomb[i].Flowtbombsource_GetPosition();
				XMFLOAT2 size = XMFLOAT2(3.2f, 3.2f);
				float cnt = m_FlowtBomb[i].Flowtbombsource_GetCount();

				int wc = 3;
				int hc = 3;

				if (cnt > (1.0f / (wc * hc)) * (m_Fbno[i] + 1))
				{
					m_Fbno[i]++;
				}

				XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

				if (m_Fbno[i] < wc * hc)
				{
					Billboard* bb = new Billboard(pos, size, col, m_Fbno[i], wc, hc, BILLBOARD_TEXTURE::EXPLOSION);
					billboardManager->Register(bb);
				}


			}
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
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_RunBomb[i].Update_RunBombSpawner(pPlayerPos, pPlayerRot);
	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		switch (m_FlowtBomb[i].Flowtbombsource_GetState())
		{
		case BOMB_NONE:
			break;
		case BOMB_ITEM:
			m_FlowtBomb[i].Flowtbombsource_Safe();
			break;
		case BOMB_ACTIVE_HAVE:
			m_FlowtBomb[i].Flowtbombsource_Active_Have(pPlayerPos,pPlayerRot);
			break;
		case BOMB_ACTIVE_THROW:
			m_FlowtBomb[i].Flowtbombsource_Active_Throw();
			break;
		case BOMB_EXPLOSION:
			m_FlowtBomb[i].Flowtbombsource_Explosion();
			break;
		case BOMB_COOL:
			m_FlowtBomb[i].Flowtbombsource_Cool();
			break;
		default:
			break;
		}
	}
}

void BOMB::Bomb_Update_Boss(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
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
			m_Bomb[i].BombSource_Active_Throw_Boss();
			break;
		case BOMB_EXPLOSION:
			m_Bomb[i].BombSource_Explosion();
			break;
		case BOMB_EXPLOSION_BOSS:
			m_Bomb[i].BombSource_Explosion_Boss();
			break;
		case BOMB_COOL:
			m_Bomb[i].BombSource_Cool();
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_RunBomb[i].Update_RunBombSpawner(pPlayerPos, pPlayerRot);
	}
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		switch (m_FlowtBomb[i].Flowtbombsource_GetState())
		{
		case BOMB_NONE:
			break;
		case BOMB_ITEM:
			m_FlowtBomb[i].Flowtbombsource_Safe();
			break;
		case BOMB_ACTIVE_HAVE:
			m_FlowtBomb[i].Flowtbombsource_Active_Have(pPlayerPos,pPlayerRot);
			break;
		case BOMB_ACTIVE_THROW:
			m_FlowtBomb[i].Flowtbombsource_Active_Throw();
			break;
		case BOMB_EXPLOSION:
			m_FlowtBomb[i].Flowtbombsource_Explosion();
			break;
		case BOMB_COOL:
			m_FlowtBomb[i].Flowtbombsource_Cool();
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

RUNBOMBSPAWNER* BOMB::Bomb_GetRunBomb()
{
	return m_RunBomb->GetRunBombSpawner_RunBombSpawner();
}

FLOWTBOMBSOURCE* BOMB::Bomb_GetFlowtBomb()
{
	return m_FlowtBomb->Flowtbombsource_GetFlowtbombsource();
}

void BOMB::Bomb_SetBoss(BOSSMONSTER* boss)
{
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		m_Bomb[i].BombSource_SetBoss(boss);
	}
}
void BOMB::Bomb_Trail_Draw()
{


}






