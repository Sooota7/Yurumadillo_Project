// ====================================================================
// 
// PlyerUI.cpp             制作者: 杉森奏太
// 日付: 12/18
// 
// ====================================================================

#include	"shader.h"
#include	"sprite.h"
#include	"bombUI.h"

void BombUI::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, BOMB* pBomb)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	m_pBomb = pBomb;
	
	// UI画像
	for(int i = 0; i < TEXTURE_MAX;i++)
	{
		//テクスチャ画像読み込み
		TexMetadata		metadata;
		ScratchImage	image;

		switch (i)
		{
		case BOMBUI_HOLDER:
			LoadFromWICFile(L"asset\\texture\\ui\\itemholder4.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_Texture[i]);
			assert(m_Texture[i]);//読み込み失敗時にダイアログを表示
			break;
		case BOMBUI_ROPE:
			LoadFromWICFile(L"asset\\texture\\ui\\rope.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_Texture[i]);
			assert(m_Texture[i]);//読み込み失敗時にダイアログを表示
			break;
		case BOMBUI_FIRE:
			LoadFromWICFile(L"asset\\texture\\ui\\fire.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_Texture[i]);
			assert(m_Texture[i]);//読み込み失敗時にダイアログを表示
			break;
		default:
			break;
		}
	}

	// 爆弾の画像
	for (int i = 0; i < BOMB_TEXTURE_MAX; i++)
	{
		//テクスチャ画像読み込み
		TexMetadata		metadata;
		ScratchImage	image;

		switch (i)
		{
		case BOMBUI_HOLDER:
			LoadFromWICFile(L"asset\\texture\\ui\\bombN.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_BombTexture[i]);
			assert(m_BombTexture[i]);//読み込み失敗時にダイアログを表示
			break;
		case BOMBUI_ROPE:
			LoadFromWICFile(L"asset\\texture\\ui\\bombwing.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_BombTexture[i]);
			assert(m_BombTexture[i]);//読み込み失敗時にダイアログを表示
			break;
		case BOMBUI_FIRE:
			LoadFromWICFile(L"asset\\texture\\ui\\BombForward.png",
				WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_BombTexture[i]);
			assert(m_BombTexture[i]);//読み込み失敗時にダイアログを表示
			break;
		default:
			break;
		}
	}

}

void BombUI::Finalize()
{
	m_pDevice = nullptr;
	m_pContext = nullptr;
	m_pBomb = nullptr;

	for (int i = 0; i < 3; i++)
	{
		SAFE_RELEASE(m_Texture[i]);
		SAFE_RELEASE(m_BombTexture[i]);
	}
}

void BombUI::Update()
{
	// 爆弾を持っているか確認する
	m_Count = CheckBombState();
}

void BombUI::BossUpdate()
{
	m_Count = CheckBossBombState();
}

void BombUI::Draw()
{
	/*XMFLOAT3	position = XMFLOAT3(900.0f, 500.0f, 0.0f);
	XMFLOAT2	size = XMFLOAT2(100.0f, 100.0f);
	XMFLOAT4	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);*/

	//画面サイズ取得
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	XMFLOAT3	position = XMFLOAT3(SCREEN_WIDTH * 0.9f, SCREEN_HEIGHT * 0.9f, 0.0f);
	XMFLOAT2	holderSize = XMFLOAT2(220.0f, 200.0f);
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

	//平行移動 表示座標
	XMMATRIX	Translation =
		XMMatrixTranslation(position.x, position.y, 0.0f);
	//回転
	XMMATRIX	Rotation = XMMatrixRotationZ(XMConvertToRadians(0.0f));
	//拡大率（0はだめ）
	XMMATRIX	Scaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//ワールド行列
	XMMATRIX	World = Scaling * Rotation * Translation;
	//スクロール用行列作成
	XMMATRIX	mat = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	mat = World * mat * Projection;

	//シェーダーへ行列をセット
	Shader_SetMatrix(mat);

	//ブレンド無し
	SetBlendState(BLENDSTATE_ALFA);

	
	// 導火線描画 ================
	//テクスチャのセット
	m_pContext->PSSetShaderResources(0, 1, &m_Texture[BOMBUI_ROPE]);


	float ratio = 1.0f;
	if (m_limit > 0.0f)
	{
		ratio = 1.0f - (m_Count / m_limit);   // 1→0
		if (ratio < 0.0f) ratio = 0.0f;
	}

	// 基本サイズ（導火線の最大長さ）
	XMFLOAT2 ropeSize = XMFLOAT2(500.0f * ratio, 100.0f);

	// 描画
	DrawSpriteRopeRight(ropeSize, color, 1, 1, 1);

	// UI背景描画 =========================
	//テクスチャのセット
	m_pContext->PSSetShaderResources(0, 1, &m_Texture[BOMBUI_HOLDER]);

	//平行移動 表示座標
	XMMATRIX	holderTranslation =
		XMMatrixTranslation(position.x, position.y - 20.f, 0.0f);
	//回転
	XMMATRIX	holderRotation = XMMatrixRotationZ(XMConvertToRadians(0.0f));
	//拡大率（0はだめ）
	XMMATRIX	holderScaling = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//ワールド行列
	XMMATRIX	holderWorld = holderScaling * holderRotation * holderTranslation;
	//スクロール用行列作成
	XMMATRIX	holderMat = holderWorld * Projection;

	//シェーダーへ行列をセット
	Shader_SetMatrix(holderMat);

	// 描画
	DrawSprite(holderSize, color, 1, 1, 1);


	// UI爆弾描画 ================
	if (m_BombType == 100)
	{ // 爆弾を持っていない時
		return;  // 終了
	}

	// 爆弾を持っている時
	// 爆弾のタイプ毎のテクスチャを設定
	SetBombTexture();
	XMFLOAT2 bombSize = XMFLOAT2(120.0f, 120.0f);
	// 描画
	DrawSprite(bombSize, color, 1, 1, 1);

	// 炎描画 ====================
	if (m_Count > 0.0f)
	{
		m_pContext->PSSetShaderResources(0, 1, &m_Texture[BOMBUI_FIRE]);
		float frameX = position.x - ropeSize.x;
		float frameY = position.y - (holderSize.y / 2 - 20.0f) + 20.0f;

		XMFLOAT2 frameSize = XMFLOAT2(180.0f, 180.0f);

		XMMATRIX flameTranslation =
			XMMatrixTranslation(frameX, frameY, 0.0f);

		XMMATRIX flameWorld = Scaling * Rotation * flameTranslation;

		XMMATRIX flameMat = flameWorld * Projection;
		Shader_SetMatrix(flameMat);

		DrawSprite(frameSize, color, 1, 1, 1);
	}
}

float BombUI::CheckBombState()
{
	BOMBSOURCE* bombs;
	RUNBOMBSPAWNER* RunBombSpawner = m_pBomb->Bomb_GetRunBomb();
	RUNBOMBSOURCE* runBombs;
	FLOWTBOMBSOURCE* flowtBombs;

	bombs = m_pBomb->Bomb_GetBomb();

	flowtBombs = m_pBomb->Bomb_GetFlowtBomb();
	
	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		if (bombs[i].BombSource_GetState() == BOMB_ACTIVE_HAVE)
		{
			m_BombType = 1;
			m_limit = 5.0f; // (仮)
			return bombs[i].BombSource_GetCount();
			break;
		}

	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		if (flowtBombs[i].Flowtbombsource_GetState() == BOMB_ACTIVE_HAVE)
		{
			m_BombType = 2;
			m_limit = 2.0f; // (仮)
			return flowtBombs[i].Flowtbombsource_GetCount();
			break;
		}

	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		runBombs = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();

		if (runBombs->Runbombsource_GetState() == RUNBOMB_ACTIVE_HAVE)
		{
			m_BombType = 3;
			m_limit = 5.0f; // (仮)
			return runBombs->Runbombsource_GetCount();
			break;
		}

	}

	// 何も持ってない
	m_BombType = 100;
	return 0.0f;
}

float BombUI::CheckBossBombState()
{
	BOMBSOURCE* bombs;
	RUNBOMBSPAWNER* RunBombSpawner = m_pBomb->Bomb_GetRunBomb();
	RUNBOMBSOURCE* runBombs;
	FLOWTBOMBSOURCE* flowtBombs;

	bombs = m_pBomb->Bomb_GetBomb();

	flowtBombs = m_pBomb->Bomb_GetFlowtBomb();

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		if (bombs[i].BombSource_GetState() == BOMB_ACTIVE_HAVE)
		{
			m_BombType = 1;
			m_limit = 10.0f; // (仮)
			return bombs[i].BombSource_GetCount();
			break;
		}

	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		if (flowtBombs[i].Flowtbombsource_GetState() == BOMB_ACTIVE_HAVE)
		{
			m_BombType = 2;
			m_limit = 2.0f; // (仮)
			return flowtBombs[i].Flowtbombsource_GetCount();
			break;
		}

	}

	for (int i = 0; i < BOMB_NUM_MAX; i++)
	{
		runBombs = RunBombSpawner[i].GetRunBombSource__RunBombSpawner();

		if (runBombs->Runbombsource_GetState() == RUNBOMB_ACTIVE_HAVE)
		{
			m_BombType = 3;
			m_limit = 5.0f; // (仮)
			return runBombs->Runbombsource_GetCount();
			break;
		}

	}

	// 何も持ってない
	m_BombType = 100;
	return 0.0f;
}

void BombUI::SetBombTexture()
{
	switch (m_BombType)
	{
	case 1:
		//テクスチャのセット(通常)
		m_pContext->PSSetShaderResources(0, 1, &m_BombTexture[0]);
		break;
	case 2:
		//テクスチャのセット(風船)
		m_pContext->PSSetShaderResources(0, 1, &m_BombTexture[1]);
		break;
	case 3:
		//テクスチャのセット(ネズミ)
		m_pContext->PSSetShaderResources(0, 1, &m_BombTexture[2]);
		break;
	default:
		break;
	}
}