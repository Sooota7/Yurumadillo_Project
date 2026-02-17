

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

	 if (claercount == 1) //ステージ1個クリア 
	 {
		 if ((Keyboard_IsKeyDownTrigger(KK_A) || IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_LEFT)) && stageselect > 0)
		 {
			 stageselect--;
		 }
		 else if ((Keyboard_IsKeyDownTrigger(KK_D) || IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_RIGHT)) && stageselect < 1)
		 {
			 stageselect++;
		 }
	 }
	 else if (claercount == 2) //ステージ2個クリア 
	 {
		 if ((Keyboard_IsKeyDownTrigger(KK_A) || IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_LEFT)) && stageselect > 0)
		 {
			 stageselect--;
		 }
		 else if ((Keyboard_IsKeyDownTrigger(KK_D) || IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_RIGHT)) && stageselect < 2)
		 {
			 stageselect++;
		 }
	 }
	 else if (claercount >= 3) //ステージ3個クリア 
	 {
		 if ((Keyboard_IsKeyDownTrigger(KK_A) || IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_LEFT)) && stageselect > 0)
		 {
			 stageselect--;
		 }
		 else if ((Keyboard_IsKeyDownTrigger(KK_D) || IsButtonTriggered(0, XINPUT_GAMEPAD_DPAD_RIGHT)) && stageselect < 3)
		 {
			 stageselect++;
		 }
	 }


	//キー入力チェック
	//スタートボタンが押されたらシーンを切り替え
	//フェード処理中はキーを受け付けない
	//何ステージまでクリアしたかで選択可能なステージが変化

	 if ((Keyboard_IsKeyDownTrigger(KK_ENTER) || IsButtonTriggered(0, XINPUT_GAMEPAD_A)) && (m_Fade->GetFadeState() == FADE_NONE))
	 {
		 if (stageselect == 0)
		 {
			 //フェードアウトさせてシーンを切り替える
			 XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
			 m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_GAME);
		 }

		 if (stageselect == 1)
		 {
			 //フェードアウトさせてシーンを切り替える
			 XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
			 m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_GIMMICK);
		 }

		 if (stageselect == 2)
		 {
			 //フェードアウトさせてシーンを切り替える
			 XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
			 m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_ENEMYLUSH);
		 }

		 if (stageselect == 3)
		 {
			 //フェードアウトさせてシーンを切り替える
			 XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
			 m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_BOSS);
		 }
	 }

	//if ((Keyboard_IsKeyDownTrigger(KK_D1)|| IsButtonTriggered(0, XINPUT_GAMEPAD_A)) 
	//	&& (m_Fade->GetFadeState() == FADE_NONE)) //いつでも選択可能
	//{
	//	XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_GAME);

	//}
	//else if ((Keyboard_IsKeyDownTrigger(KK_D2)|| IsButtonTriggered(0, XINPUT_GAMEPAD_B)) 
	//	&& (m_Fade->GetFadeState() == FADE_NONE)&&claercount>=1) //1ステージクリアしたら解放 ギミックステージ
	//{
	//	XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_GIMMICK);
	//}
	//else if ((Keyboard_IsKeyDownTrigger(KK_D3)|| IsButtonTriggered(0, XINPUT_GAMEPAD_X)) 
	//	&& (m_Fade->GetFadeState() == FADE_NONE) && claercount >= 2) //2ステージクリアしたら解放　エネミーステージ
	//{
	//	XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_ENEMYLUSH);
	//}
	//else if ((Keyboard_IsKeyDownTrigger(KK_D4)|| IsButtonTriggered(0, XINPUT_GAMEPAD_Y)) 
	//	&& (m_Fade->GetFadeState() == FADE_NONE) && claercount >= 3) //3ステージクリアしたら解放　ボスステージ
	//{
	//	XMFLOAT4	color(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Fade->Fade_SetFade(40.0f, color, FADE_OUT, SCENE_BOSS);
	//}

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
		if(stageselect==0)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[1]);//g_Textureを使うように設定する
		}
		else if(stageselect==1)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[2]);//g_Textureを使うように設定する
		}
	}
	else if (claercount == 2)
	{
		if (stageselect == 0)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[3]);//g_Textureを使うように設定する
		}
		else if (stageselect == 1)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[4]);//g_Textureを使うように設定する
		}
		else if (stageselect == 2)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[5]);//g_Textureを使うように設定する
		}
	}
	else if (claercount >= 3)
	{
		if (stageselect == 0)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[6]);//g_Textureを使うように設定する
		}
		else if (stageselect == 1)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[7]);//g_Textureを使うように設定する
		}
		else if (stageselect == 2)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[8]);//g_Textureを使うように設定する
		}
		else if (stageselect == 3)
		{
			g_pContext->PSSetShaderResources(0, 1, &g_Texture[9]);//g_Textureを使うように設定する
		}
	}
	

	static XMFLOAT2 texcoord = { 0.0f, 0.0f };

	//スプライト描画
	SetBlendState(BLENDSTATE_NONE);//ブレンド無し
	XMFLOAT4 col = { 1.0f, 1.0f, 1.0f, 1.0f };	//スプライトの色
	XMFLOAT2 pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawSprite(pos, size, col);//1枚絵を表示

}



