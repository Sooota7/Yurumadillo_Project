// ====================================================================
// 
// PlyerUI.cpp             制作者: 杉森奏太
// 日付: 12/18
// 
// ====================================================================

#include	"shader.h"
#include	"sprite.h"
#include	"playerUI.h"

void PlayerUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, PLAYER* pPlayer)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pPlayer = pPlayer;
	m_HpDigit = 0;


	// ウェーブ初期化
	m_TimeSec = 0.0f;
	m_FreqHz = FREQ_MIN;
	m_HpRatio = 1.0f;
	m_AmpPx = AMP_MIN;
	m_ScaleAmp = 0.06f;
	m_PhasePerHeart = XM_2PI / 4.0f;


	//テクスチャ画像読み込み
	// ハート（体力あり）
	{
		TexMetadata		metadata;
		ScratchImage	image;
		LoadFromWICFile(L"asset\\texture\\ui\\heart_06.png",
			WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
		CreateShaderResourceView(pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &m_Texture[0]);
		assert(m_Texture[0]);//読み込み失敗時にダイアログを表示
	}

	// ハート（体力無し）
	{
		TexMetadata		metadata;
		ScratchImage	image;
		LoadFromWICFile(L"asset\\texture\\ui\\frame.png",
			WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
		CreateShaderResourceView(pDevice, image.GetImages(),
			image.GetImageCount(), metadata, &m_Texture[1]);
		assert(m_Texture[1]);//読み込み失敗時にダイアログを表示
	}
}

void PlayerUI::Finalize()
{
	m_pDevice = nullptr;
	m_pContext = nullptr;
	m_pPlayer = nullptr;

	for (int i = 0; i < P_UI_MAX; i++)
	{
		SAFE_RELEASE(m_Texture[i]);
	}
}

void PlayerUI::Update()
{
	Update_HpDigit();

	Update_WaveParams();      // 追加：HP比率から周波数/振幅を更新
	m_TimeSec += (1.0f / 60.0f); // ご指定のフレーム固定Δt

}

void PlayerUI::Draw()
{
	if (!m_pContext || !m_pPlayer) return;
	Draw_HpDigit();

}

void PlayerUI::Update_HpDigit()
{
	if (m_pPlayer->GetPlayerHp() <= 0.0f) return;  // 0は描画しません

	float currentHp = m_pPlayer->GetPlayerHp() /*/ 20.0f*/;

	m_HpDigit = (int)currentHp;
}


// 追加：HP比率→周波数/振幅へ反映
void PlayerUI::Update_WaveParams()
{
	// HP比率: 0.0 ~ 1.0
	if (HP_DIGIT_MAX > 0) {
		m_HpRatio = (float)m_HpDigit / (float)HP_DIGIT_MAX;
	}
	else {
		m_HpRatio = 0.0f;
	}
	if (m_HpRatio < 0.0f) m_HpRatio = 0.0f;
	if (m_HpRatio > 1.0f) m_HpRatio = 1.0f;

	// HPが多いほどゆっくり、少ないほど速く
	m_FreqHz = FREQ_MIN + (FREQ_MAX - FREQ_MIN) * (1.0f - m_HpRatio);
	// HPが少ないほど上下が大きく
	m_AmpPx = AMP_MIN + (AMP_MAX - AMP_MIN) * (1.0f - m_HpRatio);
}


void PlayerUI::Draw_HpDigit()
{
	/*XMFLOAT3	position = XMFLOAT3(100.0f, 100.0f, 0.0f);
	XMFLOAT2	size = XMFLOAT2(50.0f, 80.0f);
	XMFLOAT4	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);*/

	//画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	XMFLOAT3	position = XMFLOAT3(SCREEN_WIDTH * 0.05f, SCREEN_HEIGHT * 0.92f, 0.0f);
	XMFLOAT2	size = XMFLOAT2(90.0f, 80.0f);
	XMFLOAT4	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

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


	// ウェーブの基本位相
	const float basePhase = m_TimeSec * m_FreqHz * XM_2PI;


	for(int i = 0; i < HP_DIGIT_MAX; i++)
	{
		// 位相ずらしで“波”を作る
		const float phase = basePhase + (float)i * m_PhasePerHeart;
		const float wave = sinf(phase);
		const float yOffset = m_AmpPx * wave;
		const float scale = 1.0f + m_ScaleAmp * wave;

		//平行移動 表示座標
		XMMATRIX	Translation =
			XMMatrixTranslation(position.x, position.y + yOffset, 0.0f);
		//回転
		XMMATRIX	Rotation = XMMatrixRotationZ(XMConvertToRadians(0.0f));
		//拡大率（0はだめ）
		XMMATRIX	Scaling = XMMatrixScaling(scale, scale, 1.0f);
		//ワールド行列
		XMMATRIX	World = Scaling * Rotation * Translation;
		//スクロール用行列作成
		XMMATRIX	mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

		mat = World * mat * Projection;

		float value = 0.0f;

		if (m_HpDigit > i)
		{
			//テクスチャのセット
			m_pContext->PSSetShaderResources(0, 1, &m_Texture[0]);
		}
		else
		{
			//テクスチャのセット
			m_pContext->PSSetShaderResources(0, 1, &m_Texture[1]);
			value = 18.0f;
		}

		//シェーダーへ行列をセット
		Shader_SetMatrix(mat);

		//ブレンド無し
		SetBlendState(BLENDSTATE_ALFA);

		//スプライト描画
		DrawSprite(XMFLOAT2(size.x + value, size.y), color, 1, 1, 1);

		position.x += size.x;//表示座標を１桁分ずらす
	}
}