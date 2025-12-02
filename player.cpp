
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

	m_Model[0] = ModelLoad("asset\\model\\ball.fbx");
	m_Model[1] = ModelLoad("asset\\model\\test.fbx");

	m_Position = XMFLOAT3(0.0f, 2.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Acceleration = XMFLOAT3(0.0f, -0.005f, 0.0f);

	m_Scaling = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_State = PLAYER_STATE::PLAYER_STATE_IDLE;

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
	}
	
	if (m_Velocity.y < PLAYER_FALLMAX)
	{
		m_Velocity.y = PLAYER_FALLMAX;
	}


	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;
	

	if (m_Position.y < PLAYER_DEATH)
	{
		m_State = PLAYER_STATE::PLAYER_STATE_RESPAWN;
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

void	PLAYER::Player_Move()
{
	m_Velocity.y -= PLAYER_GRAVITY; //重力

	if (Keyboard_IsKeyDown(KK_W)) //前
	{
		m_Velocity.z += PLAYER_ACCELERATION; //少しずつ加速
		m_Rotation.y = 0.0f;                 //進行方向に向く
	}
	else if (Keyboard_IsKeyDown(KK_S))  //後
	{
		m_Velocity.z -= PLAYER_ACCELERATION;
		m_Rotation.y = 180.0f;
	}
	else
	{
		m_Velocity.z = 0.0f; //離したら即停止
	}

	if (Keyboard_IsKeyDown(KK_D)) //右
	{
		m_Velocity.x += PLAYER_ACCELERATION;
		m_Rotation.y = 90.0f;
	}
	else if (Keyboard_IsKeyDown(KK_A)) //左
	{
		m_Velocity.x -= PLAYER_ACCELERATION;
		m_Rotation.y = 270.0f;
	}
	else
	{
		m_Velocity.x = 0.0f;//離したら即停止
	}

	

	if (m_Velocity.x > PLAYER_SPEEDMAX) //最高速度
	{
		m_Velocity.x = PLAYER_SPEEDMAX;
	}
	if (m_Velocity.x < -PLAYER_SPEEDMAX) //最高速度
	{
		m_Velocity.x = -PLAYER_SPEEDMAX;
	}
	if (m_Velocity.z > PLAYER_SPEEDMAX) //最高速度
	{
		m_Velocity.z = PLAYER_SPEEDMAX;
	}
	if (m_Velocity.z < -PLAYER_SPEEDMAX) //最高速度
	{
		m_Velocity.z = -PLAYER_SPEEDMAX;
	}
	
	if (Keyboard_IsKeyDownTrigger(KK_SPACE)&& JumpCount == true) //ジャンプ
	{
		m_State = PLAYER_STATE::PLAYER_STATE_JUMP;
	}

	

	//m_Velocity.x *= GENSUI;	// 速度を適当に減衰する
	////Velocity.y *= GENSUI;
	//m_Velocity.z *= GENSUI;

	// 静止チェック
	float	len = (m_Velocity.x * m_Velocity.x +
		m_Velocity.y * m_Velocity.y +
		m_Velocity.z * m_Velocity.z);

	if (len <= STOP_VELO) // 静止とみなす速度
	{
		g_StopTime++;
		if (g_StopTime > 60.0f * 2) // 2秒間続いている
		{
			m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_State = PLAYER_STATE::PLAYER_STATE_IDLE;
			g_StopTime = 0.0f;
		}
	}

	

	//float hit = PlayerFieldCollision(); 
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


void   PLAYER:: Player_BombMagic(BOMBSOURCE* pBomb)
{

	XMFLOAT3 BombPos = pBomb->BombSource_GetPosition();  // 爆弾の位置取得

	//爆弾のSTATE取得
	BOMB_STATE BombSta = pBomb->BombSource_GetState();

	 //距離の二乗で比較（高速）
	float dx = m_Position.x - BombPos.x;
	float dy = m_Position.y - BombPos.y;
	float dz = m_Position.z - BombPos.z;

	float dist2 = dx * dx + dy * dy + dz * dz;
	float range2 = PLAYER_MAGICRANGE * PLAYER_MAGICRANGE;

	if (Keyboard_IsKeyDownTrigger(KK_F))
	{
		if (dist2 <= range2)// 距離が範囲内 → 実行
		{
			if (BombSta==BOMB_STATE::BOMB_SAFE)
			{
				// 爆弾のSTATEを爆弾に変身状態に変更
				pBomb->BombSource_SetState(BOMB_STATE::BOMB_ACTIVE);
			}
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


PLAYER* PLAYER::GetPlayer()
{
	return this;
}



