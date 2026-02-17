#include"RunBombSource.h"
#include	"keyboard.h"
#include	"collision.h"
#include	"mouse.h"
#include	"inputx.h"

static bool inputB = InputKeyKonCheck();

void RUNBOMBSOURCE::Runbombsource_Initialize(XMFLOAT3 pos, RUNBOMB_STATE state,RUNBOMB_TYPE type)
{
	m_FirstPosition = pos;
	m_Position = pos;
	m_Rotation = XMFLOAT3(0, 0, 0);
	m_Velocity = XMFLOAT3(0, 0, 0);
	m_State = state;
	m_Count = 0;
	m_Touch = false;
	m_Type = type;

	m_fieldColision = false;
}

void RUNBOMBSOURCE::Runbombsource_Finalize(void)
{
}

void RUNBOMBSOURCE::Runbombsource_Safe()
{
	m_Position.y -= BOMB_GRAVITY;
	if (m_Touch)
	{
		m_State = RUNBOMB_STATE::RUNBOMB_ACTIVE_HAVE;
		m_Touch = false;
		m_Count = 0;
	}
}

void RUNBOMBSOURCE::Runbombsource_Active_Have(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
{
	m_Position = pPlayerPos;
	m_Position.y += 1.0f;



	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = RUNBOMB_STATE::RUNBOMB_EXPLOSION;
		m_Count = 0;
	}


	if (inputB) {
		if (Mouse_IsLeftDownTrigger())
		{
			// プレイヤーの向き
			float yaw = pPlayerRot.y;

			// プレイヤーの正面方向ベクトル
			float pVecX = sinf(yaw);
			float pVecZ = cosf(yaw);

			// 正規化
			float len = sqrtf(pVecX * pVecX + pVecZ * pVecZ);
			if (len > 0.0f) {
				pVecX /= len;
				pVecZ /= len;
			}

			float speed = BOMB_SPEED_MAX * BOMB_THROW_POWER;

			// 投げる速度
			m_Velocity.x = pVecX * speed;
			m_Velocity.y = BOMB_THROW_POWER;  // 上方向成分（好みで調整）
			m_Velocity.z = pVecZ * speed;

			m_State = RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW;
		}
	}
	else {
		if (IsButtonTriggered(0, XINPUT_GAMEPAD_B))
		{
			// プレイヤーの向き
			float yaw = pPlayerRot.y;

			// プレイヤーの正面方向ベクトル
			float pVecX = sinf(yaw);
			float pVecZ = cosf(yaw);

			// 正規化
			float len = sqrtf(pVecX * pVecX + pVecZ * pVecZ);
			if (len > 0.0f) {
				pVecX /= len;
				pVecZ /= len;
			}

			float speed = BOMB_SPEED_MAX * BOMB_THROW_POWER;

			// 投げる速度
			m_Velocity.x = pVecX * speed;
			m_Velocity.y = 0.1f;  // 上方向成分（好みで調整）
			m_Velocity.z = pVecZ * speed;

			m_State = RUNBOMB_STATE::RUNBOMB_ACTIVE_THROW;
		}
	}


}

void RUNBOMBSOURCE::Runbombsource_Active_Throw()
{
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	m_Position.z += m_Velocity.z;

	
	if (m_Velocity.x * m_Velocity.x + m_Velocity.y * m_Velocity.y > 0.0001f)
	{
		m_Rotation.y = atan2(m_Velocity.x, m_Velocity.z);
	}

	//落下判定
	if (m_Position.y < -8.0f)
	{
		m_State = RUNBOMB_STATE::RUNBOMB_COOL;
		return;
	}

	m_Velocity.x *= 0.99f;//速度を適当に減衰させる
	m_Velocity.y *= 0.98f;//追加する
	m_Velocity.z *= 0.99f;

	//静止チェック
	float	len =
		(
			m_Velocity.x * m_Velocity.x +
			m_Velocity.y * m_Velocity.y +
			m_Velocity.z * m_Velocity.z
			);

	if (len <= 0.0002f)//静止とみなす速度
	{
		m_StopTime++;
		if (m_StopTime > (60.0f * 2))//２秒間続いている
		{
			m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_StopTime = 0.0f;
		}
	}

	//当たり判定
	//float hit = BallField_Collision();

	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = RUNBOMB_STATE::RUNBOMB_EXPLOSION;
		m_Count = 0;
	}


	m_Velocity.y -= BOMB_GRAVITY;



}

void RUNBOMBSOURCE::Runbombsource_Cool()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 5.0f)
	{
		m_State = RUNBOMB_STATE::RUNBOMB_ITEM;
		m_Count = 0;
		m_Position = m_FirstPosition;
	}
}

void RUNBOMBSOURCE::Runbombsource_Explosion()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 2.0f)
	{
		m_State = RUNBOMB_STATE::RUNBOMB_COOL;
		m_Count = 0;
	}
}

void RUNBOMBSOURCE::Runbombsource_Enemy(XMFLOAT3 pPlayerPos)
{

	if (m_Type == RUNBOMB_TYPE_FREE)
	{
		// 敵の向きをプレイヤーに向ける
		XMFLOAT3 direction;

		direction.x = pPlayerPos.x - m_Position.x;
		direction.y = 0.0f;
		direction.z = pPlayerPos.z - m_Position.z;

		// 距離
		float length = sqrtf((direction.x * direction.x) +
			(direction.z * direction.z));

		if (length != 0.0f)
		{// 正規化
			direction.x /= length;
			direction.z /= length;

			float yaw = atan2(direction.x, direction.z);


			// [-π, π] に正規化
			if (yaw > XM_PI) yaw -= XM_2PI;
			if (yaw < -XM_PI) yaw += XM_2PI;


			m_Rotation.y = yaw;
		}

		{
			m_Position.x += m_Velocity.x;
			m_Position.y += m_Velocity.y;
			m_Position.z += m_Velocity.z;


			//落下判定
			if (m_Position.y < -15.0f)
			{
				m_State = RUNBOMB_STATE::RUNBOMB_COOL;
				return;
			}

			m_Velocity.x *= 0.99f;//速度を適当に減衰させる
			//m_Velocity.y *= 0.98f;//追加する
			m_Velocity.z *= 0.99f;

			//静止チェック
			float	len =
				(
					m_Velocity.x * m_Velocity.x +
					m_Velocity.y * m_Velocity.y +
					m_Velocity.z * m_Velocity.z
					);

			if (len <= 0.0002f)//静止とみなす速度
			{
				m_StopTime++;
				if (m_StopTime > (60.0f * 2))//２秒間続いている
				{
					m_Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
					m_StopTime = 0.0f;
				}
			}


			m_Velocity.y -= BOMB_GRAVITY;

		}
	}
	else
	{
		m_Velocity.x = 0;
		m_Velocity.y = 0;
		m_Velocity.z = 0;

		m_Velocity.y -= BOMB_GRAVITY;

		switch (m_Type)
		{
		case RUNBOMB_TYPE_UP:
			m_Velocity.z = 0.1f;
			break;
		case RUNBOMB_TYPE_DOWN:
			m_Velocity.z = -0.1f;
			break;
		case RUNBOMB_TYPE_RIGHT:
			m_Velocity.x = 0.1f;
			break;
		case RUNBOMB_TYPE_LEFT:
			m_Velocity.x = -0.1f;
			break;
		default:
			break;
		}

		XMFLOAT3 move = m_Velocity;

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

		// --- 進行方向に体の向きを合わせる ---
		if (len > 0.0f)
		{
			float angle = atan2f(m_Velocity.x, m_Velocity.z); // ← Y軸回転
			m_Rotation.y = angle;
		}
		
		m_Position.x += m_Velocity.x;
		m_Position.y += m_Velocity.y;
		m_Position.z += m_Velocity.z;

		//落下判定
		if (m_Position.y < -2.0f)
		{
			m_State = RUNBOMB_STATE::RUNBOMB_COOL;
			return;
		}

	}
}

//void RUNBOMBSOURCE::Runbombsource_Active_Type()
//{
//	switch (m_Type)
//	{
//	case TYPE_NORMAL:
//
//		break;
//	case TYPE_FLOW:
//
//		break;
//	case TYPE_RUN:
//
//		break;
//
//	default:
//		break;
//	}
//
//}
