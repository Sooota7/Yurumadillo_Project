//Player.cpp

#include	"keyboard.h"
#include	"Player.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"

#include	"billboard.h"

//ボールオブジェクト

ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pContext;

static ID3D11ShaderResourceView* g_Texture = NULL;


float g_StopTime = 0.0f;	// ボールが制止するまでの時間

void	PLAYER::Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	float downSize = 10.0f;

	for (int i = 0; i < PARTS_MAX; i++)
	{
		switch (i)
		{
		case PARTS_HEAD:
			m_Model[i].PartsInitialize(pDevice, pContext, "asset\\model\\head.fbx");
			m_Model[i].SetPartsScaling(XMFLOAT3(1.0f / downSize, 1.0f / downSize, 1.0f / downSize));
			break;
		case PARTS_BODY:
			m_Model[i].PartsInitialize(pDevice, pContext, "asset\\model\\body.fbx");
			m_Model[i].SetPartsScaling(XMFLOAT3(1.0f / downSize, 1.0f / downSize, 1.0f / downSize));
			break;
		case PARTS_ARM_RIGHT:
			m_Model[i].PartsInitialize(pDevice, pContext, "asset\\model\\hand.fbx");
			m_Model[i].SetPartsScaling(XMFLOAT3(1.0f/ downSize, 1.0f/ downSize, 1.0f/ downSize));
			break;
		case PARTS_ARM_LEFT:
			m_Model[i].PartsInitialize(pDevice, pContext, "asset\\model\\handL.fbx");
			m_Model[i].SetPartsScaling(XMFLOAT3(1.0f / downSize, 1.0f / downSize, 1.0f / downSize));
			break;
		case PARTS_LEG_RIGHT:
			m_Model[i].PartsInitialize(pDevice, pContext, "asset\\model\\leg.fbx");
			m_Model[i].SetPartsScaling(XMFLOAT3(1.0f / downSize, 1.0f / downSize, 1.0f / downSize));
			break;
		case PARTS_LEG_LEFT:
			m_Model[i].PartsInitialize(pDevice, pContext, "asset\\model\\legL.fbx");
			m_Model[i].SetPartsScaling(XMFLOAT3(1.0f / downSize, 1.0f / downSize, 1.0f / downSize));
			break;
		default:
			break;
		}
		
	}

	m_Position = XMFLOAT3(7.0f, 8.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_State = PLAYER_STATE::PLAYER_STATE_IDLE;

	m_LastPos = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_Hp = PLAYER_HP;
	JumpCount = true;
	BalloonFlag = false;
	BalloomUp = false;
	BalloomNow = false;
	g_StopTime = 0.0f;

	BombHave = false;

	//テクスチャ画像読み込み
	TexMetadata		metadata;
	ScratchImage	image;
	LoadFromWICFile(L"asset\\texture\\diamond.png",
		WIC_FLAGS_NONE, &metadata, image);//テクスチャは変更可
	CreateShaderResourceView(g_pDevice, image.GetImages(),
		image.GetImageCount(), metadata, &g_Texture);
	assert(g_Texture);//読み込み失敗時にダイアログを表示

	for (int i = 0; i < PLAYER_STATE::PLAYER_STATE_MAX; i++)
	{
		for (int y = 0; y < PLAYER_PARTS::PARTS_MAX; y++)
		{
			m_anim[i].anim[y].AnimInitialize();
		}
	}

	Player_SetAnimInis();

}
void	PLAYER::Player_Finalize()
{
	for (int i = 0; i < PARTS_MAX; i++)
	{
		m_Model[i].PartsFinalize();
	}
}


void	PLAYER::Player_Update()
{
	//Player_SetAnim();
	switch (m_State)
	{
	case PLAYER_STATE::PLAYER_STATE_IDLE:
		Player_Idle();
		break;
	case PLAYER_STATE::PLAYER_STATE_MOVE:
		Player_Move();
		break;
	case PLAYER_STATE::PLAYER_STATE_JUMP :
		Player_Jump();
		break;
	case PLAYER_STATE::PLAYER_STATE_BALLOON :
		Player_Balloon();
		break;
	case PLAYER_STATE::PLAYER_STATE_RESPAWN:
		Player_Respawn();
		break;
	case PLAYER_STATE::PLAYER_STATE_DEATH:
		Player_Death();
		break;
	}
	
	if (m_Velocity.y < PLAYER_FALLMAX)
	{
		m_Velocity.y = PLAYER_FALLMAX;
	}

	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	// --- ここだけ変更（着地判定とバウンド防止） ---
	float groundY = 0.0f; // 地面の高さ
	if (m_Position.y <= groundY)
	{
		m_Position.y = groundY;
		m_Velocity.y = 0.0f;
		JumpCount = true;
	}
	// --- ここまで ---

	
	if (Keyboard_IsKeyDownTrigger(KK_ENTER))
	{
		m_Hp-=20.0f;
	}


	if (m_Position.y < PLAYER_RESPAWN)
	{
		m_State = PLAYER_STATE::PLAYER_STATE_RESPAWN;
	}

	if (m_Hp < 0.0f)
	{
		m_State = PLAYER_STATE::PLAYER_STATE_DEATH;
	}

	Player_SetParts();


}

void	PLAYER::Player_Draw(BillboardManager* billboardManager)
{
	//ワールド行列作成
	XMMATRIX	scale = XMMatrixScaling(
		m_Scaling.x,
		m_Scaling.y,
		m_Scaling.z);
	XMMATRIX	rotation = XMMatrixRotationRollPitchYaw(
		m_Rotation.x,
		m_Rotation.y,
		m_Rotation.z);
	XMMATRIX	translation = XMMatrixTranslation(
		m_Position.x,
		m_Position.y,
		m_Position.z);
	XMMATRIX	world = scale * rotation * translation;

	//変換行列作成
	XMMATRIX	view = GetViewMatrix();
	XMMATRIX	projection = GetProjectionMatrix();
	XMMATRIX	wvp = world * view * projection;

	//シェーダーへ行列をセット
	Shader_SetWorldMatrix(world);
	Shader_SetMatrix(wvp);

	for (int i = 0; i < PARTS_MAX; i++)
	{
		//モデルの描画リクエスト
		switch (m_State)
		{
		case PLAYER_STATE::PLAYER_STATE_IDLE:
			m_Model[i].PartsDraw();
			break;
		case PLAYER_STATE::PLAYER_STATE_MOVE:
			m_Model[i].PartsDraw();
			break;
		case PLAYER_STATE::PLAYER_STATE_BALLOON:
			m_Model[i].PartsDraw();
			break;
		case PLAYER_STATE::PLAYER_STATE_RESPAWN:
			m_Model[i].PartsDraw();
			break;
		}
	}

	/*{
		XMFLOAT3 pos = m_Position;
		pos.y += 1.0f;
		XMFLOAT2 size = XMFLOAT2(0.5f, 0.5f);
		XMFLOAT4 col = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		int bno = 1;
		int wc = 1;
		int hc = 1;

		Billboard* bb = new Billboard(pos, size, col, bno, wc, hc, BILLBOARD_TEXTURE::TEST);
		billboardManager->Register(bb);
	}*/

}

void	PLAYER::Player_Idle()
{
	//動いていない時の待機モーションを入れてもいいかも
	if (Keyboard_IsKeyDown(KK_W) || //いずれかの移動キーを押したら移動状態に
		Keyboard_IsKeyDown(KK_A) || 
		Keyboard_IsKeyDown(KK_S) ||
		Keyboard_IsKeyDown(KK_D)  )
	{
		m_State = PLAYER_STATE::PLAYER_STATE_MOVE;
	}

	if (Keyboard_IsKeyDownTrigger(KK_SPACE)&& JumpCount == true)
	{
		m_State = PLAYER_STATE::PLAYER_STATE_JUMP;
	}

	//停止中も重力はかかる
	m_Velocity.y -= PLAYER_GRAVITY; 


}

void PLAYER::Player_Move()
{
    m_Velocity.y -= PLAYER_GRAVITY;

    // カメラの横の向きを取得
    float camY = XMConvertToRadians(GetCameraYoko());

    // �J������̑O�����x�N�g��
    XMFLOAT3 forward = XMFLOAT3(sinf(camY), 0.0f, cosf(camY));

    // �J������̉E�����x�N�g��
    XMFLOAT3 right = XMFLOAT3(cosf(camY), 0.0f, -sinf(camY));

    XMFLOAT3 move = XMFLOAT3(0, 0, 0);

    // WASDで移動
    if (Keyboard_IsKeyDown(KK_W)) // 前
        move.x -= forward.x, move.z -= forward.z;
    if (Keyboard_IsKeyDown(KK_S)) // 後
        move.x += forward.x, move.z += forward.z;
    if (Keyboard_IsKeyDown(KK_D)) // 右
        move.x -= right.x, move.z -= right.z;
    if (Keyboard_IsKeyDown(KK_A)) // 左
        move.x += right.x, move.z += right.z;

    // 正規化
    float len = sqrtf(move.x * move.x + move.z * move.z);
    if (len > 0.0f)
    {
        move.x /= len;
        move.z /= len;

        // 移動
        m_Velocity.x = move.x * PLAYER_SPEEDMAX;
        m_Velocity.z = move.z * PLAYER_SPEEDMAX;
    }
    else
    {
        // 停止
        m_Velocity.x = 0.0f;
        m_Velocity.z = 0.0f;
    }

	if (Keyboard_IsKeyDownTrigger(KK_ENTER) && JumpCount /*&&BalloonFlag==true*/)
	{
		BalloomUp = true;
		m_State = PLAYER_STATE::PLAYER_STATE_BALLOON;
	}
    // ジャンプ
    if (Keyboard_IsKeyDownTrigger(KK_SPACE) && JumpCount)
        m_State = PLAYER_STATE::PLAYER_STATE_JUMP;

	// --- 進行方向に体の向きを合わせる ---
	if (len > 0.0f)
	{
		float angle = atan2f(m_Velocity.x, m_Velocity.z); // ← Y軸回転
		m_Rotation.y = angle;
	}

    // 停止状態移行
    float v2 = m_Velocity.x * m_Velocity.x +
               m_Velocity.y * m_Velocity.y +
               m_Velocity.z * m_Velocity.z;

    if (v2 <= STOP_VELO)
    {
        g_StopTime++;
        if (g_StopTime > 60.0f * 0)
        {
            m_Velocity = XMFLOAT3(0, 0, 0);
            m_State = PLAYER_STATE::PLAYER_STATE_IDLE;
            g_StopTime = 0.0f;
        }
    }
}

void   PLAYER::Player_Jump()
{
	
	if (JumpCount == true)
	{
		m_Velocity.y += PLAYER_JUMP;
		JumpCount = false;

		if (Keyboard_IsKeyDown(KK_W) || //いずれかの移動キーを押したら移動状態に
			Keyboard_IsKeyDown(KK_A) ||
			Keyboard_IsKeyDown(KK_S) ||
			Keyboard_IsKeyDown(KK_D))
		{
			m_State = PLAYER_STATE::PLAYER_STATE_MOVE;
		}
		else
		{
			m_State = PLAYER_STATE::PLAYER_STATE_IDLE;
		}

	}
}

void    PLAYER::Player_Respawn()
{
	m_Position = XMFLOAT3(0.0f, 2.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);


	m_State = PLAYER_STATE::PLAYER_STATE_IDLE;
	
}

void PLAYER::Player_Death()
{
	
}

PLAYER* PLAYER::GetPlayer()
{
	return this;
}

void PLAYER::Player_Balloon()
{

		if (BalloomUp == true)
		{
			m_Velocity.y = PLAYER_BALLOON_SPEED; //上昇
		}
		else if (BalloomUp == false)
		{
			m_Velocity.y = PLAYER_BALLOON_FALLSPEED;
		}

   		if (Keyboard_IsKeyDownTrigger(KK_SPACE) && m_State == PLAYER_STATE::PLAYER_STATE_BALLOON)
		{
			m_State = PLAYER_STATE::PLAYER_STATE_MOVE;
		}

		
}

float l = 0;

//アニメーション処理
void PLAYER::Player_SetParts()
{
	
	for (int i = 0; i < PARTS_MAX; i++)
	{
		XMFLOAT3 pos = m_Position;
		XMFLOAT3 rot = m_Model[PARTS_BODY].GetPartsRotation();
		XMFLOAT3 plasRot = m_Model[PARTS_BODY].GetPartsRotation();


		if (JumpCount)
		{
			switch (i)
			{
			case PARTS_HEAD:
				pos = Player_AnimPos(m_State, PARTS_HEAD, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(m_State, PARTS_HEAD, &m_Model[i], rot, (int)l);
				break;
			case PARTS_BODY:
				pos = Player_AnimPos(m_State, PARTS_BODY, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(m_State, PARTS_BODY, &m_Model[i], rot, (int)l);
				break;
			case PARTS_ARM_RIGHT:
				pos = Player_AnimPos(m_State, PARTS_ARM_RIGHT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(m_State, PARTS_ARM_RIGHT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_ARM_LEFT:
				pos = Player_AnimPos(m_State, PARTS_ARM_LEFT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(m_State, PARTS_ARM_LEFT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_LEG_RIGHT:
				pos = Player_AnimPos(m_State, PARTS_LEG_RIGHT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(m_State, PARTS_LEG_RIGHT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_LEG_LEFT:
				pos = Player_AnimPos(m_State, PARTS_LEG_LEFT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(m_State, PARTS_LEG_LEFT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_MAX:
				break;
			default:
				break;
			}
			//rot.y = XMConvertToRadians(GetCameraYoko());
		}
		else
		{
			switch (i)
			{
			case PARTS_HEAD:
				pos = Player_AnimPos(PLAYER_STATE_JUMP, PARTS_HEAD, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_HEAD, &m_Model[i], rot, (int)l);
				break;
			case PARTS_BODY:
				pos = Player_AnimPos(PLAYER_STATE_JUMP, PARTS_BODY, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_BODY, &m_Model[i], rot, (int)l);
				break;
			case PARTS_ARM_RIGHT:
				pos = Player_AnimPos(PLAYER_STATE_JUMP, PARTS_ARM_RIGHT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_ARM_RIGHT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_ARM_LEFT:
				pos = Player_AnimPos(PLAYER_STATE_JUMP, PARTS_ARM_LEFT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_ARM_LEFT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_LEG_RIGHT:
				pos = Player_AnimPos(PLAYER_STATE_JUMP, PARTS_LEG_RIGHT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_LEG_RIGHT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_LEG_LEFT:
				pos = Player_AnimPos(PLAYER_STATE_JUMP, PARTS_LEG_LEFT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_JUMP, PARTS_LEG_LEFT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_MAX:
				break;
			default:
				break;
			}
			//rot.y = XMConvertToRadians(GetCameraYoko());
		}
		if (BombHave)
		{
			switch (i)
			{
			case PARTS_ARM_RIGHT:
				pos = Player_AnimPos(PLAYER_STATE_BALLOON, PARTS_ARM_RIGHT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_BALLOON, PARTS_ARM_RIGHT, &m_Model[i], rot, (int)l);
				break;
			case PARTS_ARM_LEFT:
				pos = Player_AnimPos(PLAYER_STATE_BALLOON, PARTS_ARM_LEFT, &m_Model[i], rot, (int)l);
				plasRot = Player_AnimRot(PLAYER_STATE_BALLOON, PARTS_ARM_LEFT, &m_Model[i], rot, (int)l);
				break;
			default:
				break;
			}
		}

		rot.x += XMConvertToRadians(plasRot.x);
		rot.y += XMConvertToRadians(plasRot.y);
		rot.z += XMConvertToRadians(plasRot.z);


		switch (i)
		{
		case PARTS_HEAD:
			
			m_Model[i].PartsSet(pos, m_Rotation);
			break;
		case PARTS_BODY:
			//pos = Player_LeftLeg2(&m_Model[i], rot);
			m_Model[i].PartsSet(pos, m_Rotation);
			break;
		case PARTS_ARM_RIGHT:
			//pos = Player_LeftLeg2(&m_Model[i], rot);
			m_Model[i].PartsSet(pos, rot);
			break;
		case PARTS_ARM_LEFT:
			//pos = Player_LeftLeg2(&m_Model[i], rot);
			m_Model[i].PartsSet(pos, rot);
			break;
		case PARTS_LEG_RIGHT:
			//pos = Player_LeftLeg2(&m_Model[i], rot);
			m_Model[i].PartsSet(pos, rot);
			break;
		case PARTS_LEG_LEFT:
			//pos = Player_LeftLeg2(&m_Model[i],rot);
			m_Model[i].PartsSet(pos, rot);
			break;
		
		default:
			break;
		}
	}
	l += (1.0f/1.0f);
	if (l >= 60)
	{
		l = 0;
	}
}

//
//float tes00 = 30.0f;
//int tes12 = 7;

XMFLOAT3 PLAYER::Player_AnimPos(PLAYER_STATE state, PLAYER_PARTS part,PARTS* parts, XMFLOAT3 rot,int frame)
{
	XMFLOAT3 pos = GetPlayerPosition();
	XMFLOAT3 InisPos = parts->GetInisPosition();
	XMFLOAT3 nowPos = parts->GetNowPosition();
	XMFLOAT3 lastPos = m_anim[state].anim[part].fps[frame].Position;
	XMFLOAT3 lastRot = m_anim[state].anim[part].fps[frame].Rotation;
	bool	 loop = parts->GetAnimLoop();
	/*parts->GetAnimLastPosition(parts->GetNowPos());*/
	//x値
	pos.x -= InisPos.x * cosf(rot.y);
	pos.z += InisPos.x * sinf(rot.y);

	//z値
	pos.x -= InisPos.z * cosf(rot.y);
	pos.z += InisPos.z * sinf(rot.y);

	//ｙ値
	pos.y += InisPos.y;

	//x値
	pos.x -= lastPos.x * cosf(rot.y);
	pos.z += lastPos.x * sinf(rot.y);

	//z値
	pos.z += lastPos.z * cosf(rot.y);
	pos.x += lastPos.z * sinf(rot.y);

	//ｙ値
	pos.y += lastPos.y;

	//XMFLOAT3 tesPos = parts->GetAnimLastPosition(0);
	//
	//nowPos.x += tesPos.x / 6;
	//nowPos.z += tesPos.z / 6;
	// 
	////nowPos = parts->m_Frame[7].GetPosition();
	///*if (nowPos.x >= tesPos.x&&tesPos.x>=0)
	//{
	//	if (tes12 == 7)
	//	{

	//	}
	//}
	//else if (nowPos.x < tesPos.x && tesPos.x<0)
	//{

	//}*/

	//nowPos.z += lastPos.z / tes00;
	//
	//pos.x += (nowPos.z * sinf(rot.y));
	//pos.z += (nowPos.z * cosf(rot.y));


	//if (loop)
	//{
	//	lastPos.z = -lastPos.z;
	//}

	//pos.x += nowPos.z * sinf(rot.y);
	//pos.z += nowPos.z * cosf(rot.y);

	//parts->SetAnimLoop(loop);
	////parts->SetAnimLastPosition(lastPos);
	//parts->SetNowPosition(nowPos);
	return pos;
}

XMFLOAT3 PLAYER::Player_AnimRot(PLAYER_STATE state, PLAYER_PARTS part, PARTS* parts, XMFLOAT3 rot, int frame)
{
	XMFLOAT3 Rot = GetPlayerPosition();
	XMFLOAT3 lastRot = m_anim[state].anim[part].fps[frame].Rotation;
	
	//x値
	Rot.x = lastRot.x;
	
	//z値
	Rot.z = lastRot.z ;
	
	//ｙ値
	Rot.y = lastRot.y;

	
	return Rot;
}

void PLAYER::Player_SetAnim()
{
	switch (m_State)
	{
	case PLAYER_STATE_IDLE:
		Player_SetAnimIdle();
		break;
	case PLAYER_STATE_MOVE:
		Player_SetAnimMove();
		break;
	case PLAYER_STATE_JUMP:
		Player_SetAnimJunp();
		break;
	case PLAYER_STATE_BALLOON:

		break;
	case PLAYER_STATE_RESPAWN:
		break;
	case PLAYER_STATE_DEATH:
		break;
	case PLAYER_STATE_MAX:
		Player_SetAnimHave_MAX();
		break;
	default:
		break;
	}
}

void PLAYER::Player_SetAnimInis()
{
	XMFLOAT3 pos = XMFLOAT3(0.25f, 0.0f, 0.25f);

	for (int i = 0; i < PARTS_MAX; i++)
	{
		for (int i = 0; i < PARTS_MAX; i++)
		{
			switch (i)
			{
			case PARTS_HEAD:
				m_Model[i].SetInisPosition(XMFLOAT3(0.0f, 0.65f, 0.0f));
				break;
			case PARTS_BODY:
				m_Model[i].SetInisPosition(XMFLOAT3(0.0f, 0.3f, 0.0f));
				break;
			case PARTS_ARM_RIGHT:
				m_Model[i].SetInisPosition(XMFLOAT3(-0.3f, 0.3f, 0.0f));
				break;
			case PARTS_ARM_LEFT:
				m_Model[i].SetInisPosition(XMFLOAT3(0.3f, 0.3f, 0.0f));
				break;
			case PARTS_LEG_RIGHT:
				m_Model[i].SetInisPosition(XMFLOAT3(-0.2f, 0.0f, 0.0f));
				break;
			case PARTS_LEG_LEFT:
				m_Model[i].SetInisPosition(XMFLOAT3(0.2f, 0.0f, 0.0f));
				break;
			default:
				break;
			}
		}

	}
	Player_SetAnimMove();
	Player_SetAnimJunp();
	Player_SetAnimHave_MAX();
}

//調整前
void PLAYER::Player_SetAnimHokan(PLAYER_STATE state)
{
	for (int y = 0; y < PLAYER_PARTS::PARTS_MAX; y++)
	{
		for (int i = 0; i < FLAME_MAX; i++)
		{
			if (i != 0 && m_anim[state].anim[y].fps[i].SetPos)
			{
				int a = 0;

				for (int i = 0; i < FLAME_MAX; i++)
				{
					if (!m_anim[state].anim[y].fps[i].Fill)
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
						(m_anim[state].anim[y].fps[i].Position.x - m_anim[state].anim[y].fps[a].Position.x) / waru,
						(m_anim[state].anim[y].fps[i].Position.y - m_anim[state].anim[y].fps[a].Position.y) / waru,
						(m_anim[state].anim[y].fps[i].Position.z - m_anim[state].anim[y].fps[a].Position.z) / waru };



				}
				else
				{
					pos = {
						(m_anim[state].anim[y].fps[i].Position.x - m_anim[state].anim[y].fps[a - 1].Position.x) / waru,
						(m_anim[state].anim[y].fps[i].Position.y - m_anim[state].anim[y].fps[a - 1].Position.y) / waru,
						(m_anim[state].anim[y].fps[i].Position.z - m_anim[state].anim[y].fps[a - 1].Position.z) / waru };

				}

				if (a == 0)
				{
					rot = {
						(m_anim[state].anim[y].fps[i].Rotation.x - m_anim[state].anim[y].fps[a].Rotation.x) / waru,
						(m_anim[state].anim[y].fps[i].Rotation.y - m_anim[state].anim[y].fps[a].Rotation.y) / waru,
						(m_anim[state].anim[y].fps[i].Rotation.z - m_anim[state].anim[y].fps[a].Rotation.z) / waru };



				}
				else
				{
					rot = {
						(m_anim[state].anim[y].fps[i].Rotation.x - m_anim[state].anim[y].fps[a - 1].Rotation.x) / waru,
						(m_anim[state].anim[y].fps[i].Rotation.y - m_anim[state].anim[y].fps[a - 1].Rotation.y) / waru,
						(m_anim[state].anim[y].fps[i].Rotation.z - m_anim[state].anim[y].fps[a - 1].Rotation.z) / waru };

				}

				for (int l = a; l < i; l++)
				{

					if (!m_anim[state].anim[y].fps[l].Fill)
					{
						if (l == 0)
						{
							m_anim[state].anim[y].fps[l].Position.x = (m_anim[state].anim[y].fps[l].Position.x + pos.x);
							m_anim[state].anim[y].fps[l].Position.y = (m_anim[state].anim[y].fps[l].Position.y + pos.y);
							m_anim[state].anim[y].fps[l].Position.z = (m_anim[state].anim[y].fps[l].Position.z + pos.z);

							if (fabs(m_anim[state].anim[y].fps[l].Position.x) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Position.x = 0.0f;
							}
							if (fabs(m_anim[state].anim[y].fps[l].Position.z) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Position.z = 0.0f;
							}

							m_anim[state].anim[y].fps[l].Rotation.x = (m_anim[state].anim[y].fps[l].Rotation.x + rot.x);
							m_anim[state].anim[y].fps[l].Rotation.y = (m_anim[state].anim[y].fps[l].Rotation.y + rot.y);
							m_anim[state].anim[y].fps[l].Rotation.z = (m_anim[state].anim[y].fps[l].Rotation.z + rot.z);

							if (fabs(m_anim[state].anim[y].fps[l].Rotation.x) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Rotation.x = 0.0f;
							}
							if (fabs(m_anim[state].anim[y].fps[l].Rotation.z) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Rotation.z = 0.0f;
							}

							m_anim[state].anim[y].fps[l].Fill = true;
						}
						else
						{

							m_anim[state].anim[y].fps[l].Position.x = (m_anim[state].anim[y].fps[l - 1].Position.x + pos.x);
							m_anim[state].anim[y].fps[l].Position.y = (m_anim[state].anim[y].fps[l - 1].Position.y + pos.y);
							m_anim[state].anim[y].fps[l].Position.z = (m_anim[state].anim[y].fps[l - 1].Position.z + pos.z);

							if (fabs(m_anim[state].anim[y].fps[l].Position.x) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Position.x = 0.0f;
							}
							if (fabs(m_anim[state].anim[y].fps[l].Position.z) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Position.z = 0.0f;
							}

							m_anim[state].anim[y].fps[l].Rotation.x = (m_anim[state].anim[y].fps[l - 1].Rotation.x + rot.x);
							m_anim[state].anim[y].fps[l].Rotation.y = (m_anim[state].anim[y].fps[l - 1].Rotation.y + rot.y);
							m_anim[state].anim[y].fps[l].Rotation.z = (m_anim[state].anim[y].fps[l - 1].Rotation.z + rot.z);

							if (fabs(m_anim[state].anim[y].fps[l].Rotation.x) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Rotation.x = 0.0f;
							}
							if (fabs(m_anim[state].anim[y].fps[l].Rotation.z) < 1e-6f)
							{
								m_anim[state].anim[y].fps[l].Rotation.z = 0.0f;
							}

							m_anim[state].anim[y].fps[l].Fill = true;
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

void PLAYER::Player_SetAnimIdle()
{
	XMFLOAT3 pos = XMFLOAT3(0.25f, 0.0f, 0.25f);

	for (int i = 0; i < PARTS_MAX; i++)
	{
		for (int i = 0; i < PARTS_MAX; i++)
		{
			switch (i)
			{
			case PARTS_HEAD:
				m_Model[i].SetlasPosMax(0);
				m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
				break;
			case PARTS_BODY:
				m_Model[i].SetlasPosMax(0);
				m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
				break;
			case PARTS_ARM_RIGHT:
				m_Model[i].SetlasPosMax(0);
				m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
				break;
			case PARTS_ARM_LEFT:
				m_Model[i].SetlasPosMax(0);
				m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
				break;
			case PARTS_LEG_RIGHT:
				m_Model[i].SetlasPosMax(0);
				m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
				break;
			case PARTS_LEG_LEFT:
				m_Model[i].SetlasPosMax(0);
				m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
				break;
			default:
				break;
			}
		}

	}
}

void PLAYER::Player_SetAnimMove()
{


	{//right_head
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(0,  { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(14, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(44, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//right_body
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(0,  { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(14, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(44, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//right_arm
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,0.0f,0.2f }, { 0.0f,-20.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,0.0f,-0.2f }, { 0.0f,20.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//left_arm
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,0.0f,-0.2f }, { 0.0f,-20.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,0.0f,0.2f }, { 0.0f,20.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}
	{//right_leg
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(14, { 0.0f,0.0f,-0.2f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(44, { 0.0f,0.0f,0.3f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//left_leg
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(14, { 0.0f,0.0f,0.3f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(44, { 0.0f,0.0f,-0.2f });
		m_anim[PLAYER_STATE::PLAYER_STATE_MOVE].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	Player_SetAnimHokan(PLAYER_STATE::PLAYER_STATE_MOVE);

	//Player_SetAnimInis();
	/*
	////7 15 22 30
	//int animPoint[] = { 7,15,22,29 };
	//for (int i = 0; i < PARTS_MAX; i++)
	//{
	//	switch (i)
	//	{
	//	case PARTS_HEAD:
	//		m_Model[i].m_Frame[7].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[15].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[22].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[29].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		break;
	//	case PARTS_BODY:
	//		m_Model[i].m_Frame[7].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[15].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[22].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[29].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		break;
	//	case PARTS_ARM_RIGHT:
	//		m_Model[i].m_Frame[7].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.4f));
	//		m_Model[i].m_Frame[15].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[22].SetPosition(XMFLOAT3(0.0f, 0.0f, -0.4f));
	//		m_Model[i].m_Frame[29].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		break;
	//	case PARTS_ARM_LEFT:
	//		m_Model[i].m_Frame[7].SetPosition(XMFLOAT3(0.0f, 0.0f, -0.4f));
	//		m_Model[i].m_Frame[15].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[22].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.4f));
	//		m_Model[i].m_Frame[29].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		break;
	//	case PARTS_LEG_RIGHT:
	//		m_Model[i].m_Frame[7].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.3f));
	//		m_Model[i].m_Frame[15].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[22].SetPosition(XMFLOAT3(0.0f, 0.0f, +0.3f));
	//		m_Model[i].m_Frame[29].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		break;
	//	case PARTS_LEG_LEFT:
	//		m_Model[i].m_Frame[7].SetPosition(XMFLOAT3(0.0f, 0.0f, -0.3f));
	//		m_Model[i].m_Frame[15].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		m_Model[i].m_Frame[22].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.3f));
	//		m_Model[i].m_Frame[29].SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//		break;
	//	default:
	//		break;
	//	}
	//}

	
	

	//for (int a = 0; a < PARTS_MAX; a++)
	//{
	//	
	//		int y = 0;

	//	switch (a)
	//	{
	//	case PARTS_HEAD:
	//	case PARTS_BODY:
	//	case PARTS_ARM_RIGHT:
	//	case PARTS_ARM_LEFT:
	//	case PARTS_LEG_RIGHT:
	//	case PARTS_LEG_LEFT:

	//		for (int i : animPoint)
	//		{

	//			XMFLOAT3 nextPos = m_Model[a].m_Frame[i].GetPosition();
	//			XMFLOAT3 pos = m_Model[a].m_Frame[y].GetPosition();

	//			for (int p = (y); p < (i - y); p++)
	//			{
	//				if (pos.x > nextPos.x)
	//				{
	//					pos.x += (pos.x - nextPos.x / (i - y)) + (-pos.x / (i - y));
	//				}
	//				else
	//				{
	//					pos.x -= ( nextPos.x - pos.x/ (i - y)) + (-pos.x / (i - y));
	//				}

	//				if (pos.y > nextPos.y)
	//				{
	//					pos.y += (pos.y - nextPos.y / (i - y)) + (-pos.y / (i - y));
	//				}
	//				else
	//				{
	//					pos.y -= (nextPos.y - pos.y / (i - y)) + (-pos.y / (i - y));
	//				}

	//				if (pos.z > nextPos.z)
	//				{
	//					pos.z += (pos.z - nextPos.z / (i - y)) + (-pos.z / (i - y));
	//				}
	//				else
	//				{
	//					pos.z -= (nextPos.z - pos.z / (i - y)) + (-pos.z / (i - y));
	//				}

	//				m_Model[a].m_Frame[p].SetPosition(pos);
	//			}

	//			y = i;
	//		}
	//		
	//		break;
	//	case PARTS_MAX:
	//		break;
	//	default:
	//		break;
	//	}
	//}
		
	*/
	//for (int i = 0; i < PARTS_MAX; i++)
	//{
	//	switch (i)
	//	{
	//	case PARTS_HEAD:
	//		m_Model[i].SetlasPosMax(0);
	//		m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
	//		break;
	//	case PARTS_BODY:
	//		m_Model[i].SetlasPosMax(0);
	//		m_Model[i].SetAnimLastPosition(XMFLOAT3(0.0f, 0.0f, 0.0f),0);
	//		break;
	//	case PARTS_ARM_RIGHT:
	//		m_Model[i].SetlasPosMax(2);
	//		m_Model[i].SetAnimLastPosition(XMFLOAT3(-0.5f, 0.0f, -0.5f),0);
	//		break;
	//	case PARTS_ARM_LEFT:
	//		m_Model[i].SetlasPosMax(0);
	//		m_Model[i].SetAnimLastPosition(XMFLOAT3(0.5f, 0.0f, 0.5f),0);
	//		break;
	//	case PARTS_LEG_RIGHT:
	//		m_Model[i].SetlasPosMax(0);
	//		m_Model[i].SetAnimLastPosition(XMFLOAT3(-0.3f, 0.0f, -0.3f),0);
	//		break;
	//	case PARTS_LEG_LEFT:
	//		m_Model[i].SetlasPosMax(0);
	//		m_Model[i].SetAnimLastPosition(XMFLOAT3(0.3f, 0.0f, 0.3f),0);
	//		break;
	//	default:
	//		break;
	//	}
	//}
}

void PLAYER::Player_SetAnimJunp()
{
	{//right_arm
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,0.2f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(6,  { 0.0f,0.1f,0.1f});
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(22, { 0.0f,0.1f,-0.13f  });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,0.2f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(36, { 0.0f,0.1f,0.1f});
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(51, { 0.0f,0.1f,-0.13f  });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.2f,0.0f });
	}



	//{//right_arm
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(0,  { 0.0f,0.2f,0.0f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(14, { 0.0f,0.1f,-0.1f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(44, { 0.0f,0.1f,0.1f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.2f,0.0f });
	//}

	{//right_arm
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(0, { 0.0f,0.2f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(6, { 0.0f,0.1f,0.1f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(22, { 0.0f,0.1f,-0.13f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,0.2f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(36, { 0.0f,0.1f,0.1f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(51, { 0.0f,0.1f,-0.13f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.2f,0.0f });
	}

	//{//left_arm
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(0,  { 0.0f,0.2f,0.0f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(14, { 0.0f,0.1f,0.1f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(44, { 0.0f,0.1f,-0.1f });
	//	m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.2f,0.0f });
	//}

	{//right_leg
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(0,  { 0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(14, { 0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(29, { 0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(44, { 0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(59, { 0.08f,0.0f,0.0f });
	}

	{//left_leg
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(0,  { -0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(14, { -0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(29, { -0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(44, { -0.08f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_JUMP].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(59, { -0.08f,0.0f,0.0f });
	}

	Player_SetAnimHokan(PLAYER_STATE::PLAYER_STATE_JUMP);

}

void PLAYER::Player_SetAnimHave_MAX()
{
	{//right_head
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(14, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(44, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_HEAD].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}
	{//right_body
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(14, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(44, { 0.0f,-0.1f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_BODY].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}
	{//right_arm
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(0, { 0.0f,0.6f,0.0f }, { 200.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_ARM_RIGHT].SetInisFlame(59, { 0.0f,0.6f,0.0f }, { 200.0f,0.0f,0.0f });
	}

	{//left_arm
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(0, { 0.0f,0.6f,0.0f }, { 200.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_ARM_LEFT].SetInisFlame(59, { 0.0f,0.6f,0.0f }, { 200.0f,0.0f,0.0f });
	}
	{//right_leg
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(14, { 0.0f,0.0f,-0.2f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(44, { 0.0f,0.0f,0.3f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_RIGHT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	{//left_leg
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(0, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(14, { 0.0f,0.0f,0.3f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(29, { 0.0f,0.0f,0.0f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(44, { 0.0f,0.0f,-0.2f });
		m_anim[PLAYER_STATE::PLAYER_STATE_BALLOON].anim[PLAYER_PARTS::PARTS_LEG_LEFT].SetInisFlame(59, { 0.0f,0.0f,0.0f });
	}

	Player_SetAnimHokan(PLAYER_STATE::PLAYER_STATE_BALLOON);

}

void PLAYER::Player_SetAnimBaloon()
{
}

void PLAYER::Player_SetAnimRespawn()
{
}

void PLAYER::Player_SetAnimDeath()
{
}

