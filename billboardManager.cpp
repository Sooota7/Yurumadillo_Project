// ============================================================================
// 
// billboardManager.cpp             制作者: 杉森奏太
// 日付: 12/16
//
// ============================================================================

#include "billboardManager.h"
#include "billboard.h"

void BillboardManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO fn)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	for (int i = 0; i < BILLBOARD_MAX; i++)
	{
		m_pBillboard[i] = nullptr;
	}

	for (int i = 0; i < (int)BILLBOARD_TEXTURE::MAX; i++)
	{
		TexMetadata metadata;
		ScratchImage image;

		switch (i)
		{
		case BILLBOARD_TEXTURE::TEST:
			// テクスチャ読み込み（テスト）
			LoadFromWICFile(L"Asset\\Texture\\Diamond.png", WIC_FLAGS_NONE, &metadata, image);
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_pSRV[i]);
			assert(m_pSRV[i]);
			break;
		case BILLBOARD_TEXTURE::EXPLOSION:
			// テクスチャ読み込み（ステージに合わせてエフェクト画像を切り替える）
			Initialize_BombEffect(&metadata, &image, fn);
			CreateShaderResourceView(pDevice, image.GetImages(),
				image.GetImageCount(), metadata, &m_pSRV[i]);
			assert(m_pSRV[i]);
			break;
		default:
			break;
		}
	}

	m_Count = 0;
}


void BillboardManager::Finalize()
{


	for (int i = 0; i < BILLBOARD_MAX; ++i)
	{
		if (m_pBillboard[i])
		{
			delete m_pBillboard[i];
			m_pBillboard[i] = nullptr;
		}
	}


	for (int i = 0; i < BILLBOARD_TEXTURE::MAX; i++)
	{
		m_pSRV[i]->Release();
		m_pSRV[i] = nullptr;
	}

	m_pContext = nullptr;
	m_pDevice = nullptr;
	m_Count = 0;
}

void BillboardManager::Draw()
{
	for (int i = 0; i < m_Count; i++)
	{// 登録したビルボード分
		switch (m_pBillboard[i]->GetTexture())
		{// 使う画像
		case BILLBOARD_TEXTURE::TEST:
			m_pContext->PSSetShaderResources(0, 1, &m_pSRV[BILLBOARD_TEXTURE::TEST]);
			break;
		case BILLBOARD_TEXTURE::EXPLOSION:
			m_pContext->PSSetShaderResources(0, 1, &m_pSRV[BILLBOARD_TEXTURE::EXPLOSION]);
			break;
		default:
			delete m_pBillboard[i];
			m_pBillboard[i] = nullptr;
			continue;
		}

		// 描画
		m_pBillboard[i]->Billboard_Draw();

		// 削除
		delete m_pBillboard[i];
		m_pBillboard[i] = nullptr;
	}

	m_Count = 0;
}

void BillboardManager::Update()
{

}

void BillboardManager::Register(Billboard* pBillboard)
{
    if (!pBillboard) return;

    if (m_Count < 0 || m_Count >= BILLBOARD_MAX)
    {
        return;
    }

    m_pBillboard[m_Count] = pBillboard;

    m_Count += 1;
}

void BillboardManager::Initialize_BombEffect(TexMetadata* tm, ScratchImage* si, FIELD_NO fn)
{
	switch (fn)
	{
	case FIELD_NO::NO_1:
		LoadFromWICFile(L"Asset\\Texture\\effect\\Bomb_Effect_Forest.png", WIC_FLAGS_NONE, tm, *si);
		break;
	case FIELD_NO::NO_2:
		LoadFromWICFile(L"Asset\\Texture\\effect\\Bomb_Effect_Teatime.png", WIC_FLAGS_NONE, tm, *si);
		break;
	case FIELD_NO::NO_3:
		LoadFromWICFile(L"Asset\\Texture\\effect\\Bomb_Effect_Test.png", WIC_FLAGS_NONE, tm, *si);
		break;
	case FIELD_NO::NO_4:
		LoadFromWICFile(L"Asset\\Texture\\effect\\Bomb_Effect_Test.png", WIC_FLAGS_NONE, tm, *si);
		break;
	case FIELD_NO::NO_5:
		LoadFromWICFile(L"Asset\\Texture\\effect\\Bomb_Effect_Test.png", WIC_FLAGS_NONE, tm, *si);
		break;
	case FIELD_NO::NO_6:
		LoadFromWICFile(L"Asset\\Texture\\effect\\Bomb_Effect_Test.png", WIC_FLAGS_NONE, tm, *si);
		break;
	case FIELD_NO::NO_ENEMYLUSH:
		LoadFromWICFile(L"Asset\\Texture\\effect\\Bomb_Effect_Test.png", WIC_FLAGS_NONE, tm, *si);
		break;
	default:
		break;
	}
}
