// ====================================================================
// 
// TargetUI.cpp             制作者: 杉森奏太
// 日付: 02/08
// 
// ====================================================================

#include	"shader.h"
#include	"sprite.h"
#include	"targetUI.h"

#include	"keyboard.h"

void TargetUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	//画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	m_Position = XMFLOAT2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.12f);
	m_Velocity = XMFLOAT2(0.0f, 0.0f);
	m_Scaling = XMFLOAT2(1.0f,1.0f);
	m_Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	bool m_IsView = true;

	//テクスチャ画像読み込み
	// 目標表示バナー
	{
		TexMetadata		metadata;
		ScratchImage	image;
		LoadFromWICFile(L"asset\\texture\\ui\\banner.png",
			WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
		CreateShaderResourceView(pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &m_Texture[0]);
		assert(m_Texture[0]);//読み込み失敗時にダイアログを表示
	}
}

void TargetUI::Finalize()
{
	m_pDevice = nullptr;
	m_pContext = nullptr;

	for (int i = 0; i < T_UI_MAX; i++)
	{
		SAFE_RELEASE(m_Texture[i]);
	}
}

void TargetUI::Update()
{
	if (Keyboard_IsKeyDownTrigger(KK_F))
	{
		m_IsView = 1 - m_IsView;
	}
}

void TargetUI::Draw()
{
	if (!m_IsView) return;
	
	//画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	XMFLOAT2	size = XMFLOAT2(600.0f, 200.0f);

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
		XMMatrixTranslation(m_Position.x, m_Position.y, 0.0f);
	//回転
	XMMATRIX	Rotation = XMMatrixRotationZ(XMConvertToRadians(0.0f));
	//拡大率（0はだめ）
	XMMATRIX	Scaling = XMMatrixScaling(m_Scaling.x, m_Scaling.y, 1.0f);
	//ワールド行列
	XMMATRIX	World = Scaling * Rotation * Translation;
	//スクロール用行列作成
	XMMATRIX	mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	mat = World * mat * Projection;

	m_pContext->PSSetShaderResources(0, 1, &m_Texture[0]);

	//シェーダーへ行列をセット
	Shader_SetMatrix(mat);

	//ブレンド無し
	SetBlendState(BLENDSTATE_ALFA);

	//スプライト描画
	DrawSprite(size, m_Color, 1, 1, 1);
}