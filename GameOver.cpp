//GameOver.cpp
#include	"Manager.h"
#include	"sprite.h"
#include	"keyboard.h"
#include	"inputx.h"

#include	"GameOver.h"

#include "fade.h"
#include "shader.h"

static	ID3D11ShaderResourceView* g_Texture = NULL;	//テクスチャ１枚を表すオブジェクト
static	ID3D11ShaderResourceView* g_TextureLogo = NULL;	//テクスチャ１枚を表すオブジェクト
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;

// アニメーション用変数
static float g_LogoY = 0.0f;           // ロゴのY位置
static float g_LogoVelocityY = 0.0f;   // Y方向の速度
static float g_LogoTargetY = 0.0f;     // 最終目標位置
static bool g_LogoAnimating = true;    // アニメーション中フラグ
static const float GRAVITY = 0.8f;     // 重力
static const float BOUNCE_DAMPING = 0.7f; // バウンドの減衰率
static const float STOP_THRESHOLD = 2.0f; // 停止判定の閾値


void GAMEOVER::GameOver_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade, MANAGER* manager)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Fade = fade;
	m_Manager = manager;

	//テクスチャ読み込みなど
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\gameover\\gameover.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);//読み込み失敗時にダイアログを表示

	LoadFromWICFile(L"asset\\texture\\gameover\\gameover_logo.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_TextureLogo);
	assert(g_TextureLogo);//読み込み失敗時にダイアログを表示

	//フェードインのセット
	XMFLOAT4	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//m_Fade->Fade_SetFade(60.0f, color, FADE_IN, SCENE_GAME);

	// アニメーション初期化
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();
	g_LogoTargetY = SCREEN_HEIGHT / 2 - 330.0f; // 最終目標位置
	g_LogoY = -200.0f;                          // 画面上部から開始
	g_LogoVelocityY = 0.0f;                     // 初期速度
	g_LogoAnimating = true;                     // アニメーション開始
}
void GAMEOVER::GameOver_Finalize()
{
	//テクスチャの解放など
	SAFE_RELEASE(g_Texture);
	SAFE_RELEASE(g_TextureLogo);



}
void GAMEOVER::GameOver_Update()
{ 
// ロゴアニメーション更新
if (g_LogoAnimating)
{
// 重力を適用
g_LogoVelocityY += GRAVITY;
g_LogoY += g_LogoVelocityY;

// 目標位置に到達したかチェック（バウンド処理）
if (g_LogoY >= g_LogoTargetY)
{
g_LogoY = g_LogoTargetY;
g_LogoVelocityY = -g_LogoVelocityY * BOUNCE_DAMPING; // バウンド（減衰あり）

// 速度が十分小さくなったら停止
if (abs(g_LogoVelocityY) < STOP_THRESHOLD)
{
g_LogoVelocityY = 0.0f;
g_LogoAnimating = false;
}
}
}

//キー入力チェック
	//スタートボタンが押されたらシーンを切り替え
	//フェード処理中はキーを受け付けない
	if ((Keyboard_IsKeyDownTrigger(KK_ENTER) || IsButtonPressed(0, XINPUT_GAMEPAD_B)) 
		&& (m_Fade->GetFadeState() == FADE_NONE)&&m_Manager->GetClearCount()<STAGE_MAX)
	{
		//フェードアウトさせてシーンを切り替える
		XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
		// 変更: 固定の SCENE_STAGESELECTION ではなく、GameOver を開いた直前のシーンへ戻す
		SCENE previous = m_Fade->Fade_GetScene();
		m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, previous);
	}
	
}
void GAMEOVER::GameOver_Draw()
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

	//-------------------------------------------------------------

	//テクスチャをセット

	g_pContext->PSSetShaderResources(0, 1, &g_TextureLogo);//g_Textureを使うように設定する

	static XMFLOAT2 texcoord2 = { 0.0f, 0.0f };

	//スプライト描画
	SetBlendState(BLENDSTATE_ALFA);//ブレンド無し
	XMFLOAT4 col2 = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
	// アニメーション中はg_LogoYを使用、完了後は最終位置を使用
	float currentY = g_LogoAnimating ? g_LogoY : g_LogoTargetY;
	XMFLOAT2 pos2 = { SCREEN_WIDTH / 2, currentY};
	XMFLOAT2 size2 = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawSprite(pos2, size2, col2);//1枚絵を表示

}


