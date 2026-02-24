//StageSelection.cpp
#include	"Manager.h"
#include	"sprite.h"
#include	"keyboard.h"
#include	"inputx.h"

#include	"Stageselection.h"

#include "fade.h"
#include "shader.h"
//aaa



static	ID3D11ShaderResourceView* g_Texture[10] ;	
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


int claercount = 0;

void STAGESELECTION::StageSelection_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FadeObject* fade,MANAGER* manager)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Fade = fade;
	m_Manager = manager;

	//テクスチャ読み込みなど
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\Stageselection1.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[0]);
	assert(g_Texture[0]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection2_1.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[1]);
	assert(g_Texture[1]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection2_2.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[2]);
	assert(g_Texture[2]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection3_1.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[3]);
	assert(g_Texture[3]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection3_2.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[4]);
	assert(g_Texture[4]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection3_3.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[5]);
	assert(g_Texture[5]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection4_1.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[6]);
	assert(g_Texture[6]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection4_2.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[7]);
	assert(g_Texture[7]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection4_3.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[8]);
	assert(g_Texture[8]);//読み込み失敗時にダイアログを表示
	LoadFromWICFile(L"asset\\texture\\Stageselection4_4.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture[9]);
	assert(g_Texture[9]);//読み込み失敗時にダイアログを表示

	stageselect = 0;
	stagechangecounter = 0; // ← 初期化を追加

	//フェードインのセット
	XMFLOAT4	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_Fade->Fade_SetFade(60.0f, color, FADE_IN, SCENE_GAME);

}
void STAGESELECTION::StageSelection_Finalize()
{
	//テクスチャの解放など
	for (int i = 0; i < STAGE_MAX; i++)
	{
		SAFE_RELEASE(g_Texture[i]);
	}

}
void STAGESELECTION::StageSelection_Update()
{
	 claercount = m_Manager->GetClearCount();

	 // フェード中はカウントしない（フェード完了後にカウント開始）
	 if (m_Fade && m_Fade->GetFadeState() == FADE_NONE)
	 {
		 stagechangecounter++;
	 }

	 // STAGE_CHANGE_TIME は秒なので、フレーム数に変換（想定フレームレート = 60fps）
	 const int targetFrames = static_cast<int>(STAGE_CHANGE_TIME * 60.0f);

	 // フラグ代わりにフェードが開始されると GetFadeState() が FADE_OUT になるため、ここは一度だけ実行される
	 if (stagechangecounter >= targetFrames && m_Fade && m_Fade->GetFadeState() == FADE_NONE)
	 {
		 XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
		 SCENE targetScene = SCENE_GAME;

		 switch (claercount)
		 {
		 case 0: targetScene = SCENE_GAME; break;
		 case 1: targetScene = SCENE_GIMMICK; break;
		 case 2: targetScene = SCENE_ENEMYLUSH; break;
		 case 3: targetScene = SCENE_BOSS; break;
		 default: targetScene = SCENE_GAME; break;
		 }

		 m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, targetScene);

		 // 再発を防ぐためカウントをリセット（または必要なら別フラグを使う）
		 stagechangecounter = 0;
	 }

	

}
void STAGESELECTION::StageSelection_Draw()
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
	if(claercount ==0)
	{ 
		g_pContext->PSSetShaderResources(0, 1, &g_Texture[0]);//g_Textureを使うように設定する
	}
	else if (claercount == 1)
	{
		g_pContext->PSSetShaderResources(0, 1, &g_Texture[2]);//g_Textureを使うように設定する

	}
	else if (claercount == 2)
	{
		g_pContext->PSSetShaderResources(0, 1, &g_Texture[5]);//g_Textureを使うように設定する

	}
	else if (claercount == 3)
	{
		g_pContext->PSSetShaderResources(0, 1, &g_Texture[9]);//g_Textureを使うように設定する

	}
	

	static XMFLOAT2 texcoord = { 0.0f, 0.0f };

	//スプライト描画
	SetBlendState(BLENDSTATE_NONE);//ブレンド無し
	XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
	XMFLOAT2 pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawSprite(pos, size, col);//1枚絵を表示

}



