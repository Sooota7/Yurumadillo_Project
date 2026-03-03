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

void TargetUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO fn)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	//画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	m_FirstPosition = XMFLOAT2(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * -0.12f);
	m_Position = m_FirstPosition;
	m_DisplayPosition = XMFLOAT2(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.12f);
	m_Velocity = XMFLOAT2(0.0f, (1 / 60.0f) * T_UI_SPEED);
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
			image.GetImageCount(), metadata, &m_Texture[T_UI_BANNER]);
		assert(m_Texture[0]);//読み込み失敗時にダイアログを表示
	}

	// 目標アイコン
	{
		TexMetadata		metadata;
		ScratchImage	image;

		switch (fn)
		{
		case FIELD_NO::NO_1:
			LoadFromWICFile(L"asset\\texture\\ui\\TargetUI_01.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_Texture[T_UI_TARGET]);
			assert(m_Texture[1]);//読み込み失敗時にダイアログを表示
			break;
		case FIELD_NO::NO_2:
			LoadFromWICFile(L"asset\\texture\\ui\\TargetUI_01.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_Texture[T_UI_TARGET]);
			assert(m_Texture[1]);//読み込み失敗時にダイアログを表示
			break;
		case FIELD_NO::NO_6:
			LoadFromWICFile(L"asset\\texture\\ui\\TargetUI_03.png",
					WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
				CreateShaderResourceView(pDevice, image.GetImages(),
					image.GetImageCount(), metadata, &m_Texture[T_UI_TARGET]);
				assert(m_Texture[1]);//読み込み失敗時にダイアログを表示
				break;
		case FIELD_NO::NO_ENEMYLUSH:
			LoadFromWICFile(L"asset\\texture\\ui\\TargetUI_02.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_Texture[T_UI_TARGET]);
			assert(m_Texture[1]);//読み込み失敗時にダイアログを表示
			break;
		default:
			LoadFromWICFile(L"asset\\texture\\ui\\TargetUI_02.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_Texture[T_UI_TARGET]);
			assert(m_Texture[1]);//読み込み失敗時にダイアログを表示
			break;
		}
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
	if (m_IsView)
	{
		if (m_Position.y < m_DisplayPosition.y)
		{
			m_Position.x += m_Velocity.x;
			m_Position.y += m_Velocity.y;
		}
		else
		{
			m_Position = m_DisplayPosition;
		}
	}
	else
	{
		if (m_Position.y > m_FirstPosition.y)
		{
			m_Position.x -= m_Velocity.x;
			m_Position.y -= m_Velocity.y;
		}
		else
		{
			m_Position = m_FirstPosition;
		}
	}

	if (Keyboard_IsKeyDownTrigger(KK_F))
	{
		m_IsView = 1 - m_IsView;
	}
}

void TargetUI::Draw()
{
	/*if (!m_IsView) return;*/
	
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

	for (int i = 0; i < T_UI_MAX; i++)
	{
		if(i == T_UI_TEXTURE::T_UI_BANNER)
		{
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

			//シェーダーへ行列をセット
			Shader_SetMatrix(mat);
		}
		else
		{
			//平行移動 表示座標
			XMMATRIX	Translation =
				XMMatrixTranslation(m_Position.x + 20.0f, m_Position.y - 10.0f, 0.0f);
			//回転
			XMMATRIX	Rotation = XMMatrixRotationZ(XMConvertToRadians(0.0f));
			//拡大率（0はだめ）
			XMMATRIX	Scaling = XMMatrixScaling(m_Scaling.x * 0.8f, m_Scaling.y * 0.8f, 1.0f);
			//ワールド行列
			XMMATRIX	World = Scaling * Rotation * Translation;
			//スクロール用行列作成
			XMMATRIX	mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

			mat = World * mat * Projection;

			//シェーダーへ行列をセット
			Shader_SetMatrix(mat);
		}

		m_pContext->PSSetShaderResources(0, 1, &m_Texture[i]);

		//ブレンド無し
		SetBlendState(BLENDSTATE_ALFA);

		//スプライト描画
		DrawSprite(size, m_Color, 1, 1, 1);
	}
}