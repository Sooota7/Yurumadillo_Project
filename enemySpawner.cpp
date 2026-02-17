#include "enemySpawner.h"
#include "Dictionary.h"
#include "camera.h"


//グローバル変数
static ID3D11Device* g_pDevice = NULL;
static ID3D11DeviceContext* g_pContext = NULL;
//頂点バッファ
static ID3D11Buffer* g_VertexBuffer = NULL;
//インデックスバッファ
static ID3D11Buffer* g_IndexBuffer = NULL;
//テクスチャ変数
static ID3D11ShaderResourceView* g_Texture;


void ENEMYSPAWNER::EnemySpawner_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FIELD_NO no)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	// テクスチャ読み込み
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"Asset\\Texture\\block_field.png", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);

	//初期化
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		m_Enemy[i].Initialize(g_pDevice, g_pContext);
		m_Enemy[i].SetEnemyNormalType(ENEMY_TYPE::ENEMY_TYPE_NONE);
	}

	// ======================================================
	// なんか違ったら変えて
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		m_EnemyButterfly[i].Initialize(g_pDevice, g_pContext);
		m_EnemyButterfly[i].SetEnemyButterflyType(ENEMY_TYPE::ENEMY_TYPE_NONE);
	}
	 
	// ======================================================
	// なんか違ったら変えて(藤原version)
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		m_EnemyGround[i].Initialize(g_pDevice, g_pContext);
		m_EnemyGround[i].SetEnemyGroundType(ENEMY_TYPE::ENEMY_TYPE_NONE);
	}
	 
	// ======================================================


	//マップのセット
	int a = 0;
	int b = 0;
	int c = 0;	//EnemyGroundの数

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
				}

				
			}
		}
	}
	

	//ブロックの作成
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		switch (i)
		{
		case ENEMY_TYPE_NONE:
			break;
		case ENEMY_TYPE_NORMAL:
			m_Model[i] = ModelLoad("asset\\model\\tree.fbx");//デバッグ
			break;
		case ENEMY_TYPE_BUTTERFLY:
			m_Model[i] = ModelLoad("asset\\model\\test_goal.fbx");//デバッグ
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}
	}

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
			m_EG_Model[i] = ModelLoad("asset\\model\\ball.fbx");
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
	//シェーダーを描画パイプラインへ設定
	Shader_Begin();


	//プロジェクション行列作成
	XMMATRIX	Projection = GetProjectionMatrix();
	//ビュー行列作成
	XMMATRIX	View = GetViewMatrix();
	//先にVP変換行列を作っておく
	XMMATRIX	VP = View * Projection;

	//MAPの表示
	int i = 0;

	static float rot = 0.0f;
	rot -= 0.5f;

	for(int i = 0; i< Enemy_Spawner_MAX;i++)
	{
		//死亡、存在しない場合書かない
		if (m_Enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_NONE&&
			m_Enemy[i].GetEnemyNormalType() != ENEMY_TYPE::ENEMY_TYPE_DEAD)
		{
			XMFLOAT3 mapPos = m_Enemy[i].GetEnemyPosition();

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
				mapPos.x,
				mapPos.y,
				mapPos.z
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
			//モデル一個しかないから追加するときに変える
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
	// いったんそのまま
	// 浮いてる敵
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		//死亡、存在しない場合書かない
		if (m_EnemyButterfly[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_NONE &&
			m_EnemyButterfly[i].GetEnemyButterflyType() != ENEMY_TYPE::ENEMY_TYPE_DEAD)
		{
			XMFLOAT3 mapPos = m_EnemyButterfly[i].GetEnemyPosition();

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
				mapPos.x,
				mapPos.y,
				mapPos.z
			);

			//回転行列の作成
			XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
			(
				XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().x),
				//XMConvertToRadians(rot),
				//XMConvertToRadians(rot),
				m_EnemyButterfly[i].GetEnemyRotation().y,
				//XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().y),
				XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().z)
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
			//モデル一個しかないから追加するときに変える
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
	//EnemyGround描画動作確認用_描画モデルバタフライのまま
	//=======================================================
	for (int i = 0; i < Enemy_Spawner_MAX; i++)
	{
		//死亡、存在しない場合書かない
		if (m_EnemyGround[i].GetEnemyGroundType() != ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_NONE &&
			m_EnemyGround[i].GetEnemyGroundType() != ENEMY_GROUND_STATE::ENEMY_GROUND_STATE_DEAD)
		{

			ENEMY_GROUND_UpdateAnim();

			XMFLOAT3 mapPos = m_EnemyGround[i].GetEnemyPosition();

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
				mapPos.x,
				mapPos.y,
				mapPos.z
			);

			//回転行列の作成
			XMMATRIX	RotationMatrix = XMMatrixRotationRollPitchYaw
			(
				XMConvertToRadians(m_EnemyGround[i].GetEnemyRotation().x),
				//XMConvertToRadians(rot),
				//XMConvertToRadians(rot),
				m_EnemyGround[i].GetEnemyRotation().y,
				//XMConvertToRadians(m_EnemyButterfly[i].GetEnemyRotation().y),
				XMConvertToRadians(m_EnemyGround[i].GetEnemyRotation().z)
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

			PARTS* EG_Parts = m_EnemyGround[i].GetEnemyGroundParts();

			//描画リクエスト
			//モデル一個しかないから追加するときに変える
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
			EnemySpawner_SetKillNum(1);						//死んだらカウントする
			m_Enemy[i].SetEnemyNormalType(ENEMY_TYPE_NONE);	//存在を消す
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}

		

	}
	
	// 浮いてる敵
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
			EnemySpawner_SetKillNum(1);						//死んだらカウントする
			m_EnemyButterfly[i].SetEnemyButterflyType(ENEMY_TYPE_NONE);	//存在を消す
			break;
		case ENEMY_TYPE_MAX:
			break;
		default:
			break;
		}



	}
	
	//==========================================
	// EnemyGroundのUpdate
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
			EnemySpawner_SetKillNum(1);						//死んだらカウントする
			m_EnemyGround[i].SetEnemyGroundType(ENEMY_TYPE_NONE);	//存在を消す
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

//配置した数、倒すべき敵の数
int ENEMYSPAWNER::EnemySpawner_GetEnemyNum()
{
	return MaxNum;
}

//倒した数をカウント、今のところ１のみ
void ENEMYSPAWNER::EnemySpawner_SetKillNum(int killnum)
{
	NowKillNum += killnum;
}

//現在倒した敵の合計数を返す
int ENEMYSPAWNER::EnemySpawner_GetKillNum()
{
	return NowKillNum;
}



//============================================================
// Enemy_Ground_Anim
//============================================================
void ENEMYSPAWNER::ENEMY_GROUND_SetAnimInis()
{
	Enemy_Ground_SetAnimMove();
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
	//x値
	position.x -= InisPos.x * cosf(rot.y);
	position.z += InisPos.x * sinf(rot.y);

	//z値
	position.x -= InisPos.z * cosf(rot.y);
	position.z += InisPos.z * sinf(rot.y);

	//ｙ値
	position.y += InisPos.y;

	//x値
	position.x -= lastPos.x * cosf(rot.y);
	position.z += lastPos.x * sinf(rot.y);

	//z値
	position.z += lastPos.z * cosf(rot.y);
	position.x += lastPos.z * sinf(rot.y);

	//ｙ値
	position.y += lastPos.y;

	return position;

}



//=====================================================================
// EnemyGroundAnimSet関数及び補間関数
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
	{//body

	}
	{//left_arm

	}
	{//left_arm

	}
	{//right_leg

	}
	{//left_leg

	}
}


