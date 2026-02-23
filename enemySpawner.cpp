#include "enemySpawner.h"
#include "Dictionary.h"
#include "camera.h"


//繧ｰ繝ｭ繝ｼ繝舌Ν螟画焚
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//鬆らせ繝舌ャ繝輔ぃ
static ID3D11Buffer* g_VertexBuffer = NULL;
//繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ
static ID3D11Buffer* g_IndexBuffer = NULL;
//繝・け繧ｹ繝√Ε螟画焚
static ID3D11ShaderResourceView* g_Texture;


void ENEMYSPAWNER::EnemySpawner_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	// 繝｡繝ｳ繝仙､画焚繧呈・遉ｺ逧・↓蛻晄悄蛹厄ｼ磯㍾隕・ｼ・
	MaxNum = 0;
	NowKillNum = 0;

	// 繝・け繧ｹ繝√Ε隱ｭ縺ｿ霎ｼ縺ｿ
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\block_field.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	//蛻晄悄蛹・
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		m_Enemy[i].Initialize(g_pDevice, g_pContext);
		m_Enemy[i].SetEnemyNormalType(ENEMY_TYPE::ENEMY_TYPE_NONE);
	}

	// ======================================================
	// 縺ｪ繧薙°驕輔▲縺溘ｉ螟峨∴縺ｦ
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		m_EnemyButterfly[i].Initialize(g_pDevice, g_pContext);
		m_EnemyButterfly[i].SetEnemyButterflyType(ENEMY_TYPE::ENEMY_TYPE_NONE);
	}
	 
	// ======================================================
	// 縺ｪ繧薙°驕輔▲縺溘ｉ螟峨∴縺ｦ(阯､蜴殼ersion)
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		m_EnemyGround[i].Initialize(g_pDevice, g_pContext);
		m_EnemyGround[i].SetEnemyGroundType(ENEMY_TYPE::ENEMY_TYPE_NONE);
	}
	 
	// ======================================================


	//繝槭ャ繝励・繧ｻ繝・ヨ
	int a = 0;
	int b = 0;
	int c = 0;	//EnemyGround縺ｮ謨ｰ

	for (int q = 0; q < FIELD_HEIGHT_Y; q++)
	{
		for (int i = 0; i < FIELD_WIDTH_Z; i++)
		{
			for (int l = 0; l < FIELD_WIDTH_X; l++)
			{

				switch (CheckMap(l, i, q, no))
				{
				case 0:
					break;
				case 6:
					m_Enemy[a].SetEnemyPosition(XMFLOAT3(l, q, i));
					m_Enemy[a].SetEnemyNormalType(ENEMY_TYPE_NORMAL);
					a++;
					MaxNum++;
					break;
				case 7:
					m_EnemyButterfly[b].SetEnemyPosition(XMFLOAT3(l, q, i));
					m_EnemyButterfly[b].SetEnemyButterflyType(ENEMY_TYPE_BUTTERFLY);
					b++;
					MaxNum++;
					break;
				case G:
					m_EnemyGround[c].SetEnemyPosition(XMFLOAT3(l, q, i));
					m_EnemyGround[c].SetEnemyGroundType(ENEMY_TYPE_GROUND);
					c++;
					MaxNum++;
				default:
					break;
				}

				
			}
		}
	}
	

	//繝悶Ο繝・け縺ｮ菴懈・
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		switch (i)
		{
		case ENEMY_TYPE_NONE:
			break;
		case ENEMY_TYPE_NORMAL:
			m_Model[i] = ModelLoad("asset\\model\\tree.fbx");//繝・ヰ繝・げ
			break;
		case ENEMY_TYPE_BUTTERFLY:
			m_Model[i] = ModelLoad("asset\\model\\test_goal.fbx");//繝・ヰ繝・げ
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}
	}

	// --- 驥崎ｦ・---
	// (縺薙・髢｢謨ｰ縺ｮ譛ｫ蟆ｾ縺ｫ) 繝ｭ繝ｼ繧ｫ繝ｫ縺ｧ `int MaxNum = 0;` 繧・`int NowKillNum = 0;` 繧定ｿｽ蜉縺励↑縺・〒縺上□縺輔＞縲・
	int MaxNum = 0;

	int NowKillNum = 0;




//============================================================
// Enemy_Ground_Anim
//============================================================

	float downSize = 1.0f;

	for (int i = 0; i < EG_PARTS_MAX; i++)
	{
		switch (i)
		{
		case EG_PARTS_BODY:
			m_EG_Model[i] = ModelLoad("asset\\model\\EnemyGround\\EnemyGroundBody.fbx");
			break;
		case EG_PARTS_ARM_RIGHT:
			m_EG_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case EG_PARTS_ARM_LEFT:
			m_EG_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case EG_PARTS_LEG_RIGHT:
			m_EG_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case EG_PARTS_LEG_LEFT:
			m_EG_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		default:
			break;
		}

	}

	for (int i = 0; i < ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MAX; i++)
	{
		for (int y = 0; y < ENEMY_GROUND_PARTS::EG_PARTS_MAX; y++)
		{
			m_EG_Anim[i].anim[y].AnimInitialize();
		}
	}

	ENEMY_GROUND_SetAnimInis();


}

void ENEMYSPAWNER::EnemySpawner_Finalize(void)
{
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		if (m_Model[i] != NULL)
		{
			ModelRelease(m_Model[i]);
			m_Model[i] = NULL;
		}

	}

	for (int i = 0; i < ENEMY_GROUND_PARTS::EG_PARTS_MAX; i++)
	{
		ModelRelease(m_EG_Model[i]);
	}

	SAFE_RELEASE(g_VertexBuffer);
	SAFE_RELEASE(g_IndexBuffer);
	SAFE_RELEASE(g_Texture);

}

void ENEMYSPAWNER::EnemySpawner_Draw(void)
{
	//繧ｷ繧ｧ繝ｼ繝繝ｼ繧呈緒逕ｻ繝代う繝励Λ繧､繝ｳ縺ｸ險ｭ螳・
	Shader_Begin();


	//繝励Ο繧ｸ繧ｧ繧ｯ繧ｷ繝ｧ繝ｳ陦悟・菴懈・
	XMMATRIX	Projection = GetProjectionMatrix();
	//繝薙Η繝ｼ陦悟・菴懈・
	XMMATRIX	View = GetViewMatrix();
	//蜈医↓VP螟画鋤陦悟・繧剃ｽ懊▲縺ｦ縺翫￥
	XMMATRIX	VP = View * Projection;

	//MAP縺ｮ陦ｨ遉ｺ
	int i = 0;

	static float rot = 0.0f;
	rot -= 0.5f;

	for(int i = 0; i< Enemy_Spawner_MAX;i++)
	{
		//豁ｻ莠｡縲∝ｭ伜惠縺励↑縺・ｴ蜷域嶌縺九↑縺・
		if (m_Enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE&&
			m_Enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_DEAD)
		{
			XMFLOAT3 mapPos = m_Enemy[i].GetEnemyPosition();

			//繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ陦悟・縺ｮ菴懈・
			XMMATRIX	ScalingMatrix = XMMatrixScaling
			(
				1.0f,
				1.0f,
				1.0f
			);
			//蟷ｳ陦檎ｧｻ蜍戊｡悟・縺ｮ菴懈・
			XMMATRIX	TranslationMatrix = XMMatrixTranslation
			(
				mapPos.x,
				mapPos.y,
				mapPos.z
			);

			//蝗櫁ｻ｢陦悟・縺ｮ菴懈・
			XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
			(
				XMConvertToRadians(0.0f),
				//XMConvertToRadians(rot),
				//XMConvertToRadians(rot),
				XMConvertToRadians(0.0f),
				XMConvertToRadians(0.0f)
			);
			//繝ｯ繝ｼ繝ｫ繝芽｡悟・縺ｮ菴懈・
			XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
			//譛邨ら噪縺ｪ螟画鋤陦悟・繧剃ｽ懈・
			XMMATRIX WVP = World * VP;//(VP = View*Projection)
			//DirectX縺ｸ陦悟・繧偵そ繝・ヨ
			Shader_SetMatrix(WVP);

			//繝・け繧ｹ繝√Ε繧偵そ繝・ヨ
			g_pContext->PSSetShaderResources(0, 1, &g_Texture);

			//鬆らせ繝舌ャ繝輔ぃ繧偵そ繝・ヨ
			UINT	stride = sizeof(Vertex3D);	//鬆らせ・大九・繝・・繧ｿ繧ｵ繧､繧ｺ
			UINT	offset = 0;
			g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

			//繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ繧偵そ繝・ヨ
			g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//謠冗判縺吶ｋ繝昴Μ繧ｴ繝ｳ縺ｮ遞ｮ鬘槭ｒ繧ｻ繝・ヨ 3鬆らせ縺ｧ繝昴Μ繧ｴ繝ｳ・第椢縺ｨ縺励※陦ｨ遉ｺ
			g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//謠冗判繝ｪ繧ｯ繧ｨ繧ｹ繝・
			//繝｢繝・Ν荳蛟九＠縺九↑縺・°繧芽ｿｽ蜉縺吶ｋ縺ｨ縺阪↓螟峨∴繧・
			switch (m_Enemy[i].GetEnemyNormalState())
			{
			case ENEMY_NORMAL_STATE_IDLE:
				ModelDraw(m_Model[ENEMY_TYPE_NORMAL]);
				break;
			case ENEMY_NORMAL_STATE_MOVE:
				ModelDraw(m_Model[ENEMY_TYPE_NORMAL]);
				break;
			case ENEMY_NORMAL_STATE_DIRECTION:
				ModelDraw(m_Model[ENEMY_TYPE_NORMAL]);
				break;
			case ENEMY_NORMAL_STATE_JUMP:
				ModelDraw(m_Model[ENEMY_TYPE_NORMAL]);
				break;
			case ENEMY_NORMAL_STATE_DEAD:
				
				break;
			default:
				break;
			}

			
		}
	}
	// 縺・▲縺溘ｓ縺昴・縺ｾ縺ｾ
	// 豬ｮ縺・※繧区雰
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		//豁ｻ莠｡縲∝ｭ伜惠縺励↑縺・ｴ蜷域嶌縺九↑縺・
		if (m_EnemyButterfly[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE &&
			m_EnemyButterfly[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_DEAD)
		{
			XMFLOAT3 mapPos = m_EnemyButterfly[i].GetEnemyPosition();

			//繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ陦悟・縺ｮ菴懈・
			XMMATRIX	ScalingMatrix = XMMatrixScaling
			(
				1.0f,
				1.0f,
				1.0f
			);
			//蟷ｳ陦檎ｧｻ蜍戊｡悟・縺ｮ菴懈・
			XMMATRIX	TranslationMatrix = XMMatrixTranslation
			(
				mapPos.x,
				mapPos.y,
				mapPos.z
			);

			//蝗櫁ｻ｢陦悟・縺ｮ菴懈・
			XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
			(
				XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().x),
				//XMConvertToRadians(rot),
				//XMConvertToRadians(rot),
				m_EnemyButterfly[i].GetEnemyRotation().y,
				//XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().y),
				XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().z)
			);
			//繝ｯ繝ｼ繝ｫ繝芽｡悟・縺ｮ菴懈・
			XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
			//譛邨ら噪縺ｪ螟画鋤陦悟・繧剃ｽ懈・
			XMMATRIX WVP = World * VP;//(VP = View*Projection)
			//DirectX縺ｸ陦悟・繧偵そ繝・ヨ
			Shader_SetMatrix(WVP);

			//繝・け繧ｹ繝√Ε繧偵そ繝・ヨ
			g_pContext->PSSetShaderResources(0, 1, &g_Texture);

			//鬆らせ繝舌ャ繝輔ぃ繧偵そ繝・ヨ
			UINT	stride = sizeof(Vertex3D);	//鬆らせ・大九・繝・・繧ｿ繧ｵ繧､繧ｺ
			UINT	offset = 0;
			g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

			//繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ繧偵そ繝・ヨ
			g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//謠冗判縺吶ｋ繝昴Μ繧ｴ繝ｳ縺ｮ遞ｮ鬘槭ｒ繧ｻ繝・ヨ 3鬆らせ縺ｧ繝昴Μ繧ｴ繝ｳ・第椢縺ｨ縺励※陦ｨ遉ｺ
			g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//謠冗判繝ｪ繧ｯ繧ｨ繧ｹ繝・
			//繝｢繝・Ν荳蛟九＠縺九↑縺・°繧芽ｿｽ蜉縺吶ｋ縺ｨ縺阪↓螟峨∴繧・
			switch (m_EnemyButterfly[i].GetEnemyButterflyState())
			{
			case ENEMY_BUTTERFLY_STATE_IDLE:
				ModelDraw(m_Model[ENEMY_TYPE_BUTTERFLY]);
				break;
			case ENEMY_BUTTERFLY_STATE_MOVE:
				ModelDraw(m_Model[ENEMY_TYPE_BUTTERFLY]);
				break;
			case ENEMY_BUTTERFLY_STATE_DIRECTION:
				ModelDraw(m_Model[ENEMY_TYPE_BUTTERFLY]);
				break;
			case ENEMY_BUTTERFLY_STATE_ATTACK:
				ModelDraw(m_Model[ENEMY_TYPE_BUTTERFLY]);
				break;
			case ENEMY_BUTTERFLY_STATE_DEAD:

				break;
			default:
				break;
			}
		}
	}

	//=======================================================
	//EnemyGround謠冗判蜍穂ｽ懃｢ｺ隱咲畑_謠冗判繝｢繝・Ν繝舌ち繝輔Λ繧､縺ｮ縺ｾ縺ｾ
	//=======================================================
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		//豁ｻ莠｡縲∝ｭ伜惠縺励↑縺・ｴ蜷域嶌縺九↑縺・
		if (m_EnemyGround[i].GetEnemyGroundType() != ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_NONE &&
			m_EnemyGround[i].GetEnemyGroundType() != ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_DEAD)
		{

			ENEMY_GROUND_UpdateAnim();

			XMFLOAT3 mapPos = m_EnemyGround[i].GetEnemyPosition();

			//繧ｹ繧ｱ繝ｼ繝ｪ繝ｳ繧ｰ陦悟・縺ｮ菴懈・
			XMMATRIX	ScalingMatrix = XMMatrixScaling
			(
				1.0f,
				1.0f,
				1.0f
			);
			//蟷ｳ陦檎ｧｻ蜍戊｡悟・縺ｮ菴懈・
			XMMATRIX	TranslationMatrix = XMMatrixTranslation
			(
				mapPos.x,
				mapPos.y,
				mapPos.z
			);

			//蝗櫁ｻ｢陦悟・縺ｮ菴懈・
			XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
			(
				XMConvertToRadians(m_EnemyGround[i].GetEnemyRotation().x),
				//XMConvertToRadians(rot),
				//XMConvertToRadians(rot),
				m_EnemyGround[i].GetEnemyRotation().y,
				//XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().y),
				XMConvertToRadians(m_EnemyGround[i].GetEnemyRotation().z)
			);
			//繝ｯ繝ｼ繝ｫ繝芽｡悟・縺ｮ菴懈・
			XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
			//譛邨ら噪縺ｪ螟画鋤陦悟・繧剃ｽ懈・
			XMMATRIX WVP = World * VP;//(VP = View*Projection)
			//DirectX縺ｸ陦悟・繧偵そ繝・ヨ
			Shader_SetMatrix(WVP);

			//繝・け繧ｹ繝√Ε繧偵そ繝・ヨ
			g_pContext->PSSetShaderResources(0, 1, &g_Texture);

			//鬆らせ繝舌ャ繝輔ぃ繧偵そ繝・ヨ
			UINT	stride = sizeof(Vertex3D);	//鬆らせ・大九・繝・・繧ｿ繧ｵ繧､繧ｺ
			UINT	offset = 0;
			g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

			//繧､繝ｳ繝・ャ繧ｯ繧ｹ繝舌ャ繝輔ぃ繧偵そ繝・ヨ
			g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			//謠冗判縺吶ｋ繝昴Μ繧ｴ繝ｳ縺ｮ遞ｮ鬘槭ｒ繧ｻ繝・ヨ 3鬆らせ縺ｧ繝昴Μ繧ｴ繝ｳ・第椢縺ｨ縺励※陦ｨ遉ｺ
			g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			PARTS* EG_Parts = m_EnemyGround[i].GetEnemyGroundParts();

			//謠冗判繝ｪ繧ｯ繧ｨ繧ｹ繝・
			//繝｢繝・Ν荳蛟九＠縺九↑縺・°繧芽ｿｽ蜉縺吶ｋ縺ｨ縺阪↓螟峨∴繧・
			for (int i = 0; i < EG_PARTS_MAX; i++)
			{
				switch (i)
				{
				case EG_PARTS_BODY:
					EG_Parts[i].PartsDraw(m_EG_Model[i]);
					break;
				case EG_PARTS_ARM_RIGHT:
					EG_Parts[i].PartsDraw(m_EG_Model[i]);
					break;
				case EG_PARTS_ARM_LEFT:
					EG_Parts[i].PartsDraw(m_EG_Model[i]);
					break;
				case EG_PARTS_LEG_RIGHT:
					EG_Parts[i].PartsDraw(m_EG_Model[i]);
					break;
				case EG_PARTS_LEG_LEFT:
					EG_Parts[i].PartsDraw(m_EG_Model[i]);
					break;
				default:
					break;
				}
			}



		}
	}
}

void ENEMYSPAWNER::EnemySpawner_Update(XMFLOAT3 pPlayerPos)
{
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		switch (m_Enemy[i].GetEnemyNormalType())
		{
		case ENEMY_TYPE_NONE:
			break;
		case ENEMY_TYPE_NORMAL:
			m_Enemy[i].Update(pPlayerPos);
			break;
		case ENEMY_TYPE_DEAD:
			EnemySpawner_SetKillNum(1);						//豁ｻ繧薙□繧峨き繧ｦ繝ｳ繝医☆繧・
			m_Enemy[i].SetEnemyNormalType(ENEMY_TYPE_NONE);	//蟄伜惠繧呈ｶ医☆
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}

		

	}
	
	// 豬ｮ縺・※繧区雰
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		switch (m_EnemyButterfly[i].GetEnemyButterflyType())
		{
		case ENEMY_TYPE_NONE:
			break;
		case ENEMY_TYPE_BUTTERFLY:
			m_EnemyButterfly[i].Update(pPlayerPos);
			break;
		case ENEMY_TYPE_DEAD:
			EnemySpawner_SetKillNum(1);						//豁ｻ繧薙□繧峨き繧ｦ繝ｳ繝医☆繧・
			m_EnemyButterfly[i].SetEnemyButterflyType(ENEMY_TYPE_NONE);	//蟄伜惠繧呈ｶ医☆
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}



	}
	
	//==========================================
	// EnemyGround縺ｮUpdate
	//==========================================

	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		switch (m_EnemyGround[i].GetEnemyGroundType())
		{
		case ENEMY_TYPE_NONE:
			break;
		case ENEMY_TYPE_GROUND:
			m_EnemyGround[i].Update(pPlayerPos);
			break;
		case ENEMY_TYPE_DEAD:
			EnemySpawner_SetKillNum(1);						//豁ｻ繧薙□繧峨き繧ｦ繝ｳ繝医☆繧・
			m_EnemyGround[i].SetEnemyGroundType(ENEMY_TYPE_NONE);	//蟄伜惠繧呈ｶ医☆
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}
	}

}

ENEMY_NORMAL* ENEMYSPAWNER::EnemySpawner_GetEnemy()
{
	return m_Enemy;
}

ENEMY_BUTTERFLY* ENEMYSPAWNER::EnemySpawner_GetEnemyButterfly()
{
	return m_EnemyButterfly;
}

ENEMY_GROUND* ENEMYSPAWNER::EnemySpawner_GetEnemyGround()
{
	return m_EnemyGround;
}

//驟咲ｽｮ縺励◆謨ｰ縲∝偵☆縺ｹ縺肴雰縺ｮ謨ｰ
int ENEMYSPAWNER::EnemySpawner_GetEnemyNum()
{
	return MaxNum;
}

//蛟偵＠縺滓焚繧偵き繧ｦ繝ｳ繝医∽ｻ翫・縺ｨ縺薙ｍ・代・縺ｿ
void ENEMYSPAWNER::EnemySpawner_SetKillNum(int killnum)
{
	NowKillNum += killnum;
}

//迴ｾ蝨ｨ蛟偵＠縺滓雰縺ｮ蜷郁ｨ域焚繧定ｿ斐☆
int ENEMYSPAWNER::EnemySpawner_GetKillNum()
{
	return NowKillNum;
}

bool ENEMYSPAWNER::EnemySpawner_SpawnButterfly(const XMFLOAT3& pos)
{
	for (int i = 0; i < Enemy_Spawner_MAX; ++i)
	{
		if (m_EnemyButterfly[i].GetEnemyButterflyType() == ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			// 菴咲ｽｮ縺ｨ繧ｿ繧､繝励ｒ險ｭ螳壹＠縺ｦ蜀榊茜逕ｨ・育憾諷九ｒ蛻晄悄蛹厄ｼ・
			m_EnemyButterfly[i].SetEnemyPosition(pos);
			m_EnemyButterfly[i].SetEnemyButterflyType(ENEMY_TYPE::ENEMY_TYPE_BUTTERFLY);
			m_EnemyButterfly[i].SetEnemyVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_EnemyButterfly[i].SetEnemyButterflyState(ENEMY_BUTTERFLY_STATE::ENEMY_BUTTERFLY_STATE_MOVE);
			m_EnemyButterfly[i].SetEnemyHp(100); // 蠢・ｦ√↑HP縺ｫ蜷医ｏ縺帙ｋ
			// 蠢・ｦ√↑繧峨ヵ繝ｬ繝ｼ繝繧ｫ繧ｦ繝ｳ繧ｿ遲峨ｂ繝ｪ繧ｻ繝・ヨ・・rivate 繝｡繝ｳ繝舌′縺ゅｋ縺ｪ繧峨Γ繧ｽ繝・ラ繧定ｿｽ蜉縺励※謇ｱ縺・ｼ・
			return true;
		}
	}
	return false; // 遨ｺ縺阪↑縺・
}

bool ENEMYSPAWNER::EnemySpawner_SpawnNormal(const XMFLOAT3& pos)
{
	for (int i = 0; i < Enemy_Spawner_MAX; ++i)
	{
		if (m_Enemy[i].GetEnemyNormalType() == ENEMY_TYPE::ENEMY_TYPE_NONE)
		{
			m_Enemy[i].SetEnemyPosition(pos);
			m_Enemy[i].SetEnemyNormalType(ENEMY_TYPE::ENEMY_TYPE_NORMAL);
			m_Enemy[i].SetEnemyVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_Enemy[i].SetEnemyHp(100); // 蠢・ｦ√↑繧芽ｪｿ謨ｴ
			// 迥ｶ諷九Μ繧ｻ繝・ヨ逕ｨ繝｡繧ｽ繝・ラ縺後≠繧後・蜻ｼ縺ｶ
			return true;
		}
	}
	return false;
}


//============================================================
// Enemy_Ground_Anim
//============================================================
void ENEMYSPAWNER::ENEMY_GROUND_SetAnimInis()
{
	Enemy_Ground_SetAnimMove();
	Enemy_Ground_SetAnimAttack();
}

void ENEMYSPAWNER::ENEMY_GROUND_UpdateAnim()
{
	

	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (m_EnemyGround[i].GetEnemyGroundState() != ENEMY_GROUND_STATE_IDLE)
		{
			float l = m_EnemyGround[i].GetEnemyGroundFrame();

			PARTS* EG_Parts = m_EnemyGround[i].GetEnemyGroundParts();

			for (int j = 0; j < ENEMY_GROUND_PARTS::EG_PARTS_MAX; j++)
			{
				XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);

				XMFLOAT3 pos = m_EnemyGround[i].GetEnemyPosition();
				XMFLOAT3 rot = m_EnemyGround[i].GetEnemyRotation();
				//XMFLOAT3 plasRot = m_EG_Anim;
				ENEMY_GROUND_STATE state = m_EnemyGround[i].GetEnemyGroundState();

				switch (j)
				{
				case EG_PARTS_BODY:
					position = ENEMY_GROUND_AnimPos(state, EG_PARTS_BODY, &EG_Parts[j], pos, rot, (int)l);
					//plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_BODY, &m_Model[i], rot, (int)l);
					break;
				case EG_PARTS_ARM_RIGHT:
					position = ENEMY_GROUND_AnimPos(state, EG_PARTS_ARM_RIGHT, &EG_Parts[j], pos, rot, (int)l);
					//plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_ARM_RIGHT, &m_Model[i], rot, (int)l);
					break;
				case EG_PARTS_ARM_LEFT:
					position = ENEMY_GROUND_AnimPos(state, EG_PARTS_ARM_LEFT, &EG_Parts[j], pos, rot, (int)l);
					//plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_ARM_LEFT, &m_Model[i], rot, (int)l);
					break;
				case EG_PARTS_LEG_RIGHT:
					position = ENEMY_GROUND_AnimPos(state, EG_PARTS_LEG_RIGHT, &EG_Parts[j], pos, rot, (int)l);
					//plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_LEG_RIGHT, &m_Model[i], rot, (int)l);
					break;
				case EG_PARTS_LEG_LEFT:
					position = ENEMY_GROUND_AnimPos(state, EG_PARTS_LEG_LEFT, &EG_Parts[j], pos, rot, (int)l);
					//plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_LEG_LEFT, &m_Model[i], rot, (int)l);
					break;
				case PARTS_MAX:
					break;
				default:
					break;
				}


				rot.x += 0;//XMConvertToRadians(plasRot.x);
				rot.y += 0;//XMConvertToRadians(plasRot.y);
				rot.z += 0;//XMConvertToRadians(plasRot.z);


				switch (j)
				{
				case PARTS_HEAD:

					EG_Parts[j].PartsSet(position, rot);
					break;
				case PARTS_BODY:
					//pos = Player_LeftLeg2(&m_Model[i], rot);
					EG_Parts[j].PartsSet(position, rot);
					break;
				case PARTS_ARM_RIGHT:
					//pos = Player_LeftLeg2(&m_Model[i], rot);
					EG_Parts[j].PartsSet(position, rot);
					break;
				case PARTS_ARM_LEFT:
					//pos = Player_LeftLeg2(&m_Model[i], rot);
					EG_Parts[j].PartsSet(position, rot);
					break;
				case PARTS_LEG_RIGHT:
					//pos = Player_LeftLeg2(&m_Model[i], rot);
					EG_Parts[j].PartsSet(position, rot);
					break;
				case PARTS_LEG_LEFT:
					//pos = Player_LeftLeg2(&m_Model[i],rot);
					EG_Parts[j].PartsSet(position, rot);
					break;

				default:
					break;
				}
			}
			/*l += (1.0f / 1.0f);
			if (l >= 60)
			{
				l = 0;
			}*/
		}
	}


}

XMFLOAT3 ENEMYSPAWNER::ENEMY_GROUND_AnimPos(ENEMY_GROUND_STATE state, ENEMY_GROUND_PARTS part, PARTS* parts, XMFLOAT3 pos,XMFLOAT3 rot, int frame)
{
	XMFLOAT3 position = pos;
	XMFLOAT3 InisPos = parts->GetInisPosition();
	XMFLOAT3 nowPos = parts->GetNowPosition();
	XMFLOAT3 lastPos = m_EG_Anim[state].anim[part].fps[frame].Position;
	XMFLOAT3 lastRot = m_EG_Anim[state].anim[part].fps[frame].Rotation;
	bool	 loop = parts->GetAnimLoop();
	/*parts->GetAnimLastPosition(parts->GetNowPos());*/
	//x蛟､
	position.x -= InisPos.x * cosf(rot.y);
	position.z += InisPos.x * sinf(rot.y);

	//z蛟､
	position.x -= InisPos.z * cosf(rot.y);
	position.z += InisPos.z * sinf(rot.y);

	//・吝､
	position.y += InisPos.y;

	//x蛟､
	position.x -= lastPos.x * cosf(rot.y);
	position.z += lastPos.x * sinf(rot.y);

	//z蛟､
	position.z += lastPos.z * cosf(rot.y);
	position.x += lastPos.z * sinf(rot.y);

	//・吝､
	position.y += lastPos.y;

	return position;

}



//=====================================================================
// EnemyGroundAnimSet髢｢謨ｰ蜿翫・陬憺俣髢｢謨ｰ
//=====================================================================

void ENEMYSPAWNER::Enemy_Ground_SetAnimHokan(ENEMY_GROUND_STATE state)
{
	for (int y = 0; y < ENEMY_GROUND_PARTS::EG_PARTS_MAX; y++)
	{
		for (int i = 0; i < FLAME_MAX; i++)
		{
			if (i != 0 && m_EG_Anim[state].anim[y].fps[i].SetPos)
			{
				int a = 0;

				for (int i = 0; i < FLAME_MAX; i++)
				{
					if (!m_EG_Anim[state].anim[y].fps[i].Fill)
					{
						a = i;
						break;
					}
				}

				XMFLOAT3 pos{ 0.0f,0.0f,0.0f };
				XMFLOAT3 rot{ 0.0f,0.0f,0.0f };

				int waru = (i - a);

				if (a == 0)
				{
					pos = {
						(m_EG_Anim[state].anim[y].fps[i].Position.x - m_EG_Anim[state].anim[y].fps[a].Position.x) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Position.y - m_EG_Anim[state].anim[y].fps[a].Position.y) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Position.z - m_EG_Anim[state].anim[y].fps[a].Position.z) / waru };



				}
				else
				{
					pos = {
						(m_EG_Anim[state].anim[y].fps[i].Position.x - m_EG_Anim[state].anim[y].fps[a - 1].Position.x) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Position.y - m_EG_Anim[state].anim[y].fps[a - 1].Position.y) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Position.z - m_EG_Anim[state].anim[y].fps[a - 1].Position.z) / waru };

				}

				if (a == 0)
				{
					rot = {
						(m_EG_Anim[state].anim[y].fps[i].Rotation.x - m_EG_Anim[state].anim[y].fps[a].Rotation.x) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Rotation.y - m_EG_Anim[state].anim[y].fps[a].Rotation.y) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Rotation.z - m_EG_Anim[state].anim[y].fps[a].Rotation.z) / waru };



				}
				else
				{
					rot = {
						(m_EG_Anim[state].anim[y].fps[i].Rotation.x - m_EG_Anim[state].anim[y].fps[a - 1].Rotation.x) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Rotation.y - m_EG_Anim[state].anim[y].fps[a - 1].Rotation.y) / waru,
						(m_EG_Anim[state].anim[y].fps[i].Rotation.z - m_EG_Anim[state].anim[y].fps[a - 1].Rotation.z) / waru };

				}

				for (int l = a; l < i; l++)
				{

					if (!m_EG_Anim[state].anim[y].fps[l].Fill)
					{
						if (l == 0)
						{
							m_EG_Anim[state].anim[y].fps[l].Position.x = (m_EG_Anim[state].anim[y].fps[l].Position.x + pos.x);
							m_EG_Anim[state].anim[y].fps[l].Position.y = (m_EG_Anim[state].anim[y].fps[l].Position.y + pos.y);
							m_EG_Anim[state].anim[y].fps[l].Position.z = (m_EG_Anim[state].anim[y].fps[l].Position.z + pos.z);

							if (fabs(m_EG_Anim[state].anim[y].fps[l].Position.x) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Position.x = 0.0f;
							}
							if (fabs(m_EG_Anim[state].anim[y].fps[l].Position.z) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Position.z = 0.0f;
							}

							m_EG_Anim[state].anim[y].fps[l].Rotation.x = (m_EG_Anim[state].anim[y].fps[l].Rotation.x + rot.x);
							m_EG_Anim[state].anim[y].fps[l].Rotation.y = (m_EG_Anim[state].anim[y].fps[l].Rotation.y + rot.y);
							m_EG_Anim[state].anim[y].fps[l].Rotation.z = (m_EG_Anim[state].anim[y].fps[l].Rotation.z + rot.z);

							if (fabs(m_EG_Anim[state].anim[y].fps[l].Rotation.x) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Rotation.x = 0.0f;
							}
							if (fabs(m_EG_Anim[state].anim[y].fps[l].Rotation.z) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Rotation.z = 0.0f;
							}

							m_EG_Anim[state].anim[y].fps[l].Fill = true;
						}
						else
						{

							m_EG_Anim[state].anim[y].fps[l].Position.x = (m_EG_Anim[state].anim[y].fps[l - 1].Position.x + pos.x);
							m_EG_Anim[state].anim[y].fps[l].Position.y = (m_EG_Anim[state].anim[y].fps[l - 1].Position.y + pos.y);
							m_EG_Anim[state].anim[y].fps[l].Position.z = (m_EG_Anim[state].anim[y].fps[l - 1].Position.z + pos.z);

							if (fabs(m_EG_Anim[state].anim[y].fps[l].Position.x) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Position.x = 0.0f;
							}
							if (fabs(m_EG_Anim[state].anim[y].fps[l].Position.z) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Position.z = 0.0f;
							}

							m_EG_Anim[state].anim[y].fps[l].Rotation.x = (m_EG_Anim[state].anim[y].fps[l - 1].Rotation.x + rot.x);
							m_EG_Anim[state].anim[y].fps[l].Rotation.y = (m_EG_Anim[state].anim[y].fps[l - 1].Rotation.y + rot.y);
							m_EG_Anim[state].anim[y].fps[l].Rotation.z = (m_EG_Anim[state].anim[y].fps[l - 1].Rotation.z + rot.z);

							if (fabs(m_EG_Anim[state].anim[y].fps[l].Rotation.x) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Rotation.x = 0.0f;
							}
							if (fabs(m_EG_Anim[state].anim[y].fps[l].Rotation.z) < 1e-6f)
							{
								m_EG_Anim[state].anim[y].fps[l].Rotation.z = 0.0f;
							}

							m_EG_Anim[state].anim[y].fps[l].Fill = true;
						}
					}
				}
			}
			else
			{
				continue;
			}
		}
	}
}

void ENEMYSPAWNER::Enemy_Ground_SetAnimMove()
{

	{//body
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(14, { 0.0f,-0.1f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(44, { 0.0f,-0.1f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//right_arm
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,0.0f,0.2f }, { 0.0f,-20.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,0.0f,-0.2f }, { 0.0f,20.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//left_arm
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,0.0f,-0.2f }, { 0.0f,-20.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,0.0f,0.2f }, { 0.0f,20.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}
	{//right_leg
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(14, { 0.0f,0.0f,-0.2f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(44, { 0.0f,0.0f,0.3f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//left_leg
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(14, { 0.0f,0.0f,0.3f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(44, { 0.0f,0.0f,-0.2f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	Enemy_Ground_SetAnimHokan(ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_MOVE);
}

void ENEMYSPAWNER::Enemy_Ground_SetAnimAttack()
{
	//ここに槍を投げるモーションを入れる
	//左手に槍を持ってます

	{//body
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(12, { 0.0f, 0.08f, -0.15f }, { -20.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(20, { 0.0f, 0.1f, -0.2f }, { -25.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(28, { 0.0f, -0.05f, 0.2f }, { 30.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(40, { 0.0f, -0.08f, 0.15f }, { 20.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_BODY].SetInisFlame(59, { 0.0f, 0.0f, 0.0f });
	}
	{//right_arm
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(12, { -0.08f, 0.15f, -0.08f }, { -40.0f, -15.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(20, { -0.1f, 0.2f, -0.1f }, { -50.0f, -20.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(28, { 0.05f, 0.05f, 0.15f }, { 25.0f, 10.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(40, { 0.03f, 0.03f, 0.08f }, { 15.0f, 5.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f, 0.0f, 0.0f });
	}
	{//left_arm 
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(8,  { 0.15f, 0.25f, -0.15f }, { -140.0f, 20.0f, 10.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(20, { 0.2f, 0.35f, -0.2f }, { -160.0f, 25.0f, 15.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(28, { -0.2f, -0.1f, 0.4f }, { 80.0f, -15.0f, -10.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(40, { -0.15f, -0.05f, 0.25f }, { 50.0f, -10.0f, -5.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f, 0.0f, 0.0f });
	}
	{//right_leg
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(12, { 0.05f, 0.0f, -0.15f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(20, { 0.08f, 0.0f, -0.2f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(28, { -0.1f, 0.0f, 0.25f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(40, { -0.05f, 0.0f, 0.15f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_RIGHT].SetInisFlame(59, { 0.0f, 0.0f, 0.0f });
	}
	{//left_leg
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(0,  { 0.0f, 0.0f, 0.0f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(12, { -0.03f, 0.0f, -0.1f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(20, { -0.05f, 0.0f, -0.12f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(28, { 0.08f, 0.0f, 0.18f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(40, { 0.04f, 0.0f, 0.1f });
		m_EG_Anim[ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK].anim[ENEMY_GROUND_PARTS::EG_PARTS_LEG_LEFT].SetInisFlame(59, { 0.0f, 0.0f, 0.0f });
	}

	Enemy_Ground_SetAnimHokan(ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_ATTACK);
}


