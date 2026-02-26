

//Prologue.cpp
#include	"Manager.h"
#include	"sprite.h"
#include	"keyboard.h"

#include	"Prologue.h"
#include	"inputx.h"

#include "fade.h"
#include "shader.h"
//aaa

static	ID3D11ShaderResourceView* g_Texture[MAX_PROLOGUE_TEXTURE];	//テクスチャ１枚を表すオブジェクト
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

int Prologue_TextureCount = 0;

void PROLOGUE::Prologue_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Fade = fade;

	//テクスチャ読み込みなど
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\kamishibai\\1.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[0]);
	assert(g_Texture[0]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\kamishibai\\2.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[1]);
	assert(g_Texture[1]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\kamishibai\\3.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[2]);
	assert(g_Texture[2]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\kamishibai\\4.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[3]);
	assert(g_Texture[3]);//読み込み失敗時にダイアログを表示
	
	//フェードインのセット
	XMFLOAT4	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Fade->Fade_SetFade(60.0f, color, FADE_IN, SCENE_GAME);

	Prologue_TextureCount = 0;

	g_SeID = LoadAudio("asset\\Audio\\SE\\kamishibai1.wav");


}
void PROLOGUE::Prologue_Finalize()
{
	//テクスチャの解放など
	for(int i=0; i < MAX_PROLOGUE_TEXTURE; i++)
	{
		SAFE_RELEASE(g_Texture[i]);
	}


	
	UnloadAudio(g_SeID);//サウンドの解放

}
void PROLOGUE::Prologue_Update()
{ 
	//キー入力で次の画像へ
	if ((Keyboard_IsKeyDownTrigger(KK_ENTER)||IsButtonTriggered(0, XINPUT_GAMEPAD_A)) 
		&& Prologue_TextureCount < MAX_PROLOGUE_TEXTURE)
	{
		PlayAudio(g_SeID, false);		//再生開始（ループあり）
		Prologue_TextureCount++;
	}
	//キー入力チェック
	//スタートボタンが押されたらシーンを切り替え
	//フェード処理中はキーを受け付けない
	if ((Keyboard_IsKeyDownTrigger(KK_ENTER)|| IsButtonTriggered(0, XINPUT_GAMEPAD_A)) 
		&& (m_Fade->GetFadeState() == FADE_NONE)&&Prologue_TextureCount==MAX_PROLOGUE_TEXTURE)
	{
		PlayAudio(g_SeID, false);		//再生開始（ループあり）

		//フェードアウトさせてシーンを切り替える
		XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
		m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_MENU);
	}
	
}
void PROLOGUE::Prologue_Draw()
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

	if (Prologue_TextureCount < MAX_PROLOGUE_TEXTURE) {
		//テクスチャをセット
		g_pContext->PSSetShaderResources(0, 1, &g_Texture[Prologue_TextureCount]);//g_Textureを使うように設定する
	}
	static XMFLOAT2 texcoord = { 0.0f, 0.0f };

	//スプライト描画
	SetBlendState(BLENDSTATE_NONE);//ブレンド無し
	XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
	XMFLOAT2 pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawSprite(pos, size, col);//1枚絵を表示

}


