#include "load.h"
#include "fade.h"
#include "Audio.h"
#include "Manager.h"


static	ID3D11ShaderResourceView* g_TextureLoad = NULL;	//テクスチャ１枚を表すオブジェクト
static	ID3D11ShaderResourceView* g_TextureRabit = NULL;	//テクスチャ１枚を表すオブジェクト

static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;



void LOAD_MANAGER::Load_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	//テクスチャ読み込みなど
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\Load\\Load.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_TextureLoad);
	assert(g_TextureLoad);//読み込み失敗時にダイアログを表示

	LoadFromWICFile(L"asset\\texture\\Load\\Rabit.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_TextureRabit);
	assert(g_TextureRabit);//読み込み失敗時にダイアログを表示

	m_LoadCount = 0;
	m_LoadTime = 0.0f;
	m_LoadActive = true;
	m_LoadComplete = false;

	m_LoadAnim[LOAD_ANIM_01] = LOAD_ANIM_01;
	m_LoadAnim[LOAD_ANIM_02] = LOAD_ANIM_02;
	m_LoadAnim[LOAD_ANIM_03] = LOAD_ANIM_03;
	m_LoadAnim[LOAD_ANIM_04] = LOAD_ANIM_04;

	m_LoadAnimCount = 0.0f;

}

void LOAD_MANAGER::Load_Finalize()
{
	//テクスチャの解放など
	SAFE_RELEASE(g_TextureLoad);

	//テクスチャの解放など
	SAFE_RELEASE(g_TextureRabit);
}

void LOAD_MANAGER::Load_Update()
{
	m_LoadTime += 1.0f / 60.0f;//1フレームの時間を加算	
	m_LoadCount++;

	m_LoadAnimCount += 1.0f / 5.0f;//アニメーションの時間を加算
	if (m_LoadAnimCount > 1.0f) {
		m_LoadAnimCount = 0.0f;
		SetLoadAnim();
	}

	if (m_LoadTime > 1.0f) {
		m_LoadTime = 0.0f;

		if (m_LoadComplete) {
			m_LoadActive = false;
		}
	}

}

void LOAD_MANAGER::Load_Draw()
{
	// シェーダーを描画パイプラインに設定
	Shader_Begin();

	// 画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	// 頂点シェーダーに変換行列を設定
	Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(
		0.0f,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		0.0f,
		0.0f,
		1.0f));
	//---------------------------------------------------


		//テクスチャをセット
	g_pContext->PSSetShaderResources(0, 1, &g_TextureLoad);//g_Textureを使うように設定する

	static XMFLOAT2 texcoord = { 0.0f, 0.0f };

	//スプライト描画
	SetBlendState(BLENDSTATE_NONE);//ブレンド無し
	XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
	XMFLOAT2 pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawSprite(pos, size, col);//1枚絵を表示

	{

		// シェーダーを描画パイプラインに設定
		Shader_Begin();

		const float START_WIDTH = 600;
		const float START_HEIGHT = 100;

		// 頂点シェーダーに変換行列を設定
		Shader_SetMatrix(XMMatrixOrthographicOffCenterLH(
			0.0f,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			0.0f,
			0.0f,
			1.0f));
		//---------------------------------------------------


		//テクスチャをセット
		g_pContext->PSSetShaderResources(0, 1, &g_TextureRabit);//g_Textureを使うように設定する

		//スプライト描画
		SetBlendState(BLENDSTATE_ALFA);//ブレンド無し

		for (int i = 0; i < LOAD_ANIM::LOAD_ANIM_MAX; i++) {

			float plasPos = 200 * i;

			XMFLOAT4 col2 = { 1.0f, 1.0f, 1.0f, 1.0f };    //スプライトの色
			XMFLOAT2 pos2 = { SCREEN_WIDTH / 2 - 300 + plasPos, SCREEN_HEIGHT / 2 + 100 };
			XMFLOAT2 size2 = { 250,250 };
			DrawSpriteEx(pos2, size2, col2, (int)m_LoadAnim[i], 2, 2);//1枚絵を表示
		}
	}
}

void LOAD_MANAGER::SetLoadAnim()
{
	for (int i = 0; i < LOAD_ANIM::LOAD_ANIM_MAX; i++) {
	
		switch (m_LoadAnim[i])
		{
		case LOAD_ANIM_01:
			m_LoadAnim[i] = LOAD_ANIM_02;
			break;
		case LOAD_ANIM_02:
			m_LoadAnim[i] = LOAD_ANIM_03;
			break;
		case LOAD_ANIM_03:
			m_LoadAnim[i] = LOAD_ANIM_04;
			break;
		case LOAD_ANIM_04:
			m_LoadAnim[i] = LOAD_ANIM_01;
			break;
		default:
			break;
		}
	}
}
