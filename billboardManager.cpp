// ============================================================================
// 
// billboardManager.cpp             制作者: 杉森奏太
// 日付: 12/16
//
// ============================================================================

#include "billboardManager.h"
#include "billboard.h"

void BillboardManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
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
			LoadFromWICFile(L"Asset\\Texture\\Heart.png", WIC_FLAGS_NONE, &metadata, image);
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
		if (m_pBillboard[i]) m_pBillboard[i] = nullptr; 
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
		default:
			continue;
		}
		// 描画
		m_pBillboard[i]->Billboard_Draw();

	}

	m_Count = 0;
}

void BillboardManager::Update()
{

}

void BillboardManager::Register(Billboard* pBillboard)
{
	if (!pBillboard) return;

	if (m_Count >= BILLBOARD_MAX)
	{
		return;
	}

	m_pBillboard[m_Count] = pBillboard;

	m_Count += 1;
}