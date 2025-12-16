//Player.cpp

#include	"keyboard.h"
#include	"Player.h"
#include	"Camera.h"
#include	"shader.h"
#include	"collision.h"

//ボールオブジェクト

ID3D11Device* g_pDevice;
ID3D11DeviceContext* g_pContext;

float g_StopTime = 0.0f;	// ボールが制止するまでの時間

void	PLAYER::Player_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	g_pDevice = pDevice;
	g_pContext = pContext;

	m_Model[0] = ModelLoad("asset\\model\\test_player.fbx");
	m_Model[1] = ModelLoad("asset\\model\\test.fbx");

	m_Position = XMFLOAT3(7.0f, 8.0f, 5.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_State = PLAYER_STATE::PLAYER_STATE_IDLE;

	m_Hp = PLAYER_HP;
	JumpCount = true;

	g_StopTime = 0.0f;

}
void	PLAYER::Player_Finalize()
{
	ModelRelease(m_Model[0]);
	ModelRelease(m_Model[1]);

}
void	PLAYER::Player_Update()
{
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
<<<<<<< HEAD

	// --- ここだけ変更（着地判定とバウンド防止） ---
	float groundY = 0.0f; // 地面の高さ
	if (m_Position.y <= groundY)
	{
		m_Position.y = groundY;
		m_Velocity.y = 0.0f;
		JumpCount = true;
	}
	// --- ここまで ---
=======
	
	if (Keyboard_IsKeyDownTrigger(KK_ENTER))
	{
		m_Hp-=20.0f;
	}
>>>>>>> fdd5f4338004d02a7260176432a10eb6fb6f6f2a

	if (m_Position.y < PLAYER_RESPAWN)
	{
		m_State = PLAYER_STATE::PLAYER_STATE_RESPAWN;
	}

	if (m_Hp < 0.0f)
	{
		m_State = PLAYER_STATE::PLAYER_STATE_DEATH;
	}

}
void	PLAYER::Player_Draw()
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

	//モデルの描画リクエスト
	switch (m_State)
	{
	case PLAYER_STATE::PLAYER_STATE_IDLE:
		ModelDraw(m_Model[0]);
		break;
	case PLAYER_STATE::PLAYER_STATE_MOVE:
		ModelDraw(m_Model[0]);
		break;
	case PLAYER_STATE::PLAYER_STATE_RESPAWN:
		ModelDraw(m_Model[0]);
		break;
	}
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
        if (g_StopTime > 60.0f * 2)
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





