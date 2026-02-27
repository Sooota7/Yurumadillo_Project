

//Menu.cpp
#include	"Manager.h"
#include	"sprite.h"
#include	"keyboard.h"

#include	"Menu.h"
#include	"inputx.h"

#include "fade.h"
#include "shader.h"
//aaa

static	ID3D11ShaderResourceView* g_Texture[2] ;	//テクスチャ１枚を表すオブジェクト
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


void MENU::Menu_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Fade = fade;

	//テクスチャ読み込みなど
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\tutorial1.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[0]);
	assert(g_Texture[0]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\tutorial2.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[1]);
	assert(g_Texture[1]);//読み込み失敗時にダイアログを表示

	//フェードインのセット
	XMFLOAT4	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Fade->Fade_SetFade(60.0f, color, FADE_IN, SCENE_GAME);

	tutorialselect = 0;

}
void MENU::Menu_Finalize()
{
	//テクスチャの解放など
	SAFE_RELEASE(g_Texture[0]);
	SAFE_RELEASE(g_Texture[1]);

}
void MENU::Menu_Update()
{
	//キー入力チェック
	//スタートボタンが押されたらシーンを切り替え
	//フェード処理中はキーを受け付けない
	
	if ((Keyboard_IsKeyDownTrigger(KK_W) || IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_UP))&&tutorialselect>0)
	{
		tutorialselect--;
	}
	else if ((Keyboard_IsKeyDownTrigger(KK_S)||IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_DOWN)) && tutorialselect < 1)
	{
		tutorialselect++;
	}

	if (((Keyboard_IsKeyDownTrigger(KK_ENTER)|| Keyboard_IsKeyDownTrigger(KK_SPACE)) || IsButtonTriggered(0, XINPUT_GAMEPAD_A)) && (m_Fade->GetFadeState() == FADE_NONE))
	{
		if (tutorialselect == 0)
		{
			//フェードアウトさせてシーンを切り替える
			XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
			m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_TUTORIAL);
		}
	
		if (tutorialselect == 1)
		{
		//フェードアウトさせてシーンを切り替える
			XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
			m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_STAGESELECTION);
		}
	}

	//if ((Keyboard_IsKeyDownTrigger(KK_D1)|| IsButtonTriggered(0, XINPUT_GAMEPAD_A)) 
	//	&& (m_Fade->GetFadeState() == FADE_NONE))
	//{
	//	//フェードアウトさせてシーンを切り替える
	//	XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_TUTORIAL);
	//}
	//
	//if ((Keyboard_IsKeyDownTrigger(KK_D2)|| IsButtonTriggered(0, XINPUT_GAMEPAD_B)) 
	//	&& (m_Fade->GetFadeState() == FADE_NONE))
	//{
	//	//フェードアウトさせてシーンを切り替える
	//	XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_STAGESELECTION);
	//}

}
void MENU::Menu_Draw()
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

	switch (tutorialselect)
	{
	case 0:
		g_pContext->PSSetShaderResources(0, 1, &g_Texture[0]);//g_Textureを使うように設定する
		break;	
	case 1:
		g_pContext->PSSetShaderResources(0, 1, &g_Texture[1]);//g_Textureを使うように設定する
		break;
	}
		//テクスチャをセット

	static XMFLOAT2 texcoord = { 0.0f, 0.0f };

	//スプライト描画
	SetBlendState(BLENDSTATE_NONE);//ブレンド無し
	XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
	XMFLOAT2 pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawSprite(pos, size, col);//1枚絵を表示

}



