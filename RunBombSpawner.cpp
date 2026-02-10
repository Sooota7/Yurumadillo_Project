#include "RunBombSpawner.h"

static const float Waru = 60.0f; 

void RUNBOMBSPAWNER::Initialize_RunBombSpawner(XMFLOAT3 pos)
{
	m_Position = pos;
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scaling  = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_Active = false;
	m_CoolTime = 0.0f;

	m_RunBomb.Runbombsource_Initialize(pos, RUNBOMB_NONE);
}

void RUNBOMBSPAWNER::Finalize_RunBombSpawner()
{
}

void RUNBOMBSPAWNER::Update_RunBombSpawner(XMFLOAT3 pPlayerPos, XMFLOAT3 pPlayerRot)
{
	if (m_Active)
	{
		switch (m_RunBomb.Runbombsource_GetState())
		{
		case RUNBOMB_NONE:
			break;
		case RUNBOMB_ENEMY:
			m_RunBomb.Runbombsource_Enemy(pPlayerPos);
			break;
		case RUNBOMB_ITEM:
			m_RunBomb.Runbombsource_Safe();
			break;
		case RUNBOMB_ACTIVE_HAVE:
			m_RunBomb.Runbombsource_Active_Have(pPlayerPos,pPlayerRot);
			break;
		case RUNBOMB_ACTIVE_THROW:
			m_RunBomb.Runbombsource_Active_Throw();
			break;
		case RUNBOMB_EXPLOSION:
			m_RunBomb.Runbombsource_Explosion();
			break;
		case RUNBOMB_COOL:
			m_RunBomb.Runbombsource_Cool();
			m_Active = false;
			break;
		case RUNBOMB_MAX:
			break;
		default:
			break;
		}


	}
	else
	{
		m_CoolTime += 1.0f / Waru;
		
		if (m_CoolTime > 3.0f)
		{
			m_RunBomb.Runbombsource_Initialize(m_Position, RUNBOMB_ENEMY);

			m_CoolTime = 0.0f;
			m_Active = true;
		}
	}
}

void RUNBOMBSPAWNER::Draw_RunBombSpawner()
{
}
