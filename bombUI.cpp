// ====================================================================
// 
// PlyerUI.cpp             制作者: 杉森奏太
// 日付: 12/18
// 
// ====================================================================

#include	"shader.h"
#include	"sprite.h"
#include	"bombUI.h"

void BombUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOMB* pBomb)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pBomb = pBomb;
	
	{
		//テクスチャ画像読み込み
		TexMetadata		metadata;
		ScratchImage	image;
		LoadFromWICFile(L"asset\\texture\\fade.bmp",
			WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
		CreateShaderResourceView(pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &m_Texture[0]);
		assert(m_Texture);//読み込み失敗時にダイアログを表示
	}


	{
		//テクスチャ画像読み込み
		TexMetadata		metadata;
		ScratchImage	image;
		LoadFromWICFile(L"asset\\texture\\Spade.png",
			WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
		CreateShaderResourceView(pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &m_Texture[1]);
		assert(m_Texture);//読み込み失敗時にダイアログを表示
	}
}

void BombUI::Finalize()
{
	m_pDevice = nullptr;
	m_pContext = nullptr;
	m_pBomb = nullptr;
}

void BombUI::Update()
{
	CheckBombState();
}

void BombUI::Draw()
{
	XMFLOAT3	position = XMFLOAT3(900.0f, 500.0f, 0.0f);
	XMFLOAT2	size = XMFLOAT2(100.0f, 100.0f);
	XMFLOAT4	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャのセット
	m_pContext->PSSetShaderResources(0, 1, &m_Texture[0]);

	//画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	//シェーダーのセット
	Shader_Begin();

	//シェーダーに２D描画の設定をする
	XMMATRIX	Projection = XMMatrixOrthographicOffCenterLH(
		0.0f,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0.0f,
		0.0f,
		1.0f
	);

	//平行移動 表示座標
	XMMATRIX	Translation =
		XMMatrixTranslation(position.x, position.y, 0.0f);
	//回転
	XMMATRIX	Rotation = XMMatrixRotationZ(XMConvertToRadians(0.0f));
	//拡大率（0はだめ）
	XMMATRIX	Scaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//ワールド行列
	XMMATRIX	World = Scaling * Rotation * Translation;
	//スクロール用行列作成
	XMMATRIX	mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	mat = World * mat * Projection;

	//シェーダーへ行列をセット
	Shader_SetMatrix(mat);

	//ブレンド無し
	SetBlendState(BLENDSTATE_ALFA);

	//UI背景描画 ================
	DrawSprite(size, color, 1, 1, 1);

	if (m_BombType == 100)
	{
		return;
	}


	// 爆弾のタイプ毎のテクスチャを設定
	SetBombTexture();
	size = XMFLOAT2(50.0f, 50.0f);
	DrawSprite(size, color, 1, 1, 1);
}

void BombUI::CheckBombState()
{
	BOMBSOURCE* bombs;

	bombs = m_pBomb->Bomb_GetBomb();

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		if (bombs[i].BombSource_GetState() == BOMB_ACTIVE_HAVE)
		{
			m_BombType = 1;
			break;
		}
		else
		{
			m_BombType = 100;
		}
	}
}

void BombUI::SetBombTexture()
{
	switch (m_BombType)
	{
	case 1:
		//テクスチャのセット
		m_pContext->PSSetShaderResources(0, 1, &m_Texture[1]);
		break;
	}
}