
//fade.cpp

#include	"fade.h"
#include	"shader.h"

static	ID3D11ShaderResourceView* g_Texture = NULL;	//テクスチャ１枚を表すオブジェクト
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;



void FadeObject::Fade_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	//テクスチャ読み込み
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\fade.bmp", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(), image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);//読み込み失敗時にダイアログを表示

	fadecolor.x = 0.0f;
	fadecolor.y = 0.0f;
	fadecolor.z = 0.0f;
	fadecolor.w = 1.0f;
	frame = 60.0f;	//60フレームでフェード完了
	state = FADE_STATE::FADE_NONE;



}

void FadeObject::Fade_Finalize()
{
	if (g_Texture != NULL)
	{
		g_Texture->Release();
		g_Texture = NULL;
	}
}

void FadeObject::Fade_Update()
{
}

void FadeObject::Fade_Draw()
{ 
	//現在の状況
	switch (state)
	{
		case FADE_STATE::FADE_NONE:
			return;
		case FADE_STATE::FADE_IN:
			if (fadecolor.w < 0.0)
			{//フェードイン終了
				fadecolor.w = 0.0f;
				state = FADE_STATE::FADE_NONE;
			}
			break;
		case FADE_STATE::FADE_OUT:
			if (fadecolor.w > 1.0f)
			{//フェードアウト終了
				fadecolor.w = 1.0;
				//フェードイン初期化
				Fade_SetFade(frame, fadecolor, FADE_STATE::FADE_IN,scene);
				//シーン切り替え
				
			}
			break;
	}

	//スプライト表示

//---------------------------------------------------
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
	g_pContext->PSSetShaderResources(0, 1, &g_Texture);

	//スプライト描画
	SetBlendState(BLENDSTATE_ALFA);//αブレンド
	XMFLOAT2 pos = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DrawSprite(pos, size, fadecolor);

	//フェード処理
	switch (state)
	{
		case FADE_STATE::FADE_IN:
			fadecolor.w -= (1.0f / frame);//透明にしていく
			break;
		case FADE_STATE::FADE_OUT:
			fadecolor.w += (1.0f / frame);//不透明にしていく
			break;
	}


}

void	FadeObject::Fade_SetFade(int fadeframe, XMFLOAT4 color, FADE_STATE sta, SCENE sce)
{ 
	frame = fadeframe;
	fadecolor = color;
	state = sta;
	scene = sce;

	if (state == FADE_IN)
	{
		fadecolor.w = 1.0f;	//不透明にする
	}
	else
	{
		fadecolor.w = 0.0f;	//透明にする
	}


}

FADE_STATE	FadeObject::GetFadeState()
{
	return	state;	//現在の状態
}


