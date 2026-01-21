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

int GetGimmick(int x, int y, int z)
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

int GetGimmick2(int x, int y, int z)
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

int GetGimmick3(int x, int y, int z)
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

int GetGimmick4(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return tutorial_balloon_pos_row[y][x];

		break;
	case(1):
		return tutorial_balloon_pos_nor[y][x];


		break;
	case(2):
		return tutorial_balloon_pos_high[y][x];


		break;
	default:
		break;
	}


}

int GetGimmick5(int x, int y, int z)
{
	switch (z)
	{
	case(0):
		return tutorial_mouse_pos_row[y][x];

		break;
	case(1):
		return tutorial_mouse_pos_nor[y][x];


		break;
	case(2):
		return tutorial_mouse_pos_high[y][x];


		break;
	default:
		break;
	}


}

int CheckGimmick(int x, int y, int z, FIELD_NO no)
{
	switch (no)
	{
	case NO_NONE:
		break;
	case NO_1:
		return GetGimmick(x, y, z);
		break;
	case NO_2:
		return GetGimmick2(x, y, z);
		break;
	case NO_3:
		return GetGimmick3(x, y, z);
		break;
	case NO_4:
		return GetGimmick4(x, y, z);
		break;
	case NO_5:
		return GetGimmick4(x, y, z);
		break;
	default:
		break;
	}
}

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
			CreateGimmickBox();
			break;
		case GIMMICK_STATE::GIMMICK_STATE_GATE:
			CreateGimmickBox();
			break;
		case GIMMICK_STATE_MAX:
			break;
		default:
			break;
		}
	}


	int a = 0;
	int b = 0;
	int c = 0;
	for (int q = 0; q < 3; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{

				switch (CheckGimmick(l, i, q, no))
				{
				case 0:
					break;
				case 1:
					break;
				case 3:
					m_GimmickButton[a].GimmickButton_Initialize(XMFLOAT3(l, q, i));
					a++;
					break;
				case 4:
					m_GimmickField[b].GimmickField_Initialize(XMFLOAT3(l, q, i));
					b++;
					break;
				case 7:
					m_GimmickGate[c].GimmickGate_Initialize(XMFLOAT3(l, q, i));
					c++;
					break;
				}


			}
		}
	}

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

	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{

		XMFLOAT3 gimmickPos = m_GimmickButton[i].GimmickButton_GetPosition();

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
			gimmickPos.x,
			gimmickPos.y,
			gimmickPos.z
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
		ModelDraw(m_Model[GIMMICK_STATE_BUTTON]);


	}
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{

		XMFLOAT3 gimmickPos = m_GimmickField[i].GimmickField_GetPosition();

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
			gimmickPos.x,
			gimmickPos.y,
			gimmickPos.z
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
		g_pContext->DrawIndexed(6 * 6, 0, 0);

	}
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{

		XMFLOAT3 gimmickPos = m_GimmickGate[i].GimmickGate_GetPosition();

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
			gimmickPos.x,
			gimmickPos.y,
			gimmickPos.z
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
		g_pContext->DrawIndexed(6 * 6, 0, 0);
		
	}
}

void GIMMICK_DATA::Gimmick_Data_Update(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
{
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{
		// ボタンの更新
	}
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{
		// フィールドの更新
	}
	for (int i = 0; i < GIMMICK_NUM_MAX; i++)
	{
		// ゲートの更新
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