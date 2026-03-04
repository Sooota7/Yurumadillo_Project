#include "gimmickData.h"
#include "camera.h"
#include "Dictionary.h"

//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;

#define 		GIMMICK_NUM_VERTEX (24)


//BOX作成関数
void CreateGimmickBox();
//
//int GetGimmick(int x, int y, int z)
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
//
//int GetGimmick2(int x, int y, int z)
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
//int GetGimmick3(int x, int y, int z)
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
//int GetGimmick4(int x, int y, int z)
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
//int GetGimmick5(int x, int y, int z)
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
//int CheckGimmick(int x, int y, int z, FIELD_NO no)
//{
//	switch (no)
//	{
//	case NO_NONE:
//		break;
//	case NO_1:
//		return GetGimmick(x, y, z);
//		break;
//	case NO_2:
//		return GetGimmick2(x, y, z);
//		break;
//	case NO_3:
//		return GetGimmick3(x, y, z);
//		break;
//	case NO_4:
//		return GetGimmick4(x, y, z);
//		break;
//	case NO_5:
//		return GetGimmick4(x, y, z);
//		break;
//	default:
//		break;
//	}
//}

//BOX頂点データ
static	Vertex3D Gimmick_vdata[GIMMICK_NUM_VERTEX] =
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
static UINT Gimmick_idxdata[6 * 6] =
{
	 0, 1, 2, 2, 1, 3,	//-Z面
	 4, 5, 6, 6, 5, 7,	//+X面
	 8, 9,10,10, 9,11,	//+Z面
	12,13,14,14,13,15,	//-X面
	16,17,18,18,17,19,	//+Y面
	20,21,22,22,21,23,	//-Y面
};

void GIMMICK_DATA::Gimmick_Data_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	{
		TexMetadata metadata;
		ScratchImage image;

		// テクスチャ読み込み
		//　マップごとにステージ切り替え
		switch (no)
		{
		case NO_NONE:
			break;
		case NO_1:
			LoadFromWICFile(L"Asset\\Texture\\Gimmik_Map.png", WIC_FLAGS_NONE, &metadata, image);
			break;
		case NO_2:
			LoadFromWICFile(L"Asset\\Texture\\Field_Map.png", WIC_FLAGS_NONE, &metadata, image);
			break;
		case NO_3:
			LoadFromWICFile(L"Asset\\Texture\\Gimmik_Map.png", WIC_FLAGS_NONE, &metadata, image);
			break;
		case NO_4:
			LoadFromWICFile(L"Asset\\Texture\\Boss_Map.png", WIC_FLAGS_NONE, &metadata, image);
			break;
		case NO_5:
			LoadFromWICFile(L"Asset\\Texture\\Boss_Map.png", WIC_FLAGS_NONE, &metadata, image);
			break;
		case NO_6:
			LoadFromWICFile(L"Asset\\Texture\\Boss_Map.png", WIC_FLAGS_NONE, &metadata, image);
			break;
		case NO_ENEMYLUSH:
			LoadFromWICFile(L"Asset\\Texture\\Boss_Map.png", WIC_FLAGS_NONE, &metadata, image);
			break;
		default:
			break;
		}
		CreateShaderResourceView(pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &g_Texture);
		assert(g_Texture);
	}

	for (int i = 0; i < GIMMICK_STATE::GIMMICK_STATE_MAX; i++)
	{
		switch (i)
		{
		case GIMMICK_STATE::GIMMICK_STATE_NONE:
			break;
		case GIMMICK_STATE::GIMMICK_STATE_BUTTON:
			m_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case GIMMICK_STATE::GIMMICK_STATE_FIELD:
			m_Model[i] = ModelLoad("asset\\model\\Gate\\GateRight.fbx");//とりあえず
			CreateGimmickBox();
			break;
		case GIMMICK_STATE::GIMMICK_STATE_GATE:
			m_Model[i] = ModelLoad("asset\\model\\Gate\\GateLeft.fbx");
			break;
		case GIMMICK_STATE_MAX:
			break;
		default:
			break;
		}
	}


	m_ButtonCount = 0;
	m_FieldCount = 0;
	m_GateCount = 0;

	Channels_Reset();

	for (int q = 0; q < FIELD_HEIGHT_Y; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{
				int chip =  CheckMap(l, i, q, no);
				if (chip < 10) { continue; }

				// ギミックの種類
				int num = chip / 10;  // 1 ボタン 2 フィールド 3 ゲート
				// ギミックのチャンネル番号
				int ch = chip % 10;   // チャンネル番号	

				if (num == 1)
				{
					if (m_ButtonCount < GIMMICK_NUM_MAX)
					{
						XMFLOAT3 pos = XMFLOAT3((float)l, (float)q, (float)i);
						m_GimmickButton[m_ButtonCount].GimmickButton_Initialize(pos);
						m_GimmickButton[m_ButtonCount].GimmickButton_SetChannel(ch);
						m_GimmickButton[m_ButtonCount].GimmickButton_SetPosition(pos);
						m_ButtonCount++;
					}
				}
				else if (num == 2)
				{
					if (m_FieldCount < GIMMICK_NUM_MAX)
					{
						XMFLOAT3 pos = XMFLOAT3((float)l, (float)q, (float)i);
						m_GimmickField[m_FieldCount].GimmickField_Initialize(pos);
						m_GimmickField[m_FieldCount].GimmickField_SetChannel(ch);
						// targetPos設定
						// ★ このFieldのチャネル
						int fieldCh = ch;

						bool find = false;
						for (int q = 0; q < FIELD_HEIGHT_Y; q++)
						{
							for (int i = 0; i < FIELD_WIDTH_Z; i++)
							{
								for (int l = 0; l < FIELD_WIDTH_X; l++)
								{

									int targetChip = CheckMap(l, i, q, no);


									int tNum = targetChip / 10;   // 4
									int tCh = targetChip % 10;   // チャネル

									// 同じチャネルの FieldTarget？
									if (tNum == 4)
									{
										if (tCh == fieldCh)
										{
											XMFLOAT3 tgt = XMFLOAT3((float)l, (float)q, (float)i);

											m_GimmickField[m_FieldCount].GimmickField_SetTargetPosition(tgt);
											
										}
									}

								}
							}
						}
						m_FieldCount++;
					}
				}
				else if (num == 3)
				{
					if (m_GateCount < GIMMICK_NUM_MAX)
					{
						XMFLOAT3 pos = XMFLOAT3((float)l, (float)q, (float)i);
						m_GimmickGate[m_GateCount].GimmickGate_Initialize(pos);
						m_GimmickGate[m_GateCount].GimmickGate_SetChannel(ch);
						m_GateCount++;
					}
				}
			}
		}
	}

	SetupGateSides(this);
}

void GIMMICK_DATA::Gimmick_Data_Finalize(void)
{
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{
		m_GimmickButton[i].GimmickButton_Finalize();
	}
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{
		m_GimmickField[i].GimmickField_Finalize();
	}
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{
		m_GimmickGate[i].GimmickGate_Finalize();
	}

	for (int i = 0; i < GIMMICK_STATE::GIMMICK_STATE_MAX; i++)
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

void GIMMICK_DATA::Gimmick_Data_Draw(void)
{

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();


	//プロジェクション行列作成
	XMMATRIX	Projection = GetProjectionMatrix();
	//ビュー行列作成
	XMMATRIX	View = GetViewMatrix();
	//先にVP変換行列を作っておく
	XMMATRIX	VP = View * Projection;

	for (int i = 0; i < m_ButtonCount; i++)
	{

		XMFLOAT3 gimmickPos = m_GimmickButton[i].GimmickButton_GetPosition();

		//スケーリング行列の作成
		XMMATRIX	ScalingMatrix = XMMatrixScaling
		(
			4.0f,
			0.5f,
			4.0f
		);

		//平行移動行列の作成
		XMMATRIX	TranslationMatrix = XMMatrixTranslation
		(
			gimmickPos.x,
			gimmickPos.y,
			gimmickPos.z
		);

		//回転行列の作成
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
		(
			XMConvertToRadians(0.0f),

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
		ModelDraw(m_Model[GIMMICK_STATE::GIMMICK_STATE_BUTTON]);


	}
	for (int i = 0; i < m_FieldCount; i++)
	{

		XMFLOAT3 gimmickPos = m_GimmickField[i].GimmickField_GetPosition();

		//スケーリング行列の作成
		XMMATRIX	ScalingMatrix = XMMatrixScaling
		(
			m_GimmickField[i].m_Scaling.x,
			m_GimmickField[i].m_Scaling.y,
			m_GimmickField[i].m_Scaling.z
		);

		//平行移動行列の作成
		XMMATRIX	TranslationMatrix = XMMatrixTranslation
		(
			gimmickPos.x,
			gimmickPos.y,
			gimmickPos.z
		);

		//回転行列の作成
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
		(
			XMConvertToRadians(0.0f),
			
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
		g_pContext->DrawIndexed(6 * 6, 0, 0);

	}
	for (int i = 0; i < m_GateCount; i++)
	{
		XMFLOAT3 base = m_GimmickGate[i].GimmickGate_GetPosition();
		float open = m_GimmickGate[i].GimmickGate_GetOpen();

		// スライド量
		float offset = GATE_MAX_OPEN_OFFSET * open;

		// 左右で符号を変える
		GATE_SIDE side = m_GimmickGate[i].GimmickGate_GetSide();
		if (side == GATE_SIDE_LEFT)
		{
			offset = -offset;
		}

		// ワールド行列：位置は X に offset を足す
		XMMATRIX ScalingMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);

		XMMATRIX TranslationMatrix = XMMatrixTranslation(base.x + offset, base.y, base.z);


		XMMATRIX RotationMatrix = XMMatrixRotationRollPitchYaw
		(
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f)
		);

		XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
		XMMATRIX WVP = World * VP;
		Shader_SetMatrix(WVP);
		g_pContext->PSSetShaderResources(0, 1, &g_Texture);

		UINT stride = sizeof(Vertex3D);
		UINT offsetVB = 0;
		g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offsetVB);
		g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//描画リクエスト
		
		if (side == GATE_SIDE_LEFT){
			ModelDraw(m_Model[GIMMICK_STATE::GIMMICK_STATE_FIELD]);
		}
		else {
			ModelDraw(m_Model[GIMMICK_STATE::GIMMICK_STATE_GATE]);
		}
	}

}

void GIMMICK_DATA::Gimmick_Data_Update(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
{

	// Button：自前アニメ（沈む）
	for (int i = 0; i < m_ButtonCount; i++)
	{
		m_GimmickButton[i].GimmickButton_Update();
	}

	static int frameCnt = 0;
	static bool turn = false;
	// Field：必要ボタン数 1
	for (int i = 0; i < m_FieldCount; i++)
	{
		// ギミックのチャンネル番号取得
		int ch = m_GimmickField[i].GimmickField_GetChannel();
		int cnt = Channels_GetCount(ch);

		bool on = false;
		if (cnt >= NEED_BUTTONS_FIELD)
		{
			on = true;
		}

		// チャンネル6以降は自動で動くフィールドにする
		if (ch >= 6)
		{
			XMFLOAT3 currentPos = m_GimmickField[i].GimmickField_GetPosition();
			XMFLOAT3 firstPos = m_GimmickField[i].GimmickField_GetFirstPosition();
			XMFLOAT3 targetPos = m_GimmickField[i].GimmickField_GetTargetPosition();

			if ((currentPos.x == firstPos.x && currentPos.z == firstPos.z))
			{
				if (frameCnt == 240)
				{
					on = true;
					turn = true;
					frameCnt = 0;
				}
				else
				{
					frameCnt++;
				}
			}

			if ((currentPos.x == targetPos.x && currentPos.z == targetPos.z))
			{
				if (frameCnt == 240)
				{
					turn = false;
					frameCnt = 0;
				}
				else
				{
					frameCnt++;
				}
			}

			if (turn)
			{
				on = true;
			}
		}

		m_GimmickField[i].GimmickField_Update(on); // on:動作中、off:停止
	}


	// Gate：必要ボタン数 2
	for (int i = 0; i < m_GateCount; i++)
	{
		/*int ch = m_GimmickGate[i].GimmickGate_GetChannel();
		int cnt = Channels_GetCount(ch);

		bool on = true;
		if (cnt >= NEED_BUTTONS_GATE)
		{
			on = false;
		}
		m_GimmickGate[i].GimmickGate_Update(on);*/

		int ch = m_GimmickGate[i].GimmickGate_GetChannel();
		int cnt = Channels_GetCount(ch);
		// 押されている間は閉じ続ける（on=true）
		// 誰も押していなければ開く（on=false）
		bool close = (cnt > 0);
		m_GimmickGate[i].GimmickGate_Update(close);

	}

}

void CreateGimmickBox()
{
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));//0でクリア
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex3D) * GIMMICK_NUM_VERTEX; //格納できる頂点数*頂点サイズ
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);


		//頂点データを頂点バッファへコピーする
		D3D11_MAPPED_SUBRESOURCE msr;
		g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		Vertex3D* vertex = (Vertex3D*)msr.pData;
		//頂点データをコピーする
		CopyMemory(&vertex[0], &Gimmick_vdata[0], sizeof(Vertex3D) * GIMMICK_NUM_VERTEX);
		//コピー完了
		g_pContext->Unmap(g_VertexBuffer, 0);

	}

	//インデックスバッファ作成
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));//0でクリア
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(UINT) * 6 * 6; //格納できる頂点数*頂点サイズ
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_pDevice->CreateBuffer(&bd, NULL, &g_IndexBuffer);

		//インデックスバッファへ書き込み
		D3D11_MAPPED_SUBRESOURCE msr;
		g_pContext->Map(g_IndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		UINT* index = (UINT*)msr.pData;

		//インデックスデータをバッファへコピー
		CopyMemory(&index[0], &Gimmick_idxdata[0], sizeof(UINT) * 6 * 6);
		g_pContext->Unmap(g_IndexBuffer, 0);

	}
}


void GIMMICK_DATA::Channels_Reset()
{
	for (int i = 0; i < MAX_CHANNEL; i++)
	{
		m_ChannelOn[i] = false;
		m_ChannelOnCount[i] = 0;
	}
}


void GIMMICK_DATA::Channels_AddCount(int ch)
{
	if (ch >= 0)
	{
		if (ch < MAX_CHANNEL)
		{
			m_ChannelOn[ch] = true;
			m_ChannelOnCount[ch] += 1;
		}
	}
}


int GIMMICK_DATA::Channels_GetCount(int ch) const
{
	if (ch >= 0)
	{
		if (ch < MAX_CHANNEL)
		{
			return m_ChannelOnCount[ch];
		}
	}
	return 0;
}


bool GIMMICK_DATA::Channels_IsOn(int ch) const
{
	if (ch >= 0)
	{
		if (ch < MAX_CHANNEL)
		{
			return m_ChannelOn[ch];
		}
	}
	return false;
}


void GIMMICK_DATA::SetupGateSides(GIMMICK_DATA* self)
{
	// チャンネルごとに Gate を集めて左右を割り振る
	for (int ch = 0; ch < MAX_CHANNEL; ch++)
	{
		// 同一 ch のゲートを集める
		int idx[2] = { -1, -1 };
		int found = 0;

		for (int i = 0; i < self->m_GateCount; i++)
		{
			if (self->m_GimmickGate[i].GimmickGate_GetChannel() == ch)
			{
				if (found < 2)
				{
					idx[found] = i;
				}
				found++;
			}
		}

		if (found == 2)
		{
			// 中央Xを基準に左右判定（Zが異なる配置でもXで左右を決める）
			XMFLOAT3 p0 = self->m_GimmickGate[idx[0]].GimmickGate_GetPosition();
			XMFLOAT3 p1 = self->m_GimmickGate[idx[1]].GimmickGate_GetPosition();
			float midX = (p0.x + p1.x) * 0.5f;

			if (p0.x < midX)
			{
				self->m_GimmickGate[idx[0]].GimmickGate_SetSide(GATE_SIDE_LEFT);
				self->m_GimmickGate[idx[1]].GimmickGate_SetSide(GATE_SIDE_RIGHT);
			}
			else
			{
				self->m_GimmickGate[idx[0]].GimmickGate_SetSide(GATE_SIDE_RIGHT);
				self->m_GimmickGate[idx[1]].GimmickGate_SetSide(GATE_SIDE_LEFT);
			}
		}
		else if (found == 1)
		{
			// 1枚だけ置かれている場合は暫定で右にしておく（必要なら仕様に合わせて変更）
			self->m_GimmickGate[idx[0]].GimmickGate_SetSide(GATE_SIDE_RIGHT);
		}
	}
}
