#include "bombSource.h"

void BOMBSOURCE::BombSource_Initialize(XMFLOAT3 pos, BOMB_STATE state)
{
	m_Position = pos;
	m_State = state;
	m_Count = 0;
}

void BOMBSOURCE::BombSource_Finalize(void)
{
}

void BOMBSOURCE::BombSource_Update(void)
{
	switch (m_State)
	{
	case BOMB_NONE:
		break;
	case BOMB_SAFE:
		break;
	case BOMB_ACTIVE:
		break;
	case BOMB_EXPLOSION:
		break;
	default:
		break;
	}
}

void BOMBSOURCE::BombSource_Safe()
{

}

void BOMBSOURCE::BombSource_Active()
{
	m_Count += 1.0f / 60.0f;
	
}

void BOMBSOURCE::BombSource_Explosion()
{

}

void BOMBSOURCE::BombSource_SetState()
{

}
