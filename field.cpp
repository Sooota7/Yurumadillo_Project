
#include "field.h"
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

#define		BOX_NUM_VERTEX (24)

//BOX作成関数
void CreateBox();

int GetMap(int x,int y,int z)
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

void MAPDATA::Field_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	//Test = ModelLoad("asset\\model\\test.fbx");//デバッグ


	g_pDevice = pDevice;
	g_pContext = pContext;

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\block_field.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	int a = 0;

	for (int q = 0; q < 3; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{

				switch (GetMap(l, i, q))
				{
				case 0:
					break;
				case 1:
					m_Map[a].MapData_Initialize(XMFLOAT3(l,q,i),FIELD_BOX);

					break;
				case 2:
					m_Map[a].MapData_Initialize(XMFLOAT3(l, q, i), FIELD_OBT_0);;
					break;
				}

				a++;
			}
		}
	}
	m_Map[a].MapData_Initialize( XMFLOAT3(2.0,-1.0f,5.0f),FIELD_MAX );
	//for (int i = 0; i < BOX_POS_Y; i++)
	//{
	//	for (int l = 0; l < BOX_POS_X; l++)
	//	{
	//		if (i == (BOX_POS_Y - 1) && l == (BOX_POS_X - 1))
	//		{
	//			Map[a] = { XMFLOAT3(2.0,-1.0f,5.0f),FIELD_MAX };
	//		}
	//		else
	//		{
	//			switch ((int)Box_pos[1][l][i])
	//			{
	//			case 0:
	//				break;
	//			case 1:
	//				Map[a] = { XMFLOAT3(l,0,i),FIELD_BOX };

	//				break;
	//			case 2:
	//				Map[a] = { XMFLOAT3(l,1,i),FIELD_OBT_0 };
	//				break;
	//			}
	//		}
	//		a++;
	//	}
	//}

	//ブロックの作成
	for (int i = 0; i < FIELD_MAX; i++)
	{
		switch (i)
		{
		case FIELD_BOX:
			CreateBox();
			break;

		case FIELD_OBT_0://障害物0
			Model[FIELD_OBT_0] = ModelLoad("asset\\model\\tree.fbx");//デバッグ
			break;

		}
	}

}

void  MAPDATA::Field_Finalize(void)
{
	for (int i = 0; i < FIELD_MAX; i++)
	{
		if (Model[i] != NULL)
		{
			ModelRelease(Model[i]);
			Model[i] = NULL;
		}

	}

	SAFE_RELEASE(g_VertexBuffer);
	SAFE_RELEASE(g_IndexBuffer);
	SAFE_RELEASE(g_Texture);
}

void  MAPDATA::Field_Draw(void)
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

	while (m_Map[i].MapData_GetNo() != FIELD_MAX)
	{
		XMFLOAT3 mapPos = m_Map[i].MapData_GetPosition();

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
		if (m_Map[i].MapData_GetNo() == FIELD_BOX)
		{
			g_pContext->DrawIndexed(6 * 6, 0, 0);
		}
		else if(m_Map[i].MapData_GetNo() == FIELD_OBT_0)
		{
			ModelDraw(Model[m_Map[i].MapData_GetNo()]);
		}
		i++;
	}

}

void  MAPDATA::Field_Update(void)
{
}


//BOXデータを作成する
void CreateBox()
{
	{
		//頂点バッファ作成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));//0でクリア
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(Vertex3D) * BOX_NUM_VERTEX; //格納できる頂点数*頂点サイズ
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		g_pDevice->CreateBuffer(&bd, NULL, &g_VertexBuffer);


		//頂点データを頂点バッファへコピーする
		D3D11_MAPPED_SUBRESOURCE msr;
		g_pContext->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		Vertex3D* vertex = (Vertex3D*)msr.pData;
		//頂点データをコピーする
		CopyMemory(&vertex[0], &Box_vdata[0], sizeof(Vertex3D) * BOX_NUM_VERTEX);
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
		CopyMemory(&index[0], &Box_idxdata[0], sizeof(UINT) * 6 * 6);
		g_pContext->Unmap(g_IndexBuffer, 0);

	}

}

MAP* MAPDATA::GetFieldMap()
{
	//return Mapとも書けるが配列と分かりずらいかも
	return m_Map->GetFieldMap();
}
