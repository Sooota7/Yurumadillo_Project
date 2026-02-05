

//Title.cpp
#include	"Manager.h"
#include	"sprite.h"
#include	"keyboard.h"

#include	"Title.h"

#include "fade.h"
#include "shader.h"
#include "inputx.h"
//aaa

static	ID3D11ShaderResourceView* g_Texture = NULL;	//テクスチャ１枚を表すオブジェクト
static	ID3D11ShaderResourceView* g_TextureTag = NULL;	//テクスチャ１枚を表すオブジェクト

static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

static float FadeC = 1.0f;
static float FadeCP = 1.0f;

void TITLE::Title_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Fade = fade;

	//テクスチャ読み込みなど
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\game_title_demo.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);//読み込み失敗時にダイアログを表示

	LoadFromWICFile(L"asset\\texture\\Game_Start.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_TextureTag);
	assert(g_TextureTag);//読み込み失敗時にダイアログを表示

	//フェードインのセット
	XMFLOAT4	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Fade->Fade_SetFade(60.0f, color, FADE_IN, SCENE_GAME);

	FadeCP = 1.0f / 60.0f;

	g_SeID = LoadAudio("asset\\Audio\\Title_Click_Test.wav");

}
void TITLE::Title_Finalize()
{
	//テクスチャの解放など
	SAFE_RELEASE(g_Texture);

	//テクスチャの解放など
	SAFE_RELEASE(g_TextureTag);

	UnloadAudio(g_SeID);//サウンドの解放


}
void TITLE::Title_Update()
{ 
	//キー入力チェック
	//スタートボタンが押されたらシーンを切り替え
	//フェード処理中はキーを受け付けない
	FadeC += FadeCP;
	if (FadeC > 1.0f)
	{
		FadeCP = -1.0f / 60.0f;
	}
	if (FadeC < 0.0f)
	{
		FadeCP = 1.0f / 60.0f;
	}

	if (Keyboard_IsKeyDownTrigger(KK_ENTER) && (m_Fade->GetFadeState() == FADE_NONE))
	{
		PlayAudio(g_SeID, false);		//再生開始（ループあり）

		//フェードアウトさせてシーンを切り替える
		XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
		m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_PROLOGUE);
	}

	if (IsButtonTriggered(0, XINPUT_GAMEPAD_A) && (m_Fade->GetFadeState() == FADE_NONE))
	{
		PlayAudio(g_SeID, false);		//再生開始（ループあり）

		//フェードアウトさせてシーンを切り替える
		XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
		m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_PROLOGUE); 
	}
}
void TITLE::Title_Draw()
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
	g_pContext->PSSetShaderResources(0, 1, &g_Texture);//g_Textureを使うように設定する

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
		g_pContext->PSSetShaderResources(0, 1, &g_TextureTag);//g_Textureを使うように設定する

		static XMFLOAT2 texcoord2 = { 0.0f, 0.0f };

		//スプライト描画
		SetBlendState(BLENDSTATE_ALFA);//ブレンド無し
		XMFLOAT4 col2 = { 1.0f, 1.0f, 1.0f, FadeC };	//スプライトの色
		XMFLOAT2 pos2 = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 200};
		XMFLOAT2 size2 = { START_WIDTH, START_HEIGHT };
		DrawSprite(pos2, size2, col2);//1枚絵を表示
	}

}


