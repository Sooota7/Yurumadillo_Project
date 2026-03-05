#include"Flowtbombsource.h"
#include	"keyboard.h"
#include	"collision.h"
#include	"mouse.h"
#include	"inputx.h"

static bool inputB = InputKeyKonCheck();


int FLOWTBOMBSOURCE::m_ExplosionSE_ID = -1;
int FLOWTBOMBSOURCE::m_ThrowSE_ID = -1;
bool FLOWTBOMBSOURCE::m_SEInitialized = false;

void FLOWTBOMBSOURCE::Flowtbombsource_Initialize(XMFLOAT3 pos, BOMB_STATE state, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_FirstPosition = pos;
	m_Position = pos;
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_State = state;
	m_Count = 0;
	m_Touch = false;
	m_ExplosionSEPlayed = false;

	// SEの初期化（最初のインスタンスでのみ実行）
	if (!m_SEInitialized)
	{
		m_ExplosionSE_ID = LoadAudio("asset\\Audio\\SE\\chomouse.wav");
		m_ThrowSE_ID = LoadAudio("asset\\Audio\\SE\\throw.wav");
		m_SEInitialized = true;
	}

	m_FlowtBombAnim.BombFlowtAnim_Initialize(pDevice, pContext);

}

void FLOWTBOMBSOURCE::Flowtbombsource_Update()
{
	if (m_Position.y < -3.0f)
	{
		m_State = BOMB_COOL;
	}

	switch (m_State)
	{
	case BOMB_NONE:
		m_FlowtBombAnim.SetBombFlowtAnimState(BOMBFLOWT_STATE_NONE);
		break;
	case BOMB_ITEM:
		m_FlowtBombAnim.SetBombFlowtAnimState(BOMBFLOWT_STATE_IDLE);
		m_FlowtBombAnim.BombFlowtAnim_Update(m_Position,m_Rotation);
		break;
	case BOMB_ACTIVE_HAVE:
		m_FlowtBombAnim.SetBombFlowtAnimState(BOMBFLOWT_STATE_MOVE);
		m_FlowtBombAnim.BombFlowtAnim_Update(m_Position, m_Rotation);
		break;
	case BOMB_ACTIVE_THROW:
		m_FlowtBombAnim.SetBombFlowtAnimState(BOMBFLOWT_STATE_MOVE);
		m_FlowtBombAnim.BombFlowtAnim_Update(m_Position, m_Rotation);
		break;
	case BOMB_EXPLOSION:
		m_FlowtBombAnim.SetBombFlowtAnimState(BOMBFLOWT_STATE_NONE);
		m_FlowtBombAnim.BombFlowtAnim_Update(m_Position, m_Rotation);
		break;
	case BOMB_COOL:
		m_FlowtBombAnim.SetBombFlowtAnimState(BOMBFLOWT_STATE_NONE);
		m_FlowtBombAnim.BombFlowtAnim_Update(m_Position, m_Rotation);
		break;
	default:
		break;
	}
}

void FLOWTBOMBSOURCE::Flowtbombsource_Draw(MODEL* model, MODEL* model2, MODEL* model3)
{
	m_FlowtBombAnim.BombFlowtAnim_Draw(model,model2,model3);
}

	

	void FLOWTBOMBSOURCE::Flowtbombsource_Finalize(void)
{
	m_FlowtBombAnim.BombFlowtAnim_Finalize();
}

void FLOWTBOMBSOURCE::Flowtbombsource_Safe()
{
	m_Position.y -= BOMB_GRAVITY;
	if (m_Touch)
	{
		m_State = BOMB_STATE::BOMB_ACTIVE_HAVE;
		m_Touch = false;
		m_Count = 0;
	}
}

void FLOWTBOMBSOURCE::Flowtbombsource_Active_Have(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
{
	m_Position = pPlayerPos;
	m_Position.y += 1.0f;



	m_Count += 1.0f / 60.0f;
	if (m_Count > 3.0f)
	{
		m_State = BOMB_STATE::BOMB_EXPLOSION;
		m_Count = 0;
	}



	//if (inputB) {
	//	if (Mouse_IsLeftDownTrigger())
	//	{
	//		// プレイヤーの向き
	//		float yaw = pPlayerRot.y;

	//		// プレイヤーの正面方向ベクトル
	//		float pVecX = sinf(yaw);
	//		float pVecZ = cosf(yaw);

	//		// 正規化
	//		float len = sqrtf(pVecX * pVecX + pVecZ * pVecZ);
	//		if (len > 0.0f) {
	//			pVecX /= len;
	//			pVecZ /= len;
	//		}

	//		float speed = BOMB_SPEED_MAX * BOMB_THROW_POWER;

	//		// 投げる速度
	//		m_Velocity.x = pVecX * speed;
	//		m_Velocity.y = BOMB_THROW_POWER;  // 上方向成分（好みで調整）
	//		m_Velocity.z = pVecZ * speed;

	//		m_State = BOMB_STATE::BOMB_ACTIVE_THROW;
	//	}
	//}
	//else {
	//	if (IsButtonTriggered(0, XINPUT_GAMEPAD_B))
	//	{
	//		// プレイヤーの向き
	//		float yaw = pPlayerRot.y;

	//		// プレイヤーの正面方向ベクトル
	//		float pVecX = sinf(yaw);
	//		float pVecZ = cosf(yaw);

	//		// 正規化
	//		float len = sqrtf(pVecX * pVecX + pVecZ * pVecZ);
	//		if (len > 0.0f) {
	//			pVecX /= len;
	//			pVecZ /= len;
	//		}

	//		float speed = BOMB_SPEED_MAX * BOMB_THROW_POWER;

	//		// 投げる速度
	//		m_Velocity.x = pVecX * speed;
	//		m_Velocity.y = BOMB_THROW_POWER;  // 上方向成分（好みで調整）
	//		m_Velocity.z = pVecZ * speed;

	//		m_State = BOMB_STATE::BOMB_ACTIVE_THROW;
	//	}
	//}



}

void FLOWTBOMBSOURCE::Flowtbombsource_Active_Throw()
{
	//m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;
	//m_Position.z += m_Velocity.z;

	//落下判定
	if (m_Position.y < -10.0f)
	{
		m_State = BOMB_STATE::BOMB_COOL;
		return;
	}

	//m_Velocity.x *= 0.98f;//速度を適当に減衰させる
	//m_Velocity.y = 0.98f;//追加する
	//m_Velocity.z *= 0.98f;

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
		m_State = BOMB_STATE::BOMB_EXPLOSION;
		m_Count = 0;
	}


	//m_Velocity.y -= BOMB_GRAVITY;



}

void FLOWTBOMBSOURCE::Flowtbombsource_Cool()
{
	m_Count += 1.0f / 60.0f;
	if (m_Count > 3.0f)
	{
		m_State = BOMB_STATE::BOMB_ITEM;
		m_Count = 0;
		m_Position = m_FirstPosition;
	}
}

void FLOWTBOMBSOURCE::Flowtbombsource_Explosion()
{
	// 爆発開始時に一度だけSEを再生
	if (!m_ExplosionSEPlayed && m_ExplosionSE_ID != -1)
	{
		PlayAudio(m_ExplosionSE_ID, false);
		m_ExplosionSEPlayed = true;
	}

	m_Count += 1.0f / 60.0f;
	if (m_Count > 2.0f)
	{
		m_State = BOMB_STATE::BOMB_COOL;
		m_Count = 0;
		m_ExplosionSEPlayed = false; // リセット
	}
}

void FLOWTBOMBSOURCE::Flowtbombsource_Active_Type()
{
	switch (m_Type)
	{
	case TYPE_NORMAL:

		break;
	case TYPE_FLOW:

		break;
	case TYPE_RUN:

		break;

	default:
		break;
	}

}
