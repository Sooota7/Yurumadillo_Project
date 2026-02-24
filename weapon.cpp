#include "weapon.h"
#include "camera.h"
#include "Dictionary.h"
#include "player.h"
#include "EnemyButterfly.h"

//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;

void WEAPON::Weapon_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		m_Weapon[i].WeaponSource_Initialize(XMFLOAT3(0.0f, 0.0f, 0.0f), WEAPON_NONE);
	}
	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		m_EG_Weapon[i].Weapon_EG_Initialize(XMFLOAT3(0.0f, 0.0f, 0.0f), EG_WEAPON_NONE);
	}

	for (int i = 0; i < WEAPON_STATE::WEAPON_MAX; i++)
	{
		switch (i)
		{
		case WEAPON_NONE:
			break;
		case WEAPON_MOVE:
			m_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case WEAPON_DIRECTION:
			m_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case WEAPON_POWER:
			m_Model[i] = ModelLoad("asset\\model\\ball.fbx");
			break;
		case WEAPON_GROUND:
			m_Model[i] = ModelLoad("asset\\model\\ball.fbx");
		case WEAPON_MAX:
			break;
		default:
			break;
		}
	}

	m_EG_Model = ModelLoad("asset\\model\\EnemyGround\\EnemyGroundWeapon.fbx");

	/*for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		m_Weapon[i].WeaponSource_Initialize(XMFLOAT3(0.0f, 2.0f, 0.0f), WEAPON_STATE::WEAPON_SAFE);
		m_Model[WEAPON_ACTIVE] = ModelLoad("asset\\model\\ball.fbx");
	}*/
}

void WEAPON::Weapon_Finalize(void)
{
	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		m_Weapon[i].WeaponSource_Finalize();
		m_EG_Weapon[i].Weapon_EG_Finalize();
	}

	for (int i = 0; i < WEAPON_STATE::WEAPON_MAX; i++)
	{
		if (m_Model[i] != NULL)
		{
			ModelRelease(m_Model[i]);
			m_Model[i] = NULL;
		}

	}

	SAFE_RELEASE(g_VertexBuffer);
	SAFE_RELEASE(g_IndexBuffer);
	SAFE_RELEASE(g_Texture);
}

void WEAPON::Weapon_Draw(void)
{

	//シェーダーを描画パイプラインへ設定
	Shader_Begin();


	//プロジェクション行列作成
	XMMATRIX	Projection = GetProjectionMatrix();
	//ビュー行列作成
	XMMATRIX	View = GetViewMatrix();
	//先にVP変換行列を作っておく
	XMMATRIX	VP = View * Projection;


	static float rot = 0.0f;
	rot -= 0.5f;

	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{

		XMFLOAT3 weaponPos = m_Weapon[i].WeaponSource_GetPosition();

		//スケーリング行列の作成
		XMMATRIX	ScalingMatrix = XMMatrixScaling
		(
			1.0f,
			1.0f,
			1.0f
		);

		//平行移動行列の作成
		XMMATRIX	TranslationMatrix = XMMatrixTranslation
		(
			weaponPos.x,
			weaponPos.y,
			weaponPos.z
		);

		//回転行列の作成
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
		(
			XMConvertToRadians(0.0f),
			//XMConvertToRadians(rot),
			//XMConvertToRadians(rot),
			XMConvertToRadians(0.0f),
			XMConvertToRadians(0.0f)
		);
		//ワールド行列の作成
		XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
		//最終的な変換行列を作成
		XMMATRIX WVP = World * VP;//(VP = View*Projection)
		//DirectXへ行列をセット
		Shader_SetMatrix(WVP);

		//テクスチャをセット
		g_pContext->PSSetShaderResources(0, 1, &g_Texture);

		//頂点バッファをセット
		UINT	stride = sizeof(Vertex3D);	//頂点１個のデータサイズ
		UINT	offset = 0;
		g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		//インデックスバッファをセット
		g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//描画するポリゴンの種類をセット 3頂点でポリゴン１枚として表示
		g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//描画リクエスト
		switch (m_Weapon[i].WeaponSource_GetState())
		{
		case WEAPON_NONE:
			g_pContext->DrawIndexed(6 * 6, 0, 0);
			break;
		case WEAPON_MOVE:
			ModelDraw(m_Model[WEAPON_MOVE]);
			break;
		case WEAPON_DIRECTION:
			ModelDraw(m_Model[WEAPON_DIRECTION]);
			break;
		case WEAPON_POWER:
			ModelDraw(m_Model[WEAPON_POWER]);
			break;
		case WEAPON_GROUND:
			ModelDraw(m_Model[WEAPON_GROUND]);
			break;
		case WEAPON_MAX:
			break;
		default:
			break;
		}

	}
	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{

		XMFLOAT3 weaponPos = m_EG_Weapon[i].Weapon_EG_GetPosition();
		XMFLOAT3 weaponRot = m_EG_Weapon[i].Weapon_EG_GetRotation();

		//スケーリング行列の作成
		XMMATRIX	ScalingMatrix = XMMatrixScaling
		(
			1.0f,
			1.0f,
			1.0f
		);

		//平行移動行列の作成
		XMMATRIX	TranslationMatrix = XMMatrixTranslation
		(
			weaponPos.x,
			weaponPos.y,
			weaponPos.z
		);

		//回転行列の作成
		XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
		(
			weaponRot.x,
			weaponRot.y,
			weaponRot.z
		);

		//ワールド行列の作成
		XMMATRIX World = ScalingMatrix * RotationMatrix * TranslationMatrix;
		//最終的な変換行列を作成
		XMMATRIX WVP = World * VP;//(VP = View*Projection)
		//DirectXへ行列をセット
		Shader_SetMatrix(WVP);

		//テクスチャをセット
		g_pContext->PSSetShaderResources(0, 1, &g_Texture);

		//頂点バッファをセット
		UINT	stride = sizeof(Vertex3D);	//頂点１個のデータサイズ
		UINT	offset = 0;
		g_pContext->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

		//インデックスバッファをセット
		g_pContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//描画するポリゴンの種類をセット 3頂点でポリゴン１枚として表示
		g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//描画リクエスト
		switch (m_EG_Weapon[i].Weapon_EG_GetState())
		{
		case EG_WEAPON_NONE:
			g_pContext->DrawIndexed(6 * 6, 0, 0);
			break;
		case EG_WEAPON_MOVE:
			ModelDraw(m_EG_Model);
			break;
		case EG_WEAPON_DIRECTION:
			ModelDraw(m_EG_Model);
			break;
		case EG_WEAPON_POWER:
			ModelDraw(m_EG_Model);
			break;
		case EG_WEAPON_MAX:
			break;
		default:
			break;
		}

	}
}

void WEAPON::Weapon_Update(XMFLOAT3 playerPos, ENEMYSPAWNER* enemySpawner)
{
	ENEMY_BUTTERFLY* eb = enemySpawner->EnemySpawner_GetEnemyButterfly();
	ENEMY_GROUND* eg = enemySpawner->EnemySpawner_GetEnemyGround();


	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		if (eb[i].GetEnemyButterflyState() == ENEMY_BUTTERFLY_STATE_ATTACK)
		{
			SetWeapon(eb[i].GetEnemyPosition());
		}
		if (eg[i].GetEnemyGroundState() == ENEMY_GROUND_STATE_CREATE_WEAPON)
		{
			SetWeaponEG(eg[i].GetEnemyPosition());
		}
	}

	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		switch (m_Weapon[i].WeaponSource_GetState())
		{
		case WEAPON_NONE:
			break;
		case WEAPON_MOVE:
			m_Weapon[i].WeaponSource_Move();
			break;
		case WEAPON_DIRECTION:
			m_Weapon[i].WeaponSource_Direction(playerPos);
			break;
		case WEAPON_POWER:
			m_Weapon[i].WeaponSource_Power();
			break;
		case WEAPON_GROUND:
			m_Weapon[i].WeaponSource_Ground();
			break;
		case WEAPON_MAX:
			break;
		default:
			break;
		}

		switch (m_EG_Weapon[i].Weapon_EG_GetState())
		{
		case EG_WEAPON_NONE:
			break;
		case EG_WEAPON_MOVE:
			m_EG_Weapon[i].Weapon_EG_Move();
			break;
		case EG_WEAPON_DIRECTION:
			m_EG_Weapon[i].Weapon_EG_Direction(playerPos);
			break;
		case EG_WEAPON_POWER:
			m_EG_Weapon[i].Weapon_EG_Power();
			break;
		case EG_WEAPON_MAX:
			break;
		default:
			break;
		}
	}
}

WEAPONSOURCE* WEAPON::Weapon_GetWeapon()
{
	return m_Weapon;
}

WEAPONENEMYGROUND* WEAPON::EG_Weapon_GetWeapon()
{
	return m_EG_Weapon;
}

void WEAPON::SetWeapon(XMFLOAT3 pos)
{
	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		if (m_Weapon[i].WeaponSource_GetState() == WEAPON_NONE)
		{
			m_Weapon[i].WeaponSource_Initialize(pos, WEAPON_STATE::WEAPON_DIRECTION);
			break;
		}
	}
}

void WEAPON::SetWeaponEG(XMFLOAT3 pos)
{
	for (int i = 0; i < WEAPON_NUM_MAX; i++)
	{
		if (m_EG_Weapon[i].Weapon_EG_GetState() == EG_WEAPON_NONE)
		{
			m_EG_Weapon[i].Weapon_EG_Initialize(pos, EG_WEAPON_STATE::EG_WEAPON_DIRECTION);
			break;
		}
	}
}
